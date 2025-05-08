#pragma once

#include "../comm/def.h"
#include <string>

FT_SERVER_NSP_START

class global_config {
public:
    static global_config& instance() {
        static global_config instance;
        return instance;
    }

private:
    global_config() {}
    virtual ~global_config() {}

    global_config(const global_config&) = delete;
    global_config(global_config&&) = delete;
    global_config& operator=(const global_config&) = delete;
    global_config& operator=(global_config&&) = delete;

public:
    void set_upload_dir(const std::string& dir) { upload_dir = dir; }
    const std::string& get_upload_dir() const { return upload_dir; }

private:
    std::string upload_dir;
};

FT_SERVER_NSP_END
