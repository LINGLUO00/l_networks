#pragma once
#include <iostream>
#include <string>
#include<strings.h>
#include<cstring>
#include <stdint.h>
#include <sys/types.h>
#include<netinet/in.h>
#include<sys/fcntl.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <sys/socket.h>
#include <cerrno>
#include<functional>
#include<pthread.h>
#include "nocopy.hpp"
#include "Log.hpp"
#include"Comm.hpp"
#include"InetAddr.hpp"
#include "ThreadPool.hpp"
#include "LockGuard.h"
#define defaultsize 1024
const static uint16_t defaultport = 8888;
const static int defaultfd=-1;

using task_t = std::function<void()>;
class UdpServer : public nocopy
{
public:
    UdpServer(uint16_t port = defaultport)
        :  _port(port),_sockfd(defaultfd)
    {
        pthread_mutex_init(&_user_mutex,nullptr);
    }
    void init()
    {
        //creat socket
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0); // DGRAM 是 “datagram”（数据报）的缩写。它表示一种面向消息的通信方式，常用于UDP协议。数据报独立发送和接收，不保证传输顺序和可靠性
        if (_sockfd < 0)
        {
            lg.LogMessage(Fatal, "Socket error, %d: %s\n", errno, strerror(errno));
            exit(Socket_Err);
        }
        lg.LogMessage(Info,"Socket sucess,sockfd: %d\n",_sockfd);
        //bind
        struct sockaddr_in local;
        bzero(&local,sizeof(local));
        local.sin_family=AF_INET;
        local.sin_port=htons(_port);//use the htons function to convert host byte order to network byte order
        // local.sin_addr.s_addr=inet_addr(_ip.c_str());
        local.sin_addr.s_addr=INADDR_ANY; //INADDR_ANY表示本机的任意IP地址
        //将上面两部分（网络信息和文件信息）进行关联
        int n = ::bind(_sockfd,(struct sockaddr*)&local,sizeof(local));
        if(n!=0)
        {
            lg.LogMessage(Fatal,"bind fail %d: %s\n",errno,strerror(errno));
            exit(Bind_Err);
        }

        ThreadPool<task_t>::GetInstance()->Start();
    }
    void AddOnlineUser(InetAddr addr)
    {
        LockGuard lockguard(&_user_mutex);
        for(auto &user : _online_user)
        {
            if(addr == user) return;
        }
        _online_user.push_back(addr);
        lg.LogMessage(Debug, "%s:%d is add to onlineuser list...\n", addr.getIP().c_str(), addr.getPort());
    }
    void Route(int sock, const std::string &message)
    {
        LockGuard lockguard(&_user_mutex);
        for(auto &user: _online_user)
        {
            sendto(sock,message.c_str(),message.size(),0,(struct sockaddr*)&user.getAddr(),sizeof(user.getAddr()));        
            lg.LogMessage(Debug, "server send message to %s:%d, message: %s\n", user.getIP().c_str(), user.getPort(), message.c_str());
        }
    }
    void Start()
    {
        char buffer[defaultsize];

        //server never exit
        for(;;)
        {
            struct sockaddr_in peer;
            socklen_t len =sizeof(peer);
            ssize_t n = recvfrom(_sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&peer,&len);
            if(n>0)
            {
                InetAddr addr(peer);
                AddOnlineUser(addr);
                buffer[n]=0;
                std::string message="[";
                message+=addr.getIP();
                message+=":";
                message+=std::to_string(addr.getPort());
                message+="]# ";
                message+=buffer;
                task_t task=std::bind(&UdpServer::Route,this,_sockfd,message);
                ThreadPool<task_t>::GetInstance()->Push(task);
            }
        }
    }
    ~UdpServer()
    {
        pthread_mutex_destroy(&_user_mutex);
    }

private:
    uint16_t _port;
    int _sockfd;
    std::vector<InetAddr> _online_user;
    pthread_mutex_t _user_mutex;
};