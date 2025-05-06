#ifndef SERVER_HTTP_SERVER_CONTEXT_H
#define SERVER_HTTP_SERVER_CONTEXT_H

#include <memory>
#include <boost/log/trivial.hpp>
#include "../comm/def.h"
#include "log.h"

class io_context;

FT_SERVER_NSP_START
class http_handler_mgr;

struct http_server_context {
    boost::asio::io_context* io_context;
    std::shared_ptr<http_handler_mgr> mgr;
    std::string base_dir;

    http_server_context& operator=(http_server_context&& ctx)
    {
        LOG_INFO() << __FUNCTION__ << " run !";
        return *this;
    }
};

FT_SERVER_NSP_END

#endif //SERVER_HTTP_SERVER_CONTEXT_H
 