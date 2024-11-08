#pragma once

#include <tower/core/numbers.hpp>

#include <bit>
#include <span>

namespace tower::net::details {
struct RudpHdr {
    u8 control{};
    u8 mode{};
    u16 seq{};

    static constexpr size_t LEN{4};

    static RudpHdr read(const std::span<const byte, LEN> src) {
        RudpHdr hdr{};
        hdr.control = std::to_integer<u8>(src[0]);
        hdr.mode = std::to_integer<u8>(src[1]);

        if constexpr (std::endian::native == std::endian::little) {
            hdr.seq = std::byteswap(std::bit_cast<u16>(*reinterpret_cast<const u16*>(&src[2])));
        } else {
            hdr.seq = std::bit_cast<u16>(*reinterpret_cast<const u16*>(&src[2]));
        }

        return hdr;
    }

    static void write(std::span<byte, LEN> dst, const RudpHdr& hdr) {
        dst[0] = byte{hdr.control};
        dst[1] = byte{hdr.mode};

        u16 seq_be;
        if constexpr (std::endian::native == std::endian::little) {
            seq_be = std::byteswap(hdr.seq);
        } else {
            seq_be = hdr.seq;
        }
        dst[2] = byte{static_cast<u8>(seq_be >> 8)};
        dst[3] = byte{static_cast<u8>(seq_be & 0xFF)};
    }

    [[nodiscard]] bool syn() const { return control & SYN; }
    [[nodiscard]] bool ack() const { return control & ACK; }
    [[nodiscard]] bool eak() const { return control & EAK; }
    [[nodiscard]] bool rst() const { return control & RST; }

    void set_syn() { control |= SYN; }
    void set_ack() { control |= ACK; }
    void set_eak() { control |= EAK; }
    void set_rst() { control |= RST; }

    [[nodiscard]] bool reliable() const { return mode & RELIABLE; }
    [[nodiscard]] bool ordered() const { return mode & ORDERED; }

    void set_reliable() { mode |= RELIABLE; }
    void set_ordered() { mode |= ORDERED; }

private:
    static constexpr u8 SYN {0b0000'0001};
    static constexpr u8 ACK {0b0000'0010};
    static constexpr u8 EAK {0b0000'0100};
    static constexpr u8 RST {0b0000'1000};

    static constexpr u8 RELIABLE {0b1000'0000};
    static constexpr u8 ORDERED {0b0000'1000};
};
}