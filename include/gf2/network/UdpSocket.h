#ifndef GF_UDP_SOCKET_H
#define GF_UDP_SOCKET_H

#include <cstdint>

#include <string>
#include <tuple>

#include <gf2/core/Span.h>
#include <gf2/core/Types.h>

#include "NetworkApi.h"
#include "Socket.h"
#include "SocketAddress.h"

namespace gf {

  class GF_NETWORK_API UdpSocket : public Socket {
  public:
    UdpSocket() = default;
    UdpSocket(AnyType /* unused */, SocketFamily family = SocketFamily::Unspec);
    UdpSocket(const std::string& service, SocketFamily family = SocketFamily::Unspec);

    SocketAddress remote_address(const std::string& hostname, const std::string& service);

    SocketResult send_raw_bytes_to(Span<const uint8_t> buffer, const SocketAddress& remote_address);
    SocketResult recv_raw_bytes_from(Span<uint8_t> buffer, SocketAddress* remote_address);

    SocketResult send_bytes_to(Span<const uint8_t> buffer, const SocketAddress& remote_address);
    SocketResult recv_bytes_from(Span<uint8_t> buffer, SocketAddress* remote_address);
  };

} // namespace gf

#endif // GF_UDP_SOCKET_H
