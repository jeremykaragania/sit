#ifndef FORMULA_H
#define FORMULA_H

#include <details/normal_forms.h>
#include <details/clause.h>

namespace sit {
  // A formula is a clause collection with OR or AND operators between each element
  template <typename T>
  class formula {
  public:
    formula(const std::vector<clause<T>> clauses_init) noexcept : clauses(clauses_init) {}

    formula(const std::initializer_list<clause<T>> clauses_init) noexcept : clauses(clauses_init) {}

    operator bool() {
      for (const clause<T>& i : clauses) {
        if (nf.formula_cond(i)) {
          return 0;
        }
      };
      return 1;
    }

    std::vector<clause<T>> clauses;
  private:
    T nf;
  };
};

#endif
