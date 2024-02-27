
#include "Driver.h"

#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	// Parses arguments.
	std::string inputFilename;
	
	po::options_description desc(
		"-- pulmtln --\n"
		"per unit length capacitance and inductance for MTL cross - sections.\n"
		"Visit https://github.com/OpenSEMBA/pulmtln for more information.\n"
		"Available options"
	);
	desc.add_options()
		("help,h", "this help message")
		("input,i", po::value(&inputFilename), "input filename .pulmtln.json")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);


	if (vm.count("help") || vm.empty()) {
		std::cout << desc << std::endl;
		return 0;
	}
	
	std::cout << desc << std::endl;
	std::cout << "Using input file: " << inputFilename << std::endl;


	// Launcher.
	std::string folder{ 
		std::filesystem::path{ inputFilename }.parent_path().string() + "/"
	};

	auto driver{ pulmtln::Driver::loadFromFile(inputFilename) };
	driver.setExportFolder(folder);
	driver.getMTLPUL();

	std::cout << "-- pulmtln finished succesfully --" << std::endl;
}
