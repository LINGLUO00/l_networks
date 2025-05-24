#include "UdpServer.hpp"
#include <memory>
#include <iostream>
#include<string>
#include <cstdio>
#include <vector>
#include "Comm.hpp"
void Usage(std::string proc)//tell user how to use
{
    std::cout<<"Usage:\n\t"<<proc<<"local_port\n"<<std::endl;
}
std::vector<std::string> black_words={
    "rm",
    "unlink",
    "mv",
    "chmod",
    "exit",
    "reboot",
    "halt",
    "shutdown",
    "top",
    "kill",
    "dd",
    "vim",
    "vi",
    "nano",
    "man"
};
std::string OnMessageDefault(std::string request)
{
    return request+"[haha,get you]";
}
bool SafeCheck(std::string command)
{
    for(auto &k:black_words)
    {
        std::size_t pos=command.find(k);
        if(pos != std::string::npos)
        {
            return false;
        }
    }
    return true;
}
std::string ExcuteCommand(std::string command)
{
    if(!SafeCheck(command)) return "forbidden";

    std::cout<<"get a message: "<<command<<std::endl;
    FILE* fp=popen(command.c_str(),"r");
    if(fp==nullptr)
    {
        return "popen error";

    }
    std::string response;
    char buffer[1024];
    while(true)
    {
        char* s=fgets(buffer,sizeof(buffer) ,fp);
        if(s==nullptr) break;
        else response+=buffer;
    }
    pclose(fp);
    return response.empty()? "success" : response;
}
int main(int argc,char*argv[])
{
    if(argc!=2)
    {
        Usage(argv[0]);
        return Usage_Err;
    }
    uint16_t port=std::stoi(argv[1]);
    std::unique_ptr<UdpServer> usvr = std::make_unique<UdpServer>(port);
    usvr->init();
    usvr->Start();
    return 0;
}
