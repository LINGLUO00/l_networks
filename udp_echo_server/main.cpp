#include "UdpServer.hpp"
#include <memory>
#include <iostream>
#include<string>
#include <cstdio>
#include "Comm.hpp"
void Usage(std::string proc)//tell user how to use
{
    std::cout<<"Usage:\n\t"<<proc<<"local_ip local_port\n"<<std::endl;
}
int main(int argc,char*argv[])
{
    if(argc!=3)
    {
        Usage(argv[0]);
        return Usage_Err;
    }
    std::string ip=argv[1];
    uint16_t port=std::stoi(argv[2]);
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>("0.0.0.0");
    usvr->init();
    usvr->Start();
    return 0;
}
