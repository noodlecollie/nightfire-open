
#include <iostream>

#include <cppassist/cmdline/CommandLineProgram.h>
#include <cppassist/cmdline/CommandLineAction.h>
#include <cppassist/cmdline/CommandLineSwitch.h>
#include <cppassist/cmdline/CommandLineOption.h>
#include <cppassist/cmdline/CommandLineParameter.h>

#include <cppfs/cppfs-version.h>
#include <cppfs/fs.h>
#include <cppfs/LoginCredentials.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FileIterator.h>


using namespace cppassist;
using namespace cppfs;


int main(int argc, char * argv[])
{
    // Declare program
    CommandLineProgram program(
        "cppfs-ls",
        "cppfs-ls " CPPFS_VERSION,
        "List all files in a directory."
    );

    CommandLineAction action("list", "List files in directory");
    program.add(&action);

    CommandLineSwitch swHelp("--help", "-h", "Print help text", CommandLineSwitch::Optional);
    action.add(&swHelp);

    CommandLineOption opConfig("--config", "-c", "file", "Load configuration from file", CommandLineOption::Optional);
    action.add(&opConfig);

    CommandLineParameter paramPath("path", CommandLineParameter::Optional);
    action.add(&paramPath);

    // Parse command line
    program.parse(argc, argv);
    if (program.hasErrors() || swHelp.activated())
    {
        // Print help and exit
        program.print(program.help());
        return 0;
    }

    // Get login credentials
    LoginCredentials login;

    std::string configFile = opConfig.value();
    if (!configFile.empty())
    {
        login.load(configFile);
    }

    // Get path
    std::string path = paramPath.value();
    if (path.empty()) path = ".";

    // Open directory
    FileHandle dir = fs::open(path, &login);
    if (dir.isDirectory())
    {
        // List files
        for (auto it = dir.begin(); it != dir.end(); ++it)
        {
            std::cout << "- " << *it;

            FileHandle f = dir.open(*it);
            if (f.isDirectory()) {
                std::cout << " (DIR)";
            } else if (f.isSymbolicLink()) {
                std::cout << " (LNK)";                
            }

            std::cout << std::endl;
        }
    }
    else
    {
        std::cout << "'" << path << "' is not a valid directory." << std::endl;
    }

    // Done
    return 0;
}
