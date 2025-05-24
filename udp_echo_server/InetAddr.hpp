#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>

class InetAddr
{
public:
    InetAddr(struct sockaddr_in& addr)
    :_addr(addr)
    {
        _ip=inet_nto(_addr.sin_addr);


    }
private:
    std::string _ip;
    uint16_t _port;
    struct sockaddr_in _addr;
};
