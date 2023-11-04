#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

namespace sit {
  // All SAT solvers have a member function that return satisfiability.
  struct sat_solver {
    virtual bool solve() = 0;
  };
}

#endif
