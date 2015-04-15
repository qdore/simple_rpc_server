/*********************************************************************************
*     File Name           :     main.cpp
*     Created By          :     QiangWei
*     Creation Date       :     [2015-04-15 13:50]
*     Last Modified       :     [2015-04-15 15:03]
*     Description         :      
**********************************************************************************/

#include "CommandHandler.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

void print()
{
    std::cout << 123 << std::endl;
}

void print2()
{
    std::cout << 321 << std::endl;
}

void print3()
{
    std::cout << "qwqw" << std::endl;
}
int main()
{
    boost::asio::io_service io;
    CommandHandler ch(io);
    ch.bind_command_with_func("haha", boost::bind(&print));
    ch.bind_command_with_func("ahah", boost::bind(&print2));
    ch.bind_command_with_func("qw", boost::bind(&print3));
    io.run();
}
