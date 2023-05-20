#ifndef GF_UDP_SOCKET_H
#define GF_UDP_SOCKET_H

#include <cstdint>

#include <string>
#include <tuple>

#include "NetworkApi.h"
#include "Socket.h"
#include "SocketAddress.h"
#include "Span.h"
#include "Types.h"

namespace gf {

  class GF_NETWORK_API UdpSocket : public Socket {
  public:
    UdpSocket() = default;
    UdpSocket(AnyType /* unused */, SocketFamily family = SocketFamily::Unspec);
    UdpSocket(const std::string& service, SocketFamily family = SocketFamily::Unspec);

    SocketAddress remote_address(const std::string& hostname, const std::string& service);

    SocketDataResult send_raw_bytes_to(Span<const uint8_t> buffer, const SocketAddress& address);
    std::tuple<SocketDataResult, SocketAddress> recv_raw_bytes_from(Span<uint8_t> buffer);

    bool send_bytes_to(Span<const uint8_t> buffer, const SocketAddress& address);
    std::tuple<bool, SocketAddress> recv_bytes_from(Span<uint8_t> buffer);
  };

} // namespace gf

#endif // GF_UDP_SOCKET_H
