/**
 * @file serial_protocol.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _NET_PROTOCOL_SERIAL_PROTOCOL_H_
#define _NET_PROTOCOL_SERIAL_PROTOCOL_H_

#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include <iostream>
#include "protocol.h"


namespace protocol{

class SerialProtocol : public Protocol{
private:
  void _asyncReadSomeData();
  void _initSerialProtocol();
  void _recvDataCallback(
        const boost::system::error_code& error,
        size_t bytes_transferred
    ); 

public:
  SerialProtocol(const ProtocolConfig& protocol_config, const size_t buff_size)
  :Protocol(protocol_config), 
   buff_size_(buff_size),
   io_service_(boost::make_shared<boost::asio::io_service>()){
    recv_data_buffer_ = new uint8_t[buff_size_];
    assert(recv_data_buffer_ != nullptr);
    _initSerialProtocol();
  }
  ~SerialProtocol() {
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
  boost::shared_ptr<boost::asio::serial_port> serial_port_;
};


};



#endif