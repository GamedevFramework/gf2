#include "SocketPortability.h"

#include <array>

#ifdef _WIN32
#  include <winsock2.h>
#  include <ws2tcpip.h>
#else
#  include <cerrno>
#  include <cstring>

#  include <sys/socket.h>
#  include <sys/types.h>

#  include <fcntl.h>
#  include <netdb.h>
#  include <unistd.h>
#endif

namespace gf::details {

#ifdef _WIN32
  bool native_close_socket(SocketHandle handle)
  {
    return ::closesocket(handle) == 0;
  }

  bool native_set_reuse_address(SocketHandle handle, bool reuse)
  {
    BOOL val = reuse ? TRUE : FALSE;
    return ::setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&val), sizeof(BOOL)) == 0;
  }

  bool native_set_block_mode(SocketHandle handle, bool blocking)
  {
    u_long mode = blocking ? 1 : 0;
    return ::ioctlsocket(handle, FIONBIO, &mode) == 0;
  }

  bool native_would_block(int err)
  {
    return err == WSAEWOULDBLOCK;
  }

  int last_error_code()
  {
    return WSAGetLastError();
  }

  std::string last_error_string()
  {
    static constexpr std::size_t BufferSize = 1024;
    int err = last_error_code();
    char buffer[BufferSize] = { '\0' };
    ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, BufferSize, nullptr);
    return buffer;
  }

#else
  bool native_close_socket(SocketHandle handle)
  {
    return ::close(handle) == 0;
  }

  bool native_set_reuse_address(SocketHandle handle, bool reuse)
  {
    int val = reuse ? 1 : 0;
    return ::setsockopt(handle, SOL_SOCKET, SO_REUSEADDR, static_cast<const void*>(&val), sizeof(int)) == 0;
  }

  bool native_set_block_mode(SocketHandle handle, bool blocking)
  {
    auto flags = ::fcntl(handle, F_GETFL, 0);

    if (flags == -1) {
      return false;
    }

    return ::fcntl(handle, F_SETFL, blocking ? (flags | O_NONBLOCK) : (flags & ~(O_NONBLOCK))) != -1; // NOLINT
  }

  bool native_would_block(int err)
  {
    return err == EAGAIN || err == EWOULDBLOCK;
  }

  int last_error_code()
  {
    return errno;
  }

  std::string last_error_string()
  {
    static constexpr std::size_t BufferSize = 1024;
    auto err = last_error_code();
    std::array<char, BufferSize> buffer = { '\0' };
    [[maybe_unused]] auto ret = ::strerror_r(err, buffer.data(), buffer.size()); // NOLINT
    return buffer.data();
  }

#endif

  SocketSelectorStatus native_poll(Span<pollfd> fds, Time duration)
  {
    auto ms = duration.as_milliseconds();

#ifdef _WIN32
    auto err = ::WSAPoll(fds.data(), static_cast<ULONG>(fds.size()), ms);
#else
    auto err = ::poll(fds.data(), static_cast<nfds_t>(fds.size()), ms);
#endif

    if (err < 0) {
      Log::error("Error while polling: {}", last_error_string());
      return SocketSelectorStatus::Error;
    }

    if (err == 0) {
      return SocketSelectorStatus::Timeout;
    }

    return SocketSelectorStatus::Event;
  }

  SocketHandle native_bind_listen(const std::string& service, SocketFamily family)
  {
    auto addresses = local_address_info(service, SocketType::Tcp, family);

    for (auto info : addresses) {
      const SocketHandle sock = ::socket(static_cast<int>(info.family), static_cast<int>(info.type), 0);

      if (sock == InvalidSocketHandle) {
        continue;
      }

      if (!native_set_reuse_address(sock, true)) {
        Log::error("Error when setting address reuse. Reason: {}", last_error_string());
      }

      if (::bind(sock, info.address.storage_ptr(), info.address.length()) != 0) {
        native_close_socket(sock);
        continue;
      }

      if (::listen(sock, SOMAXCONN) != 0) {
        native_close_socket(sock);
        continue;
      }

      return sock;
    }

    Log::error("Unable to bind service '%s'", service);
    return InvalidSocketHandle;
  }

  SocketHandle native_connect(const std::string& hostname, const std::string& service, SocketFamily family)
  {
    auto addresses = remote_address_info(hostname, service, SocketType::Tcp, family);

    for (auto info : addresses) {
      const SocketHandle sock = ::socket(static_cast<int>(info.family), static_cast<int>(info.type), 0);

      if (sock == InvalidSocketHandle) {
        continue;
      }

      if (::connect(sock, info.address.storage_ptr(), info.address.length()) != 0) {
        native_close_socket(sock);
        continue;
      }

      return sock;
    }

    Log::error("Unable to connect to '%s:%s'", hostname, service);
    return InvalidSocketHandle;
  }

  SocketHandle native_bind(const std::string& service, SocketFamily family)
  {
    auto addresses = local_address_info(service, SocketType::Udp, family);

    for (auto info : addresses) {
      const SocketHandle sock = ::socket(static_cast<int>(info.family), static_cast<int>(info.type), 0);

      if (sock == InvalidSocketHandle) {
        continue;
      }

      if (::bind(sock, info.address.storage_ptr(), info.address.length()) != 0) {
        native_close_socket(sock);
        continue;
      }

      return sock;
    }

    Log::error("Unable to bind service '%s'", service);
    return InvalidSocketHandle;
  }

  namespace {

    std::vector<SocketAddressInfo> address_info(const char* hostname, const char* service, int flags, SocketType type, SocketFamily family)
    {
      std::vector<SocketAddressInfo> result;

      addrinfo hints = {};
      addrinfo* first = nullptr;

      std::memset(&hints, 0, sizeof hints);
      hints.ai_family = static_cast<int>(family);
      hints.ai_socktype = static_cast<int>(type);
      hints.ai_protocol = 0;
      hints.ai_flags = flags;

      auto err = ::getaddrinfo(hostname, service, &hints, &first);

      if (err != 0) {
        if (hostname != nullptr) {
          gf::Log::error("Error while getting an address for hostname '%s:%s': '%s'", hostname, service, ::gai_strerror(err));
        } else {
          gf::Log::error("Error while getting an address for service '%s': '%s'", service, ::gai_strerror(err));
        }

        return result;
      }

      for (addrinfo* rp = first; rp != nullptr; rp = rp->ai_next) {
        SocketAddressInfo info;
        info.family = static_cast<SocketFamily>(rp->ai_family);
        info.type = static_cast<SocketType>(rp->ai_socktype);
        info.address = SocketAddress(rp->ai_addr, static_cast<SocketAddress::StorageLengthType>(rp->ai_addrlen));
        result.push_back(info);
      }

      ::freeaddrinfo(first);

      return result;
    }

  } // namespace

  std::vector<SocketAddressInfo> remote_address_info(const std::string& hostname, const std::string& service, SocketType type, SocketFamily family)
  {
    return address_info(hostname.c_str(), service.c_str(), NoFlag, type, family);
  }

  std::vector<SocketAddressInfo> local_address_info(const std::string& service, SocketType type, SocketFamily family)
  {
    return address_info(nullptr, service.c_str(), AI_PASSIVE, type, family);
  }

} // namespace gf::details
