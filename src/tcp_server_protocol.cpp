#include "tcp_server_protocol.h"
#include <iostream>


namespace protocol {

void TcpServerProtocol::_initTcpServerProtocol() {
    acceptor_->async_accept(*(socket_.get()), 
                           boost::bind(&TcpServerProtocol::handle_accept, this, boost::asio::placeholders::error, socket_));
    boost::thread td(boost::bind(&boost::asio::io_service::run, io_service_.get()));
}


void TcpServerProtocol::handle_accept(const boost::system::error_code& err, boost::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    if (err) {
        std::cout << "handle_accept get a err, err = " << err.message() << std::endl;
        // acceptor_.async_accept(*socket, 
        //                    boost::bind(&TcpServerProtocol::handle_accept, this, boost::asio::placeholders::error, socket));
        return;
    } 
    std::cout << "get a connection" << std::endl;
    std::cout << "remote ip : " << socket->remote_endpoint().address() << std::endl;
    std::cout << "remote port : " << socket->remote_endpoint().port() << std::endl;
    // if (socket_.available()) {
    //     std::cout << "close outdate socket" << std::endl;
    // }
    // socket_ = std::move(*socket);
    socket->async_read_some(boost::asio::buffer(recv_data_buffer_, buff_size_),
                            boost::bind(&TcpServerProtocol::_recvDataCallback, this, boost::placeholders::_1, boost::placeholders::_2)
                );
    acceptor_->async_accept(*(socket.get()), 
                           boost::bind(&TcpServerProtocol::handle_accept, this, boost::asio::placeholders::error, socket));
}


void TcpServerProtocol::_recvDataCallback(
    const boost::system::error_code& error, size_t bytes_transferred) {
  if (bytes_transferred > 0) {
    #ifndef NDEBUG
      std::cout << "Get " << bytes_transferred << " Bytes from " << socket_->remote_endpoint().address() << ":" << socket_->remote_endpoint().port() << std::endl;
    #endif
    recv_data_callback_(recv_data_buffer_, bytes_transferred);
  }
  _asyncReadSomeData();
}


void TcpServerProtocol::_asyncReadSomeData() {
  socket_->async_read_some(
      boost::asio::buffer(recv_data_buffer_, buff_size_),
      boost::bind(&TcpServerProtocol::_recvDataCallback, this,
                  boost::placeholders::_1, boost::placeholders::_2));
}
int TcpServerProtocol::ProtocolSendRawData(uint8_t* data, size_t size) {
  return 0;
}

int TcpServerProtocol::ProtocolDestory() {
  io_service_->stop();
  return 0;
}

}