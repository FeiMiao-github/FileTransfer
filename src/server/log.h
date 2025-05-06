#pragma once

#include "../comm/def.h"
#include "boost/log/trivial.hpp"

FT_SERVER_NSP_START

class log {
public:
    static void init();
};

#define LOG(severity) BOOST_LOG_TRIVIAL(severity) << __FILE_NAME__ << ":" << __LINE__ << " "
#define LOG_TRACE() BOOST_LOG_TRIVIAL(trace) << __FILE_NAME__ << ":" << __LINE__ << " "
#define LOG_DEBUG() BOOST_LOG_TRIVIAL(debug) << __FILE_NAME__ << ":" << __LINE__ << " "
#define LOG_INFO() BOOST_LOG_TRIVIAL(info) << __FILE_NAME__ << ":" << __LINE__ << " "
#define LOG_WARNING() BOOST_LOG_TRIVIAL(warning) << __FILE_NAME__ << ":" << __LINE__ << " "
#define LOG_ERROR() BOOST_LOG_TRIVIAL(error) << __FILE_NAME__ << ":" << __LINE__ << " "

FT_SERVER_NSP_END
