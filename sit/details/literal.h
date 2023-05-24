#ifndef LITERAL_H
#define LITERAL_H

#include <details/variable.h>

namespace sit {
  struct literal {
    literal(variable& init, const bool is_compl = 0) noexcept : data(init), is_complemented(is_compl) {}

    operator bool() const {
      return is_complemented != data;
    }

    variable& data;
    bool is_complemented;
  };
}

#endif
