#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <cstddef>
#include <iosfwd>
#include <set>
#include <string>
#include <vector>
#include <map>
#include <iostream>

namespace fa
{

  constexpr char Epsilon = '\0';

  class Automaton
  {
  public:
    std::map<int, std::map<char, std::set<int>>> m_statesTransitions;

    std::map<int, int> m_states;
    /**
     * the second element beeing in binary :
     * 0b00 -> is neither initial or final
     * 0b01 -> is initial
     * 0b10 -> is final
     * 0b11 -> is initial and final
     */

    std::set<char> m_alphabet;

    /**
     * Build an empty automaton (no state, no transition).
     */
    Automaton();

    /**
     * Tell if an automaton is valid.
     *
     * A valid automaton has a non-empty set of states and a non-empty set of symbols
     */
    bool isValid() const;

    /**
     * Add a symbol to the automaton
     *
     * Epsilon is not a valid symbol.
     * Returns true if the symbol was effectively added
     */
    bool addSymbol(char symbol);

    /**
     * Remove a symbol from the automaton
     *
     * Returns true if the symbol was effectively removed
     */
    bool removeSymbol(char symbol);

    /**
     * Tell if the symbol is present in the automaton
     */
    bool hasSymbol(char symbol) const;

    /**
     * Count the number of symbols
     */
    std::size_t countSymbols() const;

    /**
     * Add a state to the automaton.
     *
     * By default, a newly added state is not initial and not final.
     * Returns true if the state was effectively added and false otherwise.
     */
    bool addState(int state);

    /**
     * Remove a state from the automaton.
     *
     * The transitions involving the state are also removed.
     * Returns true if the state was effectively removed and false otherwise.
     */
    bool removeState(int state);

    /**
     * Tell if the state is present in the automaton.
     */
    bool hasState(int state) const;

    /**
     * Compute the number of states.
     */
    std::size_t countStates() const;

    /**
     * Set the state initial.
     */
    void setStateInitial(int state);

    /**
     * Tell if the state is initial.
     */
    bool isStateInitial(int state) const;

    /**
     * Set the state final.
     */
    void setStateFinal(int state);

    /**
     * Tell if the state is final.
     */
    bool isStateFinal(int state) const;

    /**
     * Add a transition
     *
     * Returns true if the transition was effectively added and false otherwise.
     * If one of the state or the symbol does not exists, the transition is not added.
     */
    bool addTransition(int from, char alpha, int to);

    /**
     * Remove a transition
     *
     * Returns true if the transition was effectively removed and false otherwise.
     */
    bool removeTransition(int from, char alpha, int to);

    /**
     * Tell if a transition is present.
     */
    bool hasTransition(int from, char alpha, int to) const;

    /**
     * Compute the number of transitions.
     */
    std::size_t countTransitions() const;

    /**
     * Print the automaton in a friendly way
     */
    void prettyPrint(std::ostream &os) const;

    /**
     * Print the automaton with respect to the DOT specification
     */
    void dotPrint(std::ostream &os) const;

    /**
     * Tell if the automaton has one or more epsilon-transition
     */
    bool hasEpsilonTransition() const;

    /**
     * Tell if the automaton is deterministic
     */
    bool isDeterministic() const;

    /**
     * Tell if the automaton is complete
     */
    bool isComplete() const;

    /**
     * Make a transition from a set of states with a character.
     */
    std::set<int> makeTransition(const std::set<int> &origin, char alpha) const;

    /**
     * Read the string and compute the state set after traversing the automaton
     */
    std::set<int> readString(const std::string &word) const;

    /**
     * Tell if the word is in the language accepted by the automaton
     */
    bool match(const std::string &word) const;

    /**
     * Remove non-accessible states
     */
    void removeNonAccessibleStates();

    /**
     * Remove non-co-accessible states
     */
    void removeNonCoAccessibleStates();

    /**
     * Check if the language of the automaton is empty
     */
    bool isLanguageEmpty() const;

    /**
     * Tell if the intersection with another automaton is empty
     */
    bool hasEmptyIntersectionWith(const Automaton &other) const;

    /**
     * Tell if the langage accepted by the automaton is included in the
     * language accepted by the other automaton
     */
    bool isIncludedIn(const Automaton &other) const;

    /**
     * Create a mirror automaton
     */
    static Automaton createMirror(const Automaton &automaton);

    /**
     * Create a complete automaton, if not already complete
     */
    static Automaton createComplete(const Automaton &automaton);

    /**
     * Create a complement automaton
     */
    static Automaton createComplement(const Automaton &automaton);

    /**
     * Create the intersection of the languages of two automata
     */
    static Automaton createIntersection(const Automaton &lhs, const Automaton &rhs);

    /**
     * Create a deterministic automaton, if not already deterministic
     */
    static Automaton createDeterministic(const Automaton &other);

    /**
     * Create an equivalent minimal automaton with the Moore algorithm
     */
    static Automaton createMinimalMoore(const Automaton &other);

    /**
     * Create an equivalent minimal automaton with the Brzozowski algorithm
     */
    static Automaton createMinimalBrzozowski(const Automaton &other);

    /**
     * Create an equivalent automaton with the epsilon
     * transition removed
     */
    static Automaton createWithoutEpsilon(const Automaton &automaton);

  private:
    void helperMakeTransition(const std::set<int> &origin, std::set<int> &result) const;
    /**
     * An implementation of the depth-first search algorithm
     * from a given state the set of states that can be reached
     * is stored in the visited set
     */
    void depthFirstSearch(int from, std::set<int> &visited) const;

    /**
     * Tell if the automaton has a transition from a state to another with any symbol
     */
    bool hasTransitionNoSymbol(int from, int to) const;

    /**
     * Returns a simple valid automaton with one initial state, one alphabet and no transition
     */
    static Automaton createBaseValid(const std::set<char> &alphabet);
  };
}

#endif // AUTOMATON_H