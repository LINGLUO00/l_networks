#include<iostream>
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cerrno>
#include <memory.h>
#include <arpa/inet.h>
#include <unistd.h>

void Usage(const std::string& process)
{
 std::cout<<"Usage: "<<process<<" server_ip server_port"<<std::endl;   
}
int main(int argc,char*argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        return 1;
    }
    std::string serverip=argv[1];
    uint16_t serverport=std::stoi(argv[2]);

    //create socket
    int sock=socket(AF_INET,SOCK_DGRAM,0);
    if(sock<0)
    {
        std::cerr<<"socket error"<<std::endl;
        return 2;
    }
    std::cout<<"create socket success: "<<sock<<std::endl;

    //bind socket
    //clienrt socket do not need bind fixed port number
    //because the system will assign a port number for client randomly
    struct sockaddr_in server;
    memset(&server,0,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_port=htons(serverport);//convert host byte order to network byte order
    server.sin_addr.s_addr= inet_addr(serverip.c_str());//convert ip address to binary

    while(true)
    {
        //the message to send
        std::string msg;
        std::cout<<"Please Enter Message: ";
        std::getline(std::cin,msg);
        //send message
        ssize_t n= sendto(sock,msg.c_str(),msg.size(),0,(struct sockaddr*)&server,sizeof(server));
        if(n>0)
        {
            char buffer[1024];
            //receive message
            struct sockaddr_in tmp;
            socklen_t len = sizeof(tmp);
            ssize_t m = recvfrom(sock,buffer,sizeof(buffer)-1,0, (struct sockaddr*)&tmp,&len);
            if(m>0)
            {
                buffer[m]='\0';
                std::cout<<"server echo# "<<buffer<<std::endl;
            }
            else
                break;

        }
        else break;
    }
    close(sock);
    return 0;



}