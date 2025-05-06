#include "http_get_list.h"
#include "../handler/http_custom_handler.h"
#include "../http_response_factory.h"
#include "../file_manager.h"

using namespace ft::server;

http_get_list_handler::http_msg_genearator http_get_list_handler::handle_request(request_t<string_body>& req) const
{
    auto url_view = boost::urls::url_view(req.target());
    auto params = url_view.params();
    auto dir = params.find("dir");
    std::vector<file> list;
    if (dir != params.end())
    {
        list = file_manager().getlist((*dir).value);
    }
    else
    {
        list = file_manager().getlist();
    }

    boost::json::array arr;
    for (auto &&p: list) {
        // LOG_INFO() << p.name;
        boost::json::object file_json = {
            {"name", std::move(p.name)},
            {"type", p.f_type == file_type::REGULAR_FILE ? "file" : "dir"}
        };
        arr.push_back(std::move(file_json));
    }

    boost::json::object data = {{"code", 0}, {"msg", "ok"}, {"data", {{"file_list", arr}}}};

    http_json_response_factory json_response_creator;
    return json_response_creator(req, data);
}