/**
 * @file udp_server_protocol.hpp
 * @author peng.zhang
 * @brief 
 * @version 0.1
 * @date 2022-08-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _NET_PROTOCOL_UDP_SERVER_PROTOCOL_H_
#define _NET_PROTOCOL_UDP_SERVER_PROTOCOL_H_

#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>
#include <assert.h>
#include <boost/shared_ptr.hpp>
#include "protocol.h"

namespace protocol {

class UdpServerProtocol : public Protocol  {
private:
    void _asyncReadSomeData();
    void _initUdpServerProtocol();
    void _recvDataCallback(
        const boost::system::error_code& error,
        size_t bytes_transferred
    );

public:
    UdpServerProtocol(const ProtocolConfig& protocol_config, const size_t buff_size)
        : Protocol(protocol_config), buff_size_(buff_size),  io_service_(boost::make_shared<boost::asio::io_service>()), socket_(boost::make_shared<boost::asio::ip::udp::socket>(*(io_service_.get()))) {
            recv_data_buffer_ = new uint8_t[buff_size_];
            assert(recv_data_buffer_ != nullptr);
            _initUdpServerProtocol();
        }
    ~UdpServerProtocol() {
        if (recv_data_buffer_ != nullptr) {
            delete[] recv_data_buffer_;
        }
    }

    int ProtocolSendRawData(uint8_t* data, size_t size) override;
    int ProtocolDestory() override;

private:
    uint8_t* recv_data_buffer_;
    size_t buff_size_;
    boost::shared_ptr<boost::asio::io_service> io_service_;
    boost::shared_ptr<boost::asio::ip::udp::socket> socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
};





};






#endif