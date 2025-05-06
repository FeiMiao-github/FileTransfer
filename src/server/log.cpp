#include "log.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

using namespace ft::server;

void log::init() {
    // 设置日志文件输出
    boost::log::add_file_log(
        boost::log::keywords::file_name = "FT_SERVER_%N.log",
        boost::log::keywords::format = "[%TimeStamp%]: [%Severity%] %Message%",
        boost::log::keywords::rotation_size = 10 * 1024 * 1024,
        boost::log::keywords::auto_flush = true
    );
    
    // 添加控制台输出
    boost::log::add_console_log(
        std::cout,
        boost::log::keywords::format = "[%TimeStamp%]: [%Severity%] %Message%",
        boost::log::keywords::auto_flush = true
    );

    // 添加通用属性（时间戳等）
    boost::log::add_common_attributes();

    // 设置全局日志级别
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity > boost::log::trivial::trace // 设置日志级别
    );
}
