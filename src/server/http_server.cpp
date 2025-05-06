#include "http_server.h"

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/log/trivial.hpp>
#include <boost/url.hpp>
#include <memory>

#include "../comm/config.h"
#include "http_handler_mgr.h"
#include "http_listener.h"
#include "http_server_context.h"
#include "log.h"

using namespace ft::server;

http_server::http_server() {
    m_mgr = std::make_shared<http_handler_mgr>();
    m_base_dir = boost::filesystem::current_path().string();
}

    // m_mgr->register_handler("/api/upload", std::make_unique<http_json_handler>([](const http_request_t& req) {
    //     http_json_handler handler;

    //     // auto http_req = req.get_http_req();
    //     // std::ofstream file()
    //     // std::string boundary = "--" +
    //     // http_req.get(http::field::content_type).substr(http_req.get(http::field::content_type).find('=') + 1);

    //     // std::string filename;
    //     // std::string filedata;

    //     // // 解析multipart数据
    //     // parse_multipart(req.body(), boundary, filename, filedata);

    //     json::object data = {{"code", 0}, {"msg", "ok"}};
    //     handler.set_json_data(json::serialize(data));
    //     return handler(req);
    // }));

void http_server::run() {
    start_accept();
}

void http_server::start_accept() {
    using tcp = boost::asio::ip::tcp;

    if (!m_mgr)
    {
        throw http_internal_error("m_mgr is nullopt.");
    }

    boost::asio::io_context io_context{LISTENER_THREAD_NUM};

    std::vector<std::thread> threads;
    threads.reserve(LISTENER_THREAD_NUM - 1);

    auto end_point = boost::asio::ip::tcp::endpoint{tcp::v4(), m_port};
    LOG_INFO() << "server running on " << end_point.address() << ":" << end_point.port();

    std::make_shared<http_listener>(http_server_context{&io_context, m_mgr, m_base_dir}, end_point)->run();

    for (int i = 1; i < LISTENER_THREAD_NUM; ++i) {
        threads.emplace_back([&io_context]() { io_context.run(); });
    }

    io_context.run();
}