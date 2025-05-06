#pragma once

#include <boost/beast.hpp>
#include "../../comm/def.h"

FT_SERVER_NSP_START

class http_response_base_factory {
public:
    template <typename Req>
    using request_t = const boost::beast::http::request<Req>;

    template <typename Rep>
    using response_t = boost::beast::http::response<Rep>;
    using file_body = boost::beast::http::file_body;
    using string_body = boost::beast::http::string_body;
    using empty_body = boost::beast::http::empty_body;
    using status = boost::beast::http::status;
    using field = boost::beast::http::field;
};

template <typename Req, typename Rep>
class http_response_base_factory_imp: public http_response_base_factory {
public:
    using request_t = http_response_base_factory::request_t<Req>;
    using response_t = http_response_base_factory::response_t<Rep>;
};

// class server_error_processor
//     : public http_response_base_factory<http_handler_base::string_body, http_handler_base::string_body> {
// public:
//     server_error_processor() : m_reason("Unknown error") {}
//     server_error_processor(const std::string& what) : m_reason(what) {}
//     virtual ~server_error_processor() = default;

//     void operator()(request_t& req, response_t& res) {
//         res.result(boost::beast::http::status::internal_server_error);
//         res.version(req.version());
//         res.set(boost::beast::http::field::server, ("FT_Server"));
//         res.set(boost::beast::http::field::content_type, "text/html");
//         res.keep_alive(req.keep_alive());
//         res.body() = "An error occurred: " + std::string(m_reason);
//         res.prepare_payload();
//     }

// private:
//     std::string m_reason;
// };

// class bad_request_processor
//     : public http_response_base_factory<http_handler_base::string_body, http_handler_base::string_body> {
// public:
//     void operator()(request_t& req, response_t& res) {
//         res.result(boost::beast::http::status::bad_request);
//         res.version(req.version());
//         res.set(boost::beast::http::field::server, ("FT_Server"));
//         res.set(boost::beast::http::field::content_type, "text/html");
//         res.keep_alive(req.keep_alive());
//         res.body() = std::string("Invalid request: ") + std::string(req.target());
//         res.prepare_payload();
//     }
// };

// class optional_processor
//     : public http_response_base_factory<http_handler_base::string_body, http_handler_base::string_body> {
// public:
//     void operator()(request_t& req, response_t& rep) {
//         rep.set(field::access_control_allow_origin, "*");
//         rep.set(field::access_control_allow_methods, "GET, POST, OPTIONS, DELETE");
//         rep.set(field::access_control_allow_headers,
//                 "Content-Type, Authorization, X-Requested-With, access-control-allow-methods, "
//                 "access-control-allow-origin, access-control-allow-headers");
//         // res.set(http::field::access_control_allow_credentials, "true");
//         rep.result(status::no_content); // 预检请求返回204
//     }
// };

FT_SERVER_NSP_END
