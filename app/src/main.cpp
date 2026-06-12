#include <iostream>

#include "stockfih/version.hpp"

int main() {
  std::cout << "Stockfih " << stockfih::version() << '\n';
  return 0;
}
