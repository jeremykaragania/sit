#ifndef VARIABLE_H
#define VARIABLE_H

namespace sit {
  class variable {
  public:
    variable() noexcept : _data{0, 0} {}

    variable(const bool value) noexcept : _data{1, value} {}

    bool is_assigned() const noexcept {
      return _data.is_assigned;
    }

    void unassign() noexcept {
      _data.is_assigned = 0;
    }

    operator bool() const {
      if (!_data.is_assigned) {
        throw;
      }
      return _data.value;
    }
  private:
    struct {
      bool is_assigned : 1;
      bool value : 1;
    } _data;
  };
}

#endif
