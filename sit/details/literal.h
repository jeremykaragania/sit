#ifndef LITERAL_H
#define LITERAL_H

#include <details/variable.h>

namespace sit {
  struct literal {
    operator bool() const {
      return is_complemented != data;
    }

    variable& data;
    bool is_complemented;
  };
}

#endif
