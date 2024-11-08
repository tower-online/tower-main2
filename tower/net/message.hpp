#pragma once

#include <tower/core/numbers.hpp>

#include <vector>

namespace tower::net {
struct InMessage {
    std::vector<byte> buffer;
};

struct OutMessage {
    std::vector<byte> buffer;
};
}

