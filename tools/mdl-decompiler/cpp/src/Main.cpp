#include <iostream>
#include <fstream>
#include <iomanip>
#include "args/args.hxx"
#include "Exceptions.h"
#include "BufferedFile.h"
#include "MDLv14/MDLFile.h"
#include "MDLv14/ComponentReflection.h"
#include "QCv10/QCFile.h"
#include "QCv10/QCEFile.h"
#include "Conversions/Activity.h"
#include "Filesystem.h"
#include "cppfs/FilePath.h"
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "Utils.h"

static cppfs::FilePath GetPathFromCurrentDirectory(const std::string path)
{
	return cppfs::FilePath(GetCurrentDirectory()).resolve(path);
}

static void
ReadActivity(const MDLv14::MDLFile& mdlFile, QCv10::QCEFile& qceFile, QCv10::QCSequence& qcSequence, size_t seqIndex)
{
	const MDLv14::Sequence& sequence = mdlFile.GetSequences().GetElementChecked(seqIndex);

	if ( sequence.activity == 0 )
	{
		return;
	}

	MDLv14::Activity activity = MDLv14::ACT_INVALID;

	try
	{
		activity = Conversion::ConvertActivity_IntToV14(sequence.activity);
	}
	catch ( const std::invalid_argument& ex )
	{
		std::cerr << mdlFile.ModelName() << ": sequence " << seqIndex << " activity could not be read. " << ex.what()
				  << std::endl;

		return;
	}

	QCv10::Activity qcActivity = QCv10::ACT_INVALID;

	try
	{
		qcActivity = Conversion::ConvertActivity_V14ToV10(activity);
	}
	catch ( const std::invalid_argument& ex )
	{
		std::cerr << mdlFile.ModelName() << ": sequence " << seqIndex << " activity could not be converted. "
				  << ex.what() << std::endl;

		return;
	}

	qcSequence.activity.activity = qcActivity;
	qcSequence.activity.weight = static_cast<float>(sequence.activityWeight);

	if ( !StartsWith(Conversion::ActivityName(qcActivity), "ACT_") )
	{
		qceFile.AddReplaceActivity(QCv10::QCEReplaceActivity(qcSequence.name, Conversion::ActivityName(activity)));
		qcSequence.activity.activity = QCv10::ACT_INVALID;
	}
}

static void SetUpQCFiles(
	const MDLv14::MDLFile& mdlFile,
	QCv10::QCFile& qcFile,
	QCv10::QCEFile& qceFile,
	const cppfs::FilePath& outputDirPath)
{
	qceFile.SetVersion(QCv10::QCEVersion(QCv10::QCEGame::HalfLife, 10, 1, 0));

	qcFile.SetDirectory(outputDirPath.toNative());
	qcFile.SetModelName({mdlFile.GetHeader().name});
	qcFile.SetBBox(QCv10::QCBBox(mdlFile.GetHeader().boundingBox.min, mdlFile.GetHeader().boundingBox.max));
	qcFile.SetCBox(QCv10::QCCBox(mdlFile.GetHeader().clippingBox.min, mdlFile.GetHeader().clippingBox.max));
	qcFile.SetEyePosition(QCv10::QCEyePosition(mdlFile.GetHeader().eyePosition.pos));
	qcFile.SetTypeFlags(QCv10::QCTypeFlags(mdlFile.GetHeader().typeFlags));
	qcFile.SetOrigin(QCv10::QCOrigin(Vec3D()));

	for ( const MDLv14::Bone& bone : mdlFile.GetBones() )
	{
		if ( bone.parent == -1 )
		{
			qcFile.SetRoot(QCv10::QCRoot(bone.name));
			break;
		}
	}

	for ( const MDLv14::Attachment& attachment : mdlFile.GetAttachments() )
	{
		qcFile.AddAttachment(QCv10::QCAttachment(
			attachment.name,
			mdlFile.GetBones().GetElementChecked(attachment.bone).name,
			attachment.position));
	}

	for ( const MDLv14::BodyGroup& mdlBodyGroup : mdlFile.GetBodyGroups() )
	{
		QCv10::QCBodyGroup qcBodyGroup;
		qcBodyGroup.name = mdlBodyGroup.name;

		for ( size_t index = 0; index < IntToSizeT(mdlBodyGroup.modelCount); ++index )
		{
			const MDLv14::Model* model = mdlFile.FindModelByOffset(
				mdlBodyGroup.modelOffset + static_cast<int32_t>((index * Reflection::ReadSize<MDLv14::Model>())));

			if ( !model )
			{
				throw ValidationException(
					"MDLFile",
					"Body group " + mdlBodyGroup.name + " model " + std::to_string(index) + " (offset " +
						std::to_string(mdlBodyGroup.modelOffset) + " bytes) could not be mapped to a model component.");
			}

			if ( model->name == "blank" )
			{
				qcBodyGroup.bodies.emplace_back(QCv10::QCBodyGroupItem("blank"));
			}
			else
			{
				qcBodyGroup.bodies.emplace_back(QCv10::QCBodyGroupItem("studio", "TODO: Parse model and dump as SMD"));
			}
		}

		qcFile.AddBodyGroup(qcBodyGroup);
	}

	for ( const MDLv14::BoneController& controller : mdlFile.GetBoneControllers() )
	{
		QCv10::QCBoneController qcController {};

		qcController.index = controller.index;
		qcController.bone = mdlFile.GetBones().GetElementChecked(controller.bone).name;
		qcController.motionFlags = controller.motionType;
		qcController.start = controller.start;
		qcController.end = controller.end;

		qcFile.AddController(qcController);
	}

	for ( const MDLv14::HitBox& hitBox : mdlFile.GetHitBoxes() )
	{
		qcFile.AddHitBox(QCv10::QCHitBox(
			hitBox.group,
			mdlFile.GetBones().GetElementChecked(hitBox.bone).name,
			hitBox.min,
			hitBox.max));
	}

	if ( !mdlFile.GetBones().empty() )
	{
		for ( size_t seqIndex = 0; seqIndex < mdlFile.GetSequences().size(); ++seqIndex )
		{
			const MDLv14::Sequence& sequence = mdlFile.GetSequences().GetElementChecked(seqIndex);
			QCv10::QCSequence qcSeq;

			qcSeq.name = sequence.name;
			qcSeq.controlFlags = sequence.motionType;
			qcSeq.framesPerSecond = sequence.framesPerSecond;
			qcSeq.loop = (sequence.flags & MDLv14::SequenceFlag_Looping) != 0;

			if ( sequence.blendType0 != CommonTypes::MotionFlag_None )
			{
				qcSeq.blends.emplace_back(
					QCv10::QCOptionBlend(sequence.blendType0, sequence.blendStart0, sequence.blendEnd0));
			}

			if ( sequence.blendType1 != CommonTypes::MotionFlag_None )
			{
				qcSeq.blends.emplace_back(
					QCv10::QCOptionBlend(sequence.blendType1, sequence.blendStart1, sequence.blendEnd1));
			}

			for ( const MDLv14::Event& event : sequence.eventCollection )
			{
				qcSeq.events.emplace_back(QCv10::QCOptionEvent(event.event, event.frame, event.options));
			}

			ReadActivity(mdlFile, qceFile, qcSeq, seqIndex);

			qcFile.AddSequence(std::move(qcSeq));
		}
	}

	const MDLv14::SkinDataHolder& skins = mdlFile.GetSkinData();

	if ( skins.SkinFamilyCount() > 1 )
	{
		const Container<MDLv14::Texture>& textures = mdlFile.GetTextures();
		QCv10::QCTextureGroup texGroup;

		texGroup.name = "Skins";
		texGroup.skins.reserve(skins.SkinFamilyCount());

		for ( size_t familyIndex = 0; familyIndex < skins.SkinFamilyCount(); ++familyIndex )
		{
			Container<std::string> textureNames;
			textureNames.reserve(skins.SkinReferenceCount());

			for ( size_t refIndex = 0; refIndex < skins.SkinReferenceCount(); ++refIndex )
			{
				const int16_t& textureIndex = skins.GetEntry(familyIndex, refIndex);
				const std::string& textureName =
					textures.GetElementChecked(static_cast<size_t>(textureIndex)).textureName;

				textureNames.emplace_back(textureName);
			}

			texGroup.skins.emplace_back(std::move(textureNames));
		}

		qcFile.SetTextureGroup(std::move(texGroup));
	}
}

static std::ostream& DumpVector(std::ostream& stream, const Vec3D& vec)
{
	stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return stream;
}

static void DumpHeader(const MDLv14::MDLFile& mdlFile, const cppfs::FilePath& outputDirPath)
{
	static constexpr size_t COL_WIDTH = 18;

	cppfs::FilePath dumpPath = outputDirPath.resolve("header.txt");
	cppfs::FileHandle dumpFile = cppfs::fs::open(dumpPath.toNative());
	std::unique_ptr<std::ostream> dumpStream = dumpFile.createOutputStream();

	if ( !dumpStream )
	{
		throw FileIOException(dumpPath.toNative(), "Could not open file for writing.");
	}

	const MDLv14::Header& header = mdlFile.GetHeader();
	std::ostream& stream = *dumpStream;

	stream << std::setw(COL_WIDTH) << std::left << "Identifier: " << std::setw(0) << header.identifier << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Version: " << std::setw(0) << header.version << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Name: " << std::setw(0) << header.name << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Length: " << std::setw(0) << header.length << " bytes" << std::endl;

	stream << std::setw(COL_WIDTH) << std::left << "Eye position: " << std::setw(0);
	DumpVector(stream, header.eyePosition.pos);
	stream << std::endl;

	stream << std::setw(COL_WIDTH) << std::left << "Bounding box: " << std::setw(0);
	DumpVector(stream, header.boundingBox.min);
	stream << " - ";
	DumpVector(stream, header.boundingBox.max);
	stream << std::endl;

	stream << std::setw(COL_WIDTH) << std::left << "Clipping box: " << std::setw(0);
	DumpVector(stream, header.clippingBox.min);
	stream << " - ";
	DumpVector(stream, header.clippingBox.max);
	stream << std::endl;

	stream << std::setw(COL_WIDTH) << std::left << "Type flags: " << std::setw(0) << header.typeFlags << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Bones: " << std::setw(0) << header.bones.count << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Bone controllers: " << std::setw(0) << header.boneControllers.count
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Hitboxes: " << std::setw(0) << header.hitBoxes.count << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Sequences: " << std::setw(0) << header.sequences.count << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Sequence groups: " << std::setw(0) << header.sequenceGroups.count
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Textures: " << std::setw(0) << header.textures.count << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Skin references: " << std::setw(0) << header.skinReferences
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Skin families: " << std::setw(0) << header.skinFamilies
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Body groups: " << std::setw(0) << header.bodyGroups.count
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Attachments: " << std::setw(0) << header.attachments.count
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Sound groups: " << std::setw(0) << header.soundGroups.count
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Transitions: " << std::setw(0) << header.transitionsCount
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "LOD flags: " << std::setw(0) << header.levelOfDetailFlags
		   << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Models: " << std::setw(0) << header.modelCount << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Vertices: " << std::setw(0) << header.vertexCount << std::endl;
	stream << std::setw(COL_WIDTH) << std::left << "Triangles: " << std::setw(0) << header.triangleCount << std::endl;
}

static void WriteOutputFiles(const MDLv14::MDLFile& mdlFile, const cppfs::FilePath& outputDirPath)
{
	cppfs::FilePath qcPath = outputDirPath.resolve(mdlFile.ModelName() + ".qc");
	cppfs::FileHandle outQc = cppfs::fs::open(qcPath.toNative());

	std::unique_ptr<std::ostream> qcStream = outQc.createOutputStream();

	if ( !qcStream )
	{
		throw FileIOException(qcPath.toNative(), "Could not open QC file for writing.");
	}

	cppfs::FilePath qcePath = outputDirPath.resolve(mdlFile.ModelName() + ".qce");
	cppfs::FileHandle outQce = cppfs::fs::open(qcePath.toNative());

	std::unique_ptr<std::ostream> qceStream = outQce.createOutputStream();

	if ( !qceStream )
	{
		throw FileIOException(qcePath.toNative(), "Could not open QCE file for writing.");
	}

	QCv10::QCFile qcFile;
	QCv10::QCEFile qceFile;

	SetUpQCFiles(mdlFile, qcFile, qceFile, outputDirPath);

	std::cout << "Writing " << qcPath.toNative() << std::endl;
	qcFile.Write(*qcStream);

	std::cout << "Writing " << qcePath.toNative() << std::endl;
	qceFile.Write(*qceStream);
}

static void ProcessFile(const cppfs::FilePath mdlPath, const cppfs::FilePath& outputDirPath, bool dumpHeader)
{
	std::cout << "Decompiling: " << mdlPath.toNative() << std::endl;

	std::shared_ptr<BufferedFile> inputFile = BufferedFile::OpenFile(mdlPath.toNative());
	MDLv14::MDLFile mdlFile(*inputFile);

	if ( !MakeDirectoryRecursive(outputDirPath.toNative()) )
	{
		throw FileIOException(outputDirPath.toNative(), "Could not create output directory.");
	}

	if ( dumpHeader )
	{
		DumpHeader(mdlFile, outputDirPath);
	}

	WriteOutputFiles(mdlFile, outputDirPath);
}

int main(int argc, char** argv)
{
	args::ArgumentParser parser(EXECUTABLE_NAME " - Decompiles Nightfire v14 MDL files to SMD/QC files.");

	parser.helpParams.showTerminator = false;

	args::HelpFlag help(parser, "help", "Display this help menu.", {'h', "help"});
	args::Positional<std::string> inputFileArg(parser, "input_file", "MDLv14 file to decompile.");

	args::Flag dumpHeaderArg(
		parser,
		"dump_header",
		"If set, dumps details from the MDL header to header.txt in the output directory.",
		{"dump-header"});

	args::ValueFlag<std::string> outputDirArg(
		parser,
		"output_dir",
		"Directory in which to place output. If this does not exist, it is created.",
		{'o'});

	try
	{
		parser.ParseCLI(argc, argv);
	}
	catch ( const args::Completion& e )
	{
		std::cout << e.what();
		return 1;
	}
	catch ( const args::Help& )
	{
		std::cout << parser;
		return 1;
	}
	catch ( const args::ParseError& e )
	{
		std::cerr << e.what() << std::endl;
		std::cerr << parser;
		return 1;
	}

	if ( !inputFileArg )
	{
		std::cerr << "No input file was specified." << std::endl;
		return 1;
	}

	try
	{
		ProcessFile(
			GetPathFromCurrentDirectory(args::get(inputFileArg)),
			GetPathFromCurrentDirectory(args::get(outputDirArg)),
			dumpHeaderArg);
	}
	catch ( BaseException& ex )
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	catch ( const std::exception& ex )
	{
		std::cerr << "An unexpected error occurred. " << ex.what() << std::endl;
		return 1;
	}
	catch ( ... )
	{
		std::cerr << "An unhandled error occurred." << std::endl;
		return 1;
	}

	return 0;
}
