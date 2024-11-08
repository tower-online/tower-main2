#include <tower/net/client.hpp>

namespace tower::net {
Client::Client(tcp::socket&& stream, const boost::asio::any_io_executor& executor)
    : _stream{std::move(stream)}, _datagram{make_strand(executor)}, _stream_strand{make_strand(executor)}
{}

void Client::open() {

}

void Client::send_stream_message(std::shared_ptr<OutMessage> message) {

}

void Client::send_datagram_message(std::shared_ptr<OutMessage> message) {
    // _datagram.async_send()
}
}
