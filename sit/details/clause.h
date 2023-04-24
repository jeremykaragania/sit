#ifndef CLAUSE_H
#define CLAUSE_H

#include <vector>
#include <details/literal.h>

namespace sit {
  enum class clause_state {unsatisfied, satisfied, unit, unresolved};

  class clause {
  public:
    clause() noexcept : _literals() {}

    clause(const std::vector<literal> literals) noexcept : _literals(literals) {}

    clause(const std::initializer_list<literal> literals) noexcept : _literals(literals) {}

    clause_state state() const noexcept {
      std::size_t assigned_1 = 0;
      std::size_t unassigned = 0;
      for (const literal& i : _literals) {
        if (i.data().is_assigned()) {
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
      for (const literal& i : _literals) {
        if (i) {
          return 1;
        }
      }
      return 0;
    }

    const std::vector<literal>& literals() const noexcept {
      return _literals;
    }

    std::vector<literal>& literals() noexcept {
      return _literals;
    }

    clause simplify() noexcept {
      std::vector<literal> simplified;
      for (literal& i : _literals) {
        bool include = 1;
        for (literal& j : simplified) {
          if (&i.data() == &j.data()) {
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
  private:
    std::vector<literal> _literals;
  };
}

#endif
