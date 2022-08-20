

#ifndef _NET_PROTOCOL_TCP_SERVER_PROTOCOL_H_
#define _NET_PROTOCOL_TCP_SERVER_PROTOCOL_H_


#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include "protocol.h"

namespace protocol{

class TcpServerProtocol : public Protocol {
private:
    void _asyncReadSomeData();
    void _initTcpServerProtocol();
    void _recvDataCallback(
        const boost::system::error_code& error,
        size_t bytes_transferred
    );


public:
    TcpServerProtocol(const ProtocolConfig& protocol_config, const size_t buff_size)
    : Protocol(protocol_config), buff_size_(buff_size), io_service_(boost::make_shared<boost::asio::io_service>()), socket_(boost::make_shared<boost::asio::ip::tcp::socket>(*(io_service_.get()))), 
    acceptor_(boost::make_shared<boost::asio::ip::tcp::acceptor>(*(io_service_.get()), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), protocol_config.tcp_server_port_))) {
        recv_data_buffer_ = new uint8_t[buff_size_];
        assert(recv_data_buffer_ != nullptr);
        _initTcpServerProtocol();
    }

    ~TcpServerProtocol() {
        if (recv_data_buffer_ != nullptr) {
            delete[] recv_data_buffer_;
        }
    }
    int ProtocolSendRawData(uint8_t* data, size_t size) override;
    int ProtocolDestory() override;
    void handle_accept(const boost::system::error_code& err, boost::shared_ptr<boost::asio::ip::tcp::socket> socket);

private:
    uint8_t* recv_data_buffer_;
    size_t buff_size_;
    boost::shared_ptr<boost::asio::io_service> io_service_;
    boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    // boost::asio::ip::tcp::socket socketTmp_;
    boost::asio::ip::tcp::endpoint remote_endpoint_;
    boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    // std::vector<boost::asio::ip::tcp::socket> sockets_;
};




}


#endif