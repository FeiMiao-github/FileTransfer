#ifndef SERVER_HTTP_HHANDLER_MGR_H
#define SERVER_HTTP_HHANDLER_MGR_H

#include <string>
#include <unordered_map>
#include <memory>

#include <boost/beast.hpp>
#include <boost/url.hpp>

#include "../comm/def.h"
#include "http_exception.h"
#include "http_handler.h"
#include "log.h"
#include "global_config.h"

FT_SERVER_NSP_START

class http_handler_mgr {
public:
    template <typename T>
    using http_request_t = boost::beast::http::request<T>;

    template <typename T>
    using request_t = boost::beast::http::request<T>;

    using http_msg_genearator = boost::beast::http::message_generator;

    using verb = boost::beast::http::verb;

    http_handler_mgr()  = default;
    ~http_handler_mgr() = default;

    http_handler_mgr(const http_handler_mgr&) = delete;
    http_handler_mgr& operator=(const http_handler_mgr&) = delete;

    void init();
    void deinit();

    void register_handler(verb v, std::string&& str, std::unique_ptr<http_base_handler> &&handler);
    void unregister_handler(verb v, const std::string &str);
    const std::unique_ptr<http_base_handler>& get_handler(verb v, const std::string &str) const;

    template <typename T>
    http_msg_genearator handle(http_request_t<T>&& req)
    {
        try {
            try {
                boost::urls::url_view url(req.target());
                const auto& handler = get_handler(req.method(), url.path());
                return handler->handle_request(req);
            }
            catch (http_not_found e) {
                auto file_handler = http_file_handler();
                auto filepath = global_config::instance().get_html_dir();
                LOG_INFO() << "filepath: " << filepath;
                file_handler.set_file_path(filepath / std::string(req.target()));
                http_base_handler* ptr = &file_handler;
                return ptr->handle_request(req);
            }
        }
        catch (http_internal_error e)
        {
            LOG_WARNING() << req.method_string() << " " << req.target() << " " << e.what();
            auto handler = http_internal_error_handler<T>();
            http_base_handler* ptr = &handler;
            return ptr->handle_request(req);
        }
        catch (http_base_exception e)
        {
            LOG_WARNING() << req.method_string() << " " << req.target() << " " << e.what();
            auto handler = http_not_found_handler<T>();
            http_base_handler* ptr = &handler;
            return ptr->handle_request(req);
        }
    }

    http_msg_genearator handle_options(http_request_t<boost::beast::http::empty_body>&& req);

private:
    std::unordered_map<verb, std::unordered_map<std::string, std::unique_ptr<http_base_handler>>> m_handlers;
};

FT_SERVER_NSP_END

#endif // SERVER_HTTP_HHANDLER_MGR_H