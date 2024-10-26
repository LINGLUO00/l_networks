#pragma once
#include <iostream>
#include <string>
#include<stdint.h>
#include "nocopy.hpp"
#include "Log.hpp"

static const uint16_t defaultport = 8888;

class UdpServer : public nocopy
{
public:
    UdpServer(const std::string &ip , uint16_t port = defaultport)
        : _ip(ip), _port(port)
    {
    }
    void init()
    {
    }
    void Start()
    {
    }
    ~UdpServer()
    {}

private:
    std::string _ip;
    uint16_t _port;
};