#ifndef __FILE_UTILS_HPP__
#define __FILE_UTILS_HPP__

#include <fstream>
#include <string>
#include <cerrno>

std::string get_file_contents(const char *filename);

#endif
