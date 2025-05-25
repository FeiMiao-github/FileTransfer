#include <vector>
#include <boost/filesystem.hpp>
#include "file_manager.h"
#include "log.h"
#include "global_config.h"

using namespace ft::server;
namespace fs = boost::filesystem;

static bool is_subpath(const boost::filesystem::path& base, const boost::filesystem::path& target) {
    // 规范化路径
    try {
        auto abs_file = fs::absolute(target).lexically_normal();
        auto abs_dir = fs::absolute(base).lexically_normal();

        auto abs_file_str = abs_file.string();
        auto abs_dir_str = abs_dir.string();

        if (abs_dir_str.ends_with("/."))
        {
            abs_dir_str.pop_back();
        }

        LOG_INFO() << "\n文件路径: " << abs_file.string();
        LOG_INFO() << "\n目录路径: " << abs_dir.string();
        return abs_file_str.starts_with(abs_dir_str);
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR() << "Filesystem error: " << e.what();
        return false;
    }
}

std::vector<file> file_manager::getlist(const std::string_view path)
{
    std::vector<file> res;

    // auto current_dir = boost::filesystem::current_path();
    boost::filesystem::path current_dir = global_config::instance().get_upload_dir();
    auto target_path = current_dir / path;

    for (const auto& entry : fs::directory_iterator(target_path)) {
        const fs::path& path = entry.path();

        if (fs::is_directory(path))
        {
            res.push_back({path.filename().string(), file_type::DIRECTORY});
            continue;
        }
        // 获取文件名、类型、大小
        // LOG_INFO() << "\n文件名: " << path.filename().string();
        // LOG_INFO() << "\n类型: " << (fs::is_directory(path) ? "目录" : "文件");
        
        // if (fs::is_regular_file(path)) {
        //     LOG_INFO() << "\n大小: " << fs::file_size(path) << " 字节";
        // }
        
        // LOG_INFO() << "\n完整路径: " << path << std::endl;
        if (fs::is_regular_file(path))
        {
            res.push_back({path.filename().string(), file_type::REGULAR_FILE});
            continue;
        }
    }
    return res;
}

void file_manager::remove(const std::string_view path) {
    // auto current_dir = boost::filesystem::current_path();
    auto current_dir = boost::filesystem::path(global_config::instance().get_upload_dir());
    auto target_path = current_dir / path;

    if (!is_subpath(current_dir, target_path))
    {
        LOG_WARNING() << target_path << " is not a subpath of " << current_dir;
        return;
    }

    try {
        if (boost::filesystem::exists(target_path)) {
            bool removed = boost::filesystem::remove(target_path);
            if (removed) {
                LOG_INFO() << target_path << " has been removed.";
            } else {
                LOG_WARNING() << target_path << " has not been removed.";
            }
        } else {
            LOG_WARNING() << target_path << " does not exist.";
        }
    } catch (const boost::filesystem::filesystem_error& e) {
        LOG_ERROR() << "Error: " << e.what();
    }
}