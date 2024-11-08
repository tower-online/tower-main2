#include <tower/net/connection_rudp.hpp>
#include <tower/net/details/rudp.hpp>

namespace tower::net {
ConnectionRudp::ConnectionRudp(udp::socket&& socket, boost::asio::strand<boost::asio::any_io_executor>&& strand)
    : _socket{std::move(socket)}, _strand{std::move(strand)} {}
}
