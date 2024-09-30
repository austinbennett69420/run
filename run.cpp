#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;



int main(int argc, char** argv) {
    //clear when we start
    system("cls");
    ifstream run_commands;
    //flags
    bool run_with_no_error = false;
    bool clear_when_done = false;

    //documentation
    const char* doc = R"(
    ========RUN COMMAND DOCUMENTATION========

    "run" runs a list of powershell commands written in a file, see additional documentation below

    Arguments:
    FILE- the file from which to read the commands, if no file is specified then it will search for a config.run file in the current working directory


    Parameters:

    a '#' denotes the beginning of a parameter

    Currently there are 2 parameters that can be used

        - #no-error: runs the entire list of commands, regardless of if a command fails to execute or otherwise, is useful if not every part of the command process is neccesary, 
            or a command returns a value other than 0 for success
        - #clear-after: after finishing, the command outputs will be cleared.

    Comments:
    "//" at the beginning of a line denotes a comment
    
    
    Example:

    Assume we have a directory:
    
    dir/
    ->main.cpp
    ->debug.txt
    ->config.run
    ->test.run
    ->lib.dll
    

    lets say all of these files compile and run main.cpp with different parameters and maybe with or without lib.dll,
    the command "run" will run the config.run file
    the command "run debug.txt" will run the commands in the debug.txt file
    and the command "run test" will run the test.run file

    Example Files:

    config.run
    ```
    #clear-after
    g++ main.cpp -l lib.dll -o main.exe
    main --example_flag
    ```

    debug.txt
    ```
    g++ main.cpp -l lib.dll -o main.exe
    main --debug
    ```

    test.run
    ```
    // run with no error
    #no-error
    #clear-after
    g++ main.cpp -o main.exe
    main
    ```


    To see this text again easily, simply type "run -?"
    )";
    
    //check for input file
    if (argc == 1) {
        //if there is none look for a config.run file
        run_commands.open("config.run");
        if (!run_commands.is_open()) {
            cout << "Error: No config file found or file is invalid!\n";
            exit(1);
        } else {
            cout << "Found config.run, executing...\n";
        }
    } else {
        //check if the user is asking for the documentation
        if (strcmp(argv[1], "-?") == 0) {
            
            cout << doc;
            
            return 0;
        }
        //other wise we check if they added a file extension or not, and if not assume the file ends with ".run"
        string f = argv[1];
        if (f.find(".") == string::npos) {
            f += ".run";
        }
        
        //open and check if the file is open
        run_commands.open(f);
        if (!run_commands.is_open()) {
            cout << "Error opening file " << f << "\n";
            exit(1);
        } else {
            cout << "Found file " << f << " executing...\n";
        }
    }

    //run the file
    string command;
    while (getline(run_commands, command)) {
        if (command.length() == 0) {
            continue;
        } else {
            if (command.length() >= 2) {
                if (command.substr(0, 2) == "//") {
                    continue;
                }
            }
            if (command[0] == '#') {
                if (command == "#no-error") {
                    run_with_no_error = true;
                    continue;
                }
                if (command == "#clear-after") {
                    clear_when_done = true;
                    continue;
                }
            }
            cout << "Executing: " << command << endl;
            if (system(command.c_str()) != 0) {
                if (!run_with_no_error) {
                    cout << "Error occured running command: " << command << "\nExiting...\n";
                    exit(2);
                }
            }
        }

    }
    //clean up
    cout << "\n\nFinished running script\n";
    if (clear_when_done) {
        system("cls");
    }
    run_commands.close();

    return 0;


}
