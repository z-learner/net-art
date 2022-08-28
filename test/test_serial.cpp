#include "protocol.h"
#include <boost/test/unit_test.hpp>
using boost::asio::ip::address;
using boost::asio::ip::udp;


BOOST_AUTO_TEST_SUITE(s_serial_test);

BOOST_AUTO_TEST_CASE(t_BoostReadData) {
  using namespace protocol; // NOLINT
  ProtocolConfig proto_config;
  proto_config.protocol_type_ = PROTOCOL_TYPE::SERIAL;
  proto_config.serial_baut_ = 115200;
  proto_config.serial_address_ = "/dev/ttyS1";
  auto protocol = protocol::GetProtocolByConfig(proto_config, 100);
  /*
    关于Lambda:
    - [ capture ] ( params ) opt -> ret { body; };
    - 其中 capture 是捕获列表，params 是参数表，opt 是函数选项，ret
    是返回值类型，body是函数体
  */
  std::string send_data("QQQQQQQQLLLLLLLL");

  protocol->SetDataRecvCallback([send_data](uint8_t* str, size_t size) -> void {
    std::string data((char*)str, size);
    std::cout << data << std::endl;
    BOOST_WARN_EQUAL(send_data.data(), data.data());
  });
  // auto protocol2 = protocol::GetProtocolByConfig(proto_config, 100);
  // protocol2->ProtocolSendRawData((uint8_t*)send_data.data(), send_data.size());
  sleep(3);
  protocol->ProtocolDestory();
}
BOOST_AUTO_TEST_SUITE_END()
