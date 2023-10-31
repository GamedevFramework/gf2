
#include "bits/GameHub.h"

#include "config.h"

int main() {
  home::GameHub hub(GF_HOME_ASSETS_DIRECTORY);
  return hub.run();
}
