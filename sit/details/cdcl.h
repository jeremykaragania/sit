#ifndef CDCL_H
#define CDCL_H

#include <random>
#include <details/formula.h>
#include <details/sat_solver.h>

namespace sit {
  class cdcl : public sat_solver {
  public:
    cdcl(formula& form, const std::vector<variable>& vars) noexcept : _formula(form), _variables(vars) {}

    bool solve() override {
      _decision_level = 0;
      while (1) {
        if (!unit_propagation()) {
          if (_decision_level == 0) {
            return 0;
          }
          _decision_level = conflict_analysis();
          backtrack(_decision_level);
        }
        else {
          if (all_variables_assigned()) {
            return 1;
          }
          ++_decision_level;
          ++_variables_assigned;
          literal* branch = pick_branching_literal();
          _implication_graph.push_back({branch, 0, 0, _decision_level});
        }
      }
    }
  private:
    struct node { 
      literal* value = nullptr;
      bool has_antecedent = 0;
      std::size_t antecedent = 0;
      std::size_t decision_level = 0;
    };

    bool all_variables_assigned() const noexcept {
      return _variables_assigned == _variables.size();
    }

    literal* pick_branching_literal() const noexcept {
      std::vector<literal*> branching_literals;
      for (clause& i : _formula.clauses()) {
        for (literal& j : i.literals()) {
          if (!j.data().is_assigned()) {
            branching_literals.push_back(&j);
          }
        }
      }
      std::random_device rd;
      std::mt19937 mt(rd());
      std::uniform_int_distribution<> uid(0, branching_literals.size() - 1);
      literal* branching_literal = branching_literals[uid(mt)];
      branching_literal->data() = !branching_literal->is_complemented();
      return branching_literal;
    }

    bool unit_propagation() noexcept {
      while (1) {
        bool is_unit = 0;
        for (std::size_t i = 0; i < _formula.clauses().size(); ++i) {
          switch(_formula.clauses()[i].state()) {
            case clause_state::unsatisfied:
              _implication_graph.push_back({nullptr, 1, i, _decision_level});
              return 0;
            case clause_state::unit:
              for (literal& j : _formula.clauses()[i].literals()) {
                if (!j.data().is_assigned()) {
                  j.data() = !j.is_complemented();
                  ++_variables_assigned;
                  _implication_graph.push_back({&j, 1, i, _decision_level});
                }
              }
              is_unit = 1;
              break;
            default:
              break;
          }
        }
        if (!is_unit) {
          break;
        }
      }
      return 1;
    }

    node* find_node(const literal& l) {
      for (node& i : _implication_graph) {
        if (&l.data() == &i.value->data()) {
          return &i;
        }
      }
      throw;
    }

    clause resolve(const node& lhs, const clause& rhs) const noexcept {
      clause ret;
      for (const literal& i : _formula.clauses()[lhs.antecedent].literals()) {
        if (&i.data() != &lhs.value->data()) {
          ret.literals().push_back(i);
        }
      }
      for (const literal& i : rhs.literals()) {
        if (&i.data() != &lhs.value->data()) {
          ret.literals().push_back(i);
        }
      }
      ret = ret.simplify();
      return ret;
    }

    std::size_t conflict_analysis() noexcept {
      clause learned = _formula.clauses()[_implication_graph.back().antecedent];
      _implication_graph.pop_back();
      learned = learned.simplify();
      while (1) {
        std::size_t literal_count = 0;
        node* premise = nullptr;
        for (const literal& i : learned.literals()) {
          node* n = find_node(i);
          if (n->decision_level == _decision_level) {
            ++literal_count;
            if (n->has_antecedent) {
              premise = n;
            }
          }
        }
        if (literal_count == 1) {
          break;
        }
        learned = resolve(*premise, learned);
      }
      _formula.clauses().push_back(learned);
      std::size_t ret = 0;
      if (learned.literals().size() > 0) {
        std::size_t first = _decision_level;
        for (const literal& i : learned.literals()) {
          std::size_t decision_level = find_node(i)->decision_level;
          if (decision_level < first && decision_level > ret) {
            ret = decision_level;
          }
        }
      }
      return ret;
    }

    void backtrack(const std::size_t b) noexcept {
      std::size_t new_size = 0;
      for (std::size_t i = 0; i < _implication_graph.size(); ++i) {
        if (_implication_graph[i].decision_level > b) {
          new_size = i;
          break;
        }
      }
      for (std::size_t i = new_size; i < _implication_graph.size(); ++i) {
        _implication_graph[i].value->data().unassign();
        --_variables_assigned;
      }
      _implication_graph.resize(new_size);
    }

    formula& _formula;
    const std::vector<variable>& _variables;
    std::vector<node> _implication_graph;
    std::size_t _decision_level;
    std::size_t _variables_assigned;
  };
}

#endif
