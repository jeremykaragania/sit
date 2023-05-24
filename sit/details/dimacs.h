#ifndef DIMACS_H
#define DIMACS_H

#include <fstream>
#include <limits>
#include <string>
#include <details/formula.h>

namespace sit {
  void read_dimacs(std::string filename, formula& init_formula, std::vector<variable>& init_variables) {
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
    ifs >> buffer;
    if (buffer != "cnf") {
      throw;
    }
    ifs >> buffer;
    std::size_t variable_number = std::stoull(buffer);
    init_variables.resize(variable_number);
    ifs >> buffer;
    std::size_t clause_number = std::stoull(buffer);
    init_formula.clauses.resize(clause_number);
    for (clause& i : init_formula.clauses) {
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
        i.literals.push_back({init_variables[std::stoull(buffer) - 1], complemented});
      }
    }
  }
}

#endif
