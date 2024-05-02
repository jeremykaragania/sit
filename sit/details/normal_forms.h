#ifndef NORMAL_FORMS_H
#define NORMAL_FORMS_H

#include <string>

namespace sit {
  // A conjunctive normal form (CNF) formula is an AND of ORs.
  struct cnf {
    bool clause_cond(const bool& x) const noexcept {
      return x;
    }

    bool formula_cond(const bool& x) const noexcept {
      return !x;
    }

    std::string name() const noexcept {
      return "cnf";
    }
  };

  // A disjunctive normal (DNF) form formula is an OR of ANDs.
  struct dnf {
    bool clause_cond(const bool& x) const noexcept {
      return !x;
    }

    bool formula_cond(const bool& x) const noexcept{
      return x;
    }

    std::string name() const noexcept {
      return "dnf";
    }
  };
}

#endif
