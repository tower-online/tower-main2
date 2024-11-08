#pragma once

#include <tower/net/client.hpp>

namespace tower::net {
class Server final {
    tcp::acceptor _acceptor;

public:
    Server();
};
}