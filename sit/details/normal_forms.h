#ifndef NORMAL_FORMS_H
#define NORMAL_FORMS_H

#include <string>

namespace sit {
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
