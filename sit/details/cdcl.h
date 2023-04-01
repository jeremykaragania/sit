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
          literal* branch = pick_branching_literal();
          _implication_graph.push_back({branch, nullptr, _decision_level});
        }
      }
    }

  private:
    struct node { 
      literal* lit;
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

    literal* pick_branching_literal() {
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
      branching_literal->data() = 1 != branching_literal->is_complemented();
      return branching_literal;
    }

    bool unit_propagation() {
      for (clause& i : _formula.clauses()) {
        switch(i.state()) {
          case clause_state::unsatisfied:
            _implication_graph.push_back({nullptr, &i, _decision_level});
            return 0;
          case clause_state::unit:
            for (literal& j : i.literals()) {
              if (!j.data().is_assigned()) {
                j.data() = !j.is_complemented();
                _implication_graph.push_back({&j, &i, _decision_level});
              }
            }
            if (!unit_propagation()) {
              return 0;
            }
        }
      }
      return 1;
    }

    clause resolve(node& lhs, clause& rhs) {
      clause ret;
      literal* remove = lhs.lit;
      bool removed = 0;
      for (literal& i : lhs.ant->literals()) {
        if (!removed && &i.data() == &remove->data()) {
          removed = 1;
          continue;
        }
        ret.literals().push_back(i);
      }
      removed = 0;
      for (literal& i : rhs.literals()) {
        if (!removed && &i.data() == &remove->data() && remove->is_complemented() != i.is_complemented()) {
          removed = 1;
          continue;
        }
        ret.literals().push_back(i);
      }
      ret.simplify();
      return ret;
    }

    bool predicate(std::vector<node*> dl_nodes, clause& w, node* n) {
      if (n->ant == nullptr || n->dl != _decision_level) {
        return 0;
      }
      for (literal& i : w.literals()) {
        if (n->lit == &i) {
          return 1;
        }
      }
      return 0;
    }

    std::size_t lits_in_decision_level(std::vector<node*>& dl_nodes, clause& w) {
      std::size_t ret = 0;
      for (node* i : dl_nodes) {
        for (literal& j : w.literals()) {
          if (i->lit == &j) {
            ++ret;
          }
        }
      }
      return ret;
    }

    std::size_t decision_level(const literal* l) {
      if (!l->data().is_assigned()) {
        throw;
      }
      std::size_t ret;
      for (node& i : _implication_graph) {
        if (l == i.lit) {
          ret = i.dl;
          break;
        }
      }
      return ret;
    }

    std::size_t conflict_analysis() {
      std::vector<node*> dl_nodes;
      for (std::size_t i = 2; i <= _implication_graph.size(); ++i) {
        if (_implication_graph[_implication_graph.size() - i].dl != _decision_level) {
          break;
        }
        dl_nodes.push_back(&_implication_graph[_implication_graph.size() - i]);
      }
      clause learned = *_implication_graph.back().ant;
      _implication_graph.pop_back();
      for (std::size_t i = 0; i < dl_nodes.size(); ++i) {
        if (lits_in_decision_level(dl_nodes, learned) == 1) {
          break;
        }
        else if (predicate(dl_nodes, learned, dl_nodes[i]) == 1) {
          learned = resolve(*dl_nodes[i], learned);
        }
      }
      _formula.clauses().push_back(learned);
      std::size_t ret = 0;
      if (learned.literals().size() > 0) {
        std::size_t first = _decision_level;
        for (literal& i : learned.literals()) {
          std::size_t dl = decision_level(&i);
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
        if (i.dl > b) {
          i.lit->data().is_assigned() = !i.lit->data().is_assigned();
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
