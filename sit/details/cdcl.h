#ifndef CDCL_H
#define CDCL_H

#include <random>
#include <details/formula.h>
#include <details/sat_solver.h>

namespace sit {
  class cdcl : public sat_solver {
  public:
    cdcl(formula& form, std::vector<variable>& vars) noexcept : _formula(form), _variables(vars) {}

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
          variable* branch = pick_branching_variable();
          _implication_graph.push_back({branch, nullptr, _decision_level});
        }
      }
    }

  private:
    struct node { 
      variable* var;
      clause* ant;
      std::size_t dl;
    };

    bool all_variables_assigned() {
      for (variable& i : _variables) {
        if (!i.is_assigned()) {
          return 0;
        }
      }
      return 1;
    }

    variable* pick_branching_variable() {
      std::vector<literal*> branching_variables;
      for (clause& i : _formula.data()) {
        for (literal& j : i.data()) {
          if (!j.data().is_assigned()) {
            branching_variables.push_back(&j);
          }
        }
      }
      std::random_device rd;
      std::mt19937 mt(rd());
      std::uniform_int_distribution<> uid(0, branching_variables.size() - 1);
      literal* branching_variable = branching_variables[uid(mt)];
      branching_variable->data() = 1 != branching_variable->is_complemented();
      return &branching_variable->data();
    }

    bool unit_propagation() {
      for (clause& i : _formula.data()) {
        switch(i.state()) {
          case clause_state::unsatisfied:
            _implication_graph.push_back({nullptr, &i, _decision_level});
            return 0;
          case clause_state::unit:
            for (literal& j : i.data()) {
              if (!j.data().is_assigned()) {
                j.data() = !j.is_complemented();
                _implication_graph.push_back({&j.data(), &i, _decision_level});
              }
            }
            if (!unit_propagation()) {
              return 0;
            }
        }
      }
      return 1;
    }

    clause resolve(clause& lhs, clause& rhs) {
      clause ret;
      std::vector<literal*> exclude;
      for (literal & i : lhs.data()) {
        bool include = 1;
        for (literal& j : rhs.data()) {
          if (&i.data() == &j.data() && i.is_complemented() != j.is_complemented()) {
            include = 0;
            exclude.push_back(&j);
            break;
          }
          for (literal& k : ret.data()) {
            if (&k.data() == &i.data()) {
              include = 0;
              break;
            }
          }
        }
        if (include) {
          ret.data().push_back(i);
        }
      }
      for (literal& i : rhs.data()) {
        bool include = 1;
        for (literal* j : exclude) {
          if (&i == j) {
            include = 0;
            break;
          }
          for (literal& j : ret.data()) {
            if (&i.data() == &j.data()) {
              include = 0;
              break;
            }
          }
        }
        if (include) {
          ret.data().push_back(i);
        }
      }
      return ret;
    }

    bool predicate(std::vector<node*> dl_nodes, clause& w, variable* v) {
      for (node* i : dl_nodes) {
        for (literal& j : w.data()) {
          if (v == i->var && v == &j.data()) {
            return 1;
          }
        }
      }
      return 0;
    }

    std::size_t vars_in_decision_level(std::vector<node*>& dl_nodes, clause& w) {
      std::size_t ret = 0;
      for (node* i : dl_nodes) {
        for (literal& j : w.data()) {
          if (i->var == &j.data()) {
            ++ret;
          }
        }
      }
      return ret;
    }

    std::size_t decision_level(const variable* v) {
      if (!v->is_assigned()) {
        throw;
      }
      std::size_t ret;
      for (node& i : _implication_graph) {
        if (v == i.var) {
          ret = i.dl;
          break;
        }
      }
      return ret;
    }

    std::size_t conflict_analysis() {
      std::vector<node*> dl_nodes;
      for (std::size_t i = 1; i <= _implication_graph.size(); ++i) {
        if (_implication_graph[_implication_graph.size() - i].dl != _decision_level) {
          break;
        }
        dl_nodes.push_back(&_implication_graph[_implication_graph.size() - i]);
      }
      clause learned = *dl_nodes.front()->ant;
      for (std::size_t i = 1; i < dl_nodes.size(); ++i) {
        if (vars_in_decision_level(dl_nodes, learned) == 1) {
          break;
        }
        else if (predicate(dl_nodes, learned, dl_nodes[i]->var) == 1) {
          learned = resolve(learned, *dl_nodes[i]->ant);
        }
      }
      _formula.data().push_back(learned);
      std::size_t ret = 0;
      if (learned.data().size() > 0) {
        std::size_t first = _decision_level;
        for (literal& i : learned.data()) {
          std::size_t dl = decision_level(&i.data());
          if (dl < first && dl > ret) {
            ret = dl;
          }
        }
      }
      return ret;
    }

    void backtrack(const std::size_t b) {
      std::vector<node> new_ig;
      for (node& i : _implication_graph) {
        if (i.dl >= b) {
          if (i.var != nullptr) {
            i.var->is_assigned() = !i.var->is_assigned();
          }
        }
        else {
          new_ig.push_back(i);
        }
      }
      _implication_graph = new_ig;
    }

    formula& _formula;
    std::vector<variable>& _variables;
    std::vector<node> _implication_graph;
    std::size_t _decision_level;
  };
}

#endif
