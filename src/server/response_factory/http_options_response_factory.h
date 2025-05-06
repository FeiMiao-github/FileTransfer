#pragma once

#include <boost/json.hpp>
#include "../../comm/def.h"
#include "http_response_base_factory.h"

FT_SERVER_NSP_START

class http_options_response_factory
    : public http_response_base_factory_imp<http_response_base_factory::empty_body, http_response_base_factory::empty_body> {
public:
    response_t operator()(request_t& req) {
        // 创建响应
        // http::response<http::empty_body> res{http::status::ok, 11};
        response_t res;
        res.result(status::ok);
        res.version(req.version());

#if defined (FT_SERVER_ENABLE_CROS)
        // 设置允许的请求方法
        // res.set(field::allow, "GET, POST, OPTIONS, DELETE");
        // res.set(field::content_type, "text/plain");

         // 设置 CORS 相关的响应头
        res.set(field::access_control_allow_origin, "*"); // 允许所有来源访问
        res.set(field::access_control_allow_methods, "GET, POST, OPTIONS, DELETE");
        res.set(field::access_control_allow_headers, "Content-Type, Access-Control-Allow-Headers, Access-Control-Allow-Methods, Access-Control-Allow-Origin");
#endif
        res.prepare_payload();
        
        return res;
    }
};

FT_SERVER_NSP_END
