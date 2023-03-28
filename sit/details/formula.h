#ifndef FORMULA_H
#define FORMULA_H

#include <details/clause.h>

namespace sit {
  class formula {
  public:
    formula() noexcept {}

    formula(const std::vector<clause> clauses) noexcept : _clauses(clauses) {}

    formula(const std::initializer_list<clause> clauses) noexcept : _clauses(clauses) {}

    operator bool() {
      bool value = _clauses[0];
      for (const clause& i : _clauses) {
        if (i == 0) {
          return 0;
        }
      };
      return 1;
    }

    std::vector<clause>& clauses() {
      return _clauses;
    }
  private:
    std::vector<clause> _clauses;
  };
};

#endif
