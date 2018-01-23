#ifndef __COMMAND_LINE_OPTIONS_H_
#define __COMMAND_LINE_OPTIONS_H_


#include <cstdio>
#include <fstream>
#include <string>
#include <vector>


#include <getopt.h>
#include <sys/stat.h>


namespace cmdline {


struct Options {
    std::vector<std::string> genome_fps;
    std::string kmer_db_fp;
    std::string output_dir;
};

void print_help(FILE *stdst);
void print_version(FILE *stdst);
Options get_arguments(int argc, char **argv);
bool is_file(std::string &filepath);
bool is_directory(std::string &filepath);


} // namespace cmdline


#endif
