#ifndef SERVER_HTTP_SERVER_H
#define SERVER_HTTP_SERVER_H

#include <memory>
#include <string>
#include "../comm/def.h"

FT_SERVER_NSP_START

class http_handler_mgr;

class http_server {
public:
    http_server();
    ~http_server() = default;

    void set_port(short port) { m_port = port; }
    void set_base_dir(const std::string& base_dir) { m_base_dir = std::forward<const std::string>(base_dir); }

    void set_handler_mgr(std::shared_ptr<http_handler_mgr>& mgr) { m_mgr = std::forward<std::shared_ptr<http_handler_mgr>>(mgr); }

    void run();

private:
    void start_accept();

    static constexpr int THREAD_COUNT = 1;

    unsigned short m_port;
    std::shared_ptr<http_handler_mgr> m_mgr;
    std::string m_base_dir;
};

FT_SERVER_NSP_END

#endif // SERVER_HTTP_SERVER_H