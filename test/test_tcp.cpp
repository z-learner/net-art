#include "protocol.h"
#include <iostream>
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(s_test_tcp)


BOOST_AUTO_TEST_CASE(t_TcpAcceptor) {
    using namespace protocol;
    ProtocolConfig proto_config;
    proto_config.protocol_type_ = PROTOCOL_TYPE::TCP_SERVER;
    proto_config.tcp_server_ip_ = "127.0.0.1"; 
    proto_config.tcp_server_port_ = 5555;
    auto protocol = GetProtocolByConfig(proto_config, 1000);
    protocol->SetDataRecvCallback([](uint8_t* str, size_t size) -> void {
        std::string data((char*)str, size);
        std::cout << data << std::endl;
    });
    sleep(10);
}




BOOST_AUTO_TEST_SUITE_END()

