/*********************************************************************************
*     File Name           :     CommandHandler.cpp
*     Created By          :     QiangWei
*     Creation Date       :     [2015-04-15 13:35]
*     Last Modified       :     [2015-04-15 15:14]
*     Description         :      
**********************************************************************************/

#include "CommandHandler.h"
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using std::string;

namespace chl {

//去尾部空白字符
inline string rtrim(string ss) 
{ 
    int (*pf)(int) = std::iscntrl; 
    string::reverse_iterator p = find_if(ss.rbegin(), ss.rend(),
            not1(std::ptr_fun(pf))); 
    ss.erase(p.base(), ss.end()); 
    return ss;
} 

CommandHandler::CommandHandler(boost::asio::io_service& io, int port_number) :
    __io_service(io), __acceptor(io, boost::asio::ip::tcp::endpoint(
                boost::asio::ip::tcp::v4(), port_number))
{
    start_listen();
}

void CommandHandler::bind_command_with_func(const std::string& command,
        const boost::function<void()>& func)
{
    __comand_and_funcs.insert(std::make_pair(command, func));
}

void CommandHandler::start_listen()
{
    boost::asio::ip::tcp::socket* new_socket =
            new boost::asio::ip::tcp::socket(__io_service);
    __acceptor.async_accept(*new_socket,
            boost::bind(&CommandHandler::handle_accept, this, new_socket,
                boost::asio::placeholders::error));
}

void CommandHandler::handle_accept(boost::asio::ip::tcp::socket* socket,
        const boost::system::error_code& error)
{
    if (!error) {
        char* __data = new char[max_length];
        socket->async_read_some(boost::asio::buffer(__data, max_length),
                boost::bind(&CommandHandler::handle_read, this,
                    socket, __data, boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }
    else {
        delete socket;
    }
    this->start_listen();
}

void CommandHandler::handle_read(boost::asio::ip::tcp::socket* socket,
        char* __data, const boost::system::error_code& error,
        size_t bytes)
{
    if (!error) {
        string command(rtrim(__data));
        if (__comand_and_funcs.find(command) != __comand_and_funcs.end())
        {
            boost::thread t1(__comand_and_funcs[command]);
            t1.detach();
        }
        memset(__data, 0, sizeof (char) * max_length);
        socket->async_read_some(boost::asio::buffer(__data, max_length),
                boost::bind(&CommandHandler::handle_read, this,
                    socket, __data, boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }
    else {
        delete[] __data;
        delete socket;
    }
}

}
