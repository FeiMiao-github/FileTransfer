#include <boost/log/trivial.hpp>
#include <boost/version.hpp>

#include "info.h"
#include "log.h"

using namespace ft::server;

void info::dump() const {
    LOG_INFO() << "Beast version: " << BOOST_LIB_VERSION;
}