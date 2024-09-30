# **Run Command Documentation**

  "run" runs a list of powershell commands written in a file, see additional documentation below

  Arguments:
  FILE- the file from which to read the commands, if no file is specified then it will search for a config.run file in the current working directory


  Parameters:

  a '#' denotes the beginning of a parameter

  Currently there are 2 parameters that can be used

  - #no-error: runs the entire list of commands, regardless of if a command fails to execute or otherwise, is useful if not every part of the command process is neccesary, 
      or a command returns a value other than 0 for success
  - #clear-after: after finishing, the command outputs will be cleared.

  

## Example:

Assume we have a directory:
```
dir/
  ->main.cpp
  ->debug.txt
  ->config.run
  ->test.run
  ->lib.dll
```

lets say all of these files compile and run main.cpp with different parameters and maybe with or without lib.dll,
the command "run" will run the config.run file
the command "run debug.txt" will run the commands in the debug.txt file
and the command "run test" will run the test.run file

## Example Files
**config.run**
```
#clear-after
g++ main.cpp -l lib.dll -o main.exe
main --example_flag
```

**debug.txt**
```
g++ main.cpp -l lib.dll -o main.exe
main --debug
```

**test.run**
```
#no-error
#clear-after
g++ main.cpp -o main.exe
main
```


**To see this text again easily, simply type run "-?"**

## Installation Instructions:

the included executable is built for windows and this installation guid likewise is for windows
compile run.cpp or download the executable run.exe
put the executable in a directory of your choice
to use the command from anywhere youll need to add the directory to your PATH variable- [tutorial](https://www.youtube.com/watch?v=ow2jROvxyH4) by ProgrammingKnowledge2
