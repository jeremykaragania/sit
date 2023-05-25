#ifndef FORMULA_H
#define FORMULA_H

#include <details/clause.h>

namespace sit {
  struct formula {
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
