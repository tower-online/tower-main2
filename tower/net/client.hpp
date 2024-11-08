#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <tower/net/message.hpp>

namespace tower::net {
using namespace boost::asio::ip;

class Client final {
    tcp::socket _stream;
    udp::socket _datagram;
    boost::asio::strand<boost::asio::any_io_executor> _stream_strand;

public:
    Client(tcp::socket&& stream, const boost::asio::any_io_executor& executor);

    void open();
    void close();

    void send_stream_message(std::shared_ptr<OutMessage> message);
    void send_datagram_message(std::shared_ptr<OutMessage> message);

private:
    boost::asio::awaitable<InMessage> receive_stream_message();
    boost::asio::awaitable<InMessage> receive_datagram_message();
};
}