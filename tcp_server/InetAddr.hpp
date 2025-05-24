#pragma once
#include<iostream>
#include<string>
#include<netinet/in.h> 
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

class InetAddr
{
public:
    InetAddr(struct sockaddr_in addr):_addr(addr)
    {
        char ipbuffer[64];
        //inet_ntop支持ipv4和ipv6，inet_ntoa(ASCII)只支持ipv4
        inet_ntop(AF_INET,&_addr.sin_addr,ipbuffer,sizeof(ipbuffer));
        _ip=ipbuffer;
        //_ip=inet_ntoa(_addr.sin_addr);//返回的char*,指向的是函数维护的静态缓冲区，该缓冲区的生命周期与程序一致，但是下一次调用该函数时会被覆盖，故不是线程安全的
        _port=ntohs(_addr.sin_port);
    }
    ~InetAddr(){}
    std::string GetIP() {return _ip;}
    uint16_t GetPort() {return _port;}
    const struct sockaddr_in& GetAddr() {return _addr;}

    bool operator==(const InetAddr& addr)
    {
        return this->_ip==addr._ip && this->_port==addr._port;
    }

    std::string PrintDebug()
    {
        std::string info = _ip;
        info +=": ";
        info+=std::to_string(_port);
        return info;
    }

private:
    std::string _ip;
    uint16_t _port;
    struct sockaddr_in _addr;

};