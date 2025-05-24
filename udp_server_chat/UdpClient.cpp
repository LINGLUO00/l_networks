#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cerrno>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mythread.h"
#include "InetAddr.hpp"
void Usage(const std::string &process)
{
    std::cout << "Usage: " << process << " server_ip server_port" << std::endl;
}
class ThreadData
{
public:
    ThreadData(int sock, struct sockaddr_in &server): _sockfd(sock), _serveraddr(server)
    {
    }
    ~ThreadData()
    {
    }

public:
    int _sockfd;
    InetAddr _serveraddr;
};
void ReceverRoutine(ThreadData &td)
{
    char buffer[4096];
    while(true)
    {
        struct sockaddr_in temp;
        socklen_t len = sizeof(temp);
        ssize_t n = recvfrom(td._sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&temp, &len);
        if(n>0)
        {
            buffer[n]='\0';
            //std::cerr<<buffer<<std::endl;//方便调试
        }
        else break;
    }
}
//该线程只负责发消息
void SenderRoutine(ThreadData &td)
{
    while(true)
    {
        std::string inbuffer;
        std::cout<<"Please Enter:"<<std::endl;
        std::getline(std::cin,inbuffer);

        auto server=td._serveraddr.getAddr();
        ssize_t n= sendto(td._sockfd,inbuffer.c_str(),inbuffer.size(),0,(struct sockaddr*)&server,sizeof(server));
        if(n<=0)
        {
            std::cout<<"send error"<<std::endl;
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    std::string serverip = argv[1];
    uint16_t serverport = std::stoi(argv[2]);

    // create socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        std::cerr << "socket error" << std::endl;
        return 2;
    }
    std::cout << "create socket success: " << sock << std::endl;

    // bind socket
    // clienrt socket do not need bind fixed port number
    // because the system will assign a port number for client randomly
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(serverport);                  // convert host byte order to network byte order
    server.sin_addr.s_addr = inet_addr(serverip.c_str()); // convert ip address to binary

    ThreadData td(sock,server);
    Thread<ThreadData> recver("recver", ReceverRoutine, td);
    Thread<ThreadData> sender("sender", SenderRoutine, td);
    recver.Start();
    sender.Start();
    recver.Join();
    sender.Join();
    
    close(sock);
    return 0;
}