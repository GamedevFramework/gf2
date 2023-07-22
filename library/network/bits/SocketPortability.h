#ifndef GF_SOCKET_PORTABILITY_H
#define GF_SOCKET_PORTABILITY_H

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <array>
#include <string>
#include <vector>

#ifdef _WIN32
#  include <winsock2.h>
#else
#  include <sys/socket.h>
#  include <sys/types.h>

#  include <poll.h>
#endif

#include <gf2/Log.h>
#include <gf2/SocketAddress.h>
#include <gf2/SocketTypes.h>
#include <gf2/Span.h>
#include <gf2/Time.h>

namespace gf::details {

  bool native_close_socket(SocketHandle handle);

  bool native_set_reuse_address(SocketHandle handle, bool reuse = true);

  bool native_set_block_mode(SocketHandle handle, bool blocking);

  bool native_would_block(int err);

  SocketSelectorStatus native_poll(Span<pollfd> fds, Time duration);

  SocketHandle native_bind_listen(const std::string& service, SocketFamily family);

  SocketHandle native_connect(const std::string& host, const std::string& service, SocketFamily family);

  SocketHandle native_bind(const std::string& service, SocketFamily family);

#ifdef _WIN32
  inline int send_length(Span<const uint8_t> buffer)
  {
    return static_cast<int>(buffer.size());
  }

  inline const char* send_pointer(Span<const uint8_t> buffer)
  {
    return reinterpret_cast<const char*>(buffer.data()); // NOLINT
  }

  inline int recv_length(Span<uint8_t> buffer)
  {
    return static_cast<int>(buffer.size());
  }

  inline char* recv_pointer(Span<uint8_t> buffer)
  {
    return reinterpret_cast<char*>(buffer.data());
  }
#else
  inline std::size_t send_length(Span<const uint8_t> buffer)
  {
    return buffer.size();
  }

  inline const void* send_pointer(Span<const uint8_t> buffer)
  {
    return reinterpret_cast<const void*>(buffer.data()); // NOLINT
  }

  inline std::size_t recv_length(Span<uint8_t> buffer)
  {
    return buffer.size();
  }

  inline void* recv_pointer(Span<uint8_t> buffer)
  {
    return reinterpret_cast<void*>(buffer.data()); // NOLINT
  }
#endif

  int last_error_code();
  std::string last_error_string();

#ifdef _WIN32
  constexpr int InvalidCommunication = SOCKET_ERROR;
#else
  constexpr ssize_t InvalidCommunication = -1;
#endif

  enum SocketType : int {
    Tcp = SOCK_STREAM,
    Udp = SOCK_DGRAM,
  };

  struct SocketAddressInfo {
    SocketFamily family = SocketFamily::Unspec;
    SocketType type = SocketType::Tcp;
    SocketAddress address;
  };

  constexpr int NoFlag = 0;

  std::vector<SocketAddressInfo> remote_address_info(const std::string& hostname, const std::string& service, SocketType type, SocketFamily family = SocketFamily::Unspec);
  std::vector<SocketAddressInfo> local_address_info(const std::string& service, SocketType type, SocketFamily family = SocketFamily::Unspec);

  using SizeHeader = std::array<uint8_t, 8>;

  constexpr SizeHeader encode_header(uint64_t size)
  {
    SizeHeader header = {
      {0, 0, 0, 0, 0, 0, 0, 0}
    };

    for (int i = 0; i < 8; ++i) {
      header[8 - i - 1] = static_cast<uint8_t>(size % 0x100); // NOLINT
      size >>= 8;
    }

    return header;
  }

  constexpr uint64_t decode_header(const SizeHeader& header)
  {
    uint64_t size = 0;

    for (const uint64_t x : header) {
      size = (size << 8) + x;
    }

    return size;
  }

} // namespace gf::details

#endif // GF_SOCKET_PORTABILITY_H
