#include <gf2/UdpSocket.h>

void doc_UdpSocket() {
  // tag::udp_socket[]
  // Create a socket
  gf::UdpSocket socket(gf::Any);

  if (!socket) {
    // Handle error
    return;
  }

  gf::SocketAddress address = socket.remote_address("example.com", "24680");

  uint8_t bytes[] = { 0x42, 0x69, 0x13, 0x12 };

  if (!socket.send_bytes_to(bytes, address)) {
    // Handle error
    return;
  }

  auto [success, remote] = socket.recv_bytes_from(bytes);

  if (!success) {
    // Handle error
    return;
  }
  // end::udp_socket[]
}
