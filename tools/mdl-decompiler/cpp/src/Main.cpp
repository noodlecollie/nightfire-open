#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <set>
#include <assert.h>
#include <atomic>
#include "args/args.hxx"
#include "Exceptions.h"
#include "BufferedFile.h"
#include "MDLv14/MDLFile.h"
#include "MDLv14/ComponentReflection.h"
#include "QCv10/QCFile.h"
#include "QCv10/QCEFile.h"
#include "SMDv10/SMDFile.h"
#include "SMDv10/SMDFileWriter.h"
#include "SMDv10/SMDReferencePopulator.h"
#include "SMDv10/SMDAnimationPopulator.h"
#include "Conversions/Activity.h"
#include "Filesystem.h"
#include "cppfs/FilePath.h"
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "Utils.h"
#include "QCv10/QCFilePopulator.h"
#include "AnalysisOptions.h"
#include "SigintHandler.h"

class CancelException : public std::exception
{
};

static cppfs::FilePath GetPathFromCurrentDirectory(const std::string path)
{
	return cppfs::FilePath(GetCurrentDirectory()).resolve(path);
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

static bool IsMultiBlendList(const std::vector<int8_t>& boneIndices)
{
	int8_t referencedBone = -1;

	for ( int8_t boneIndex : boneIndices )
	{
		if ( boneIndex < 0 )
		{
			continue;
		}

		if ( referencedBone < 0 )
		{
			referencedBone = boneIndex;
			continue;
		}

		if ( referencedBone != boneIndex )
		{
			return true;
		}
	}

	return false;
}

static std::string BuildMultiBlendGroupName(const MDLv14::MDLFile& mdlFile, const std::vector<int8_t>& boneIndices)
{
	std::string out;

	for ( int8_t boneIndex : boneIndices )
	{
		if ( boneIndex < 0 )
		{
			continue;
		}

		if ( !out.empty() )
		{
			out += ", ";
		}

		out += mdlFile.GetBones().GetElementChecked(boneIndex).name;
	}

	if ( out.empty() )
	{
		out = "<none>";
	}

	return out;
}

static void DumpMultiBlends(const MDLv14::MDLFile& mdlFile, const cppfs::FilePath& outputDirPath)
{
	cppfs::FilePath dumpPath = outputDirPath.resolve("blends.txt");
	cppfs::FileHandle dumpFile = cppfs::fs::open(dumpPath.toNative());
	std::unique_ptr<std::ostream> dumpStream = dumpFile.createOutputStream();

	if ( !dumpStream )
	{
		throw FileIOException(dumpPath.toNative(), "Could not open file for writing.");
	}

	std::ostream& stream = *dumpStream;
	std::set<std::string> multiBlendGroupsFound;

	for ( const MDLv14::Model& model : mdlFile.GetModels() )
	{
		for ( const MDLv14::ModelInfo& modelInfo : model.modelInfos )
		{
			for ( const MDLv14::Mesh& mesh : modelInfo.meshList )
			{
				for ( size_t triangleIndex = 0; triangleIndex < mesh.triangleCount; ++triangleIndex )
				{
					const uint16_t vIndex =
						mdlFile.GetTriangleMap().GetElementChecked(mesh.triangleIndex + triangleIndex).vertexIndex;

					std::vector<int8_t> boneIndices = mdlFile.GetBoneIndicesUsedByMeshVertex(mesh, vIndex);

					if ( IsMultiBlendList(boneIndices) )
					{
						multiBlendGroupsFound.insert(BuildMultiBlendGroupName(mdlFile, boneIndices));
					}
				}
			}
		}
	}

	if ( multiBlendGroupsFound.empty() )
	{
		stream << "No multi-blend vertices found." << std::endl;
		return;
	}

	stream << multiBlendGroupsFound.size()
		   << " unique groups of bones were found that affected multi-blend vertices:" << std::endl;

	for ( const std::string& group : multiBlendGroupsFound )
	{
		stream << "  " << group << std::endl;
	}
}

static void WriteSMDReferenceFiles(
	const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
	const cppfs::FilePath& outputDirPath,
	const std::vector<SMDv10::SMDName>& submodels)
{
	for ( const SMDv10::SMDName& ref : submodels )
	{
		SMDv10::SMDReferencePopulator populator(mdlFile, ref.nameInMDL);
		std::shared_ptr<SMDv10::SMDFile> smdFile = populator.Populate();

		cppfs::FilePath outSMDPath = outputDirPath.resolve(ref.outputNameOnDisk + ".smd");
		cppfs::FileHandle outSMD = cppfs::fs::open(outSMDPath.toNative());

		std::unique_ptr<std::ostream> smdStream = outSMD.createOutputStream();

		if ( !smdStream )
		{
			throw FileIOException(outSMDPath.toNative(), "Could not open SMD file for writing.");
		}

		std::cout << "Writing " << outSMDPath.toNative() << std::endl;

		SMDv10::SMDFileWriter writer(smdFile);
		writer.Write(*smdStream);
	}
}

static void WriteSMDAnimationFiles(
	const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
	const cppfs::FilePath& outputDirPath,
	const std::vector<SMDv10::SMDName>& animations)
{
	for ( const SMDv10::SMDName& anim : animations )
	{
		SMDv10::SMDAnimationPopulator populator(mdlFile, anim.nameInMDL);
		std::shared_ptr<SMDv10::SMDFile> smdFile = populator.Populate();

		cppfs::FilePath outSMDPath = outputDirPath.resolve(anim.outputNameOnDisk + ".smd");
		cppfs::FileHandle outSMD = cppfs::fs::open(outSMDPath.toNative());

		std::unique_ptr<std::ostream> smdStream = outSMD.createOutputStream();

		if ( !smdStream )
		{
			throw FileIOException(outSMDPath.toNative(), "Could not open SMD file for writing.");
		}

		std::cout << "Writing " << outSMDPath.toNative() << std::endl;

		SMDv10::SMDFileWriter writer(smdFile);
		writer.Write(*smdStream);
	}
}

static void WriteOutputFiles(const std::shared_ptr<MDLv14::MDLFile>& mdlFile, const cppfs::FilePath& outputDirPath)
{
	cppfs::FilePath qcPath = outputDirPath.resolve(mdlFile->ModelName() + ".qc");
	cppfs::FileHandle outQc = cppfs::fs::open(qcPath.toNative());

	std::unique_ptr<std::ostream> qcStream = outQc.createOutputStream();

	if ( !qcStream )
	{
		throw FileIOException(qcPath.toNative(), "Could not open QC file for writing.");
	}

	cppfs::FilePath qcePath = outputDirPath.resolve(mdlFile->ModelName() + ".qce");
	cppfs::FileHandle outQce = cppfs::fs::open(qcePath.toNative());

	std::unique_ptr<std::ostream> qceStream = outQce.createOutputStream();

	if ( !qceStream )
	{
		throw FileIOException(qcePath.toNative(), "Could not open QCE file for writing.");
	}

	std::shared_ptr<QCv10::QCFile> qcFile = std::make_shared<QCv10::QCFile>();
	std::shared_ptr<QCv10::QCEFile> qceFile = std::make_shared<QCv10::QCEFile>();

	QCv10::QCFilePopulator populator(mdlFile, qcFile, qceFile, outputDirPath.toNative());

	populator.Populate();

	std::cout << "Writing " << qcPath.toNative() << std::endl;
	qcFile->Write(*qcStream);

	std::cout << "Writing " << qcePath.toNative() << std::endl;
	qceFile->Write(*qceStream);

	WriteSMDReferenceFiles(mdlFile, outputDirPath, populator.GetReferenceSMDNames());
	WriteSMDAnimationFiles(mdlFile, outputDirPath, populator.GetAnimationSMDNames());
}

static void
ProcessFile(const cppfs::FilePath mdlPath, const cppfs::FilePath& outputDirPath, const AnalysisOptions& analysisOptions)
{
	std::cout << "Decompiling: " << mdlPath.toNative() << std::endl;

	std::shared_ptr<BufferedFile> inputFile = BufferedFile::OpenFile(mdlPath.toNative());
	std::shared_ptr<MDLv14::MDLFile> mdlFile = std::make_shared<MDLv14::MDLFile>(*inputFile);

	if ( !MakeDirectoryRecursive(outputDirPath.toNative()) )
	{
		throw FileIOException(outputDirPath.toNative(), "Could not create output directory.");
	}

	if ( analysisOptions.dumpHeader )
	{
		DumpHeader(*mdlFile, outputDirPath);
	}

	if ( analysisOptions.dumpMultiBlends )
	{
		DumpMultiBlends(*mdlFile, outputDirPath);
	}

	WriteOutputFiles(mdlFile, outputDirPath);
}

static void ProcessDirectoryRecursively(
	const cppfs::FilePath inputDirPath,
	const cppfs::FilePath& outputDirPath,
	const AnalysisOptions& analysisOptions)
{
	const std::string inputRoot = inputDirPath.path();

	try
	{
		cppfs::fs::open(inputRoot).traverse(
			[&](cppfs::FileHandle& item) -> bool
			{
				if ( SigintSignalled() )
				{
					throw CancelException();
				}

				const cppfs::FilePath itemPath(item.path());
				assert(itemPath.path().find(inputRoot) == 0);

				if ( DirectoryExists(itemPath.toNative()) )
				{
					// Traverse the directory.
					return true;
				}

				std::string extension = itemPath.extension();

				std::transform(
					extension.begin(),
					extension.end(),
					extension.begin(),
					[](char ch) -> char
					{
						return static_cast<char>(std::tolower(ch));
					});

				if ( extension != ".mdl" )
				{
					// Skip this file and return.
					return true;
				}

				std::string relPath(itemPath.path().substr(inputRoot.size()));
				relPath = relPath.substr(0, relPath.size() - extension.size());

				const cppfs::FilePath outputItemPath = outputDirPath.resolve(relPath);

				try
				{
					// TODO: Submit to a thread pool?
					ProcessFile(itemPath, outputItemPath, analysisOptions);
				}
				catch ( const BaseException& ex )
				{
					std::cerr << "An error occurred processing " << itemPath.toNative() << std::endl;
					std::cerr << "  " << ex.what() << std::endl;
					std::cerr << "Skipping this file." << std::endl;
				}

				return true;
			});
	}
	catch ( const CancelException& )
	{
		std::cout << "User cancelled operation." << std::endl;
	}
}

int main(int argc, char** argv)
{
	if ( !SetSigintHandler() )
	{
		std::cerr << "Could not set console Ctrl-C handler" << std::endl;
		return 1;
	}

	args::ArgumentParser parser(EXECUTABLE_NAME " - Decompiles Nightfire v14 MDL files to SMD/QC files.");

	parser.helpParams.showTerminator = false;

	args::HelpFlag help(parser, "help", "Display this help menu.", {'h', "help"});

	args::Positional<std::string> inputArg(
		parser,
		"input",
		"MDLv14 file to decompile, or a directory under which to recursively search for files.");

	args::Flag dumpHeaderArg(
		parser,
		"dump_header",
		"If set, dumps details from the MDL header to header.txt in the output directory.",
		{"dump-header"});

	args::Flag dumpMultiBlendsArg(
		parser,
		"dump_multi_blends",
		"If set, dumps details about whether the model uses blending on vertices to blends.txt in the output "
		"directory.",
		{"dump-multi-blends"});

	args::ValueFlag<std::string> outputDirArg(
		parser,
		"output_dir",
		"Directory in which to place output. If this does not exist, it is created. "
		"If the input is a directory, the output directory tree will mirror the input directory tree.",
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

	std::cout << EXECUTABLE_NAME << " - Based on original code by Ford & UltimateSniper" << std::endl;

	if ( !inputArg )
	{
		std::cerr << "No input file or directory was specified." << std::endl;
		return 1;
	}

	try
	{
		AnalysisOptions analysisOptions {};

		analysisOptions.dumpHeader = dumpHeaderArg;
		analysisOptions.dumpMultiBlends = dumpMultiBlendsArg;

		const cppfs::FilePath inputPath = GetPathFromCurrentDirectory(args::get(inputArg));
		const cppfs::FilePath outputPath = GetPathFromCurrentDirectory(args::get(outputDirArg));

		if ( FileExists(inputPath.toNative()) )
		{
			ProcessFile(inputPath, outputPath, analysisOptions);
		}
		else if ( DirectoryExists(inputPath.toNative()) )
		{
			ProcessDirectoryRecursively(inputPath, outputPath, analysisOptions);
		}
		else
		{
			throw FileIOException(inputPath.toNative(), "Input file or directory was not found");
		}
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
