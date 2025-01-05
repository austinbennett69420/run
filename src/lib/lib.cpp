#include "lib.hpp"
#include <iostream>
#include <sstream>
#include "misc.hpp"

std::unordered_map<std::string, parameter> parameters = {
    {"#no-error", NO_ERROR},
    {"#clear-after", CLEAR_AFTER},
    {"#run", RUN},
    {"#define", DEFINE},
};

const int max_param_len = 12;
const int min_param_len = 4;

std::string replace_defines(std::string line, std::unordered_map<std::string, std::string> defines) {
    std::stringstream builder;
    for (auto[name, def]: defines) {
        
        for (long long i = 0; i <= static_cast<long long>(line.length())-static_cast<long long>(name.length()); i++) {
            
            
            if (line.substr(i, name.length()) == name) {
                if (i > 0 && line[i-1] != ' ') {
                    continue;
                }
                if (i < (static_cast<long long>(line.length())-static_cast<long long>(name.length())) && line[i+name.length()] != ' ') {
                    continue;
                }

                builder << line.substr(0, i) << def;
                line.erase(0, i+name.length());
                i = -1;
            }
        }
    }

    builder << line;

    return builder.str();
}

std::string clean_line(std::string l) {
    //remove these comments
    size_t find = 0;
    if ((find = l.find("//")) != l.npos) {
        l.erase(find);
    }
    
    std::string result;
    bool charFound = false;
    
    for (char currentChar : l) {
        if (iswspace(currentChar)) {
            if (!charFound) {
                result += currentChar;
                charFound = true;
            }
        } else {
            result += currentChar;
            charFound = false;
        }
    }

    l = result;

    //trim
    if (iswspace(l[0])) {
        l = l.substr(1);
    }
    if (iswspace(l[l.length()-1])) {
        l = l.substr(0, l.size()-1);
    }

    

    return l;
}


parameter find_parameter(std::string l) {
    if (l.size() < min_param_len) return NONE;
    std::string builder = l.substr(0, min_param_len);
    
    while (parameters.find(builder) == parameters.end() && builder.size() < l.size() && builder.size() < max_param_len) {
        builder += l[builder.size()];
    }

    if (parameters.find(builder) != parameters.end()) {
        return parameters[builder];
    }
    return NONE;


}

script parse(std::ifstream& file, std::unordered_map<std::string, std::string>& definitions) {
    script Script = {{}, false, false};
    std::string line;
    bool arg_mode = false;
    std::vector<std::string> tokens = {};
    parameter p;
    while (std::getline(file, line)) {
        
        line = clean_line(line);
        tokens = split(line);
        //start parsing
        line = replace_defines(line, definitions);
        if (arg_mode) {
            if (line == "}") {
                arg_mode = false;
                continue;
            }
            Script.lines.back().command += " "+line;
            continue;
        }

        if ((p = find_parameter(line)) != NONE) {
            if (p == NO_ERROR && line.length() == 9) {
                Script.no_error = true;
                continue;
            } else if (p == NO_ERROR) {
                Script.lines.push_back({line.substr(10), true});
                continue;
            }
            if (p == CLEAR_AFTER) {
                Script.clear_after = true;
                continue;
            }
            if (p == DEFINE) {
                if (tokens.size() < 3) {
                    std::cerr << "Error parsing line: " << line << "\n";
                    std::cerr << "Expected definition\n";
                    abort();
                }
                definitions[tokens[1]] = line.substr(9+tokens[1].size());
                continue;
            }
            if (p == RUN) {
                if (tokens.size() != 3) {
                    std::cerr << "Error parsing line: " << line << "\n";
                    if (tokens.size() == 1) {
                        std::cerr << "Expected command\n";
                    }
                    if (tokens.size() == 2 && tokens[1] == "{") {
                        std::cerr << "Expected command\n";
                    }
                    if (tokens.size() == 2 && tokens[1] != "{") {
                        std::cerr << "Expected '{'\n";
                    }
                    abort();
                }
                Script.lines.push_back({tokens[1], false});
                arg_mode = true;
                continue;
            }
        }
        if (line != "") Script.lines.push_back({line, false});
    }

    return Script;
}