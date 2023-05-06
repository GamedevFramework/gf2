#include <iostream>

#include <gf2/Tarball.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 0;
  }

  gf::Tarball tarball(argv[1]);
  auto entries = tarball.entries();

  for (auto entry : entries) {
    std::cout << entry << '\n';
  }

  return 0;
}
