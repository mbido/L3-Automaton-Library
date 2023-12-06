#include "Automaton.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>

namespace fa
{

  Automaton::Automaton()
  {
  }

  bool Automaton::isValid() const
  {
    return !m_states.empty() && !m_alphabet.empty();
  }

  //----------Methods on Symbols------------------

  bool Automaton::addSymbol(char symbol)
  {
    if (!isgraph(symbol) || hasSymbol(symbol))
    {
      return false;
    }
    m_alphabet.insert(symbol);
    return true;
  }

  bool Automaton::removeSymbol(char symbol)
  {
    auto iterator = m_alphabet.find(symbol);
    if (iterator == m_alphabet.end())
    {
      return false;
    }
    m_alphabet.erase(iterator);

    // removing every transition with this symbol in it
    auto iteratorTransitions = m_statesTransitions.begin();
    while (iteratorTransitions != m_statesTransitions.end())
    {
      auto &[fromState, withSymbol] = *iteratorTransitions;
      auto iteratorSymbol = withSymbol.find(symbol);
      if (iteratorSymbol != withSymbol.end())
      {
        withSymbol.erase(iteratorSymbol);
        if (withSymbol.empty())
        {
          iteratorTransitions = m_statesTransitions.erase(iteratorTransitions);
        }
        else
        {
          ++iteratorTransitions;
        }
      }
      else
      {
        ++iteratorTransitions;
      }
    }
    return true;
  }

  bool Automaton::hasSymbol(char symbol) const
  {
    return m_alphabet.find(symbol) != m_alphabet.end();
  }

  std::size_t Automaton::countSymbols() const
  {
    return m_alphabet.size();
  }

  //----------Methods on States------------------

  bool Automaton::addState(int state)
  {
    if (hasState(state) || state < 0)
      return false;
    m_states[state] = 0b00;
    return true;
  }

  bool Automaton::hasState(int state) const
  {
    return m_states.find(state) != m_states.end();
  }

  bool Automaton::removeState(int state)
  {
    if (!hasState(state))
      return false;

    m_statesTransitions.erase(state);

    for (auto iteratorStates = m_statesTransitions.begin(); iteratorStates != m_statesTransitions.end();)
    {
      auto &[fromState, withSymbol] = *iteratorStates;
      for (auto iteratorSymbol = withSymbol.begin(); iteratorSymbol != withSymbol.end();)
      {
        auto &[symbol, toStates] = *iteratorSymbol;
        if (toStates.count(state) > 0)
        {
          toStates.erase(state);
          if (toStates.empty())
          {
            iteratorSymbol = withSymbol.erase(iteratorSymbol);
          }
          else
          {
            ++iteratorSymbol;
          }
        }
        else
        {
          ++iteratorSymbol;
        }
      }
      if (withSymbol.empty())
      {
        iteratorStates = m_statesTransitions.erase(iteratorStates);
      }
      else
      {
        ++iteratorStates;
      }
    }

    m_states.erase(state);
    return true;
  }

  std::size_t Automaton::countStates() const
  {
    return m_states.size();
  }

  void Automaton::setStateInitial(int state)
  {
    if (hasState(state))
    {
      m_states[state] |= 1; // 0b01
    }
  }

  bool Automaton::isStateInitial(int state) const
  {
    if (hasState(state))
    {
      return m_states.at(state) & 0b01;
    }
    return false;
  }

  void Automaton::setStateFinal(int state)
  {
    if (hasState(state))
    {
      m_states[state] |= 2; // 0b10
    }
  }

  bool Automaton::isStateFinal(int state) const
  {
    if (hasState(state))
    {
      return m_states.at(state) & 0b10;
    }
    return false;
  }

  //-----------------Methods on Transitions------------------

  bool Automaton::addTransition(int from, char alpha, int to)
  {
    if (!hasState(from) || !hasState(to) || (!hasSymbol(alpha) && alpha != Epsilon) || hasTransition(from, alpha, to))
    {
      return false;
    }
    m_statesTransitions[from][alpha].insert(to);
    return true;
  }

  bool Automaton::removeTransition(int from, char alpha, int to)
  {
    if (!hasTransition(from, alpha, to))
    {
      return false;
    }
    m_statesTransitions[from][alpha].erase(to);
    if (m_statesTransitions[from][alpha].empty())
    {
      m_statesTransitions[from].erase(alpha);
      if (m_statesTransitions[from].empty())
      {
        m_statesTransitions.erase(from);
      }
    }
    return true;
  }

  bool Automaton::hasTransition(int from, char alpha, int to) const
  {
    if (!hasState(from) || !hasState(to) || (!hasSymbol(alpha) && alpha != Epsilon))
    {
      return false;
    }

    auto it = m_statesTransitions.find(from);
    if (it == m_statesTransitions.end())
    {
      return false;
    }

    auto it2 = it->second.find(alpha);
    if (it2 == it->second.end())
    {
      return false;
    }

    return it2->second.find(to) != it2->second.end();
  }

  std::size_t Automaton::countTransitions() const
  {
    std::size_t count = 0;
    for (const auto &state : m_statesTransitions)
    {
      for (const auto &symbol : state.second)
      {
        count += symbol.second.size();
      }
    }
    return count;
  }

  void Automaton::prettyPrint(std::ostream &os) const
  {
    os << "Inital states :\n";
    // iterate over the states -> map
    bool first = true;
    for (auto const &state : m_states)
    {
      if (isStateInitial(state.first))
      {
        if (first)
        {
          os << "\t";
          first = false;
        }
        os << state.first << " ";
      }
    }
    os << "\nFinal states :\n";
    // iterate over the states -> map
    first = true;
    for (auto const &state : m_states)
    {
      if (isStateFinal(state.first))
      {
        if (first)
        {
          os << "\t";
          first = false;
        }
        os << state.first << " ";
      }
    }

    std::map<int, std::map<char, std::set<int>>> transitions = m_statesTransitions;
    os << "\nTransitions :";
    // iterate over the states
    for (auto iterator = m_states.begin(); iterator != m_states.end(); iterator++)
    {
      int state = iterator->first;
      os << "\n\t For state " << state << " : ";
      // iterate over the alphabet
      for (auto it = m_alphabet.begin(); it != m_alphabet.end(); it++)
      {
        os << "\n\t\t For lettre " << *it << " :";
        // iterate over the image states
        for (auto it2 = transitions[state][*it].begin(); it2 != transitions[state][*it].end(); it2++)
        {
          os << " " << *it2;
        }
      }
    }
    os << "\n";
  }

  void Automaton::dotPrint(std::ostream &os) const
  {
    // initialize the nodes
    os << "digraph G {" << std::endl;
    os << "\trankdir=LR;" << std::endl;
    for (auto state : m_states)
    {
      os << "\t" << state.first << " [";
      if (isStateFinal(state.first))
      {
        os << "shape=doublecircle";
      }
      else
      {
        os << "shape=circle";
      }
      os << "];" << std::endl;

      if (isStateInitial(state.first))
      {
        os << "\t"
           << "X" << state.first << " [style=invis];" << std::endl;
        os << "\t"
           << "X" << state.first << " -> " << state.first << ";" << std::endl;
      }
    }
    // initialize the transitions
    for (auto state : m_statesTransitions)
    {
      for (auto symbol : state.second)
      {
        for (const auto &toState : symbol.second)
        {
          os << "\t" << state.first << " -> " << toState << " [label=\"";
          if (symbol.first == Epsilon)
          {
            os << "epsilon";
          }
          else
          {
            os << symbol.first;
          }
          os << "\"];\n";
        }
      }
    }
    os << "}";
  }

  // bool Automaton::hasEpsilonTransition() const
  // {
  //   return false;
  // }

  bool Automaton::hasEpsilonTransition() const
  {
    assert(isValid());
    for (const auto &state : m_statesTransitions)
    {
      if (state.second.find(Epsilon) != state.second.end())
      {
        return true;
      }
    }
    return false;
  }

  bool Automaton::isDeterministic() const
  {
    assert(isValid());
    if (hasEpsilonTransition())
    {
      return false;
    }
    int nbInitialStates = 0;
    for (const auto &state : m_states)
    {
      if (isStateInitial(state.first) && ++nbInitialStates > 1)
      {
        return false;
      }
    }
    for (const auto &state : m_statesTransitions)
    {
      for (const auto &symbol : state.second)
      {
        if (symbol.second.size() > 1)
        {
          return false;
        }
      }
    }
    return nbInitialStates != 0;
  }

  bool Automaton::isComplete() const
  {
    assert(isValid());
    for (const auto &state : m_states)
    {
      for (const auto &symbol : m_alphabet)
      {
        auto it = m_statesTransitions.find(state.first);
        if (it == m_statesTransitions.end() || it->second.find(symbol) == it->second.end())
        {
          return false;
        }
      }
    }
    return true;
  }

  bool Automaton::isIncludedIn(const Automaton &other) const // !!! testing transition with letter different not accessible
  {
    assert(isValid());
    assert(other.isValid());

    Automaton base = *this;
    base.removeNonAccessibleStates();
    base.removeNonCoAccessibleStates();

    // checking if there is any useful symbol not in the other automaton
    std::set<char> usefulSymbols;
    std::set_difference(base.m_alphabet.begin(), base.m_alphabet.end(), other.m_alphabet.begin(), other.m_alphabet.end(),
                        std::inserter(usefulSymbols, usefulSymbols.begin()));
    for(const auto &symbol : usefulSymbols)
    {
      // checking if there is any transition with this symbol
      for(const auto &state : base.m_states)
      {
        if(base.m_statesTransitions.find(state.first) != base.m_statesTransitions.end())
        {
          if(base.m_statesTransitions.at(state.first).find(symbol) != base.m_statesTransitions.at(state.first).end())
          {
            return false;
          }
        }
      }
    }

    Automaton complement = createComplement(other);
    
    return base.hasEmptyIntersectionWith(complement);
  }

  Automaton Automaton::createMirror(const Automaton &automaton)
  {
    assert(automaton.isValid());
    Automaton mirrorAutomaton;
    mirrorAutomaton.m_states = automaton.m_states;
    mirrorAutomaton.m_alphabet = automaton.m_alphabet;
    for (const auto &state : automaton.m_statesTransitions)
    {
      for (const auto &symbol : state.second)
      {
        for (const auto &toState : symbol.second)
        {
          mirrorAutomaton.addTransition(toState, symbol.first, state.first);
        }
      }
    }
    /** changing the states of the automaton
     * 0b00 -> 0b00
     * 0b01 -> 0b10
     * 0b10 -> 0b01
     * 0b11 -> 0b11
     *
     * We just swap the last two bits
     */
    for (const auto &state : automaton.m_states)
    {
      mirrorAutomaton.m_states[state.first] = ((state.second & 0b01) << 1) | ((state.second & 0b10) >> 1);
    }
    return mirrorAutomaton;
  }

  Automaton Automaton::createComplete(const Automaton &automaton)
  {
    assert(automaton.isValid());
    Automaton completeAutomaton = automaton;
    bool alreadyComplete = true;
    if (completeAutomaton.isLanguageEmpty())
    {
      return createBaseValid(automaton.m_alphabet);
    }

    // creating a bin state

    int binState = 0;
    while (completeAutomaton.hasState(binState))
    {
      binState++;
    }
    completeAutomaton.addState(binState);

    auto notSeenStates = completeAutomaton.m_states;

    for (const auto &transition : completeAutomaton.m_statesTransitions)
    {
      int state = transition.first;
      notSeenStates.erase(state);
      auto mapTransition = transition.second;
      for (const auto &symbol : completeAutomaton.m_alphabet)
      {
        if (mapTransition.find(symbol) == mapTransition.end())
        {
          completeAutomaton.addTransition(state, symbol, binState);
          alreadyComplete = false;
        }
      }
    }
    for (const auto &state : notSeenStates)
    {
      for (const auto &symbol : completeAutomaton.m_alphabet)
      {
        completeAutomaton.addTransition(state.first, symbol, binState);
        if (state.first != binState)
        {
          alreadyComplete = false;
        }
      }
    }
    if (alreadyComplete)
    {
      completeAutomaton.removeState(binState);
    }

    return completeAutomaton;
  }

  Automaton Automaton::createComplement(const Automaton &automaton)
  {
    assert(automaton.isValid());
    Automaton complement = createComplete(createDeterministic(automaton));
    for (const auto &state : complement.m_states)
    {
      complement.m_states[state.first] ^= 0b10;
    }
    return complement;
  }

  Automaton Automaton::createIntersection(const Automaton &lhs, const Automaton &rhs)
  {
    assert(lhs.isValid() && rhs.isValid());

    Automaton intersection;
    std::map<int, std::pair<int, int>> newStatesName;
    int currentNewStateName = 0;
    intersection.m_alphabet = lhs.m_alphabet;

    for (const auto &stateL : lhs.m_states)
    {
      for (const auto &stateR : rhs.m_states)
      {
        intersection.addState(currentNewStateName);
        newStatesName.emplace(currentNewStateName, std::make_pair(stateL.first, stateR.first));

        bool isInitial = lhs.isStateInitial(stateL.first) && rhs.isStateInitial(stateR.first);
        bool isFinal = lhs.isStateFinal(stateL.first) && rhs.isStateFinal(stateR.first);

        if (isInitial)
        {
          intersection.setStateInitial(currentNewStateName);
        }
        if (isFinal)
        {
          intersection.setStateFinal(currentNewStateName);
        }

        currentNewStateName++;
      }
    }

    if (!intersection.isValid())
    {
      return createBaseValid(lhs.m_alphabet);
    }

    for (const auto &s1 : intersection.m_states)
    {
      for (const auto &s2 : intersection.m_states)
      {
        for (const auto &a : intersection.m_alphabet)
        {
          int s1Lhs = newStatesName[s1.first].first;
          int s1Rhs = newStatesName[s1.first].second;
          int s2Lhs = newStatesName[s2.first].first;
          int s2Rhs = newStatesName[s2.first].second;

          if (lhs.hasTransition(s1Lhs, a, s2Lhs) && rhs.hasTransition(s1Rhs, a, s2Rhs))
          {
            intersection.addTransition(s1.first, a, s2.first);
          }
        }
      }
    }
    return intersection;
  }

  Automaton Automaton::createDeterministic(const Automaton &other)
  {
    assert(other.isValid());
    Automaton determinist;
    if (other.isDeterministic())
    {
      determinist = createWithoutEpsilon(other);
      determinist.removeNonAccessibleStates();
      return determinist;
    }

    bool hasInitialState = std::any_of(other.m_states.begin(), other.m_states.end(), [&](const auto &state)
                                       { return other.isStateInitial(state.first); });

    if (!hasInitialState)
    {
      return createBaseValid(other.m_alphabet);
    }

    Automaton base = createWithoutEpsilon(other);
    base.removeNonCoAccessibleStates();


    determinist.m_alphabet = base.m_alphabet;
    std::queue<std::set<int>> statesQueue;
    std::map<std::set<int>, int> statesMap;
    std::set<int> currentState;

    bool isInitialAlsoFinal = false;
    for (const auto &state : base.m_states)
    {
      if (base.isStateInitial(state.first))
      {
        currentState.insert(state.first);
        if (base.isStateFinal(state.first))
        {
          isInitialAlsoFinal = true;
        }
      }
    }

    statesQueue.push(currentState);
    statesMap.emplace(currentState, 0);
    determinist.addState(0);
    determinist.setStateInitial(0);
    if (isInitialAlsoFinal)
    {
      determinist.setStateFinal(0);
    }

    while (!statesQueue.empty())
    {
      currentState = statesQueue.front();
      statesQueue.pop();
      int currentStateId = statesMap[currentState];

      for (auto symbol : determinist.m_alphabet)
      {

        std::set<int> newState = base.makeTransition(currentState, symbol);

        if (newState.empty())
        {
          continue;
        }

        auto [it, inserted] = statesMap.emplace(newState, statesMap.size());
        if (inserted)
        {
          statesQueue.push(newState);
          determinist.addState(it->second);

          if (std::any_of(newState.begin(), newState.end(), [&](int state)
                          { return base.isStateFinal(state); }))
          {
            determinist.setStateFinal(it->second);
          }
        }

        determinist.addTransition(currentStateId, symbol, it->second);
      }
    }

    if (!determinist.isValid())
    {
      return createBaseValid(base.m_alphabet);
    }
    return determinist;
  }

  Automaton Automaton::createMinimalMoore(const Automaton &base)
  {
    assert(base.isValid());
    Automaton automaton = createComplete(createDeterministic(base));

    std::map<int, int> lastFamily;
    std::map<int, int> currentFamily;
    for (const auto &state : automaton.m_states)
    {
      currentFamily[state.first] = automaton.isStateFinal(state.first) ? 0 : 1;
    }

    std::map<int, std::vector<int>> familyTransitions;
    do
    {
      familyTransitions.clear();
      lastFamily = std::move(currentFamily);
      currentFamily.clear();
      for (const auto &state : automaton.m_states)
      {
        auto &transitions = familyTransitions[state.first];
        transitions.reserve(automaton.m_alphabet.size() + 1);
        transitions.emplace_back(lastFamily[state.first]);
        for (const auto &symbol : automaton.m_alphabet)
        {
          int toState = *automaton.makeTransition({state.first}, symbol).begin();
          transitions.emplace_back(lastFamily[toState]);
        }
        bool found = false;
        for (const auto &pair : familyTransitions)
        {
          if (pair.first != state.first && pair.second == transitions)
          {
            currentFamily[state.first] = currentFamily[pair.first];
            found = true;
            break;
          }
        }
        if (!found)
        {
          currentFamily[state.first] = currentFamily.size();
        }
      }
    } while (lastFamily != currentFamily);

    Automaton minimal;
    minimal.m_alphabet = automaton.m_alphabet;
    for (const auto &state : currentFamily)
    {
      minimal.addState(state.second);
    }
    for (const auto &from : automaton.m_statesTransitions)
    {
      for (const auto &symbol : from.second)
      {
        for (const auto &to : symbol.second)
        {
          minimal.addTransition(currentFamily[from.first], symbol.first, currentFamily[to]);
          if (automaton.isStateInitial(from.first))
          {
            minimal.setStateInitial(currentFamily[from.first]);
          }
          if (automaton.isStateFinal(from.first))
          {
            minimal.setStateFinal(currentFamily[from.first]);
          }
        }
      }
    }

    if (!minimal.isValid())
    {
      return createBaseValid(base.m_alphabet);
    }

    return minimal;
  }

  Automaton Automaton::createMinimalBrzozowski(const Automaton &other)
  {
    assert(other.isValid());

    Automaton mirrored = createMirror(other);
    Automaton deterministic = createDeterministic(mirrored);
    Automaton mirroredAgain = createMirror(deterministic);
    Automaton deterministicAgain = createDeterministic(mirroredAgain);
    Automaton res = createComplete(deterministicAgain);

    return res;
  }

  void Automaton::helperMakeTransition(const std::set<int> &origin, std::set<int> &result) const
  {
    std::set<int> addedStates;
    for (const auto &state : origin)
    {
      const auto &transitionIterator = m_statesTransitions.find(state);
      if (transitionIterator != m_statesTransitions.end())
      {
        const auto &symbolMap = (*transitionIterator).second;
        const auto &statesToAddIterator = symbolMap.find(Epsilon);
        if (statesToAddIterator != symbolMap.end())
        {
          const auto &statesToAdd = (*statesToAddIterator).second;
          for (const auto &st : statesToAdd)
          {
            if (result.insert(st).second)
              addedStates.insert(st);
          }
        }
      }
    }
    if (!addedStates.empty())
      helperMakeTransition(addedStates, result);
  }

  std::set<int> Automaton::makeTransition(const std::set<int> &origin, char alpha) const
  {
    assert(isValid());
    std::set<int> result;
    for (const auto &state : origin)
    {
      const auto &transitionIterator = m_statesTransitions.find(state);
      if (transitionIterator != m_statesTransitions.end())
      {
        const auto &symbolMap = (*transitionIterator).second;
        const auto &statesToAddIterator = symbolMap.find(alpha);
        if (statesToAddIterator != symbolMap.end())
        {
          const auto &statesToAdd = (*statesToAddIterator).second;
          for (const auto &st : statesToAdd)
          {
            result.insert(st);
          }
        }
      }
    }
    helperMakeTransition(result, result);
    return result;
  }

  std::set<int> Automaton::readString(const std::string &word) const
  {
    assert(isValid());
    std::set<int> result;
    for (const auto &state : m_states)
    {
      if (isStateInitial(state.first))
      {
        result.insert(state.first);
      }
    }

    std::set<int> initialStateModuloByEpsilonTransitions = makeTransition(result, Epsilon);
    result.insert(initialStateModuloByEpsilonTransitions.begin(), initialStateModuloByEpsilonTransitions.end());

    for (std::size_t i = 0; i < word.size(); i++)
    {
      result = makeTransition(result, word[i]);
    }
    return result;
  }

  bool Automaton::match(const std::string &word) const
  {
    assert(isValid());
    std::set<int> result = readString(word);
    for (const auto &state : result)
    {
      if (isStateFinal(state))
      {
        return true;
      }
    }
    return false;
  }

  bool Automaton::hasTransitionNoSymbol(int from, int to) const
  {
    assert(isValid());
    auto iterator = m_statesTransitions.find(from);
    if (iterator == m_statesTransitions.end())
    {
      return false;
    }
    for (auto &symbol : iterator->second)
    {
      if (std::find(symbol.second.begin(), symbol.second.end(), to) != symbol.second.end())
      {
        return true;
      }
    }
    return false;
  }

  void Automaton::depthFirstSearch(int from, std::set<int> &visited) const
  {
    visited.insert(from);
    for (const auto &state : m_states)
    {
      if (!visited.count(state.first) && hasTransitionNoSymbol(from, state.first))
      {
        depthFirstSearch(state.first, visited);
      }
    }
  }

  bool Automaton::isLanguageEmpty() const
  {
    assert(isValid());
    std::set<int> visited;
    for (const auto &state : m_states)
    {
      if (isStateInitial(state.first))
      {
        depthFirstSearch(state.first, visited);
        for (auto reachedState : visited)
        {
          if (isStateFinal(reachedState))
          {
            return false;
          }
        }
      }
    }
    return true;
  }

  bool Automaton::hasEmptyIntersectionWith(const Automaton &other) const
  {
    assert(isValid());
    assert(other.isValid());

    // to avoid making useless call to complex methods like createIntersection
    if (this->isLanguageEmpty() || other.isLanguageEmpty())
    {
      return true;
    }

    Automaton intersection = Automaton::createIntersection(*this, other);
    return intersection.isLanguageEmpty();
  }

  void Automaton::removeNonAccessibleStates()
  {
    assert(isValid());
    std::set<int> visited;
    std::vector<int> statesToRemove;

    for (auto state : m_states)
    {
      if (isStateInitial(state.first))
      {
        depthFirstSearch(state.first, visited);
      }
      else
      {
        statesToRemove.push_back(state.first);
      }
    }

    for (int state : statesToRemove)
    {
      if (!visited.count(state))
      {
        removeState(state);
      }
    }

    if (!isValid())
    {
      addState(0);
      setStateInitial(0);
    }
  }

  void Automaton::removeNonCoAccessibleStates()
  {
    assert(isValid());
    *this = createMirror(*this);
    removeNonAccessibleStates();
    if (isValid())
    {
      *this = createMirror(*this);
    }
    else
    {
      addState(0);
      setStateFinal(0);
    }
  }

  Automaton Automaton::createWithoutEpsilon(const Automaton &automaton)
  {
    assert(automaton.isValid());
    Automaton withoutEpsilon = automaton;

    for (auto &state : withoutEpsilon.m_states)
    {
      std::set<int> statesModuleByEpsilon = withoutEpsilon.makeTransition({state.first}, Epsilon);

      for (auto &toState : statesModuleByEpsilon)
      {
        if (withoutEpsilon.isStateFinal(toState))
        {
          withoutEpsilon.setStateFinal(state.first);
        }

        withoutEpsilon.removeTransition(state.first, Epsilon, toState);

        auto iterator = withoutEpsilon.m_statesTransitions.find(toState);
        if (iterator != withoutEpsilon.m_statesTransitions.end())
        {
          for (auto &symbol : iterator->second)
          {
            if (symbol.first == Epsilon)
            {
              continue;
            }

            for (auto &toState2 : symbol.second)
            {
              withoutEpsilon.addTransition(state.first, symbol.first, toState2);
            }
          }
        }
      }
    }

    return withoutEpsilon;
  }

  Automaton Automaton::createBaseValid(const std::set<char> &alphabet)
  {
    Automaton baseValid;
    baseValid.m_states = {{0, 1}};
    baseValid.m_alphabet = alphabet;
    for(const auto &letter : alphabet)
    {
      baseValid.addTransition(0, letter, 0);
    }
    return baseValid;
  }
}