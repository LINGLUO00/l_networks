#pragma once
#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<cstring>
#include<functional>
#include<cstdlib>
#include<cerrno>
#include"nocopy.hpp"
#include"InetAddr.hpp"
#include"Log.hpp"
const static int default_backlog=5;
using task_t =std::function<void()>;
using callback_t = std::function<void(int,InetAddr&)>;

class TcpServer: public nocopy
{
public:
    TcpServer(uint16_t port):_port(port),_isrunning(false)
    {}
    ~TcpServer(){}

    void Init()
    {
        
        _listensock=socket(AF_INET,SOCK_STREAM,0);
        if(_listensock<0)
        {
            log.LogMessage(Fatal,"create socket error, error code:%d, error message:%s", errno,strerror(errno));
            exit(Fatal);
        }
        log.LogMessage(Info,"create socket sucessfully, sockfd:%d",_listensock);

        int opt=1;
        setsockopt(_listensock,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&opt,sizeof(opt));
        
    }
private:
    uint16_t _port;
    int _listensock;
    bool _isrunning;
    std::unordered_map<std::string,callback_t> funcs;
};

