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

    variable& data() const {
      return _variable;
    }

    bool& is_complemented() {
      return _is_complemented;
    }
  private:
    bool _is_complemented;
    variable& _variable;
  };
}

#endif
