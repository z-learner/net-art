
#ifndef _NET_PROTOCOL_PROTOCOL_H_
#define _NET_PROTOCOL_PROTOCOL_H_


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <functional>
#include <iostream>
#include <string>
#include <memory>


namespace protocol {

enum class PROTOCOL_TYPE : int {
    SERIAL = 0,
    UDP_SERVER,
    TCP_SERVER
};


class ProtocolConfig {
public:
    PROTOCOL_TYPE protocol_type_;
    /**
     * @brief 
     * serial config
     */
    int serial_baut_;
    std::string serial_address_;

    /**
     * @brief 
     * udp config
     */
    int udp_server_port_;
    std::string udp_server_ip_;

    int tcp_server_port_;
    std::string tcp_server_ip_;

    ProtocolConfig& operator=(const ProtocolConfig& config) {
        protocol_type_ = config.protocol_type_;
        serial_baut_ = config.serial_baut_;
        serial_address_ = config.serial_address_;
        udp_server_port_ = config.udp_server_port_;
        udp_server_ip_ = config.udp_server_ip_;
        tcp_server_port_ = config.tcp_server_port_;
        tcp_server_ip_ =config.tcp_server_ip_;
        return *this;
    }

    ProtocolConfig() = default;
    ~ProtocolConfig() = default;
    

};


class Protocol {
protected:
    std::function<void(uint8_t*, size_t)> recv_data_callback_;
    ProtocolConfig protocol_config_;
public:
public:
    Protocol(const ProtocolConfig& protocol_config);
    ~Protocol() = default;
    virtual int ProtocolSendRawData(uint8_t* data, size_t size) = 0;
    virtual int ProtocolDestory() = 0;
    void SetDataRecvCallback(std::function<void(uint8_t*, size_t)> callback);
};

std::shared_ptr<Protocol> GetProtocolByConfig(
    const ProtocolConfig& protocol_config
);


}














#endif