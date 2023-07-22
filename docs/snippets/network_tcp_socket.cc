#include <gf2/TcpSocket.h>

void doc_TcpSocket() {
  // tag::tcp_socket[]
  // Create a socket to example.com on port 24680
  gf::TcpSocket socket("example.com", "24680");

  if (!socket) {
    // Handle error
    return;
  }

  uint8_t bytes[] = { 0x42, 0x69, 0x13, 0x12 };

  auto send_result = socket.send_bytes(bytes);

  if (!send_result) {
    // Handle error
    return;
  }


  auto recv_result = socket.recv_bytes(bytes);
  if (!recv_result) {
    // Handle error
    return;
  }
  // end::tcp_socket[]
}
