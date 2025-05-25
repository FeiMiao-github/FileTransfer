#ifndef HTTP_DOWNLOAD_H
#define HTTP_DOWNLOAD_H

#include "../../comm/def.h"
#include "http_api_handler.h"

FT_SERVER_NSP_START

class http_download_handler
    : public http_api_handler {
public:
    http_msg_genearator handle_request(request_t<string_body>& req) const override;    
};

FT_SERVER_NSP_END

#endif // HTTP_DOWNLOAD_H
