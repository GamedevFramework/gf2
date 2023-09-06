#include <gf2/network/TcpListener.h>

void doc_TcpListener_port() {
  // tag::listener_port[]
  // Create a listener on port 24680
  gf::TcpListener listener("24680");
  // end::listener_port[]
}

void doc_TcpListener_service() {
  // tag::listener_service[]
  // Create a listener on port 80 (http)
  gf::TcpListener listener("http");
  // end::listener_service[]
}

void doc_TcpListener_loop() {
  // tag::listener_loop[]
  // Create a listener
  gf::TcpListener listener("24680");

  for (;;) {
    // Accept a new connection ...
    gf::TcpSocket client = listener.accept();

    if (client) {
      // and handle the client...

    }
  }
  // end::listener_loop[]
}
