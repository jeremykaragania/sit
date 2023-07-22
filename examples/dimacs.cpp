#include <iostream>
#include <sit.h>

int main() {
  // We declare our set of variables.
  std::vector<sit::variable> x;

  // We declare our formula.
  sit::formula<sit::cnf> f;

  // We load our DIMACS file.
  sit::read_dimacs("filename", f, x);

  // We use a CDCL SAT solver and provide the formula and the related set of variables.
  sit::cdcl c(f, x);

  // SAT.
  if (c.solve()) {
    std::cout << "SAT\n";
    for (std::size_t i = 0; i < x.size(); ++i) {
      std::cout << "X[" << i << "] = " << x[i] << std::endl;
    }
  }
  // UNSAT.
  else {
    std::cout << "UNSAT\n";
  }

  return 0;
}
