#ifndef VARIABLE_H
#define VARIABLE_H

namespace sit {
  class variable {
  public:
    variable() noexcept : value(0), _is_assigned(0) {}

    variable(const bool value) noexcept : value(value), _is_assigned(1) {}

    bool is_assigned() const noexcept {
      return _is_assigned;
    }

    void unassign() noexcept {
      _is_assigned = 0;
    }

    operator bool() const {
      if (!_is_assigned) {
        throw;
      }
      return value;
    }

    bool value;
  private:
    bool _is_assigned;
  };
}

#endif
