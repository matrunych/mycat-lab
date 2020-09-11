#include <iostream>
#include <boost/program_options.hpp>
#include <vector>
//#include "operations/operations.hpp"
#include <fcntl.h>
#include <iomanip>

#define BYTECOUNT 255


int write_buffer(int fd, char *buffer, ssize_t size, int *status) {
    ssize_t written_bytes = 0;
    while (written_bytes < size) {
        ssize_t written_now = write(fd, buffer + written_bytes, size - written_bytes);
        if (written_now == -1) {
            if (errno == EINTR)
                continue;
            else {
                *status = errno;
//                std::cerr << "Cannot write " << fd << std::endl;
                return -1;
            }
        } else
            written_bytes += written_now;
    }
    return 0;
}

int read_buffer(int fd, char *buffer, ssize_t size, int *status) {
    ssize_t read_bytes = 0;
    while (read_bytes < size) {
        ssize_t read_now = read(fd, buffer + read_bytes, size - read_bytes);
        if (read_now == -1) {
            if (errno == EINTR)
                continue;
            else {
                *status = errno;
//                std::cerr << "Cannot read " << fd << std::endl;
                return -1;
            }
        } else
            read_bytes += read_now;
    }
    return 0;
}

std::string char_to_hex(int n){
    std::stringstream ss;
    ss << "\\x" << std::setfill('0') << std::setw(2) << std::right<< std::hex << n;
    return ss.str();
}

char* convert_buffer(char *buffer, int& buf_size, int byte_count){
    int size = 0;
    for(int i = 0; i < byte_count; i++) {
        if (isprint(buffer[i])) {
            size += 1;
        } else {
            size += 4;
        }
    }

    buf_size = size;

    char* new_buffer;
    new_buffer = (char *) malloc(size);

    int idx = 0;
    for(int i = 0; i < byte_count; i++){
        if(isprint(buffer[i])){
            new_buffer[idx] = buffer[i];
            idx += 1;
        }
        else {
            std::string hex_char = char_to_hex(buffer[i]);
            for (int j = 0; j < 4; j++) {
                new_buffer[idx+j] = hex_char[j];
            }
            idx += 4;
        }
    }

    return new_buffer;
}

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
    std::cout << a << std::endl;


    int fd;
    std::vector<int> descriptors;
    for (auto f : files) {
        std::cout << f << std::endl;
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
    int s;
    ssize_t size;

//    for (auto fd: descriptors) {
//        size = lseek(fd, 0, SEEK_END);
//        lseek(fd, 0, SEEK_SET);
//        s = 0;
//        while (s + BYTECOUNT < size) {
////            if (read_buffer(fd, buffer, BYTECOUNT, status) == -1) {
////                std::cerr << "Cannot read " << fd << std::endl;
////
////            }
//            read_buffer(fd, buffer, BYTECOUNT, status);
//            write_buffer(1, buffer, BYTECOUNT, status);
//            s += BYTECOUNT;
//        }
//        int extra = size - s;
//        read_buffer(fd, buffer, extra, status);
//        write_buffer(1, buffer, extra, status);
//
//        if (close(fd) == -1) {
//            std::cerr << "Cannot close " << std::endl;
//            return -1;
//        }
//    }


//    char *new_buffer;
//    new_buffer = (char *) malloc(BYTECOUNT + 1);


    if(a){
        int new_buf_size;
        char* new_buffer;

        for (auto fd: descriptors) {
            size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            s = 0;
            while (s + BYTECOUNT < size) {
//            if (read_buffer(fd, buffer, BYTECOUNT, status) == -1) {
//                std::cerr << "Cannot read " << fd << std::endl;
//
//            }
                read_buffer(fd, buffer, BYTECOUNT, status);

                new_buffer = convert_buffer(buffer, new_buf_size, BYTECOUNT);
//                for(int i = 0; i < BYTECOUNT; i++) {
//                    if (isprint(buffer[i])) {
//                        buf_size += 1;
//                    } else {
//                        buf_size += 4;
//                    }
//                }
//
//                char* new_buffer;
//                new_buffer = (char *) malloc(buf_size);
//
//                int idx = 0;
//                for(int i = 0; i < BYTECOUNT; i++){
//                    if(isprint(buffer[i])){
//                        new_buffer[idx] = buffer[i];
//                        idx += 1;
//                    }
//                    else {
//                        std::string hex_char = char_to_hex(buffer[i]);
//                        for (int j = 0; j < 4; j++) {
//                            new_buffer[idx+j] = hex_char[j];
//                        }
//                        idx += 4;
//                    }
//                }

                write_buffer(1, new_buffer, new_buf_size, status);
                s += BYTECOUNT;
            }
            int extra = size - s;
            read_buffer(fd, buffer, extra, status);

            new_buffer = convert_buffer(buffer, new_buf_size, extra);

            write_buffer(1, new_buffer, new_buf_size, status);

            if (close(fd) == -1) {
                std::cerr << "Cannot close " << std::endl;
                return -1;
            }


        }
    }

    free(buffer);

    return EXIT_SUCCESS;
}