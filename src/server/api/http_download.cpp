 #include "http_download.h"
#include "../http_handler.h"
#include "../global_config.h"
#include "../log.h"
#include <boost/url.hpp>

using namespace ft::server;

http_download_handler::http_msg_genearator 
    http_download_handler::handle_request(request_t<string_body>& req) const 
{
    auto &base_dir = global_config::instance().get_upload_dir();
    auto url = boost::urls::url_view(req.target());
    auto params = url.params();
    auto path = params.find("path");
    if (path == params.end())
    {
        LOG_WARNING() << "path not found.";
        throw http_not_found("path not found.");
    }

    auto file_handler = http_file_handler();
    file_handler.set_file_path(base_dir / (*path).value);
    http_base_handler* ptr = &file_handler;
    return ptr->handle_request(req);
}  
