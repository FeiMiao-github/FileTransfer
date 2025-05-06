#pragma once

#include "../../comm/def.h"
#include "http_response_base_factory.h"

FT_SERVER_NSP_START

template <typename T>
class http_not_found_response_factory
    : public http_response_base_factory_imp<T, http_response_base_factory::string_body> {
public:
    using base_type = http_response_base_factory_imp<T, http_response_base_factory::string_body>;
    using response_t = base_type::response_t;
    using request_t = base_type::request_t;

    response_t operator()(const request_t& req) {
        response_t res;
        res.result(boost::beast::http::status::not_found);
        res.version(req.version());
        res.set(boost::beast::http::field::server, ("FT_Server"));
        res.set(boost::beast::http::field::content_type, "text/html");
        res.keep_alive(req.keep_alive());
        res.body() = "The resource '" + std::string(req.target()) + "' was not found.";
        res.prepare_payload();
        return res;
    }
};

FT_SERVER_NSP_END
