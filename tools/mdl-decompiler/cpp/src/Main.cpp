#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include "args/args.hxx"
#include "Exceptions.h"
#include "BufferedFile.h"
#include "MDLv14/MDLFile.h"
#include "MDLv14/ComponentReflection.h"
#include "QCv10/QCFile.h"
#include "QCv10/QCEFile.h"
#include "SMDv10/SMDFile.h"
#include "Conversions/Activity.h"
#include "Filesystem.h"
#include "cppfs/FilePath.h"
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "Utils.h"
#include "QCv10/QCFilePopulator.h"

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

static void WriteSMDFiles(
	const std::shared_ptr<MDLv14::MDLFile>& mdlFile,
	const cppfs::FilePath& outputDirPath,
	const std::unordered_map<std::string, std::string>& modelNameMap)
{
	const Container<MDLv14::TriangleMap>& triangleMap = mdlFile->GetTriangleMap();
	const Container<MDLv14::Vertex>& vertices = mdlFile->GetVertices();
	const Container<MDLv14::Normal>& normals = mdlFile->GetNormals();
	const Container<MDLv14::TextureCoOrdinate> texCoOrds = mdlFile->GetTextureCoOrdinates();

	for ( const auto& it : modelNameMap )
	{
		SMDv10::SMDFile smdFile;

		const MDLv14::Model* submodel = mdlFile->FindModelByName(it.first);

		if ( !submodel )
		{
			// Should never happen
			throw std::runtime_error("Lookup failed for submodel " + it.first);
		}

		for ( const MDLv14::ModelInfo& modelInfo : submodel->modelInfos )
		{
			for ( const MDLv14::Mesh& mesh : modelInfo.meshList )
			{
				for ( size_t base = 0; base < mesh.triangleCount; base += 3 )
				{
					// NOTE: If we want to retain multi-blends in the decompiled models (because
					// the Xash model compiler should support that), this is the place to do it.
					// In the original decompiler code, only blend 0 was used and assigned to
					// the vertices generated here, and this behaviour has been retained for now.

					const auto createVertex = [&](size_t triangleMapIndex) -> SMDv10::Vertex
					{
						SMDv10::Vertex vertex;

						uint16_t vIndex = triangleMap.GetElementChecked(triangleMapIndex).vertexIndex;
						vertex.position = vertices.GetElementChecked(vIndex).position;
						vertex.normal = normals.GetElementChecked(vIndex).position;
						vertex.bone = mdlFile->GetBoneIndicesUsedByMeshVertex(mesh, vIndex).GetElementChecked(0);
						vertex.texU = texCoOrds.GetElementChecked(vIndex).u;
						vertex.texV = texCoOrds.GetElementChecked(vIndex).v;

						return vertex;
					};

					SMDv10::Vertex v0 = createVertex(mesh.triangleIndex + base + 1);
					SMDv10::Vertex v1 = createVertex(mesh.triangleIndex + base + 0);
					SMDv10::Vertex v2 = createVertex(mesh.triangleIndex + base + 2);

					smdFile.AddTriangle(SMDv10::Triangle(std::move(v0), std::move(v1), std::move(v2)));
				}
			}
		}

		// TODO: Other aspects of SMD, and output
		(void)outputDirPath;
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

	std::cout << "MDL file references " << populator.GetReferencedModelNames().size() << " submodels:" << std::endl;

	for ( const auto& it : populator.GetReferencedModelNames() )
	{
		std::cout << "  " << it.first << std::endl;
	}

	std::cout << "Writing " << qcPath.toNative() << std::endl;
	qcFile->Write(*qcStream);

	std::cout << "Writing " << qcePath.toNative() << std::endl;
	qceFile->Write(*qceStream);

	WriteSMDFiles(mdlFile, outputDirPath, populator.GetReferencedModelNames());
}

static void ProcessFile(const cppfs::FilePath mdlPath, const cppfs::FilePath& outputDirPath, bool dumpHeader)
{
	std::cout << "Decompiling: " << mdlPath.toNative() << std::endl;

	std::shared_ptr<BufferedFile> inputFile = BufferedFile::OpenFile(mdlPath.toNative());
	std::shared_ptr<MDLv14::MDLFile> mdlFile = std::make_shared<MDLv14::MDLFile>(*inputFile);

	if ( !MakeDirectoryRecursive(outputDirPath.toNative()) )
	{
		throw FileIOException(outputDirPath.toNative(), "Could not create output directory.");
	}

	if ( dumpHeader )
	{
		DumpHeader(*mdlFile, outputDirPath);
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
