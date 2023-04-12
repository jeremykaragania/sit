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
      for (const clause& i : _clauses) {
        if (!i) {
          return 0;
        }
      };
      return 1;
    }

    const std::vector<clause>& clauses() const {
      return _clauses;
    }

    std::vector<clause>& clauses() {
      return _clauses;
    }
  private:
    std::vector<clause> _clauses;
  };
};

#endif
