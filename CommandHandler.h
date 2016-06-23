#ifndef  INCLUDE_COMMANDHANDLE_H
#define  INCLUDE_COMMANDHANDLE_H

#include <vector>
#include <boost/asio.hpp>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/algorithm/string.hpp>

namespace chl {

class CommandHandler
{
public:
    CommandHandler(boost::asio::io_service& io,
            std::string local_socket);
    virtual ~CommandHandler() {};

    void start_listen();          //开始监听端口
    void bind_command_with_func(const std::string& command,
            const boost::function<std::string()>& func);

protected:
    void handle_accept(boost::asio::local::stream_protocol::socket* socket,
            const boost::system::error_code& error);
    void handle_read(boost::asio::local::stream_protocol::socket* socket,
            char* __data,
            const boost::system::error_code& error, size_t bytes);

private:
    void process_command(const std::string& command,
            boost::asio::local::stream_protocol::socket* socket);

    boost::asio::io_service& __io_service;
    boost::asio::local::stream_protocol::acceptor __acceptor;
    boost::unordered_map<std::string, boost::function<std::string()> > __comand_and_funcs;
    static const int __port_number = 22341;     //默认绑定的端口号
    enum { max_length = 128 };

    CommandHandler(const CommandHandler& src);
    CommandHandler& operator=(const CommandHandler& chl);
};

}

#endif
