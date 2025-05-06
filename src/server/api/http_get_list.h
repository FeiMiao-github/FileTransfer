#pragma once

#include <boost/json.hpp>
#include <boost/url.hpp>
#include "../../comm/def.h"
#include "http_api_handler.h"

FT_SERVER_NSP_START

class http_get_list_handler: public http_api_handler {
public:
    http_msg_genearator handle_request(request_t<string_body>& req) const override;
};

FT_SERVER_NSP_END