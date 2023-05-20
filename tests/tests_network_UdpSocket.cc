#include <cstdint>

#include <thread>
#include <vector>

#include <gf2/SerializationOps.h>
#include <gf2/UdpSocket.h>

#include "gtest/gtest.h"

namespace {
  constexpr const char* UdpTestService = "12345";
  constexpr const char* UdpHost = "localhost";

  template<gf::SocketFamily Family>
  void test_udp_socket_service()
  {
    gf::UdpSocket socket(UdpTestService, Family);

    ASSERT_TRUE(socket);

    auto address = socket.local_address();
    std::cout << "host: " << address.hostname(gf::SocketAddressFormat::Numeric) << '\n';
    std::cout << "service: " << address.service(gf::SocketAddressFormat::Numeric) << '\n';
  }

  template<gf::SocketFamily Family>
  void test_udp_socket_any()
  {
    gf::UdpSocket socket(gf::Any, Family);

    ASSERT_TRUE(socket);

    auto address = socket.local_address();
    std::cout << "host: " << address.hostname(gf::SocketAddressFormat::Numeric) << '\n';
    std::cout << "service: " << address.service(gf::SocketAddressFormat::Numeric) << '\n';
  }

  template<gf::SocketFamily Family>
  void test_udp_socket_one_way_communication()
  {
    gf::UdpSocket socket(UdpTestService, Family);
    ASSERT_TRUE(socket);

    std::thread client_thread([]() {
      gf::UdpSocket socket(gf::Any, Family);
      ASSERT_TRUE(socket);

      auto actual_family = socket.local_address().family();

      gf::SocketAddress address = socket.remote_address(UdpHost, UdpTestService);
      EXPECT_EQ(address.family(), actual_family);

      uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
      auto res = socket.send_raw_bytes_to(buffer, address);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);
    });

    auto actual_family = socket.local_address().family();

    uint8_t buffer[10];
    auto [res, address] = socket.recv_raw_bytes_from(buffer);
    EXPECT_EQ(res.status, gf::SocketStatus::Data);
    EXPECT_EQ(res.length, 4u);
    EXPECT_EQ(buffer[0], 0x42);
    EXPECT_EQ(buffer[1], 0x69);
    EXPECT_EQ(buffer[2], 0xFF);
    EXPECT_EQ(buffer[3], 0x12);

    EXPECT_EQ(address.family(), actual_family);

    client_thread.join();
  }

  template<gf::SocketFamily Family>
  void test_udp_socket_two_way_communication()
  {
    gf::UdpSocket socket(UdpTestService, Family);
    ASSERT_TRUE(socket);

    std::thread client_thread([]() {
      gf::UdpSocket socket(gf::Any, Family);
      ASSERT_TRUE(socket);

      auto actual_family = socket.local_address().family();

      {
        gf::SocketAddress address = socket.remote_address(UdpHost, UdpTestService);
        EXPECT_EQ(address.family(), actual_family);
        uint8_t buffer[4] = { 0x42, 0x69, 0xFF, 0x12 };
        auto res = socket.send_raw_bytes_to(buffer, address);
        EXPECT_EQ(res.status, gf::SocketStatus::Data);
        EXPECT_EQ(res.length, 4u);
      }

      {
        uint8_t buffer[10];
        auto [res, address] = socket.recv_raw_bytes_from(buffer);
        EXPECT_EQ(res.status, gf::SocketStatus::Data);
        EXPECT_EQ(res.length, 4u);
        EXPECT_EQ(buffer[0], 0x23);
        EXPECT_EQ(buffer[1], 0x17);
        EXPECT_EQ(buffer[2], 0x21);
        EXPECT_EQ(buffer[3], 0x23);
        EXPECT_EQ(address.family(), actual_family);
      }
    });

    auto actual_family = socket.local_address().family();
    gf::SocketAddress origin;

    {
      uint8_t buffer[10];
      auto [res, address] = socket.recv_raw_bytes_from(buffer);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);
      EXPECT_EQ(buffer[0], 0x42);
      EXPECT_EQ(buffer[1], 0x69);
      EXPECT_EQ(buffer[2], 0xFF);
      EXPECT_EQ(buffer[3], 0x12);
      EXPECT_EQ(address.family(), actual_family);
      origin = address;
    }

    {
      uint8_t buffer[4] = { 0x23, 0x17, 0x21, 0x23 };
      auto res = socket.send_raw_bytes_to(buffer, origin);
      EXPECT_EQ(res.status, gf::SocketStatus::Data);
      EXPECT_EQ(res.length, 4u);
    }

    client_thread.join();
  }

} // namespace

TEST(SocketTest, UdpSocketDefault) {
  gf::UdpSocket socket;

  EXPECT_FALSE(socket);
}

TEST(SocketTest, UdpSocketServiceUnspec) {
  test_udp_socket_service<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, UdpSocketServiceV4) {
  test_udp_socket_service<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, UdpSocketServiceV6) {
  test_udp_socket_service<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, UdpSocketAnyUnspec) {
  test_udp_socket_any<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, UdpSocketAnyV4) {
  test_udp_socket_any<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, UdpSocketAnyV6) {
  test_udp_socket_any<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, UdpSocketOneWayCommunicationUnspec) {
  test_udp_socket_one_way_communication<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, UdpSocketOneWayCommunicationV4) {
  test_udp_socket_one_way_communication<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, UdpSocketOneWayCommunicationV6) {
  test_udp_socket_one_way_communication<gf::SocketFamily::IPv6>();
}

TEST(SocketTest, UdpSocketTwoWayCommunicationUnspec) {
  test_udp_socket_two_way_communication<gf::SocketFamily::Unspec>();
}

TEST(SocketTest, UdpSocketTwoWayCommunicationV4) {
  test_udp_socket_two_way_communication<gf::SocketFamily::IPv4>();
}

TEST(SocketTest, UdpSocketTwoWayCommunicationV6) {
  test_udp_socket_two_way_communication<gf::SocketFamily::IPv6>();
}
