# Sit
A header-only SAT solver library.

## Installation
Clone the repository.
```bash
git clone https://github.com/jeremykaragania/sit.git
```
Include the source file [sit.h](sit/sit.h).
```cpp
#include <sit.h>
```

## Usage
A `variable` has a `bool` value.
```cpp
sit::variable a; // Unassigned.
sit::variable b = 1; // Assigned.
```
A `literal` encapsulates a `variable` reference.
```cpp
sit::variable a;
// ...
sit::literal b = a; // Uncomplemented.
sit::literal b = {a, 1} // Complemented.
```
A `clause` is a `literal` collection with `or` or `and` operators between each element.
```cpp
std::vector<variable> a(3);
// ...
sit::clause<sit::cnf> b = {{a[0], 1}, a[1], a[2]}; // ¬a₁ ∨ a₂ ∨ a₃.
sit::clause<sit::dnf> c = {{a[0], 1}, a[1], a[2]}; // ¬a₁ ∧ a₂ ∧ a₃.
```
A `formula` is a `clause` collection with `or` or `and` operators between each element.
```cpp
std::vector<clause<sit::cnf>> a(3);
std::vector<clause<sit::dnf>> b(3);
// ...
sit::formula<sit::cnf> c = {a[0], a[1], a[2]}; // a₁ ∧ a₂ ∧ a₃.
sit::formula<sit::dnf> d = {b[0], b[1], b[2]}; // b₁ ∨ b₂ ∨ b₃.
```
A `sat_solver` normally requires a `formula` and a related `variable` collection.
```cpp
std::vector<variable> a;
sit::formula<sit::cnf> b;
// ...
sit::cdcl c(b, a);
bool satisfiable = c.solve();
```

## License
[MIT](LICENSE)
