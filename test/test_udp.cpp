
#include "protocol.h"
#include "gtest/gtest.h"

using boost::asio::ip::address;
using boost::asio::ip::udp;

void Sender(std::string in, std::string ip, int port) {
  boost::asio::io_service io_service;
  udp::socket socket(io_service);
  udp::endpoint remote_endpoint = udp::endpoint(address::from_string(ip), port);
  socket.open(udp::v4());

  boost::system::error_code err;
  auto sent = socket.send_to(boost::asio::buffer(in), remote_endpoint, 0, err);
  (void)sent;
  std::cout << "Send " << sent << "Bytes to " << ip << ":" << port << std::endl;
  socket.close();
}

TEST(TestUdpServerProtocol, BoostSendData) {
  using namespace protocol;  // NOLINT
  Sender("\x5A\x01\x02\x51\x02\x50\x01\x5A", "192.168.0.106", 3474);
}

TEST(TestUdpServerProtocol, ReadData) {
  using namespace protocol;  // NOLINT
  ProtocolConfig proto_config;
  proto_config.protocol_type_ = PROTOCOL_TYPE::UDP_SERVER;
  proto_config.udp_server_ip_ = "0.0.0.0";
  proto_config.udp_server_port_ = 3474;

  auto protocol = protocol::GetProtocolByConfig(proto_config);

  std::string send_data("\x5A\x01\x02\x51\x02\x50\x01\x5A");
  protocol->SetDataRecvCallback([send_data](uint8_t* str, size_t size) -> void {
        std::string data((char*)str, size);
        std::cout << data << std::endl;
        EXPECT_STREQ(send_data.data(), data.data());
  });

  Sender("\x5A\x01\x02\x51\x02\x50\x01\x5A", proto_config.udp_server_ip_,
         proto_config.udp_server_port_);
  /*
    关于Lambda:
    - [ capture ] ( params ) opt -> ret { body; };
    - 其中 capture 是捕获列表，params 是参数表，opt 是函数选项，ret
    是返回值类型，body是函数体
  */
  sleep(2);
  protocol->ProtocolDestory();
}

TEST(TestUdpServerProtocol, ReadDataRemote) {
  using namespace protocol;  // NOLINT
  ProtocolConfig proto_config;
  proto_config.protocol_type_ = PROTOCOL_TYPE::UDP_SERVER;
  proto_config.udp_server_ip_ = "0.0.0.0";
  proto_config.udp_server_port_ = 3474;

  auto protocol = protocol::GetProtocolByConfig(proto_config);

  /*
    关于Lambda:
    - [ capture ] ( params ) opt -> ret { body; };
    - 其中 capture 是捕获列表，params 是参数表，opt 是函数选项，ret
    是返回值类型，body是函数体
  */
  protocol->SetDataRecvCallback([](uint8_t* str, size_t size) -> void {
    // print_frame_to_hex("recvframe", data.data(), (uint16_t)data.size());
    std::cout << std::string((char*)str, size) << std::endl;
  });

  sleep(3);
  protocol->ProtocolDestory();
}
