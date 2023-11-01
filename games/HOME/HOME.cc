
#include "config.h"

#include "bits/GameHub.h"

int main()
{
  home::GameHub hub(GF_HOME_ASSETS_DIRECTORY);
  return hub.run();
}
