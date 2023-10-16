#include <iostream>
#include "args/args.hxx"

int main(int argc, char** argv)
{
	args::ArgumentParser parser(EXECUTABLE_NAME " - Decompiles Nightfire v14 MDL files to SMD/QC files.");

	parser.helpParams.showTerminator = false;

	args::HelpFlag help(parser, "help", "Display this help menu.", {'h', "help"});
	args::Positional<std::string> inputFileArg(parser, "input_file", "MDLv14 file to decompile.");

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

	// TODO
	std::string inputFilePath = args::get(inputFileArg);
	std::cout << "Decompiling: " << inputFilePath << std::endl;

	return 0;
}
