#include <iostream>
#include "http_listener.h"
#include "http_exception.h"
#include "http_server_context.h"
#include "http_session.h"

using namespace ft::server;
namespace net = boost::asio;

http_listener::http_listener(http_server_context&& ctx, endpoint_t endpoint)
    : m_io_context(*(ctx.io_context)), m_acceptor(boost::asio::make_strand(*(ctx.io_context))), m_mgr(ctx.mgr) {
        boost::beast::error_code ec;
        m_acceptor.open(endpoint.protocol(), ec);
        if (ec)
        {
            std::cout << "open error: " << ec.message() << std::endl;
            throw http_base_exception(ec.message());
        }

        m_acceptor.set_option(boost::asio::socket_base::reuse_address(true));
        if (ec)
        {
            throw http_base_exception(ec.message());
        }
        
        m_acceptor.bind(endpoint, ec);
        if (ec)
        {
            throw http_base_exception(ec.message());
        }
        
        m_acceptor.listen(boost::asio::socket_base::max_connections, ec);
        if (ec)
        {
            throw http_base_exception(ec.message());
        }
}

void http_listener::run() {
    net::dispatch(m_acceptor.get_executor(), boost::beast::bind_front_handler(&http_listener::do_accept, shared_from_this()));
}

void http_listener::do_accept() {
    m_acceptor.async_accept(
        boost::asio::make_strand(m_io_context),
        boost::beast::bind_front_handler(&http_listener::on_accept, shared_from_this()));
}

void http_listener::on_accept(error_code_t ec, tcp_socket_t sock) {
    if (ec)
    {
        throw http_base_exception(ec.message());
    }
    else
    {
        std::make_shared<http_session>(std::move(sock), m_mgr)->run();
    }

    do_accept();
}
