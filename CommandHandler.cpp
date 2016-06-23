/***************************************************************************
 * 
 * Copyright (c) 2016 Baidu.com, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 /**
 * @file CommandHandler.cpp
 * @author qiangwei(qiangwei@baidu.com)
 * @date 2016/06/23 15:16:17
 * @version $Revision$ 
 * @brief 
 *  
 **/

#include "CommandHandler.h"

namespace chl {

CommandHandler::CommandHandler(boost::asio::io_service& io, std::string local_socket) :
    __io_service(io), __acceptor(io, boost::asio::local::stream_protocol::endpoint(local_socket))
{
    start_listen();
}

void CommandHandler::bind_command_with_func(const std::string& command,
        const boost::function<std::string()>& func)
{
    __comand_and_funcs.insert(std::make_pair(command, func));
}

void CommandHandler::start_listen()
{
    boost::asio::local::stream_protocol::socket* new_socket =
            new boost::asio::local::stream_protocol::socket(__io_service);
    __acceptor.async_accept(*new_socket,
            boost::bind(&CommandHandler::handle_accept, this, new_socket,
                boost::asio::placeholders::error));
}

void CommandHandler::handle_accept(boost::asio::local::stream_protocol::socket* socket,
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

void CommandHandler::process_command(const std::string& command,
        boost::asio::local::stream_protocol::socket* socket) {
    std::string ret(__comand_and_funcs[command]());
    boost::system::error_code ignored_error;
    socket->write_some(boost::asio::buffer(ret), ignored_error);
    if (socket != NULL) {
        delete socket;
        socket = NULL;
    }
}

void CommandHandler::handle_read(boost::asio::local::stream_protocol::socket* socket,
        char* __data, const boost::system::error_code& error,
        size_t bytes)
{
    if (!error) {
        std::string command(boost::trim_copy(std::string(__data)));
        if (__data != NULL) {
            delete[] __data;
            __data = NULL;
        }
        if (__comand_and_funcs.find(command) != __comand_and_funcs.end())
        {
            boost::thread t1(boost::bind(&CommandHandler::process_command, this, command, socket));
            t1.detach();
        }
        else {
            boost::system::error_code ignored_error;
            socket->write_some(boost::asio::buffer(std::string("error: this command [")
                        + command + "] not found!"), ignored_error);
            if (socket != NULL) {
                delete socket;
                socket = NULL;
            }
        }
    }
    else {
        if (__data != NULL) {
            delete[] __data;
            __data = NULL;
        }
        if (socket != NULL) {
            delete socket;
            socket = NULL;
        }
    }
}

}

/* vim: set ts=4 sw=4 sts=4 tw=100 */
