#include "http_handler_mgr.h"
#include "log.h"
#include "api.h"
#include "global_config.h"

using namespace ft::server;

template <typename T>
using request_parser_t = boost::beast::http::request_parser<T>;
using empty_body_t = boost::beast::http::empty_body;

void http_handler_mgr::init() {
    // auto file_handler = std::make_unique<http_file_handler>();
    // file_handler->set_file_path(global_config::instance().get_html_dir());
    // register_handler(verb::get, "/", std::move(file_handler));
    register_handler(verb::get, "/api/getlist", std::make_unique<http_get_list_handler>());
    register_handler(verb::delete_, "/api/delete", std::make_unique<http_del_handler>());
    register_handler(verb::post, "/api/upload", std::make_unique<http_upload_handler>());
    register_handler(verb::get, "/api/download", std::make_unique<http_download_handler>());
}

void http_handler_mgr::deinit() {
}

void http_handler_mgr::register_handler(verb v, std::string&& str, std::unique_ptr<http_base_handler>&& handler) {
    auto verb_ump_iter = m_handlers.find(v);
    if (verb_ump_iter == m_handlers.end()) {
        m_handlers.emplace(v, std::unordered_map<std::string, std::unique_ptr<http_base_handler>>());
    }

    auto& url_umap = m_handlers[v];
    if (url_umap.contains(str))
    {
        throw http_internal_error("handler already exists.");
    }
    url_umap.emplace(std::forward<std::string>(str), std::forward<std::unique_ptr<http_base_handler>>(handler));
}

void http_handler_mgr::unregister_handler(verb v, const std::string &str) {
    auto ump = m_handlers.find(v);
    if (ump == m_handlers.end()) {
        throw http_internal_error("verb not found.");
    }

    auto it = ump->second.find(str);
    if (it == ump->second.end()) {
        throw http_internal_error("handler not found.");
    }

    ump->second.erase(it);
}

const std::unique_ptr<http_base_handler>& http_handler_mgr::get_handler(verb v, const std::string &url) const {
    auto verb_ump = m_handlers.find(v);
    if (verb_ump == m_handlers.end()) {
        LOG_WARNING() << "verb " << v << " not found.";
        throw http_not_found("verb not found.");
    }

    auto url_ump_iter = verb_ump->second.find(url);
    if (url_ump_iter == verb_ump->second.end()) {
        LOG_WARNING() << "url " << url << " not found.";
        throw http_not_found("handler not found.");
    }

    return url_ump_iter->second;
}

http_handler_mgr::http_msg_genearator http_handler_mgr::handle_options(http_request_t<boost::beast::http::empty_body>&& req) {
    auto handler = http_options_handler();
    return handler.handle_request(req);
}
