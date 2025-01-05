#include "lib/argparse.hpp"
#include <fstream>
#include "lib/lib.hpp"
#include <chrono>

using namespace std::chrono;

int main(int argc, char** argv) {

    
    system("cls");
    argument_parser parser = argument_parser(argc, argv);
    parser.add_argument("help", "-?", BOOL);
    parser.add_argument("file", "-f", STRING, false);
    parser.parse_arguments();

    if (cast_arg<ARG_BOOL>(parser.get_argument("help"))) {
        
        std::cout << "Help.\n";
        return 0;
    }

    std::string file_name = cast_arg_or<ARG_STRING>(parser.get_argument("file"), "config.run");
    
    bool directory1 = file_name.rfind("/") != std::string::npos;
    bool directory2 = file_name.rfind("\\") != std::string::npos;

    if (file_name.rfind(".") == std::string::npos || 
    (directory1 && file_name.rfind("/") > file_name.rfind(".")) || (directory2 &&(file_name.rfind("\\") > file_name.rfind(".")))) {
        file_name += ".run";
    }

    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Could not find file " << file_name << std::endl;
        abort();
    } else {
        std::cout << "Executing script: " << file_name << "\n" << std::endl;
    }


    std::unordered_map<std::string, std::string> definitions;

    //parse and add the commands to the commands vector
    script s = parse(file, definitions);

    
    auto start = high_resolution_clock::now();
    for (auto [command, no_error]: s.lines) {
        std::cout << "Executing {" << command << "}" << "\n";
        int exit = system(command.c_str());
        if (s.no_error || no_error) continue;
        if (exit != 0) {
            std::cerr << "Error, Command: {" << command << "} " << "exited with error code " << exit << "\n";
            std::cerr << "Stopping execution...\n\n";
            goto exit;
        }
    }


    exit:
    
    std::cout << "\nFinished running script in " << (duration_cast<milliseconds>(high_resolution_clock::now()-start).count())<< "ms\n\n";
    if (s.clear_after) system("cls");
    return 0;
}