#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/udp.hpp>
#include <tower/net/message.hpp>

namespace tower::net {
using namespace boost::asio::ip;

class ConnectionRudp final {
    udp::socket _socket;
    boost::asio::strand<boost::asio::any_io_executor> _strand;

    std::atomic<bool> _is_disconnecting{false};
    std::atomic<bool> _is_disconnected{false};

public:
    ConnectionRudp(udp::socket&& socket, boost::asio::strand<boost::asio::any_io_executor>&& strand);

    void open(const boost::asio::any_io_executor& executor);
    void disconnect();

    void send(std::shared_ptr<OutMessage> message, bool reliable, bool ordered);
    boost::asio::awaitable<std::unique_ptr<InMessage>> receive();

    static std::shared_ptr<OutMessage> prepare_out_message(size_t payload_size);

    bool is_connected() const { return !_is_disconnecting && !_is_disconnected; }
};
}