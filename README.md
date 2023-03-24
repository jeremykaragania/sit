# Sit
A toy header-only SAT solver library.

## Usage
A `variable` has a `bool` value.
```cpp
sit::variable x; // Unassigned.
// ...
sit::variable x = 1; // Assigned.
```
A `literal` encapsulates a `variable` reference.
```cpp
sit::variable x;
// ...
sit::literal l = x; // Uncomplemented.
// ...
sit::literal l = {x, 1} // Complemented.
```
A `clause` is a `literal` collection with `or` operators between each element.
```cpp
std::vector<variable> x(3);
// ...
sit::clause w = {{x[0], 1}, x[1], x[2]}; // ¬x₁ ∨ x₂ ∨ x₃.
```
A `formula` is a `clause` collection with `and` operators between each element.
```cpp
std::vector<clause> w(3);
// ...
sit::formula formula = {w[0], w[1], w[2]}; // w₁ ∧ w₂ ∧ w₃.
```
A `sat_solver` normally requires a `formula` and a related `variable` collection.
```cpp
std::vector<variable> x;
sit::formula f;
// ...
sit::cdcl c(f, x);
bool satisfiable = c.solve();
```

## License
[MIT](LICENSE)
