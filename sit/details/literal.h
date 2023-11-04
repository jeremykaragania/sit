#ifndef LITERAL_H
#define LITERAL_H

#include <details/variable.h>

namespace sit {
  // A literal encapsulates a variable reference and can be uncomplemented or complemented.
  struct literal {
    literal(variable& data_init, const bool is_complemented_init = 0) : data(data_init), is_complemented(is_complemented_init) {}

    operator bool() const {
      return is_complemented != data;
    }

    variable& data;
    bool is_complemented;
  };
}

#endif
