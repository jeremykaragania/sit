#ifndef LITERAL_H
#define LITERAL_H

#include <details/variable.h>

namespace sit {
  class literal {
  public:
    literal(variable& x, const bool is_complemented = 0) noexcept : _variable(&x), _is_complemented(is_complemented) {}

    literal(const literal& other) noexcept  : _variable(other._variable), _is_complemented(other._is_complemented) {}

    literal(literal&& other) noexcept  : _variable(other._variable), _is_complemented(other._is_complemented) {
      other._is_complemented = 0;
      other._variable = nullptr;
    }

    literal operator=(const literal& rhs) noexcept {
      _is_complemented = rhs._is_complemented;
      _variable = rhs._variable;
      return *this;
    }

    literal operator=(literal&& rhs) noexcept {
      _is_complemented = rhs._is_complemented;
      _variable = rhs._variable;
      rhs._is_complemented = 0;
      rhs._variable = nullptr;
      return *this;
    }

    operator bool() const {
      return _is_complemented != *_variable;
    }

    const variable* data() const {
      return _variable;
    }

    variable* data() {
      return _variable;
    }

    const bool& is_complemented() const {
      return _is_complemented;
    }

    bool& is_complemented() {
      return _is_complemented;
    }
  private:
    bool _is_complemented;
    variable* _variable;
  };
}

#endif
