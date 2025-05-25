#pragma once

#include "../comm/def.h"
#include <string>
#include <boost/filesystem.hpp>

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
    void set_upload_dir(const std::string& dir) { m_upload_dir = boost::filesystem::canonical(dir); }
    const auto& get_upload_dir() const { return m_upload_dir; }

    void set_html_dir(const std::string& dir) { m_html_dir = boost::filesystem::canonical(dir); }
    const auto& get_html_dir() const { return m_html_dir; }

    void dump() const;

private:
    boost::filesystem::path m_upload_dir;
    boost::filesystem::path m_html_dir;
};

FT_SERVER_NSP_END
