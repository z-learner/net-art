/**
 * @file serial_protocol.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "serial_protocol.h"

namespace protocol{

void SerialProtocol::_initSerialProtocol() {
  boost::system::error_code ec;
  std::cout << "open with " << protocol_config_.serial_address_ << std::endl;
  serial_port_->open(protocol_config_.serial_address_, ec);
  assert(!ec);
  serial_port_->set_option(
      boost::asio::serial_port_base::baud_rate(protocol_config_.serial_baut_));
  serial_port_->set_option(boost::asio::serial_port_base::character_size(8));
  serial_port_->set_option(boost::asio::serial_port_base::stop_bits(
      boost::asio::serial_port_base::stop_bits::one));
  serial_port_->set_option(boost::asio::serial_port_base::parity(
      boost::asio::serial_port_base::parity::none));
  serial_port_->set_option(boost::asio::serial_port_base::flow_control(
      boost::asio::serial_port_base::flow_control::none));
  _asyncReadSomeData();
  boost::thread td(boost::bind(&boost::asio::io_service::run, io_service_.get())); 
  std::cout << "open done " << std::endl;
}


void SerialProtocol::_recvDataCallback(const boost::system::error_code& error,
                                       size_t bytes_transferred) {
  if (bytes_transferred > 0) {
    #ifndef NDEBUG
      std::cout << "Get " << bytes_transferred << " Bytes" << std::endl;
    #endif
    recv_data_callback_(recv_data_buffer_, bytes_transferred);
  }
  _asyncReadSomeData();
}


void SerialProtocol::_asyncReadSomeData() {
  serial_port_->async_read_some(
      boost::asio::buffer(recv_data_buffer_, buff_size_),
      boost::bind(&SerialProtocol::_recvDataCallback, this,
                  boost::placeholders::_1, boost::placeholders::_2));
}


int SerialProtocol::ProtocolSendRawData(uint8_t* data, size_t size) {
  if (serial_port_->is_open()) {
    std::cout << "send" << size << std::endl;
    int send_len = serial_port_->write_some(boost::asio::buffer(data, size));
    return send_len;
  }
  return 0;
}

int SerialProtocol::ProtocolDestory() {
  io_service_->stop();
  return 0;
}


};