#pragma once

#include <boost/beast.hpp>
#include <boost/filesystem.hpp>
#include <boost/json.hpp>
#include <boost/log/trivial.hpp>

#include "../../comm/def.h"
#include "../http_exception.h"

FT_SERVER_NSP_START

class http_base_handler {
public:
    using http_msg_genearator = boost::beast::http::message_generator;
    using error_code_t = boost::beast::error_code;

    using string_view = boost::beast::string_view;
    using string_body = boost::beast::http::string_body;
    using file_body = boost::beast::http::file_body;
    using empty_body = boost::beast::http::empty_body;
    using dynamic_body = boost::beast::http::dynamic_body;

    using status = boost::beast::http::status;
    using field = boost::beast::http::field;

    template <typename T>
    using request_t = boost::beast::http::request<T>;

    template <typename T>
    using response_t = boost::beast::http::response<T>;

#if defined (DEF_FUNC_HANDLE_REQUEST)
#error "DEF_FUNC_HANDLE_REQUEST already defined."
#else
    #define DEF_FUNC_HANDLE_REQUEST(T) \
    public: \
    virtual http_msg_genearator handle_request(request_t<T>& req) const { \
        throw http_not_implemented(#T " not implemented."); \
    }

    DEF_FUNC_HANDLE_REQUEST(string_body);
    DEF_FUNC_HANDLE_REQUEST(dynamic_body);
    DEF_FUNC_HANDLE_REQUEST(file_body);
    DEF_FUNC_HANDLE_REQUEST(empty_body);

    #undef DEF_FUNC_HANDLE_REQUEST
#endif

public:
    http_base_handler() = default;
    virtual ~http_base_handler() = default;
};

FT_SERVER_NSP_END