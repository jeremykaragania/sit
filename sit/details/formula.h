#ifndef FORMULA_H
#define FORMULA_H

#include <details/clause.h>

namespace sit {
  struct formula {
    formula() noexcept {}

    formula(const std::vector<clause> init) noexcept : clauses(init) {}

    formula(const std::initializer_list<clause> init) noexcept : clauses(init) {}

    operator bool() {
      for (const clause& i : clauses) {
        if (!i) {
          return 0;
        }
      };
      return 1;
    }

    std::vector<clause> clauses;
  };
};

#endif
