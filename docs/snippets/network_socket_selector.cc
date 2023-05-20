#include <gf2/SocketSelector.h>
#include <gf2/TcpListener.h>
#include <gf2/TcpSocket.h>

void doc_SocketSelector(const std::string& service) {
  // tag::socket_selector[]
  gf::TcpListener listener(service);

  gf::SocketSelector selector;
  selector.add_socket(listener);

  std::vector<gf::TcpSocket> sockets;

  for (;;) {
    if (selector.wait() == gf::SocketSelectorStatus::Event) {
      for (auto& socket : sockets) {
        if (selector.ready(socket)) {
          // read something on the socket
          // ...
        }
      }

      if (selector.ready(listener)) {
        // the listener is ready, accept a new connection
        gf::TcpSocket socket = listener.accept();
        sockets.push_back(std::move(socket));
        selector.add_socket(sockets.back());
      }
    }
  }
  // end::socket_selector[]
}
