#include "cmdline_parser.h"
#include "http_server.h"
#include "http_handler_mgr.h"
#include "info.h"
#include "log.h"
#include "global_config.h"

int main(int argc, char** argv)
{
    ft::server::cmdline_parser parser;
    parser.parse(argc, argv);

    ft::server::global_config::instance().set_upload_dir(parser.get_base_dir());
    ft::server::global_config::instance().set_html_dir(parser.get_html_dir());

    ft::server::log::init();
    ft::server::info().dump();
    ft::server::global_config::instance().dump();

    auto mgr = std::make_shared<ft::server::http_handler_mgr>();
    mgr->init();

    ft::server::http_server server;
    server.set_port(parser.get_port());
    server.set_base_dir(std::string("."));
    server.set_handler_mgr(mgr);
    server.run();

    mgr->deinit();

    return 0;
}
