#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include "../comm/def.h"

FT_SERVER_NSP_START

class info {
public:
    info() = default;
    ~info() = default;

    void dump() const;
};

FT_SERVER_NSP_END

#endif // SERVER_INFO_H
