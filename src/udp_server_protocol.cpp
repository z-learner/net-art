#include "udp_server_protocol.h"
#include <iostream>
namespace protocol {

void UdpServerProtocol::_initUdpServerProtocol() {
  using boost::asio::ip::address;
  using boost::asio::ip::udp;
  boost::system::error_code ec;
  socket_.open(udp::v4());
  socket_.bind(
      udp::endpoint(address::from_string(protocol_config_.udp_server_ip_),
                    protocol_config_.udp_server_port_));
  _asyncReadSomeData();
  boost::thread td(boost::bind(&boost::asio::io_service::run, &io_service_));
} 

void UdpServerProtocol::_recvDataCallback(
    const boost::system::error_code& error, size_t bytes_transferred) {
  if (bytes_transferred > 0) {
    #ifndef NDEBUG
      std::cout << "Get " << bytes_transferred << "Bytes from " << protocol_config_.udp_server_ip_ << ":" << protocol_config_.udp_server_port_ << std::endl;
    #endif
    recv_data_callback_(recv_data_buffer_, bytes_transferred);
  }
  _asyncReadSomeData();
}


void UdpServerProtocol::_asyncReadSomeData() {
  socket_.async_receive_from(
      boost::asio::buffer(recv_data_buffer_, buff_size_), remote_endpoint_,
      boost::bind(&UdpServerProtocol::_recvDataCallback, this,
                  boost::placeholders::_1, boost::placeholders::_2));
}


int UdpServerProtocol::ProtocolSendRawData(uint8_t* data, size_t size) {
  if (remote_endpoint_.port() != 0) {
    int send_len = socket_.send_to(boost::asio::buffer(data, size), remote_endpoint_);
    #ifndef NDEBUG
      std::cout << "Sent " << send_len << "Bytes to " << protocol_config_.udp_server_ip_ << ":" << protocol_config_.udp_server_port_ << std::endl;
    #endif
    return send_len;
  }
  return -1;
}

int UdpServerProtocol::ProtocolDestory() {
  io_service_.stop();
  return 0;
}

}