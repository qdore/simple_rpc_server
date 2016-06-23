#ifndef  INCLUDE_COMMANDHANDLE_H
#define  INCLUDE_COMMANDHANDLE_H

#include <vector>
#include <boost/asio.hpp>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace chl {

class CommandHandler
{
public:
    CommandHandler(boost::asio::io_service& io,
            int port_number = __port_number);
    virtual ~CommandHandler() {};

    void start_listen();          //开始监听端口
    void bind_command_with_func(const std::string& command,
            const boost::function<std::string()>& func);

protected:
    void handle_accept(boost::asio::ip::tcp::socket* socket,
            const boost::system::error_code& error);
    void handle_read(boost::asio::ip::tcp::socket* socket,
            char* __data,
            const boost::system::error_code& error, size_t bytes);

private:
    void process_command(const std::string& command,
            boost::asio::ip::tcp::socket* socket);

    boost::asio::io_service& __io_service;
    boost::asio::ip::tcp::acceptor __acceptor;
    boost::unordered_map<std::string, boost::function<std::string()> > __comand_and_funcs;
    static const int __port_number = 22341;     //默认绑定的端口号
    enum { max_length = 128 };

    CommandHandler(const CommandHandler& src);
    CommandHandler& operator=(const CommandHandler& chl);
};

}
#endif
