#include "cmdline_parser.h"

#include <iostream>

#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/filesystem.hpp>

using namespace ft::server;

void cmdline_parser::parse(int argc, char** argv)
{
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("port,p", boost::program_options::value<short>(&m_port)->default_value(8080), "Port number")
        ("dir,d", boost::program_options::value<std::string>(&m_base_dir)->default_value(boost::filesystem::current_path().string()), "Base directory")
        ("html_dir", boost::program_options::value<std::string>(&m_html_dir)->default_value(boost::filesystem::current_path().string()), "HTML directory");
    
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    boost::program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        exit(EXIT_FAILURE);
    }
}
