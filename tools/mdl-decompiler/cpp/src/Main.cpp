#include <iostream>
#include "args/args.hxx"
#include "Exceptions.h"
#include "BufferedFile.h"
#include "MDLv14/MDLFile.h"

static void ProcessFile(const std::string& path)
{
	std::cout << "Decompiling: " << path << std::endl;

	std::shared_ptr<BufferedFile> inputFile = BufferedFile::OpenFile(path);
	MDLv14::MDLFile mdlFile(*inputFile);
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
		ProcessFile(args::get(inputFileArg));
	}
	catch ( const FileIOException& ex )
	{
		std::cerr << ex.FilePath() << ": " << ex.Description() << std::endl;
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
