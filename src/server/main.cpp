#include "cmdline_parser.h"
#include "http_server.h"
#include "http_handler_mgr.h"
#include "info.h"
#include "log.h"

int main(int argc, char** argv)
{
    ft::server::cmdline_parser parser;
    parser.parse(argc, argv);

    ft::server::log::init();
    ft::server::info().dump();

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
