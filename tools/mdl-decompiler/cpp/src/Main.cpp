#include <iostream>
#include <fstream>
#include "args/args.hxx"
#include "Exceptions.h"
#include "BufferedFile.h"
#include "MDLv14/MDLFile.h"
#include "MDLv14/ComponentReflection.h"
#include "QCv14/QCFile.h"
#include "QCv14/QCEFile.h"
#include "Filesystem.h"
#include "cppfs/FilePath.h"
#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"

static cppfs::FilePath GetPathFromCurrentDirectory(const std::string path)
{
	return cppfs::FilePath(GetCurrentDirectory()).resolve(path);
}

static void SetUpQCFiles(const MDLv14::MDLFile& mdlFile, QCv14::QCFile& qcFile, QCv14::QCEFile& qceFile)
{
	qceFile.SetVersion(QCv14::QCEVersion(QCv14::QCEGame::HalfLife, 10, 1, 0));

	qcFile.SetModelName({mdlFile.GetHeader().name});

	for ( const MDLv14::Attachment& attachment : mdlFile.GetAttachments() )
	{
		qcFile.AddAttachment(QCv14::QCAttachment(
			attachment.name,
			mdlFile.GetBones().GetElementChecked(attachment.bone).name,
			attachment.position));
	}
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

	QCv14::QCFile qcFile;
	QCv14::QCEFile qceFile;

	SetUpQCFiles(mdlFile, qcFile, qceFile);

	qcFile.Write(*qcStream);
	qceFile.Write(*qceStream);
}

static void ProcessFile(const cppfs::FilePath mdlPath, const cppfs::FilePath& outputDirPath)
{
	std::cout << "Decompiling: " << mdlPath.toNative() << std::endl;

	std::shared_ptr<BufferedFile> inputFile = BufferedFile::OpenFile(mdlPath.toNative());
	MDLv14::MDLFile mdlFile(*inputFile);

	if ( !MakeDirectoryRecursive(outputDirPath.toNative()) )
	{
		throw FileIOException(outputDirPath.toNative(), "Could not create output directory.");
	}

	WriteOutputFiles(mdlFile, outputDirPath);
}

int main(int argc, char** argv)
{
	args::ArgumentParser parser(EXECUTABLE_NAME " - Decompiles Nightfire v14 MDL files to SMD/QC files.");

	parser.helpParams.showTerminator = false;

	args::HelpFlag help(parser, "help", "Display this help menu.", {'h', "help"});
	args::Positional<std::string> inputFileArg(parser, "input_file", "MDLv14 file to decompile.");
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
			GetPathFromCurrentDirectory(args::get(outputDirArg)));
	}
	catch ( const FileIOException& ex )
	{
		std::cerr << ex.FilePath() << ": " << ex.Description() << std::endl;
		return 1;
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
