#ifndef LITERAL_H
#define LITERAL_H

#include <details/variable.h>

namespace sit {
  class literal {
  public:
    literal(variable& x, const bool is_complemented = 0) noexcept : _variable(x), _is_complemented(is_complemented) {}

    operator bool() const {
      return _is_complemented != _variable;
    }

    const variable& data() const noexcept {
      return _variable;
    }

    variable& data() noexcept {
      return _variable;
    }

    const bool& is_complemented() const noexcept {
      return _is_complemented;
    }

    bool& is_complemented() noexcept {
      return _is_complemented;
    }
  private:
    variable& _variable;
    bool _is_complemented;
  };
}

#endif
