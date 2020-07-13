#include "CLI.h"
#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, char **argv) {
  vector<string> arguments((size_t(argc)));
  for (size_t argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
    arguments[argumentIndex] = argv[argumentIndex];
  }
  CLI cli(arguments);
  cli.run();
  return 0;
}