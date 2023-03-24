#include <iostream>
#include <sit.h>

int main() {
  // We declare our set of variables.
  std::vector<sit::variable> x(12);

  // We declare our set of clauses.
  std::vector<sit::clause> w(11);

  // We define our set of clauses.
  w[0] = {{x[0], 1}, {x[1], 1}}; // ¬x₁ ∨ ¬x₂.
  w[1] = {{x[0], 1}, x[2]}; // ¬x₁ ∨ x₃.
  w[2] = {{x[2], 1}, {x[3], 1}}; // ¬x₃ ∨ ¬x₄.
  w[3] = {x[1], x[3], x[4]}; // x₂ ∨ x₄ ∨ x₅.
  w[4] = {{x[4], 1}, x[5], {x[6], 1}}; // ¬x₅ ∨ x₆ ∨ ¬x₇.
  w[5] = {x[1], x[6], x[7]}; //x₂ ∨ x₇ ∨ x₈.
  w[6] = {{x[7], 1}, {x[8], 1}}; // ¬x₈ ∨ ¬x₉.
  w[7] = {{x[7], 1}, x[9]}; // ¬x₈ ∨ x₁₀.
  w[8] = {x[8], {x[9], 1}, x[10]}; // x₉ ∨ ¬x₁₀ ∨ x₁₁.
  w[9] = {{x[9], 1}, {x[11], 1}}; // ¬x₁₀ ∨ ¬x₁₂.
  w[10] = {{x[10], 1}, x[11]}; // ¬x₁₁ ∨ x₁₂.

  // We define our formula.
  sit::formula f(w);

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
