#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>


extern std::string replace_defines(std::string line, std::unordered_map<std::string, std::string> defines);

extern std::string clean_line(std::string l);

enum parameter {
    NONE,
    CLEAR_AFTER,
    NO_ERROR,
    DEFINE,
    RUN
};

extern std::unordered_map<std::string, parameter> parameters;

struct command {
    std::string command;
    bool run_without_error;
};

struct script {
    std::vector<command> lines;
    bool no_error;
    bool clear_after;
};

extern parameter find_parameter(std::string l);

extern script parse(std::ifstream& file, std::unordered_map<std::string, std::string>& definitions);