#include "global_config.h"
#include "log.h"
#include <boost/filesystem.hpp>

using namespace ft::server;

void global_config::dump() const {
    // 输出真实的绝对路径
    LOG_INFO() << "upload dir: " << m_upload_dir;
    LOG_INFO() << "html dir: " << m_html_dir;
}
