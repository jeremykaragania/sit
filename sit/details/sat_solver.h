#ifndef SAT_SOLVER_H
#define SAT_SOLVER_H

namespace sit {
  struct sat_solver {
    virtual bool solve() = 0;
  };
}

#endif
