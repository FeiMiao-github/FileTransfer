#include "http_json_response_factory.h"

using namespace ft::server;

http_json_response_factory::response_t http_json_response_factory::operator()(request_t& req, const json_value_t& json_value) {
    response_t res;
    res.result(boost::beast::http::status::ok);
    res.version(req.version());
    res.set(boost::beast::http::field::server, ("FT_Server"));
    res.set(boost::beast::http::field::content_type, "application/json");
    res.keep_alive(req.keep_alive());
    res.body() = boost::json::serialize(json_value);
    res.prepare_payload();

#if defined(FT_SERVER_ENABLE_CROS)
    res.set(field::access_control_allow_origin, "*");
#endif

    return res;
}
