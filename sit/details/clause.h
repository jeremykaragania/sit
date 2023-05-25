#ifndef CLAUSE_H
#define CLAUSE_H

#include <vector>
#include <details/literal.h>

namespace sit {
  enum class clause_state {unsatisfied, satisfied, unit, unresolved};

  struct clause {
    clause() noexcept : literals() {}

    clause(const std::vector<literal> init) noexcept : literals(init) {}

    clause(const std::initializer_list<literal> init) noexcept : literals(init) {}

    clause_state state() const noexcept {
      std::size_t assigned_1 = 0;
      std::size_t unassigned = 0;
      for (const literal& i : literals) {
        if (i.data.is_assigned()) {
          if (i) {
            ++assigned_1;
          }
        }
        else {
          ++unassigned;
        }
      }
      if (unassigned == 0) {
        if (assigned_1 == 0) {
          return clause_state::unsatisfied;
        }
        else if (assigned_1 > 0) {
          return clause_state::satisfied;
        }
      }
      else if (unassigned == 1 && assigned_1 == 0) {
        return clause_state::unit;
      }
      return clause_state::unresolved;
    }

    operator bool() const {
      for (const literal& i : literals) {
        if (i) {
          return 1;
        }
      }
      return 0;
    }

    clause simplify() noexcept {
      std::vector<literal> simplified;
      for (literal& i : literals) {
        bool include = 1;
        for (literal& j : simplified) {
          if (&i.data == &j.data) {
            include = 0;
            break;
          }
        }
        if (include) {
          simplified.push_back(i);
        }
      }
      return simplified;
    }

    std::vector<literal> literals;
  };
}

#endif
