#ifndef SERVER_CMDLINE_PARSER_H
#define SERVER_CMDLINE_PARSER_H

#include <string>
#include "../comm/def.h"

FT_SERVER_NSP_START

class cmdline_parser {
   public:
    cmdline_parser() = default;
    ~cmdline_parser() = default;

    void parse(int argc, char** argv);
    short get_port() const { return m_port; }
    const std::string& get_base_dir() const { return m_base_dir; }
    const std::string& get_html_dir() const { return m_html_dir; }

   private:
    short m_port;
    std::string m_base_dir;
    std::string m_html_dir;
};

FT_SERVER_NSP_END

#endif // SERVER_CMDLINE_PARSER_H