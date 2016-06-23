/*********************************************************************************
*     File Name           :     main.cpp
*     Created By          :     QiangWei
*     Creation Date       :     [2015-04-15 13:50]
*     Last Modified       :     [2015-04-15 15:14]
*     Description         :      
**********************************************************************************/

#include "CommandHandler.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

std::string print()
{
    return "haha";
}

std::string print2()
{
    return "ahah";
}

std::string print3()
{
    return "qw";
}
int main()
{
    boost::asio::io_service io;
    chl::CommandHandler ch(io, 33334);
    ch.bind_command_with_func("haha", boost::bind(&print));
    ch.bind_command_with_func("ahah", boost::bind(&print2));
    ch.bind_command_with_func("qw", boost::bind(&print3));
    io.run();
}
