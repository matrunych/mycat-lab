#ifndef MYCAT_BUFFER_OPERATIONS_HPP
#define MYCAT_BUFFER_OPERATIONS_HPP

#include <iostream>
#include <boost/program_options.hpp>
#include <vector>
#include <fcntl.h>

#define BYTECOUNT 512

int write_buffer(int fd, char *buffer, ssize_t size, int *status);

int read_buffer(int fd, char *buffer, ssize_t size, int *status);

char *convert_buffer(char *buffer, ssize_t &buf_size, int byte_count);

int read_write_files(std::vector<int> descriptors, char *buffer, ssize_t size, int *status);

int read_write_files_a(std::vector<int> descriptors, char *buffer, ssize_t size, int *status);

std::string char_to_hex(int n);

#endif //MYCAT_BUFFER_OPERATIONS_HPP
