#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/udp.hpp>
#include <tower/core/numbers.hpp>

namespace tower::net {
using namespace boost::asio::ip;

class ConnectionRudp {
    udp::socket _socket;
    boost::asio::strand<boost::asio::any_io_executor> _strand;

public:
    ConnectionRudp(udp::socket&& socket, boost::asio::strand<boost::asio::any_io_executor>&& strand);

    void send(std::vector<uint8_t>&& buffer);
    boost::asio::awaitable<std::vector<byte>> receive();
};
}