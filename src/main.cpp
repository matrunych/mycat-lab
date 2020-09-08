#include <iostream>
#include <boost/program_options.hpp>
#include <vector>
//#include "operations/operations.hpp"
#include <fcntl.h>


int writebuffer(int fd, char* buffer, ssize_t size, int* status){
    ssize_t written_bytes = 0;
    while( written_bytes < size ) {
        ssize_t written_now = write(fd, buffer + written_bytes, size - written_bytes );
        if ( written_now == -1){
            if (errno == EINTR)
                continue;
            else{
                *status = errno;
                return -1;
            }
        }else
            written_bytes += written_now;
    }
    return 0;
}

int read_buffer(int fd, char* buffer, ssize_t size, int* status){
    ssize_t read_bytes = 0;
    while (read_bytes < size){
        ssize_t read_now = read(fd, buffer + read_bytes, size - read_bytes);
        if (read_now == -1){
            if (errno == EINTR)
                continue;
            else{
                *status = errno;
                return -1;
            }
        } else
            read_bytes += read_now;
    }
    return 0;
}



int main(int argc, char **argv) {
//    int variable_a, variable_b;
//
    namespace po = boost::program_options;
//
//    po::options_description visible("Supported options");
//    visible.add_options()
//            ("help,h", "Print this help message.");
//
//    po::options_description hidden("Hidden options");
//    hidden.add_options()
//            ("a", po::value<int>(&variable_a)->default_value(0), "Variable A.")
//            ("b", po::value<int>(&variable_b)->default_value(0), "Variable B.");
//
//    po::positional_options_description p;
//    p.add("a", 1);
////    p.add("b", 1);
//
//    po::options_description all("All options");
//    all.add(visible).add(hidden);
//
//    po::variables_map vm;
//    po::store(po::command_line_parser(argc, argv).options(all).positional(p).run(), vm);
//    po::notify(vm);
//
//
//
//    if (vm.count("help")) {
//        std::cout << "Usage:\n  add [a] [b]\n" << visible << std::endl;
//        return EXIT_SUCCESS;
//    }

//    int result = operations::add(variable_a, variable_b);
//    std::cout << result << std::endl;

    std::cout<< argc << std::endl;

    int fd;
    std::vector <int> descriptors;
    std::vector <std::string> files;
    for(int i = 1; i < argc; i++){
        std::cout<< "sdcc" << std::endl;

        fd = open(argv[i], O_RDONLY);
        if(fd == -1){
            return -1;
        }
        descriptors.push_back(fd);
    }
    for (auto x: descriptors){
        std::cout<< x << std::endl;
    }


    return EXIT_SUCCESS;
}
