#ifndef FORMULA_H
#define FORMULA_H

#include <details/normal_forms.h>
#include <details/clause.h>

namespace sit {
  template <typename T>
  class formula {
  public:
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
