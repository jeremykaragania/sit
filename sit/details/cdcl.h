#ifndef CDCL_H
#define CDCL_H

#include <random>
#include <details/formula.h>
#include <details/sat_solver.h>

namespace sit {
  /*
    Conflict-driven clause learning is driven by conflict to learn new clauses
    to iteratively reduce the size of the search space.
  */
  class cdcl : public sat_solver {
  public:
    /*
      cdcl::cdcl constructs a sit::cdcl from a sit::formula "form" and a
      std::vector of sit::variable "vars". "form" provides the constraint and
      "vars" provides the search space.
    */
    cdcl(formula<cnf>& form, const std::vector<variable>& vars) noexcept : _formula(form), _variables(vars), _decision_level(0), _variables_assigned(0) {}

    /*
      cdcl::solve returns the satisfiability of a sit::formula using
      conflict-driven clause learning.
    */
    bool solve() override {
      while (1) {
        if (!unit_propagation()) {
          if (_decision_level == 0) {
            return 0;
          }
          _decision_level = conflict_analysis();
          backtrack(_decision_level);
        }
        /*
          Unit propagation will not always yield new assignments, therefore, we
          must resort to manually assigning literals with unassigned variables.
        */
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
    /*
      cdcl::node tracks the cause of assignments in the implication graph. If
      we look at a node, we can say its literal is true because of its
      antecedents, or its antecedents imply its literal's truth value. We can't
      keep a pointer to the antecedent as you can't rely on using pointers to
      items in an std::vector, they keep changing.
    */
    struct node {
      literal* value = nullptr;
      bool has_antecedent = 0;
      std::size_t antecedent = 0;
      std::size_t decision_level = 0;
    };

    bool all_variables_assigned() const noexcept {
      return _variables_assigned == _variables.size();
    }

    /*
      cdcl::pick_branching_literal picks a random literal from a pool of
      unassigned literals and returns a pointer to it. It randomly picks a
      literal with an unassigned variable, and assigns its variable a value so
      that the literal evaluates to true. After this assignment, we always
      perform unit propagation.
    */
    literal* pick_branching_literal() const noexcept {
      std::vector<literal*> branching_literals;
      for (clause<cnf>& i : _formula.clauses) {
        for (literal& j : i.literals) {
          if (!j.data.is_assigned()) {
            branching_literals.push_back(&j);
          }
        }
      }
      std::random_device rd;
      std::mt19937 mt(rd());
      std::uniform_int_distribution<> uid(0, branching_literals.size() - 1);
      literal* branching_literal = branching_literals[uid(mt)];
      branching_literal->data = !branching_literal->is_complemented;
      return branching_literal;
    }

    /*
      cdcl::unit_propagation analyses each clause in the formula, hoping it can
      automatically deduce any variable assignments. A clause has the state
      clause_state::unit if all but one literal in it evaluates to false and
      that one literal has an unassigned variable. Therefore, that one literal
      must be true in order for the clause to be true, so unit propagation
      deduces its truth value. But it can't just stop there, it must see if
      this new assignment has made any other clauses unit and continue this
      until there are no more unit clauses.
    */
    bool unit_propagation() noexcept {
      while (1) {
        bool is_unit = 0;
        for (std::size_t i = 0; i < _formula.clauses.size(); ++i) {
          switch(_formula.clauses[i].state()) {
            case clause_state::unsatisfied:
              _implication_graph.push_back({nullptr, 1, i, _decision_level});
              return 0;
            case clause_state::unit:
              for (literal& j : _formula.clauses[i].literals) {
                if (!j.data.is_assigned()) {
                  j.data = !j.is_complemented;
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

    /*
      cdcl::find_node searches for a literal "l" in the implication graph with
      a common underlying variable.
    */
    node* find_node(const literal& l) {
      for (node& i : _implication_graph) {
        if (&l.data == &i.value->data) {
          return &i;
        }
      }
      throw;
    }

    /*
      cdcl::conflict_analysis analyses conflict within the implication graph.
      Conflicts occur as a result of unit propagation when variables have been
      assigned, such that one or more clauses evaluates to false. This is
      always discovered through contradictions of assignments in unit
      propagation. At a high level it is caused by assigning a variable to be
      true in one clause, but in another clause it has to be false. Unit
      propagation has good intentions, it just does not know the future
      consequences of its actions.
    */
    std::size_t conflict_analysis() noexcept {
      clause<cnf> learned = _formula.clauses[_implication_graph.back().antecedent];
      _implication_graph.pop_back();
      learned = learned.simplify();
      while (1) {
        std::size_t literal_count = 0;
        node* premise = nullptr;
        for (const literal& i : learned.literals) {
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
      _formula.clauses.push_back(learned);
      std::size_t ret = 0;
      if (learned.literals.size() > 0) {
        std::size_t first = _decision_level;
        for (const literal& i : learned.literals) {
          std::size_t decision_level = find_node(i)->decision_level;
          if (decision_level < first && decision_level > ret) {
            ret = decision_level;
          }
        }
      }
      return ret;
    }

    /*
      cdcl::resolve resolves two clauses "lhs" and "rhs". The resolution
      operator is defined if both clauses have a common literal, however in one
      clause, the literal is complemented and in the other the literal is
      uncomplemented. The resolution operator then returns a new clause, which
      is a combination of the two clauses with the exception of the common
      literals.
    */
    clause<cnf> resolve(const node& lhs, const clause<cnf>& rhs) const noexcept {
      clause<cnf> ret;
      for (const literal& i : _formula.clauses[lhs.antecedent].literals) {
        if (&i.data != &lhs.value->data) {
          ret.literals.push_back(i);
        }
      }
      for (const literal& i : rhs.literals) {
        if (&i.data != &lhs.value->data) {
          ret.literals.push_back(i);
        }
      }
      ret = ret.simplify();
      return ret;
    }

    /*
      cdcl::backtrack is called after conflict analysis. It simply backtracks
      to the decision level "b" which is returned from cdcl::conflict_analysis.
    */
    void backtrack(const std::size_t b) noexcept {
      std::size_t new_size = 0;
      for (std::size_t i = 0; i < _implication_graph.size(); ++i) {
        if (_implication_graph[i].decision_level > b) {
          new_size = i;
          break;
        }
      }
      for (std::size_t i = new_size; i < _implication_graph.size(); ++i) {
        _implication_graph[i].value->data.unassign();
        --_variables_assigned;
      }
      _implication_graph.resize(new_size);
    }

    formula<cnf>& _formula;
    const std::vector<variable>& _variables;
    std::vector<node> _implication_graph;
    std::size_t _decision_level;
    std::size_t _variables_assigned;
  };
}

#endif
