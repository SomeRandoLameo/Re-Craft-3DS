#include <amethyst.hpp>

#include "ReCraftCore.h"

int main() {
  Amy::RegisterCxxExceptionHandler();
  ReCraftCore core;
  core.Run();
  return 0;
}
