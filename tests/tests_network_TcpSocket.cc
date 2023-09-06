#include <cstdint>

#include <thread>
#include <vector>

#include <gf2/core/SerializationOps.h>
#include <gf2/network/TcpListener.h>
#include <gf2/network/TcpSocket.h>

#include "gtest/gtest.h"

namespace {
  constexpr const char* TcpTestService = "12345";
  constexpr const char* TcpHost = "localhost";

  template<gf::SocketFamily Family>
  void test_tcp_listener_service()
  {
    gf::TcpListener listener(TcpTestService, Family);

    ASSERT_TRUE(listener);

    auto address = listener.local_address();
    std::cout << "host: " << address.hostname(gf::SocketAddressFormat::Numeric) << '\n';
    std::cout << "service: " << address.service(gf::SocketAddressFormat::Numeric) << '\n';
  }

  template<gf::SocketFamily Family>
  void test_tcp_listerner_one_client()
  {
    gf::TcpListener listener(TcpTestService, Family);
    ASSERT_TRUE(listener);

    std::thread client_thread([]() {
      gf::TcpSocket socket(TcpHost, TcpTestService, Family);
      ASSERT_TRUE(socket);

      uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
      auto result = socket.send_raw_bytes(buffer);
      EXPECT_TRUE(result);
      EXPECT_EQ(*result, 4u);

      auto local = socket.local_address();
      std::cout << "local host: " << local.hostname(gf::SocketAddressFormat::Numeric) << '\n';
      std::cout << "local service: " << local.service(gf::SocketAddressFormat::Numeric) << '\n';

      auto remote = socket.remote_address();
      std::cout << "remote host: " << remote.hostname(gf::SocketAddressFormat::Numeric) << '\n';
      std::cout << "remote service: " << remote.service(gf::SocketAddressFormat::Numeric) << '\n';
    });

    gf::TcpSocket socket = listener.accept();
    ASSERT_TRUE(socket);

    uint8_t buffer[10] = { 0 };
    auto result = socket.recv_raw_bytes(buffer);
    EXPECT_TRUE(result);
    EXPECT_EQ(*result, 4u);
    EXPECT_EQ(buffer[0], 0x42);
    EXPECT_EQ(buffer[1], 0x69);
    EXPECT_EQ(buffer[2], 0xFF);
    EXPECT_EQ(buffer[3], 0x12);

    result = socket.recv_raw_bytes(buffer);
    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), gf::SocketStatus::Close);

    client_thread.join();
  }

  template<gf::SocketFamily Family>
  void test_tcp_listener_multiple_client()
  {
    static constexpr int ClientCount = 10;

    gf::TcpListener listener(TcpTestService, Family);
    ASSERT_TRUE(listener);

    std::thread client_thread([]() {
      for (int i = 0; i < ClientCount; ++i) {
        gf::TcpSocket socket(TcpHost, TcpTestService, Family);
        ASSERT_TRUE(socket);

        uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
        auto result = socket.send_raw_bytes(buffer);
        EXPECT_TRUE(result);
        EXPECT_EQ(*result, 4u);
      }
    });

    for (int i = 0; i < ClientCount; ++i) {
      gf::TcpSocket socket = listener.accept();
      ASSERT_TRUE(socket);

      uint8_t buffer[10] = { 0 };
      auto result = socket.recv_raw_bytes(buffer);
      EXPECT_TRUE(result);
      EXPECT_EQ(*result, 4u);
      EXPECT_EQ(buffer[0], 0x42);
      EXPECT_EQ(buffer[1], 0x69);
      EXPECT_EQ(buffer[2], 0xFF);
      EXPECT_EQ(buffer[3], 0x12);

      result = socket.recv_raw_bytes(buffer);
      EXPECT_FALSE(result);
      EXPECT_EQ(result.error(), gf::SocketStatus::Close);
    }

    client_thread.join();
  }

  template<gf::SocketFamily Family>
  void test_tcp_listener_non_blocking()
  {
    gf::TcpListener listener(TcpTestService, Family);
    ASSERT_TRUE(listener);

    std::thread client_thread([]() {
      gf::TcpSocket socket(TcpHost, TcpTestService, Family);
      ASSERT_TRUE(socket);

      socket.set_non_blocking();

      uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
      auto result = socket.send_raw_bytes(buffer);
      EXPECT_TRUE(result);
      EXPECT_EQ(*result, 4u);

      result = socket.recv_raw_bytes(buffer);
      EXPECT_FALSE(result);
      EXPECT_EQ(result.error(), gf::SocketStatus::Block);
    });

    gf::TcpSocket socket = listener.accept();
    ASSERT_TRUE(socket);

    uint8_t buffer[10] = { 0 };
    auto result = socket.recv_raw_bytes(buffer);
    EXPECT_TRUE(result);
    EXPECT_EQ(*result, 4u);
    EXPECT_EQ(buffer[0], 0x42);
    EXPECT_EQ(buffer[1], 0x69);
    EXPECT_EQ(buffer[2], 0xFF);
    EXPECT_EQ(buffer[3], 0x12);

    result = socket.recv_raw_bytes(buffer);
    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), gf::SocketStatus::Close);

    client_thread.join();
  }

} // namespace

TEST(TcpSocketTest, TcpSocketDefault) {
  gf::TcpSocket socket;

  EXPECT_FALSE(socket);
}

TEST(TcpSocketTest, TcpListenerDefault) {
  gf::TcpListener listener;

  EXPECT_FALSE(listener);
}

TEST(TcpSocketTest, TcpListenerServiceUnspec) {
  test_tcp_listener_service<gf::SocketFamily::Unspec>();
}

TEST(TcpSocketTest, TcpListenerServiceV4) {
  test_tcp_listener_service<gf::SocketFamily::IPv4>();
}

TEST(TcpSocketTest, TcpListenerServiceV6) {
  test_tcp_listener_service<gf::SocketFamily::IPv6>();
}

TEST(TcpSocketTest, TcpListenerOneClientUnspec) {
  test_tcp_listerner_one_client<gf::SocketFamily::Unspec>();
}

TEST(TcpSocketTest, TcpListenerOneClientV4) {
  test_tcp_listerner_one_client<gf::SocketFamily::IPv4>();
}

TEST(TcpSocketTest, TcpListenerOneClientV6) {
  test_tcp_listerner_one_client<gf::SocketFamily::IPv6>();
}

TEST(TcpSocketTest, TcpListenerMultipleClientUnspec) {
  test_tcp_listener_multiple_client<gf::SocketFamily::Unspec>();
}

TEST(TcpSocketTest, TcpListenerMultipleClientV4) {
  test_tcp_listener_multiple_client<gf::SocketFamily::IPv4>();
}

TEST(TcpSocketTest, TcpListenerMultipleClientV6) {
  test_tcp_listener_multiple_client<gf::SocketFamily::IPv6>();
}

TEST(TcpSocketTest, TcpListenerNonBlockingUnspec) {
  test_tcp_listener_non_blocking<gf::SocketFamily::Unspec>();
}

TEST(TcpSocketTest, TcpListenerNonBlockingV4) {
  test_tcp_listener_non_blocking<gf::SocketFamily::IPv4>();
}

TEST(TcpSocketTest, TcpListenerNonBlockingV6) {
  test_tcp_listener_non_blocking<gf::SocketFamily::IPv6>();
}
