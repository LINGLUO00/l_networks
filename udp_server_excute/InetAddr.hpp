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
        _port=ntohs(addr.sin_port);
        _ip=inet_ntoa(addr.sin_addr);
    }
    std::string getIP()
    {
        return _ip;
    }
    uint16_t getPort()
    {
        return _port;
    }
    std::string PrintDEbug()
    {
        std::string info=_ip;
        info += ":";
        info +=std::to_string(_port);
        return info;
    }
    ~InetAddr()
    {}
private:
    std::string _ip;
    uint16_t _port;
    struct sockaddr_in _addr;
};
