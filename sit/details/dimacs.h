#ifndef DIMACS_H
#define DIMACS_H

#include <fstream>
#include <limits>
#include <details/formula.h>
#include <string>

namespace sit {
  class dimacs {
    public:
    dimacs(std::string filename) {
      std::ifstream ifs(filename, std::ios::in);
      if (!ifs.is_open()) {
        throw;
      }
      while (1) {
        if ((char)ifs.peek() == 'p') {
          break;
        }
        ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
      ifs.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
      std::string buffer;
      std::getline(ifs, buffer, ' ');
      if (buffer != "cnf") {
        throw;
      }
      std::getline(ifs, buffer, ' ');
      std::size_t variable_number = std::stoull(buffer);
      _variables.resize(variable_number);
      std::getline(ifs, buffer, '\n');
      std::size_t clause_number = std::stoull(buffer);
      _formula.clauses().resize(clause_number);
      for (clause& i : _formula.clauses()) {
        while (1) {
          ifs >> buffer;
          if (buffer == "0" || ifs.eof()) {
            break;
          }
          bool complemented = 0;
          if (buffer[0] == '-') {
            complemented = 1;
            buffer.erase(buffer.begin());
          }
          i.literals().push_back({_variables[std::stoull(buffer) - 1], complemented});
        }
      }
    }

    const formula& data() const noexcept {
      return _formula;
    }

    formula& data() noexcept{
      return _formula;
    }

    const std::vector<variable>& variables() const noexcept {
      return _variables;
    }

    std::vector<variable>& variables() noexcept {
      return _variables;
    }
    private:
      formula _formula;
      std::vector<variable> _variables;
  };
}

#endif
