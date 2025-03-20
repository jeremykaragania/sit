#ifndef CLAUSE_H
#define CLAUSE_H

#include <vector>
#include <details/literal.h>

namespace sit {
  enum class clause_state {unsatisfied, satisfied, unit, unresolved};

  /*
    A clause is a formula collection with OR or AND operators between each
    element.
  */
  template <typename T>
  class clause {
  public:
    clause() noexcept = default;

    clause(const std::vector<literal> init) noexcept : literals(init) {}

    clause(const std::initializer_list<literal> init) noexcept : literals(init) {}

    /*
      clause::state returns the state of the current clause. It can either be
      unsatisfied, satsified, unit, or unresolved.
    */
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
        if (nf.clause_cond(i)) {
          return 1;
        }
      }

      return 0;
    }

    /*
      clause::simplify creates a new simplified from clause from this clause by
      removing duplicate literals. In CNF, removing duplicate literals does not
      affect the truth value of a clause.
    */
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
  private:
    T nf;
  };
}

#endif
