#include <iostream>

#include <gf2/Tarball.h>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "gf2_tarball_listfiles <tarball>" << std::endl;
    return 0;
  }

  gf::Tarball tarball(argv[1]);
  auto entries = tarball.entries();

  for (auto entry : entries) {
    std::cout << entry << '\n';
  }

  return 0;
}
