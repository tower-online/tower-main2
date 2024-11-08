#include <boost/asio/as_tuple.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <tower/net/connection_rudp.hpp>
#include <tower/net/details/rudp.hpp>

namespace tower::net {
using namespace tower::net::details;

ConnectionRudp::ConnectionRudp(udp::socket&& socket, boost::asio::strand<boost::asio::any_io_executor>&& strand)
    : _socket{std::move(socket)}, _strand{std::move(strand)} {}

void ConnectionRudp::open(const boost::asio::any_io_executor& executor) {
    co_spawn(executor, [this]->boost::asio::awaitable<void> {
        while (is_connected()) {
            auto in_message {co_await receive()};
        }
    }, boost::asio::detached);
}

void ConnectionRudp::disconnect() {
    if (_is_disconnecting.exchange(true)) return;
    if (_is_disconnected) {
        _is_disconnecting = false;
        return;
    }

    //TODO: 1. Send RST packet

    //TODO: 2. Wait for response?

    _is_disconnecting = false;
    _is_disconnected = true;
}

void ConnectionRudp::send(std::shared_ptr<OutMessage> message, const bool reliable, const bool ordered) {
    dispatch(_strand, [this, message = std::move(message)]->boost::asio::awaitable<void> {
        co_await _socket.async_send(boost::asio::buffer(message->buffer.data(), message->buffer.size()));
    });
}

boost::asio::awaitable<std::unique_ptr<InMessage>> ConnectionRudp::receive() {
    //TODO: Resize buffer
    std::vector<byte> buffer(1024);

    const auto [ec, len] = co_await _socket.async_receive(
        boost::asio::buffer(buffer.data(), buffer.size()), boost::asio::as_tuple(boost::asio::use_awaitable));
    if (ec) {
        disconnect();
        co_return nullptr;
    }

    co_return std::make_unique<InMessage>(std::move(buffer));
}

std::shared_ptr<OutMessage> ConnectionRudp::prepare_out_message(const size_t payload_size) {
    return std::make_shared<OutMessage>(std::vector<byte>(RudpHdr::LEN + payload_size));
}
}
