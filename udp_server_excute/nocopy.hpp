#include <iostream>
#include <string>
#include<cstdio>
#include "UdpServer.hpp"
#include "Log.hpp"
class nocopy
{
public:
    nocopy()
    {}
    nocopy(const nocopy&)=delete;
    const nocopy& operator=(const nocopy&)=delete;
    ~nocopy(){}
};