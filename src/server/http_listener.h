#ifndef SERVER_HTTP_LISTENER_H
#define SERVER_HTTP_LISTENER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/noncopyable.hpp>
#include <memory>
#include "../comm/def.h"

FT_SERVER_NSP_START

class http_server_context;
class http_handler_mgr;

class http_listener: public std::enable_shared_from_this<http_listener> {
public:
    using context_t = boost::asio::io_context;
    using acceptor_t = boost::asio::ip::tcp::acceptor;
    using endpoint_t = boost::asio::ip::tcp::endpoint;
    using error_code_t = boost::beast::error_code;
    using tcp_socket_t = boost::asio::ip::tcp::socket;

    http_listener(http_server_context&& ctx,
                   endpoint_t endpoint);
    ~http_listener() = default;

    void run();

private:
    void do_accept();
    void on_accept(error_code_t ec, tcp_socket_t sock);

    context_t& m_io_context;
    acceptor_t m_acceptor;
    std::weak_ptr<http_handler_mgr> m_mgr;
};

FT_SERVER_NSP_END

#endif // SERVER_HTTP_LISTENER_H