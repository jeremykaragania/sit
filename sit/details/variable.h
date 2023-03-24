#ifndef VARIABLE_H
#define VARIABLE_H

namespace sit {
  class variable {
  public:
    variable() noexcept : _is_assigned(0), _value(0) {}

    variable(const bool value) noexcept : _is_assigned(1), _value(value) {}

    const bool& is_assigned() const {
      return _is_assigned;
    }

    bool& is_assigned() {
      return _is_assigned;
    }

    operator bool() const {
      if (!_is_assigned) {
        throw;
      }
      return _value;
    }
  private:
    bool _is_assigned;
    bool _value;
  };
}

#endif
