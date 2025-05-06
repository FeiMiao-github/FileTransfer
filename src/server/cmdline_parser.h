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

   private:
    short m_port;
    std::string m_base_dir;
};

FT_SERVER_NSP_END

#endif // SERVER_CMDLINE_PARSER_H