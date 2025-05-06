#include "http_del.h"
#include "../file_manager.h"
#include "../log.h"
#include "../http_response_factory.h"

using namespace ft::server;

http_del_handler::http_msg_genearator http_del_handler::handle_request(request_t<string_body>& req) const
{
    auto url = boost::urls::url_view(req.target());
    auto params = url.params();
    auto dir = params.find("path");
    if (dir != params.end())
    {
        LOG_INFO() << "delete " << (*dir).value;
        file_manager().remove((*dir).value);
    }

    boost::json::object data = {{"code", 0}, {"msg", "ok"}};
    http_json_response_factory json_response_creator;
    return json_response_creator(req, data);
}
