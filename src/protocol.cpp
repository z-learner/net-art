#include "protocol.h"
#include "udp_server_protocol.h"
#include "tcp_server_protocol.h"
#include "serial_protocol.h"
#define BUFF_SIZE 1024

namespace protocol {

    Protocol::Protocol(const ProtocolConfig& protocol_config) {
        protocol_config_ = protocol_config;
    }


    void Protocol::SetDataRecvCallback(
        std::function<void(uint8_t*, size_t)> callback) {
            recv_data_callback_ = callback;
        }






boost::shared_ptr<Protocol> GetProtocolByConfig(
    const ProtocolConfig& protocol_config, const size_t buff_size) {
  switch (protocol_config.protocol_type_) {
    case PROTOCOL_TYPE::TCP_SERVER:
      return boost::make_shared<TcpServerProtocol>(protocol_config, buff_size);
      break;
    case PROTOCOL_TYPE::UDP_SERVER:
      return boost::make_shared<UdpServerProtocol>(protocol_config, buff_size);
      break;
    case PROTOCOL_TYPE::SERIAL:
      return boost::make_shared<SerialProtocol>(protocol_config, buff_size);
      break;
  }
  return boost::make_shared<UdpServerProtocol>(protocol_config, buff_size);
}
}  // namespace protocol