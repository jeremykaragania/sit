#include <iostream>
#include <sit.h>

int main() {
  // We load our DIMACS file.
  sit::dimacs d("filename");

  // We use a CDCL SAT solver and provide the formula and the related set of variables.
  sit::cdcl c(d.data(), d.variables());

  // SAT.
  if (c.solve()) {
    std::cout << "SAT\n";
    for (std::size_t i = 0; i < d.variables().size(); ++i) {
      std::cout << "X[" << i << "] = " << d.variables()[i] << std::endl;
    }
  }
  // UNSAT.
  else {
    std::cout << "UNSAT\n";
  }

  return 0;
}
