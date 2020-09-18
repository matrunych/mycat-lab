#include <iostream>
#include <boost/program_options.hpp>
#include <vector>
#include <fcntl.h>
#include "buffer_operations.hpp"


int main(int argc, char **argv) {

    namespace po = boost::program_options;

    std::vector<std::string> files;
    po::options_description visible("Supported options");
    visible.add_options()
            ("help,h", "Print this help message.")
            (",A", "Show invisible symbols.")
            ("input-file", po::value<std::vector<std::string>>(&files), "Input files.");

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
            options(visible).positional(p).run(), vm);
    po::notify(vm);


    if (vm.count("help")) {
        std::cout << "Concatenate given files and output to stdout.\n" << visible << std::endl;
        return EXIT_SUCCESS;
    }

    bool a = vm.count("-A") != 0;

    int fd;
    std::vector<int> descriptors;
    for (auto f : files) {
        fd = open(f.c_str(), O_RDONLY);
        if (fd == -1) {
            std::cerr << "Cannot open " << f << std::endl;
            return -1;
        }
        descriptors.push_back(fd);
    }


    char *buffer;
    buffer = (char *) malloc(BYTECOUNT);
    int *status = 0;
    ssize_t size;


    if (a) {
        read_write_files_a(descriptors, buffer, size, status);
    } else {
        read_write_files(descriptors, buffer, size, status);
    }

    for (auto fd: descriptors) {
        if (close(fd) == -1) {
            std::cerr << "Cannot close " << fd << std::endl;
            return -4;
        }
    }

    free(buffer);

    return 0;
}