#ifndef _UTILS_HPP
#define _UTILS_HPP

#include <fstream>
#include <string>

template <typename Lambda>
void save_to_file_repeated(const Lambda &&lambda, std::string filename) {
    std::ofstream output_file(filename, std::ofstream::app);

    output_file.close();
    output_file.open(filename, std::ofstream::out | std::ofstream::trunc);

    for (int i = 0; i < 100; i++) {
        lambda(output_file);
    }

    output_file.close();
}

template <typename... Args>
std::string format(const char *fmt, Args... args) {
    char buffer[256];
    sprintf(buffer, fmt, args...);

    return std::string(buffer);
}

#endif
