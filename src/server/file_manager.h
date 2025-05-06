#ifndef SERVER_FILE_MANAGER_H
#define SERVER_FILE_MANAGER_H

#include "../comm/def.h"
#include <vector>
#include <string>

FT_SERVER_NSP_START

enum class file_type {
    REGULAR_FILE,
    DIRECTORY
};

struct file {
    std::string name;
    file_type f_type;
};

class file_manager {
public:
    std::vector<file> getlist(const std::string_view path = "");
    void remove(std::string_view path);
};

FT_SERVER_NSP_END

#endif // SERVER_FILE_MANAGER_H
