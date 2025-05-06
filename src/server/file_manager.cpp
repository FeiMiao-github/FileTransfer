#include <vector>
#include <boost/filesystem.hpp>
#include "file_manager.h"
#include "log.h"

using namespace ft::server;
namespace fs = boost::filesystem;

static bool is_subpath(const boost::filesystem::path& base, const boost::filesystem::path& target) {
    try {
        boost::filesystem::path base_canonical = boost::filesystem::canonical(base);
        boost::filesystem::path target_canonical = boost::filesystem::canonical(target);

        // 判断是否是子路径
        auto mismatch = std::mismatch(base_canonical.begin(), base_canonical.end(), target_canonical.begin());
        return mismatch.first == base_canonical.end();
    } catch (const boost::filesystem::filesystem_error& e) {
        return false;
    }
}

std::vector<file> file_manager::getlist(const std::string_view path)
{
    std::vector<file> res;

    auto current_dir = boost::filesystem::current_path();
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
    auto current_dir = boost::filesystem::current_path();
    auto target_path = current_dir / path;

    if (!is_subpath(current_dir, target_path))
    {
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