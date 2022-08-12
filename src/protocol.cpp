#include "protocol.h"
#include "udp_server_protocol.h"

#define BUFF_SIZE 1024

namespace protocol {

    Protocol::Protocol(const ProtocolConfig& protocol_config) {
        protocol_config_ = protocol_config;
    }


    void Protocol::SetDataRecvCallback(
        std::function<void(uint8_t*, size_t)> callback) {
            recv_data_callback_ = callback;
        }






std::shared_ptr<Protocol> GetProtocolByConfig(
    const ProtocolConfig& protocol_config) {
  switch (protocol_config.protocol_type_) {
    // case PROTOCOL_TYPE::SERIAL:
    //   return std::make_shared<SerialProtocol>(protocol_config);
    //   break;
    case PROTOCOL_TYPE::UDP_SERVER:
      return std::make_shared<UdpServerProtocol>(protocol_config, BUFF_SIZE);
      break;
    // default:
    //   break;
  }
  return std::make_shared<UdpServerProtocol>(protocol_config, BUFF_SIZE);
}
}  // namespace protocol