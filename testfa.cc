#include "gtest/gtest.h"

#include "Automaton.h"

#include <cstddef>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <climits>
// #include <cstdalign>
#include <queue>
#include <time.h>
#include <fstream>

//#define DEVELOPMENT

//============================================================================
//================== Setting up some special automata =========================

fa::Automaton beginLetterA;
fa::Automaton endLetterA;
fa::Automaton beginAndEndLetterA;
fa::Automaton alternateLetterAThenB;
fa::Automaton alternateLetterAThenBBeginA;
fa::Automaton alternateLetterAThenBEndA;
fa::Automaton alternateLetterAThenBBeginAEndA;
fa::Automaton beginLetterAComplement;
fa::Automaton endLetterAComplement;
fa::Automaton beginAndEndLetterAComplement;
fa::Automaton alternateLetterAThenBComplement;
fa::Automaton alternateLetterAThenBBeginAComplement;
fa::Automaton alternateLetterAThenBEndAComplement;
fa::Automaton alternateLetterAThenBBeginAEndAComplement;
fa::Automaton beginLetterB;
fa::Automaton endLetterB;
fa::Automaton beginAndEndLetterB;
fa::Automaton alternateLetterAThenBBeginB;
fa::Automaton alternateLetterAThenBEndB;
fa::Automaton alternateLetterAThenBBeginBEndB;
fa::Automaton emptyLanguage;
fa::Automaton allWords;

// Automaton not to use because they have epsilon transitions
fa::Automaton td3_exo10;
fa::Automaton td3_exo11;

//============================================================================
//======================= global variables ===================================
std::vector<std::string> wordsToTest;

//============================================================================
//======================= useful functions ===================================

void generateWords(std::string currentWord, long unsigned int maxSize, std::vector<std::string> &words)
{
  words.push_back(currentWord);
  if (currentWord.size() != maxSize)
  {
    generateWords(currentWord + 'a', maxSize, words);
    generateWords(currentWord + 'b', maxSize, words);
  }
}

bool hasOppositeMatchingWords(const fa::Automaton a1, const fa::Automaton a2)
{
  for (std::string word : wordsToTest)
  {
    if (a1.match(word) == a2.match(word))
    {
      std::cout << "Match the word : '" << word << "'" << std::endl;
      return false;
    }
  }
  return true;
}

//============================================================================
//============================== The tests ===================================

TEST(testIsValid, inValidAutomaton)
{
  fa::Automaton automaton;
  EXPECT_FALSE(automaton.isValid());
}
TEST(testIsValid, validAutomaton)
{
  fa::Automaton automaton;
  automaton.addSymbol('a');
  automaton.addState(276);
  EXPECT_TRUE(automaton.isValid());
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(276));
  EXPECT_FALSE(automaton.isStateInitial(276));
  EXPECT_FALSE(automaton.isStateFinal(276));
  // checking the elements size
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countStates(), 1u);
}

//---------Tests on Symbols------------

TEST(testAddSymbol, notInAlphabet)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_TRUE(automaton.addSymbol('a'));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  // checking the elements size
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testAddSymbol, inAlphabet)
{
  fa::Automaton automaton;
  automaton.addSymbol('a');
  // checking the methods
  EXPECT_FALSE(automaton.addSymbol('a'));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  // checking the elements size
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testAddSymbol, afterBeingRemoved)
{
  fa::Automaton automaton;
  automaton.addSymbol('a');
  automaton.removeSymbol('a');
  // checking the methods
  EXPECT_TRUE(automaton.addSymbol('a'));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  // checking the elements size
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testAddSymbol, allInvalidSymbol)
{
  fa::Automaton automaton;
  // checking the methods
  for (int i = 0; i < 256; i++)
  {
    char symbol = static_cast<char>(i);
    if (!isgraph(symbol))
    {
      EXPECT_FALSE(automaton.addSymbol(symbol));
      EXPECT_FALSE(automaton.hasSymbol(symbol));
    }
  }
  // checking the elements size
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testAddSymbol, allValidSymbol)
{
  fa::Automaton automaton;
  // checking the methods
  for (int i = 0; i < 256; i++)
  {
    char symbol = static_cast<char>(i);
    if (isgraph(symbol))
    {
      EXPECT_TRUE(automaton.addSymbol(symbol));
      EXPECT_TRUE(automaton.hasSymbol(symbol));
    }
  }
  // checking the elements size
  EXPECT_EQ(automaton.countSymbols(), 94u);
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testRemoveSymbol, notInAlphabet)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_FALSE(automaton.removeSymbol('a'));
  // checking the elements size
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testRemoveSymbol, inAlphabetAndInTransition)
{
  fa::Automaton automaton;
  automaton.addState(56787);
  automaton.addState(54);
  automaton.addSymbol('a');
  automaton.addTransition(56787, 'a', 54);
  // checking the methods
  EXPECT_TRUE(automaton.removeSymbol('a'));
  // checking the elements
  EXPECT_FALSE(automaton.hasTransition(56787, 'a', 54));
  EXPECT_FALSE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(56787));
  EXPECT_FALSE(automaton.isStateInitial(56787));
  EXPECT_FALSE(automaton.isStateFinal(56787));
  EXPECT_TRUE(automaton.hasState(54));
  EXPECT_FALSE(automaton.isStateInitial(56787));
  EXPECT_FALSE(automaton.isStateFinal(56787));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testHasSymbol, notInAlphabet)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_FALSE(automaton.hasSymbol('a'));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testHasSymbol, inAlphabet)
{
  fa::Automaton automaton;
  automaton.addSymbol('a');
  // checking the methods
  EXPECT_TRUE(automaton.hasSymbol('a'));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a')); // to test if it removes the symbol
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 0u);
}

TEST(testCountSymbol, emptyAlphabet)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_EQ(automaton.countSymbols(), 0u);
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testCountSymbol, nonEmptyAlphabet)
{
  fa::Automaton automaton;
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  // checking the methods
  EXPECT_EQ(automaton.countSymbols(), 2u);
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasSymbol('b'));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 2u);
  EXPECT_EQ(automaton.countStates(), 0u);
}

//---------Tests on States-----------

TEST(testAddState, doesNotExist)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_TRUE(automaton.addState(7));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(7));
  EXPECT_FALSE(automaton.isStateInitial(7));
  EXPECT_FALSE(automaton.isStateFinal(7));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 1u);
}
TEST(testAddState, negativeState)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_FALSE(automaton.addState(-123));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testAddState, alreadyExists)
{
  fa::Automaton automaton;
  automaton.addState(42);
  // checking the methods
  EXPECT_FALSE(automaton.addState(42));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(42));
  EXPECT_FALSE(automaton.isStateInitial(42));
  EXPECT_FALSE(automaton.isStateFinal(42));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 1u);
}
TEST(testAddState, afterBeingRemoved)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.removeState(69);
  // checking the methods
  EXPECT_TRUE(automaton.addState(69));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_FALSE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 1u);
}
TEST(testAddState, stateWithLargeValue)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_TRUE(automaton.addState(INT_MAX));
  // checking the elements
  EXPECT_EQ(automaton.countStates(), 1u);
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 1u);
}

TEST(testHasState, emptySetTable)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_FALSE(automaton.hasState(0));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testHasState, stateExist)
{
  fa::Automaton automaton;
  automaton.addState(567);
  // checking the methods
  EXPECT_TRUE(automaton.hasState(567));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(567));
  EXPECT_FALSE(automaton.isStateInitial(567));
  EXPECT_FALSE(automaton.isStateFinal(567));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 1u);
}
TEST(testHasState, stateDoesNotExist)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_FALSE(automaton.hasState(0));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}

TEST(testRemoveState, stateDoesNotExist)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_FALSE(automaton.removeState(0));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testRemoveState, stateExist)
{
  fa::Automaton automaton;
  automaton.addState(867);
  // checking the methods
  EXPECT_TRUE(automaton.removeState(867));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testRemoveState, stateExistWithTransitions)
{
  fa::Automaton automaton;
  automaton.addState(76);
  automaton.addState(65678);
  automaton.addState(6765544);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(76, 'a', 65678);
  automaton.addTransition(76, 'a', 6765544);      // will be removed
  automaton.addTransition(65678, 'b', 6765544);   // will be removed
  automaton.addTransition(6765544, 'a', 6765544); // will be removed
  automaton.addTransition(6765544, 'b', 76);      // will be removed
  // checking the methods
  EXPECT_TRUE(automaton.removeState(6765544));
  // checking the elements
  EXPECT_TRUE(automaton.hasTransition(76, 'a', 65678));
  EXPECT_TRUE(automaton.hasState(76));
  EXPECT_FALSE(automaton.isStateInitial(76));
  EXPECT_FALSE(automaton.isStateFinal(76));
  EXPECT_TRUE(automaton.hasState(65678));
  EXPECT_FALSE(automaton.isStateInitial(65678));
  EXPECT_FALSE(automaton.isStateFinal(65678));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasSymbol('b'));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 1u);
  EXPECT_EQ(automaton.countSymbols(), 2u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testCountStates, emptyStateTable)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_EQ(automaton.countStates(), 0u);
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testCountStates, nonEmptyStateTable)
{
  fa::Automaton automaton;
  automaton.addState(5678);
  automaton.addState(678);
  // checking the methods
  EXPECT_EQ(automaton.countStates(), 2u);
  // checking the elements
  EXPECT_TRUE(automaton.hasState(5678));
  EXPECT_FALSE(automaton.isStateInitial(5678));
  EXPECT_FALSE(automaton.isStateFinal(5678));
  EXPECT_TRUE(automaton.hasState(678));
  EXPECT_FALSE(automaton.isStateInitial(678));
  EXPECT_FALSE(automaton.isStateFinal(678));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testSetStateInitial, stateDoesNotExist)
{
  fa::Automaton automaton;
  automaton.setStateInitial(0);
  // checking the methods
  EXPECT_FALSE(automaton.isStateInitial(0));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testSetStateInitial, stateExist_Plus_testIsInitialWhenExists)
{
  fa::Automaton automaton;
  automaton.addState(6787654);
  automaton.setStateInitial(6787654);
  // checking the methods
  EXPECT_TRUE(automaton.isStateInitial(6787654));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(6787654));
  EXPECT_TRUE(automaton.isStateInitial(6787654));
  EXPECT_FALSE(automaton.isStateFinal(6787654));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 1u);
}

TEST(testIsStateInitial, stateDoesNotExist)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_FALSE(automaton.isStateInitial(0));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}

TEST(testSetStateFinal, stateDoesNotExist)
{
  fa::Automaton automaton;
  automaton.setStateFinal(0);
  // checking the methods
  EXPECT_FALSE(automaton.isStateFinal(0));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testSetStateFinal, stateExist_Plus_testIsFinalWhenExists)
{
  fa::Automaton automaton;
  automaton.addState(54876);
  automaton.setStateFinal(54876);
  // checking the methods
  EXPECT_TRUE(automaton.isStateFinal(54876));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(54876));
  EXPECT_FALSE(automaton.isStateInitial(54876));
  EXPECT_TRUE(automaton.isStateFinal(54876));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 1u);
}

TEST(testIsStateFinal, stateDoesNotExist)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_FALSE(automaton.isStateFinal(0));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}

//------------Tests on Transitions---------------
TEST(testAddTransition, transitionsThatShouldWork)
{
  fa::Automaton automaton;
  automaton.addState(476);
  automaton.addState(456789);
  automaton.addSymbol('a');
  // checking the methods
  EXPECT_TRUE(automaton.addTransition(476, 'a', 456789));
  // checking the elements
  EXPECT_TRUE(automaton.hasTransition(476, 'a', 456789));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(476));
  EXPECT_FALSE(automaton.isStateInitial(476));
  EXPECT_FALSE(automaton.isStateFinal(476));
  EXPECT_TRUE(automaton.hasState(456789));
  EXPECT_FALSE(automaton.isStateInitial(456789));
  EXPECT_FALSE(automaton.isStateFinal(456789));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 1u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}
TEST(testAddTransition, epsilonTransitions)
{
  fa::Automaton automaton;
  automaton.addState(56776543);
  automaton.addState(9765);
  // checking the methods
  EXPECT_TRUE(automaton.addTransition(56776543, fa::Epsilon, 9765));
  // checking the elements
  EXPECT_TRUE(automaton.hasTransition(56776543, fa::Epsilon, 9765));
  EXPECT_TRUE(automaton.hasState(56776543));
  EXPECT_FALSE(automaton.isStateInitial(56776543));
  EXPECT_FALSE(automaton.isStateFinal(56776543));
  EXPECT_TRUE(automaton.hasState(9765));
  EXPECT_FALSE(automaton.isStateInitial(9765));
  EXPECT_FALSE(automaton.isStateFinal(9765));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 1u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 2u);
}
TEST(testAddTransition, transitionsOnSameState)
{
  fa::Automaton automaton;
  automaton.addState(756);
  automaton.addSymbol('a');
  // checking the methods
  EXPECT_TRUE(automaton.addTransition(756, 'a', 756));
  // checking the elements
  EXPECT_TRUE(automaton.hasTransition(756, 'a', 756));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(756));
  EXPECT_FALSE(automaton.isStateInitial(756));
  EXPECT_FALSE(automaton.isStateFinal(756));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 1u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 1u);
}
TEST(testAddTransition, alreadyExists)
{
  fa::Automaton automaton;
  automaton.addState(546542);
  automaton.addState(54);
  automaton.addSymbol('a');
  automaton.addTransition(546542, 'a', 54);
  // checking the methods
  EXPECT_FALSE(automaton.addTransition(546542, 'a', 54));
  // checking the elements
  EXPECT_TRUE(automaton.hasTransition(546542, 'a', 54));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(546542));
  EXPECT_FALSE(automaton.isStateInitial(546542));
  EXPECT_FALSE(automaton.isStateFinal(546542));
  EXPECT_TRUE(automaton.hasState(54));
  EXPECT_FALSE(automaton.isStateInitial(54));
  EXPECT_FALSE(automaton.isStateFinal(54));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 1u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}
TEST(testAddTransition, stateDoesNotExist)
{
  fa::Automaton automaton;
  automaton.addState(7865);
  automaton.addSymbol('a');
  // checking the methods
  EXPECT_FALSE(automaton.addTransition(7865, 'a', 1));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(7865));
  EXPECT_FALSE(automaton.isStateInitial(7865));
  EXPECT_FALSE(automaton.isStateFinal(7865));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 1u);
}
TEST(testAddTransition, symbolDoesNotExist)
{
  fa::Automaton automaton;
  automaton.addState(676);
  automaton.addState(666);
  // checking the methods
  EXPECT_FALSE(automaton.addTransition(676, 'a', 666));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(676));
  EXPECT_FALSE(automaton.isStateInitial(676));
  EXPECT_FALSE(automaton.isStateFinal(676));
  EXPECT_TRUE(automaton.hasState(666));
  EXPECT_FALSE(automaton.isStateInitial(666));
  EXPECT_FALSE(automaton.isStateFinal(666));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testRemoveTransition, transitionExists)
{
  fa::Automaton automaton;
  automaton.addState(876);
  automaton.addState(12);
  automaton.addSymbol('a');
  automaton.addTransition(876, 'a', 12);
  // checking the methods
  EXPECT_TRUE(automaton.removeTransition(876, 'a', 12));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(876));
  EXPECT_FALSE(automaton.isStateInitial(876));
  EXPECT_FALSE(automaton.isStateFinal(876));
  EXPECT_TRUE(automaton.hasState(12));
  EXPECT_FALSE(automaton.isStateInitial(12));
  EXPECT_FALSE(automaton.isStateFinal(12));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}
TEST(testRemoveTransition, transitionDoesNotExistAnymore)
{
  fa::Automaton automaton;
  automaton.addState(567);
  automaton.addState(76);
  automaton.addSymbol('a');
  automaton.addTransition(567, 'a', 76);
  automaton.removeTransition(567, 'a', 76);
  // checking the methods
  EXPECT_FALSE(automaton.removeTransition(567, 'a', 76));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(567));
  EXPECT_FALSE(automaton.isStateInitial(567));
  EXPECT_FALSE(automaton.isStateFinal(567));
  EXPECT_TRUE(automaton.hasState(76));
  EXPECT_FALSE(automaton.isStateInitial(76));
  EXPECT_FALSE(automaton.isStateFinal(76));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}
TEST(testRemoveTransition, stateDoesNotExist)
{
  fa::Automaton automaton;
  automaton.addState(55);
  automaton.addSymbol('a');
  // checking the methods
  EXPECT_FALSE(automaton.removeTransition(55, 'a', 1));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(55));
  EXPECT_FALSE(automaton.isStateInitial(55));
  EXPECT_FALSE(automaton.isStateFinal(55));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 1u);
}
TEST(testRemoveTransition, symbolDoesNotExist)
{
  fa::Automaton automaton;
  automaton.addState(765);
  automaton.addState(12);
  // checking the methods
  EXPECT_FALSE(automaton.removeTransition(765, 'a', 12));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(765));
  EXPECT_FALSE(automaton.isStateInitial(765));
  EXPECT_FALSE(automaton.isStateFinal(765));
  EXPECT_TRUE(automaton.hasState(12));
  EXPECT_FALSE(automaton.isStateInitial(12));
  EXPECT_FALSE(automaton.isStateFinal(12));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testHasTransition, transitionExists)
{
  fa::Automaton automaton;
  automaton.addState(56);
  automaton.addState(876);
  automaton.addSymbol('a');
  automaton.addTransition(56, 'a', 876);
  // checking the methods
  EXPECT_TRUE(automaton.hasTransition(56, 'a', 876));
  // checking the elements
  EXPECT_TRUE(automaton.hasTransition(56, 'a', 876));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(56));
  EXPECT_FALSE(automaton.isStateInitial(56));
  EXPECT_FALSE(automaton.isStateFinal(56));
  EXPECT_TRUE(automaton.hasState(876));
  EXPECT_FALSE(automaton.isStateInitial(876));
  EXPECT_FALSE(automaton.isStateFinal(876));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 1u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}
TEST(testHasTransition, transitionDoesNotExist)
{
  fa::Automaton automaton;
  automaton.addState(0);
  automaton.addState(1);
  automaton.addSymbol('a');
  // checking the method
  EXPECT_FALSE(automaton.hasTransition(0, 'a', 1));
  EXPECT_TRUE(automaton.hasState(0));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}
TEST(testHasTransition, stateDoesNotExist)
{
  fa::Automaton automaton;
  automaton.addState(0);
  automaton.addSymbol('a');
  // checking the methods
  EXPECT_FALSE(automaton.hasTransition(0, 'a', 1));
  // checking the elements
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(0));
  EXPECT_FALSE(automaton.isStateInitial(0));
  EXPECT_FALSE(automaton.isStateFinal(0));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 1u);
}
TEST(testHasTransition, symbolDoesNotExist)
{
  fa::Automaton automaton;
  automaton.addState(0);
  automaton.addState(1);
  // checking the methods
  EXPECT_FALSE(automaton.hasTransition(0, 'a', 1));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(0));
  EXPECT_FALSE(automaton.isStateInitial(0));
  EXPECT_FALSE(automaton.isStateFinal(0));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_FALSE(automaton.isStateFinal(1));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testCountTransition, emptyTransitionTable)
{
  fa::Automaton automaton;
  // checking the methods
  EXPECT_EQ(automaton.countTransitions(), 0u);
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 0u);
  EXPECT_EQ(automaton.countSymbols(), 0u);
  EXPECT_EQ(automaton.countStates(), 0u);
}
TEST(testCountTransition, nonEmptyTransitionTable)
{
  fa::Automaton automaton;
  automaton.addState(0);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(0, 'a', 1);
  // checking the methods
  EXPECT_EQ(automaton.countTransitions(), 1u);
  // checking the elements
  EXPECT_TRUE(automaton.hasTransition(0, 'a', 1));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasState(0));
  EXPECT_FALSE(automaton.isStateInitial(0));
  EXPECT_FALSE(automaton.isStateFinal(0));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_FALSE(automaton.isStateFinal(1));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 1u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

//----------testPrettyPrint------------
TEST(testPrettyPrint, prettyPrintTest)
{
  fa::Automaton automaton;
  automaton.addState(0);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.addState(4);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(0, 'a', 1);
  automaton.addTransition(0, 'a', 2);
  automaton.addTransition(0, 'a', 3);
  automaton.addTransition(1, 'b', 3);
  automaton.addTransition(2, 'a', 3);
  automaton.addTransition(2, 'b', 4);
  automaton.addTransition(3, 'a', 3);
  automaton.addTransition(3, 'b', 4);
  automaton.addTransition(4, 'a', 4);
  automaton.setStateInitial(0);
  automaton.setStateInitial(1);
  automaton.setStateFinal(1);
  automaton.setStateFinal(4);
  automaton.prettyPrint(std::cout);
  // checking the elements
  EXPECT_TRUE(automaton.hasTransition(0, 'a', 1));
  EXPECT_TRUE(automaton.hasTransition(0, 'a', 2));
  EXPECT_TRUE(automaton.hasTransition(0, 'a', 3));
  EXPECT_TRUE(automaton.hasTransition(1, 'b', 3));
  EXPECT_TRUE(automaton.hasTransition(2, 'a', 3));
  EXPECT_TRUE(automaton.hasTransition(2, 'b', 4));
  EXPECT_TRUE(automaton.hasTransition(3, 'a', 3));
  EXPECT_TRUE(automaton.hasTransition(3, 'b', 4));
  EXPECT_TRUE(automaton.hasTransition(4, 'a', 4));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasSymbol('b'));
  EXPECT_TRUE(automaton.hasState(0));
  EXPECT_TRUE(automaton.isStateInitial(0));
  EXPECT_FALSE(automaton.isStateFinal(0));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_TRUE(automaton.isStateInitial(1));
  EXPECT_TRUE(automaton.isStateFinal(1));
  EXPECT_TRUE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_FALSE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasState(3));
  EXPECT_FALSE(automaton.isStateInitial(3));
  EXPECT_FALSE(automaton.isStateFinal(3));
  EXPECT_TRUE(automaton.hasState(4));
  EXPECT_FALSE(automaton.isStateInitial(4));
  EXPECT_TRUE(automaton.isStateFinal(4));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 9u);
  EXPECT_EQ(automaton.countSymbols(), 2u);
  EXPECT_EQ(automaton.countStates(), 5u);
}

//----------testHasEpsilonTransition---------------------

TEST(testHasEpsilonTransition, NoEpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(0);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(0, 'a', 1);
  // checking the method
  ASSERT_FALSE(automaton.hasEpsilonTransition());
}

TEST(testHasEpsilonTransition, OneEpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(69, fa::Epsilon, 1);
  // checking the method
  ASSERT_TRUE(automaton.hasEpsilonTransition());
}

TEST(testHasEpsilonTransition, epsilonTransitionOnItSelf)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addSymbol('a');
  automaton.addTransition(69, fa::Epsilon, 69);
  // checking the method
  ASSERT_TRUE(automaton.hasEpsilonTransition());
}

TEST(testHasEpsilonTransition, MultipleEpsilonTransitions)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addTransition(69, fa::Epsilon, 1);
  automaton.addTransition(1, 'a', 2);
  automaton.addTransition(2, fa::Epsilon, 69);
  // checking the method
  ASSERT_TRUE(automaton.hasEpsilonTransition());
}

//----------testIsDeterministic---------------------

#ifdef DEVELOPMENT

TEST(testIsDeterministic, NotDeterministicAutomatonEpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, fa::Epsilon, 42);
  // checking the method
  ASSERT_FALSE(automaton.isDeterministic());
}

#endif // DEVELOPMENT

TEST(testIsDeterministic, DeterministicAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 1);
  // checking the method
  ASSERT_TRUE(automaton.isDeterministic());
}

TEST(testIsDeterministic, noInitialStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 1);
  // checking the method
  ASSERT_FALSE(automaton.isDeterministic());
}

TEST(testIsDeterministic, DeterministicAutomatonNoTransition)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  // checking the method
  ASSERT_TRUE(automaton.isDeterministic());
}

TEST(testIsDeterministic, DeterministicAutomatonTransitionOnItSelf)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addSymbol('a');
  // checking the method
  automaton.addTransition(69, 'a', 69);
  ASSERT_TRUE(automaton.isDeterministic());
}

TEST(testIsDeterministic, NotDeterministicAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(69, 'b', 1);
  // checking the method
  ASSERT_FALSE(automaton.isDeterministic());
}

TEST(testIsDeterministic, notDeterministicAutomatonMultipleInitialStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateInitial(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 1);
  // checking the method
  ASSERT_FALSE(automaton.isDeterministic());
}

//----------testIsComplete---------------------

#ifdef DEVELOPMENT

TEST(testIsComplete, CompleteAutomatonWithEpsilon)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(42, 'b', 1);
  automaton.addTransition(1, 'a', 1);
  automaton.addTransition(1, 'b', 1);
  automaton.addTransition(42, fa::Epsilon, 42);
  // checking the method
  ASSERT_TRUE(automaton.isComplete());
}

#endif // DEVELOPMENT

TEST(testIsComplete, CompleteAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 1);
  automaton.addTransition(1, 'a', 1);
  automaton.addTransition(1, 'b', 1);
  // checking the method
  ASSERT_TRUE(automaton.isComplete());
}

TEST(testIsComplete, NonCompleteAutomatonNoTransition)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  ASSERT_FALSE(automaton.isComplete());
}

TEST(testIsComplete, NonCompleteAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 1);
  automaton.addTransition(1, 'b', 1);
  // checking the method
  ASSERT_FALSE(automaton.isComplete());
}

//------------testCreateMirror--------------------------------

#ifdef DEVELOPMENT

TEST(testCreateMirror, testWithEpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.setStateFinal(3);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, 'b', 2);
  automaton.addTransition(2, 'a', 3);
  automaton.addTransition(3, fa::Epsilon, 42);
  fa::Automaton mirror = fa::Automaton::createMirror(automaton);
  // checking the elements
  EXPECT_TRUE(mirror.hasState(42));
  EXPECT_FALSE(mirror.isStateInitial(42));
  EXPECT_TRUE(mirror.isStateFinal(42));
  EXPECT_TRUE(mirror.hasState(1));
  EXPECT_FALSE(mirror.isStateInitial(1));
  EXPECT_FALSE(mirror.isStateFinal(1));
  EXPECT_TRUE(mirror.hasState(2));
  EXPECT_FALSE(mirror.isStateInitial(2));
  EXPECT_FALSE(mirror.isStateFinal(2));
  EXPECT_TRUE(mirror.hasState(3));
  EXPECT_TRUE(mirror.isStateInitial(3));
  EXPECT_FALSE(mirror.isStateFinal(3));
  EXPECT_TRUE(mirror.hasTransition(3, 'a', 2));
  EXPECT_TRUE(mirror.hasTransition(2, 'b', 1));
  EXPECT_TRUE(mirror.hasTransition(1, 'a', 42));
  EXPECT_TRUE(mirror.hasTransition(42, fa::Epsilon, 3));
}

#endif // DEVELOPMENT

TEST(testCreateMirror, testPresets)
{
  fa::Automaton automaton1 = fa::Automaton::createMirror(beginLetterA);
  EXPECT_TRUE(automaton1.isIncludedIn(endLetterA));
  EXPECT_TRUE(endLetterA.isIncludedIn(automaton1));
  fa::Automaton automaton2 = fa::Automaton::createMirror(endLetterA);
  EXPECT_TRUE(automaton2.isIncludedIn(beginLetterA));
  EXPECT_TRUE(beginLetterA.isIncludedIn(automaton2));
  fa::Automaton automaton3 = fa::Automaton::createMirror(alternateLetterAThenBBeginA);
  EXPECT_TRUE(automaton3.isIncludedIn(alternateLetterAThenBEndA));
  EXPECT_TRUE(alternateLetterAThenBEndA.isIncludedIn(automaton3));
  fa::Automaton automaton4 = fa::Automaton::createMirror(alternateLetterAThenBEndA);
  EXPECT_TRUE(automaton4.isIncludedIn(alternateLetterAThenBBeginA));
  EXPECT_TRUE(alternateLetterAThenBBeginA.isIncludedIn(automaton4));
  fa::Automaton automaton5 = fa::Automaton::createMirror(alternateLetterAThenB);
  EXPECT_TRUE(automaton5.isIncludedIn(alternateLetterAThenB));
  EXPECT_TRUE(alternateLetterAThenB.isIncludedIn(automaton5));
  fa::Automaton automaton6 = fa::Automaton::createMirror(emptyLanguage);
  EXPECT_TRUE(automaton6.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton6));
  fa::Automaton automaton7 = fa::Automaton::createMirror(allWords);
  EXPECT_TRUE(automaton7.isIncludedIn(allWords));
  EXPECT_TRUE(allWords.isIncludedIn(automaton7));
}

TEST(testCreateMirror, testCompleteDeterministAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(69, 'b', 1);
  automaton.addTransition(1, 'a', 69);
  automaton.addTransition(1, 'b', 1);
  fa::Automaton mirror = fa::Automaton::createMirror(automaton);

  EXPECT_FALSE(mirror.match(""));
  EXPECT_FALSE(mirror.match("a"));
  EXPECT_FALSE(mirror.match("aa"));
  EXPECT_FALSE(mirror.match("aaa"));
  EXPECT_FALSE(mirror.match("aaaa"));
  EXPECT_FALSE(mirror.match("aaaaa"));
  EXPECT_FALSE(mirror.match("aaaab"));
  EXPECT_FALSE(mirror.match("aaab"));
  EXPECT_FALSE(mirror.match("aaaba"));
  EXPECT_FALSE(mirror.match("aaabb"));
  EXPECT_FALSE(mirror.match("aab"));
  EXPECT_FALSE(mirror.match("aaba"));
  EXPECT_FALSE(mirror.match("aabaa"));
  EXPECT_FALSE(mirror.match("aabab"));
  EXPECT_FALSE(mirror.match("aabb"));
  EXPECT_FALSE(mirror.match("aabba"));
  EXPECT_FALSE(mirror.match("aabbb"));
  EXPECT_FALSE(mirror.match("ab"));
  EXPECT_FALSE(mirror.match("aba"));
  EXPECT_FALSE(mirror.match("abaa"));
  EXPECT_FALSE(mirror.match("abaaa"));
  EXPECT_FALSE(mirror.match("abaab"));
  EXPECT_FALSE(mirror.match("abab"));
  EXPECT_FALSE(mirror.match("ababa"));
  EXPECT_FALSE(mirror.match("ababb"));
  EXPECT_FALSE(mirror.match("abb"));
  EXPECT_FALSE(mirror.match("abba"));
  EXPECT_FALSE(mirror.match("abbaa"));
  EXPECT_FALSE(mirror.match("abbab"));
  EXPECT_FALSE(mirror.match("abbb"));
  EXPECT_FALSE(mirror.match("abbba"));
  EXPECT_FALSE(mirror.match("abbbb"));
  EXPECT_TRUE(mirror.match("b"));
  EXPECT_TRUE(mirror.match("ba"));
  EXPECT_TRUE(mirror.match("baa"));
  EXPECT_TRUE(mirror.match("baaa"));
  EXPECT_TRUE(mirror.match("baaaa"));
  EXPECT_TRUE(mirror.match("baaab"));
  EXPECT_TRUE(mirror.match("baab"));
  EXPECT_TRUE(mirror.match("baaba"));
  EXPECT_TRUE(mirror.match("baabb"));
  EXPECT_TRUE(mirror.match("bab"));
  EXPECT_TRUE(mirror.match("baba"));
  EXPECT_TRUE(mirror.match("babaa"));
  EXPECT_TRUE(mirror.match("babab"));
  EXPECT_TRUE(mirror.match("babb"));
  EXPECT_TRUE(mirror.match("babba"));
  EXPECT_TRUE(mirror.match("babbb"));
  EXPECT_TRUE(mirror.match("bb"));
  EXPECT_TRUE(mirror.match("bba"));
  EXPECT_TRUE(mirror.match("bbaa"));
  EXPECT_TRUE(mirror.match("bbaaa"));
  EXPECT_TRUE(mirror.match("bbaab"));
  EXPECT_TRUE(mirror.match("bbab"));
  EXPECT_TRUE(mirror.match("bbaba"));
  EXPECT_TRUE(mirror.match("bbabb"));
  EXPECT_TRUE(mirror.match("bbb"));
  EXPECT_TRUE(mirror.match("bbba"));
  EXPECT_TRUE(mirror.match("bbbaa"));
  EXPECT_TRUE(mirror.match("bbbab"));
  EXPECT_TRUE(mirror.match("bbbb"));
  EXPECT_TRUE(mirror.match("bbbba"));
  EXPECT_TRUE(mirror.match("bbbbb"));
}

TEST(testCreateMirror, testDeterministAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'b', 1);
  automaton.addTransition(1, 'a', 69);
  fa::Automaton mirror = fa::Automaton::createMirror(automaton);

  EXPECT_FALSE(mirror.match(""));
  EXPECT_FALSE(mirror.match("a"));
  EXPECT_FALSE(mirror.match("aa"));
  EXPECT_FALSE(mirror.match("aaa"));
  EXPECT_FALSE(mirror.match("aaaa"));
  EXPECT_FALSE(mirror.match("aaaaa"));
  EXPECT_FALSE(mirror.match("aaaab"));
  EXPECT_FALSE(mirror.match("aaab"));
  EXPECT_FALSE(mirror.match("aaaba"));
  EXPECT_FALSE(mirror.match("aaabb"));
  EXPECT_FALSE(mirror.match("aab"));
  EXPECT_FALSE(mirror.match("aaba"));
  EXPECT_FALSE(mirror.match("aabaa"));
  EXPECT_FALSE(mirror.match("aabab"));
  EXPECT_FALSE(mirror.match("aabb"));
  EXPECT_FALSE(mirror.match("aabba"));
  EXPECT_FALSE(mirror.match("aabbb"));
  EXPECT_FALSE(mirror.match("ab"));
  EXPECT_FALSE(mirror.match("aba"));
  EXPECT_FALSE(mirror.match("abaa"));
  EXPECT_FALSE(mirror.match("abaaa"));
  EXPECT_FALSE(mirror.match("abaab"));
  EXPECT_FALSE(mirror.match("abab"));
  EXPECT_FALSE(mirror.match("ababa"));
  EXPECT_FALSE(mirror.match("ababb"));
  EXPECT_FALSE(mirror.match("abb"));
  EXPECT_FALSE(mirror.match("abba"));
  EXPECT_FALSE(mirror.match("abbaa"));
  EXPECT_FALSE(mirror.match("abbab"));
  EXPECT_FALSE(mirror.match("abbb"));
  EXPECT_FALSE(mirror.match("abbba"));
  EXPECT_FALSE(mirror.match("abbbb"));
  EXPECT_TRUE(mirror.match("b"));
  EXPECT_FALSE(mirror.match("ba"));
  EXPECT_FALSE(mirror.match("baa"));
  EXPECT_FALSE(mirror.match("baaa"));
  EXPECT_FALSE(mirror.match("baaaa"));
  EXPECT_FALSE(mirror.match("baaab"));
  EXPECT_FALSE(mirror.match("baab"));
  EXPECT_FALSE(mirror.match("baaba"));
  EXPECT_FALSE(mirror.match("baabb"));
  EXPECT_TRUE(mirror.match("bab"));
  EXPECT_FALSE(mirror.match("baba"));
  EXPECT_FALSE(mirror.match("babaa"));
  EXPECT_TRUE(mirror.match("babab"));
  EXPECT_FALSE(mirror.match("babb"));
  EXPECT_FALSE(mirror.match("babba"));
  EXPECT_FALSE(mirror.match("babbb"));
  EXPECT_FALSE(mirror.match("bb"));
  EXPECT_FALSE(mirror.match("bba"));
  EXPECT_FALSE(mirror.match("bbaa"));
  EXPECT_FALSE(mirror.match("bbaaa"));
  EXPECT_FALSE(mirror.match("bbaab"));
  EXPECT_FALSE(mirror.match("bbab"));
  EXPECT_FALSE(mirror.match("bbaba"));
  EXPECT_FALSE(mirror.match("bbabb"));
  EXPECT_FALSE(mirror.match("bbb"));
  EXPECT_FALSE(mirror.match("bbba"));
  EXPECT_FALSE(mirror.match("bbbaa"));
  EXPECT_FALSE(mirror.match("bbbab"));
  EXPECT_FALSE(mirror.match("bbbb"));
  EXPECT_FALSE(mirror.match("bbbba"));
  EXPECT_FALSE(mirror.match("bbbbb"));
}

TEST(testCreateMirror, testCompleteAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 1);
  automaton.addTransition(1, 'a', 69);
  automaton.addTransition(1, 'b', 1);
  fa::Automaton mirror = fa::Automaton::createMirror(automaton);
  // checking the elements*

  EXPECT_FALSE(mirror.match(""));
  EXPECT_TRUE(mirror.match("a"));
  EXPECT_TRUE(mirror.match("aa"));
  EXPECT_TRUE(mirror.match("aaa"));
  EXPECT_TRUE(mirror.match("aaaa"));
  EXPECT_TRUE(mirror.match("aaaaa"));
  EXPECT_TRUE(mirror.match("aaaab"));
  EXPECT_TRUE(mirror.match("aaab"));
  EXPECT_TRUE(mirror.match("aaaba"));
  EXPECT_TRUE(mirror.match("aaabb"));
  EXPECT_TRUE(mirror.match("aab"));
  EXPECT_TRUE(mirror.match("aaba"));
  EXPECT_TRUE(mirror.match("aabaa"));
  EXPECT_TRUE(mirror.match("aabab"));
  EXPECT_TRUE(mirror.match("aabb"));
  EXPECT_TRUE(mirror.match("aabba"));
  EXPECT_TRUE(mirror.match("aabbb"));
  EXPECT_FALSE(mirror.match("ab"));
  EXPECT_FALSE(mirror.match("aba"));
  EXPECT_FALSE(mirror.match("abaa"));
  EXPECT_FALSE(mirror.match("abaaa"));
  EXPECT_FALSE(mirror.match("abaab"));
  EXPECT_FALSE(mirror.match("abab"));
  EXPECT_FALSE(mirror.match("ababa"));
  EXPECT_FALSE(mirror.match("ababb"));
  EXPECT_FALSE(mirror.match("abb"));
  EXPECT_FALSE(mirror.match("abba"));
  EXPECT_FALSE(mirror.match("abbaa"));
  EXPECT_FALSE(mirror.match("abbab"));
  EXPECT_FALSE(mirror.match("abbb"));
  EXPECT_FALSE(mirror.match("abbba"));
  EXPECT_FALSE(mirror.match("abbbb"));
  EXPECT_TRUE(mirror.match("b"));
  EXPECT_TRUE(mirror.match("ba"));
  EXPECT_TRUE(mirror.match("baa"));
  EXPECT_TRUE(mirror.match("baaa"));
  EXPECT_TRUE(mirror.match("baaaa"));
  EXPECT_TRUE(mirror.match("baaab"));
  EXPECT_TRUE(mirror.match("baab"));
  EXPECT_TRUE(mirror.match("baaba"));
  EXPECT_TRUE(mirror.match("baabb"));
  EXPECT_TRUE(mirror.match("bab"));
  EXPECT_TRUE(mirror.match("baba"));
  EXPECT_TRUE(mirror.match("babaa"));
  EXPECT_TRUE(mirror.match("babab"));
  EXPECT_TRUE(mirror.match("babb"));
  EXPECT_TRUE(mirror.match("babba"));
  EXPECT_TRUE(mirror.match("babbb"));
  EXPECT_TRUE(mirror.match("bb"));
  EXPECT_TRUE(mirror.match("bba"));
  EXPECT_TRUE(mirror.match("bbaa"));
  EXPECT_TRUE(mirror.match("bbaaa"));
  EXPECT_TRUE(mirror.match("bbaab"));
  EXPECT_TRUE(mirror.match("bbab"));
  EXPECT_TRUE(mirror.match("bbaba"));
  EXPECT_TRUE(mirror.match("bbabb"));
  EXPECT_TRUE(mirror.match("bbb"));
  EXPECT_TRUE(mirror.match("bbba"));
  EXPECT_TRUE(mirror.match("bbbaa"));
  EXPECT_TRUE(mirror.match("bbbab"));
  EXPECT_TRUE(mirror.match("bbbb"));
  EXPECT_TRUE(mirror.match("bbbba"));
  EXPECT_TRUE(mirror.match("bbbbb"));
}

TEST(testCreateMirror, testWithFinalAndInitialState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.setStateFinal(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.setStateFinal(3);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'b', 2);
  automaton.addTransition(2, 'a', 3);
  fa::Automaton mirror = fa::Automaton::createMirror(automaton);
  // checking the elements
  EXPECT_TRUE(mirror.match(""));
  EXPECT_FALSE(mirror.match("a"));
  EXPECT_FALSE(mirror.match("aa"));
  EXPECT_FALSE(mirror.match("aaa"));
  EXPECT_FALSE(mirror.match("aaaa"));
  EXPECT_FALSE(mirror.match("aaaaa"));
  EXPECT_FALSE(mirror.match("aaaab"));
  EXPECT_FALSE(mirror.match("aaab"));
  EXPECT_FALSE(mirror.match("aaaba"));
  EXPECT_FALSE(mirror.match("aaabb"));
  EXPECT_FALSE(mirror.match("aab"));
  EXPECT_FALSE(mirror.match("aaba"));
  EXPECT_FALSE(mirror.match("aabaa"));
  EXPECT_FALSE(mirror.match("aabab"));
  EXPECT_FALSE(mirror.match("aabb"));
  EXPECT_FALSE(mirror.match("aabba"));
  EXPECT_FALSE(mirror.match("aabbb"));
  EXPECT_FALSE(mirror.match("ab"));
  EXPECT_TRUE(mirror.match("aba"));
  EXPECT_FALSE(mirror.match("abaa"));
  EXPECT_FALSE(mirror.match("abaaa"));
  EXPECT_FALSE(mirror.match("abaab"));
  EXPECT_FALSE(mirror.match("abab"));
  EXPECT_FALSE(mirror.match("ababa"));
  EXPECT_FALSE(mirror.match("ababb"));
  EXPECT_FALSE(mirror.match("abb"));
  EXPECT_FALSE(mirror.match("abba"));
  EXPECT_FALSE(mirror.match("abbaa"));
  EXPECT_FALSE(mirror.match("abbab"));
  EXPECT_FALSE(mirror.match("abbb"));
  EXPECT_FALSE(mirror.match("abbba"));
  EXPECT_FALSE(mirror.match("abbbb"));
  EXPECT_FALSE(mirror.match("b"));
  EXPECT_FALSE(mirror.match("ba"));
  EXPECT_FALSE(mirror.match("baa"));
  EXPECT_FALSE(mirror.match("baaa"));
  EXPECT_FALSE(mirror.match("baaaa"));
  EXPECT_FALSE(mirror.match("baaab"));
  EXPECT_FALSE(mirror.match("baab"));
  EXPECT_FALSE(mirror.match("baaba"));
  EXPECT_FALSE(mirror.match("baabb"));
  EXPECT_FALSE(mirror.match("bab"));
  EXPECT_FALSE(mirror.match("baba"));
  EXPECT_FALSE(mirror.match("babaa"));
  EXPECT_FALSE(mirror.match("babab"));
  EXPECT_FALSE(mirror.match("babb"));
  EXPECT_FALSE(mirror.match("babba"));
  EXPECT_FALSE(mirror.match("babbb"));
  EXPECT_FALSE(mirror.match("bb"));
  EXPECT_FALSE(mirror.match("bba"));
  EXPECT_FALSE(mirror.match("bbaa"));
  EXPECT_FALSE(mirror.match("bbaaa"));
  EXPECT_FALSE(mirror.match("bbaab"));
  EXPECT_FALSE(mirror.match("bbab"));
  EXPECT_FALSE(mirror.match("bbaba"));
  EXPECT_FALSE(mirror.match("bbabb"));
  EXPECT_FALSE(mirror.match("bbb"));
  EXPECT_FALSE(mirror.match("bbba"));
  EXPECT_FALSE(mirror.match("bbbaa"));
  EXPECT_FALSE(mirror.match("bbbab"));
  EXPECT_FALSE(mirror.match("bbbb"));
  EXPECT_FALSE(mirror.match("bbbba"));
  EXPECT_FALSE(mirror.match("bbbbb"));
}

//-----------testCreateComplete--------------------------------

#ifdef DEVELOPMENT
TEST(testCreateComplete, testWithEpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.setStateFinal(3);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, 'b', 2);
  automaton.addTransition(2, 'a', 3);
  automaton.addTransition(3, fa::Epsilon, 42);
  fa::Automaton complete = fa::Automaton::createComplete(automaton);
  EXPECT_TRUE(complete.hasState(42));
  EXPECT_TRUE(complete.isStateInitial(42));
  EXPECT_FALSE(complete.isStateFinal(42));
  EXPECT_TRUE(complete.hasState(1));
  EXPECT_FALSE(complete.isStateInitial(1));
  EXPECT_FALSE(complete.isStateFinal(1));
  EXPECT_TRUE(complete.hasState(2));
  EXPECT_FALSE(complete.isStateInitial(2));
  EXPECT_FALSE(complete.isStateFinal(2));
  EXPECT_TRUE(complete.hasState(3));
  EXPECT_FALSE(complete.isStateInitial(3));
  EXPECT_TRUE(complete.isStateFinal(3));
  EXPECT_TRUE(complete.hasTransition(42, 'a', 1));
  EXPECT_TRUE(complete.hasTransition(1, 'b', 2));
  EXPECT_TRUE(complete.hasTransition(2, 'a', 3));
  EXPECT_TRUE(complete.hasTransition(3, fa::Epsilon, 42));
}
#endif

TEST(testCreateComplete, testPresets)
{
  fa::Automaton automaton1 = fa::Automaton::createComplete(beginLetterA);
  EXPECT_TRUE(automaton1.isIncludedIn(beginLetterA));
  EXPECT_TRUE(beginLetterA.isIncludedIn(automaton1));
  EXPECT_TRUE(automaton1.isComplete());
  fa::Automaton automaton2 = fa::Automaton::createComplete(endLetterA);
  EXPECT_TRUE(automaton2.isIncludedIn(endLetterA));
  EXPECT_TRUE(endLetterA.isIncludedIn(automaton2));
  EXPECT_TRUE(automaton2.isComplete());
  fa::Automaton automaton3 = fa::Automaton::createComplete(alternateLetterAThenBBeginA);
  EXPECT_TRUE(automaton3.isIncludedIn(alternateLetterAThenBBeginA));
  EXPECT_TRUE(alternateLetterAThenBBeginA.isIncludedIn(automaton3));
  EXPECT_TRUE(automaton3.isComplete());
  fa::Automaton automaton4 = fa::Automaton::createComplete(alternateLetterAThenBEndA);
  EXPECT_TRUE(automaton4.isIncludedIn(alternateLetterAThenBEndA));
  EXPECT_TRUE(alternateLetterAThenBEndA.isIncludedIn(automaton4));
  EXPECT_TRUE(automaton4.isComplete());
  fa::Automaton automaton5 = fa::Automaton::createComplete(alternateLetterAThenB);
  EXPECT_TRUE(automaton5.isIncludedIn(alternateLetterAThenB));
  EXPECT_TRUE(alternateLetterAThenB.isIncludedIn(automaton5));
  EXPECT_TRUE(automaton5.isComplete());
  fa::Automaton automaton6 = fa::Automaton::createComplete(emptyLanguage);
  EXPECT_TRUE(automaton6.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton6));
  EXPECT_TRUE(automaton6.isComplete());
  fa::Automaton automaton7 = fa::Automaton::createComplete(allWords);
  EXPECT_TRUE(automaton7.isIncludedIn(allWords));
  EXPECT_TRUE(allWords.isIncludedIn(automaton7));
  EXPECT_TRUE(automaton7.isComplete());
}

TEST(createComplete, noInitialState)
{
  fa::Automaton automaton;
  EXPECT_TRUE(automaton.addState(43));
  EXPECT_TRUE(automaton.addState(313));
  automaton.setStateFinal(313);
  EXPECT_TRUE(automaton.addState(13));
  automaton.setStateFinal(13);
  EXPECT_TRUE(automaton.addSymbol('a'));
  EXPECT_TRUE(automaton.addSymbol('b'));
  EXPECT_TRUE(automaton.addTransition(43, 'a', 313));
  EXPECT_TRUE(automaton.addTransition(43, 'a', 13));

  fa::Automaton complete = fa::Automaton::createComplete(automaton);
  EXPECT_FALSE(automaton.isComplete());
  EXPECT_TRUE(complete.isValid());
  EXPECT_TRUE(complete.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(complete));
}

TEST(testCreateComplete, testWithFinalAndInitialState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.setStateFinal(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.setStateFinal(3);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'b', 2);
  automaton.addTransition(2, 'a', 3);
  fa::Automaton complete = fa::Automaton::createComplete(automaton);
  EXPECT_TRUE(complete.isComplete());
  EXPECT_TRUE(complete.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(complete));
}

TEST(testCreateComplete, testCompleteAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 1);
  automaton.addTransition(1, 'a', 1);
  automaton.addTransition(1, 'b', 1);
  fa::Automaton complete = fa::Automaton::createComplete(automaton);
  EXPECT_TRUE(complete.isComplete());
  EXPECT_TRUE(complete.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(complete));
}

TEST(testCreateComplete, testNotDeterministic)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(1, 'b', 1);
  automaton.addTransition(1, 'a', 69);
  fa::Automaton complete = fa::Automaton::createComplete(automaton);
  EXPECT_TRUE(complete.isComplete());
  EXPECT_TRUE(complete.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(complete));
}

TEST(testCreateComplete, testWithMaxIntState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.setStateFinal(69);
  automaton.addState(INT_MAX);
  automaton.setStateFinal(INT_MAX);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', INT_MAX);
  automaton.addTransition(INT_MAX, 'b', 69);
  fa::Automaton complete = fa::Automaton::createComplete(automaton);
  EXPECT_TRUE(complete.isComplete());
  EXPECT_TRUE(complete.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(complete));
}

TEST(testCreateComplete, testNoTransitions)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  fa::Automaton complete = fa::Automaton::createComplete(automaton);
  EXPECT_TRUE(complete.isComplete());
  EXPECT_TRUE(complete.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(complete));
}

TEST(testCreateComplete, testOneStateNotFinal)
{
  fa::Automaton automaton = fa::Automaton::createComplete(emptyLanguage);
  EXPECT_TRUE(automaton.isLanguageEmpty());
  EXPECT_TRUE(automaton.isComplete());
  EXPECT_TRUE(automaton.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton));
}

TEST(createComplete, notConnexeState)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.setStateFinal(42);
  automaton.addState(69);

  automaton.addSymbol('a');
  automaton.addSymbol('b');

  automaton.addTransition(42, 'a', 42);

  fa::Automaton complete = fa::Automaton::createComplete(automaton);
  EXPECT_FALSE(automaton.isComplete());
  EXPECT_TRUE(complete.isComplete());
  EXPECT_TRUE(complete.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(complete));
}

//----------testCreateComplement--------------------------------

#ifdef DEVELOPMENT

TEST(testCreateComplement, testWithEpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.setStateFinal(42);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.setStateFinal(3);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, 'b', 2);
  automaton.addTransition(2, 'a', 3);
  automaton.addTransition(3, fa::Epsilon, 42);
  fa::Automaton complement = fa::Automaton::createComplement(automaton);
}

#endif

TEST(testCreateComplement, noInitialState)
{
  fa::Automaton automaton;
  EXPECT_TRUE(automaton.addState(43));
  EXPECT_TRUE(automaton.addState(313));
  EXPECT_TRUE(automaton.addState(13));
  automaton.setStateFinal(313);
  automaton.setStateFinal(13);
  EXPECT_TRUE(automaton.addSymbol('a'));
  EXPECT_TRUE(automaton.addSymbol('b'));
  EXPECT_TRUE(automaton.addTransition(43, 'a', 313));
  EXPECT_TRUE(automaton.addTransition(43, 'a', 13));
  EXPECT_FALSE(automaton.isDeterministic());
  EXPECT_FALSE(automaton.isComplete());

  fa::Automaton complement = fa::Automaton::createComplement(automaton);

  EXPECT_TRUE(complement.isValid());
  EXPECT_TRUE(hasOppositeMatchingWords(complement, automaton));
}

TEST(testCreateComplement, testPresets)
{
  fa::Automaton automaton1 = fa::Automaton::createComplement(beginLetterA);
  EXPECT_TRUE(automaton1.isIncludedIn(beginLetterAComplement));
  EXPECT_TRUE(beginLetterAComplement.isIncludedIn(automaton1));
  fa::Automaton automaton2 = fa::Automaton::createComplement(endLetterA);
  EXPECT_TRUE(automaton2.isIncludedIn(endLetterAComplement));
  EXPECT_TRUE(endLetterAComplement.isIncludedIn(automaton2));
  fa::Automaton automaton3 = fa::Automaton::createComplement(alternateLetterAThenBBeginA);
  EXPECT_TRUE(automaton3.isIncludedIn(alternateLetterAThenBBeginAComplement));
  EXPECT_TRUE(alternateLetterAThenBBeginAComplement.isIncludedIn(automaton3));
  fa::Automaton automaton4 = fa::Automaton::createComplement(alternateLetterAThenBEndA);
  EXPECT_TRUE(automaton4.isIncludedIn(alternateLetterAThenBEndAComplement));
  EXPECT_TRUE(alternateLetterAThenBEndAComplement.isIncludedIn(automaton4));
  fa::Automaton automaton5 = fa::Automaton::createComplement(alternateLetterAThenB);
  EXPECT_TRUE(automaton5.isIncludedIn(alternateLetterAThenBComplement));
  EXPECT_TRUE(alternateLetterAThenBComplement.isIncludedIn(automaton5));
  fa::Automaton automaton6 = fa::Automaton::createComplement(emptyLanguage);
  EXPECT_TRUE(automaton6.isIncludedIn(allWords));
  EXPECT_TRUE(allWords.isIncludedIn(automaton6));
  fa::Automaton automaton7 = fa::Automaton::createComplement(allWords);
  EXPECT_TRUE(automaton7.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton7));
}

TEST(testCreateComplement, testCompleteDeterministAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 69);
  automaton.addTransition(1, 'b', 69);
  automaton.addTransition(1, 'a', 1);

  fa::Automaton complement = fa::Automaton::createComplement(automaton);

  EXPECT_TRUE(hasOppositeMatchingWords(complement, automaton));
}

TEST(testCreateComplement, testNotCompleteAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'b', 1);
  automaton.addTransition(1, 'a', 69);
  fa::Automaton complement = fa::Automaton::createComplement(automaton);

  EXPECT_TRUE(hasOppositeMatchingWords(complement, automaton));
}

TEST(testCreateComplement, testNotDeterministicAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('b');
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(69, 'b', 1);
  automaton.addTransition(1, 'a', 1);
  automaton.addTransition(1, 'b', 1);
  fa::Automaton complement = fa::Automaton::createComplement(automaton);

  EXPECT_TRUE(hasOppositeMatchingWords(complement, automaton));
}

//----------testMakeTransition--------------------------------

#ifdef DEVELOPMENT

TEST(testMakeTransition, EpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, fa::Epsilon, 42);

  std::set<int> origin = {42};
  auto result = automaton.makeTransition(origin, 'a');

  // Vérification
  EXPECT_EQ(result.size(), 2u);
  EXPECT_TRUE(result.find(1) != result.end());
  EXPECT_TRUE(result.find(42) != result.end());
}

TEST(testMakeTransition, LoopEpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, fa::Epsilon, 2);
  automaton.addTransition(2, fa::Epsilon, 42);
  automaton.addTransition(42, fa::Epsilon, 1);

  std::set<int> origin = {42};
  auto result = automaton.makeTransition(origin, 'a');

  // Vérification
  EXPECT_EQ(result.size(), 3u);
  EXPECT_TRUE(result.find(2) != result.end());
  EXPECT_TRUE(result.find(1) != result.end());
  EXPECT_TRUE(result.find(42) != result.end());
}

#endif

TEST(testMakeTransition, NominalCase)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);

  std::set<int> origin = {69};
  auto result = automaton.makeTransition(origin, 'a');

  // Vérification
  EXPECT_EQ(result.size(), 1u);
  EXPECT_TRUE(result.find(1) != result.end());
}

TEST(testMakeTransition, MultipleTransitionsWithSameSymbol)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'a', 2);

  std::set<int> origin = {69};
  auto result = automaton.makeTransition(origin, 'a');

  // Vérification
  EXPECT_EQ(result.size(), 2u);
  EXPECT_TRUE(result.find(1) != result.end());
  EXPECT_TRUE(result.find(2) != result.end());
}

TEST(testMakeTransition, NoTransitionsForGivenSymbol)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');

  std::set<int> origin = {69};
  auto result = automaton.makeTransition(origin, 'a');

  // Vérification
  EXPECT_TRUE(result.empty());
}

TEST(testMakeTransition, MultipleOriginStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'a', 2);

  std::set<int> origin = {69, 1};
  auto result = automaton.makeTransition(origin, 'a');

  // Vérification
  EXPECT_EQ(result.size(), 2u);
  EXPECT_TRUE(result.find(1) != result.end());
  EXPECT_TRUE(result.find(2) != result.end());
}

TEST(testMakeTransition, testInvalidSymbol)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);

  std::set<int> origin = {69};
  std::set<int> result = automaton.makeTransition(origin, 'b');

  // Vérification
  EXPECT_TRUE(result.empty());
}

TEST(testMakeTransition, testInvalidState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);

  std::set<int> origin = {2};
  std::set<int> result = automaton.makeTransition(origin, 'a');

  EXPECT_TRUE(result.empty());
}

//---------testReadString--------------------------------

#ifdef DEVELOPMENT

TEST(testReadString, epsilonTransitions)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(42, fa::Epsilon, 1);

  // Test method
  std::set<int> result = automaton.readString("");

  // Check result
  std::set<int> expected = {42, 1};
  EXPECT_EQ(result, expected);

  // Check elements size
  EXPECT_EQ(automaton.countTransitions(), 1u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testReadString, loopEpsilonTransitions)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.addSymbol('a');
  automaton.addTransition(42, fa::Epsilon, 1);
  automaton.addTransition(1, fa::Epsilon, 42);

  // Test method
  std::set<int> result = automaton.readString("");

  // Check result
  std::set<int> expected = {42, 1};
  EXPECT_EQ(result, expected);

  // Check elements size
  EXPECT_EQ(automaton.countTransitions(), 2u);
  EXPECT_EQ(automaton.countSymbols(), 1u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

#endif

TEST(testReadString, testReadStringDeterministicAndCompleteAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 69);
  automaton.addTransition(1, 'b', 69);
  automaton.addTransition(1, 'a', 1);

  // checking the method
  std::set<int> result = automaton.readString("ababbbaaababab");
  EXPECT_EQ(result.size(), 1u);
  EXPECT_TRUE(result.count(69));
  // checking the elements
  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_TRUE(automaton.isStateFinal(1));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasSymbol('b'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 1));
  EXPECT_TRUE(automaton.hasTransition(1, 'b', 69));
  EXPECT_TRUE(automaton.hasTransition(69, 'b', 69));
  EXPECT_TRUE(automaton.hasTransition(1, 'a', 1));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 4u);
  EXPECT_EQ(automaton.countSymbols(), 2u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testReadString, testReadStringNotDeterministic)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(69, 'b', 69);
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'a', 2);
  automaton.addTransition(1, 'b', 2);
  automaton.addTransition(2, 'a', 2);
  automaton.addTransition(2, 'b', 2);

  // checking the method
  std::set<int> result = automaton.readString("aba");
  EXPECT_EQ(result.size(), 3u);
  EXPECT_TRUE(result.count(69));
  EXPECT_TRUE(result.count(1));
  EXPECT_TRUE(result.count(2));

  // checking the elements
  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_TRUE(automaton.isStateFinal(1));
  EXPECT_TRUE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_FALSE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasSymbol('b'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 69));
  EXPECT_TRUE(automaton.hasTransition(69, 'b', 69));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 1));
  EXPECT_TRUE(automaton.hasTransition(1, 'a', 2));
  EXPECT_TRUE(automaton.hasTransition(1, 'b', 2));
  EXPECT_TRUE(automaton.hasTransition(2, 'a', 2));
  EXPECT_TRUE(automaton.hasTransition(2, 'b', 2));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 7u);
  EXPECT_EQ(automaton.countSymbols(), 2u);
  EXPECT_EQ(automaton.countStates(), 3u);
}

TEST(testReadString, testReadStringNotDeterministicAndNotComplete)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(69, 'b', 69);
  automaton.addTransition(69, 'a', 1);

  // checking the method
  std::set<int> result = automaton.readString("aba");
  EXPECT_EQ(result.size(), 2u);
  EXPECT_TRUE(result.count(69));
  EXPECT_TRUE(result.count(1));

  // checking the elements
  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_TRUE(automaton.isStateFinal(1));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasSymbol('b'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 69));
  EXPECT_TRUE(automaton.hasTransition(69, 'b', 69));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 1));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 3u);
  EXPECT_EQ(automaton.countSymbols(), 2u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

TEST(testReadString, testReadStringInvalidSymbol)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 69);
  automaton.addTransition(1, 'b', 69);
  automaton.addTransition(1, 'a', 1);

  // checking the method
  std::set<int> result = automaton.readString("abc");
  EXPECT_EQ(result.size(), 0u);
  // checking the elements
  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_TRUE(automaton.isStateFinal(1));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasSymbol('b'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 1));
  EXPECT_TRUE(automaton.hasTransition(1, 'b', 69));
  EXPECT_TRUE(automaton.hasTransition(69, 'b', 69));
  EXPECT_TRUE(automaton.hasTransition(1, 'a', 1));
  // checking the elements size
  EXPECT_EQ(automaton.countTransitions(), 4u);
  EXPECT_EQ(automaton.countSymbols(), 2u);
  EXPECT_EQ(automaton.countStates(), 2u);
}

//---------testMatch----------

#ifdef DEVELOPMENT

TEST(testMatch, epsilonTransitions)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(42, 'a', 42);
  automaton.addTransition(42, 'b', 42);
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, fa::Epsilon, 42);

  EXPECT_TRUE(automaton.match("a"));
  EXPECT_TRUE(automaton.match("ba"));
  EXPECT_TRUE(automaton.match("abbbbbbaaaababaa"));
  EXPECT_FALSE(automaton.match(""));
  EXPECT_FALSE(automaton.match("b"));
  EXPECT_FALSE(automaton.match("abbbbbcbaaaababaa"));
  EXPECT_FALSE(automaton.match("aabbabbabb"));
}

TEST(testMatch, loopEpsilonTransitions)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(42, 'a', 42);
  automaton.addTransition(42, 'b', 42);
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, fa::Epsilon, 42);
  automaton.addTransition(1, fa::Epsilon, 1);

  EXPECT_TRUE(automaton.match("a"));
  EXPECT_TRUE(automaton.match("ba"));
  EXPECT_TRUE(automaton.match("abbbbbbaaaababaa"));
  EXPECT_FALSE(automaton.match(""));
  EXPECT_FALSE(automaton.match("b"));
  EXPECT_FALSE(automaton.match("abbbbbcbaaaababaa"));
  EXPECT_FALSE(automaton.match("ab"));
}

#endif

TEST(testMatch, testReadStringDeterministicAndCompleteAutomaton)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'b', 69);
  automaton.addTransition(1, 'b', 69);
  automaton.addTransition(1, 'a', 1);

  EXPECT_TRUE(automaton.match("a"));
  EXPECT_TRUE(automaton.match("ba"));
  EXPECT_TRUE(automaton.match("abbbbbbaaaababaa"));
  EXPECT_FALSE(automaton.match(""));
  EXPECT_FALSE(automaton.match("b"));
  EXPECT_FALSE(automaton.match("abbbbbcbaaaababaa"));
  EXPECT_FALSE(automaton.match("aabbabbabb"));
}

TEST(testMatch, notDeterministic)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(69, 'b', 69);
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'a', 2);
  automaton.addTransition(1, 'b', 2);
  automaton.addTransition(2, 'a', 2);
  automaton.addTransition(2, 'b', 2);

  EXPECT_TRUE(automaton.match("a"));
  EXPECT_TRUE(automaton.match("ba"));
  EXPECT_TRUE(automaton.match("abbbbbbaaaababaa"));
  EXPECT_FALSE(automaton.match(""));
  EXPECT_FALSE(automaton.match("b"));
  EXPECT_FALSE(automaton.match("abbbbbcbaaaababaa"));
  EXPECT_FALSE(automaton.match("aabbabbabb"));
}

TEST(testMatch, notDeterministicAndNotComplete)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 69);
  automaton.addTransition(69, 'b', 69);
  automaton.addTransition(69, 'a', 1);

  EXPECT_TRUE(automaton.match("a"));
  EXPECT_TRUE(automaton.match("ba"));
  EXPECT_TRUE(automaton.match("abbbbbbaaaababaa"));
  EXPECT_FALSE(automaton.match(""));
  EXPECT_FALSE(automaton.match("b"));
  EXPECT_FALSE(automaton.match("abbbbbcbaaaababaa"));
  EXPECT_FALSE(automaton.match("aabbabbabb"));
}

TEST(testMatch, twoWaysButOnlyOneWorks)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.addState(4);
  automaton.setStateFinal(4);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'a', 3);
  automaton.addTransition(1, 'b', 2);
  automaton.addTransition(3, 'b', 4);

  EXPECT_TRUE(automaton.match("ab"));
}

//----------------TestIsLanguageEmpty------------------------

TEST(testIsLanguageEmpty, notEmpty)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.setStateFinal(2);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'a', 2);

  EXPECT_FALSE(automaton.isLanguageEmpty());
}

TEST(testIsLanguageEmpty, noTransitionsNotEmpty)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.setStateFinal(69);
  automaton.addSymbol('a');

  EXPECT_FALSE(automaton.isLanguageEmpty());
}

TEST(testIsLanguageEmpty, noTransitionsEmpty)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addSymbol('a');

  EXPECT_TRUE(automaton.isLanguageEmpty());
}

TEST(testIsLanguageEmpty, noInitialStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateFinal(69);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 69);

  EXPECT_TRUE(automaton.isLanguageEmpty());
}

TEST(testIsLanguageEmpty, noFinalStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 69);

  EXPECT_TRUE(automaton.isLanguageEmpty());
}

TEST(testIsLanguageEmpty, notLinked)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.setStateFinal(2);
  automaton.addSymbol('a');
  automaton.addTransition(1, 'a', 69);
  automaton.addTransition(1, 'a', 2);

  EXPECT_TRUE(automaton.isLanguageEmpty());
}

TEST(testIsLanguageEmpty, notEmptyWithLoop)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.setStateFinal(2);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'a', 2);
  automaton.addTransition(2, 'a', 69);

  EXPECT_FALSE(automaton.isLanguageEmpty());
}

TEST(testIsLanguageEmpty, emptyWithLoop)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'a', 2);
  automaton.addTransition(2, 'a', 69);

  EXPECT_TRUE(automaton.isLanguageEmpty());
}

//------------TestRemoveNonAccessibleStates--------------

TEST(testRemoveNonAccessibleStates, testRemoveNoState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.setStateFinal(69);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 69);

  automaton.removeNonAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_TRUE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 69));
}

TEST(testRemoveNonAccessibleStates, testRemoveNoStateNoTransitions)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateInitial(1);
  automaton.addState(2);
  automaton.setStateInitial(2);
  automaton.addSymbol('a');

  automaton.removeNonAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_TRUE(automaton.isStateInitial(1));
  EXPECT_FALSE(automaton.isStateFinal(1));
  EXPECT_TRUE(automaton.hasState(2));
  EXPECT_TRUE(automaton.isStateInitial(2));
  EXPECT_FALSE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasSymbol('a'));
}

TEST(testRemoveNonAccessibleStates, testRemoveOneNonAccessibleStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.setStateFinal(3);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 2);
  automaton.addTransition(1, 'a', 3);
  automaton.addTransition(2, 'a', 3);

  automaton.removeNonAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_FALSE(automaton.hasState(1));
  EXPECT_TRUE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_FALSE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasState(3));
  EXPECT_FALSE(automaton.isStateInitial(3));
  EXPECT_TRUE(automaton.isStateFinal(3));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 2));
  EXPECT_FALSE(automaton.hasTransition(1, 'a', 3));
  EXPECT_TRUE(automaton.hasTransition(2, 'a', 3));
}

TEST(testRemoveNonAccessibleStates, testRemoveMultipleNonAccessibleStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.setStateFinal(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.addState(4);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 2);
  automaton.addTransition(1, 'a', 3);
  automaton.addTransition(2, 'a', 3);
  automaton.addTransition(4, 'a', 3);

  automaton.removeNonAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_TRUE(automaton.isStateFinal(69));
  EXPECT_FALSE(automaton.hasState(1));
  EXPECT_TRUE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_FALSE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasState(3));
  EXPECT_FALSE(automaton.isStateInitial(3));
  EXPECT_FALSE(automaton.isStateFinal(3));
  EXPECT_FALSE(automaton.hasState(4));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 2));
  EXPECT_FALSE(automaton.hasTransition(1, 'a', 3));
  EXPECT_TRUE(automaton.hasTransition(2, 'a', 3));
  EXPECT_FALSE(automaton.hasTransition(3, 'a', 4));
}

TEST(testRemoveNonAccessibleStates, notConnexeState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(2, 'a', 2);

  automaton.removeNonAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_TRUE(automaton.isStateFinal(1));
  EXPECT_FALSE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_FALSE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 1));
  EXPECT_FALSE(automaton.hasTransition(2, 'a', 2));
}

TEST(testRemoveNonAccessibleStates, noInitialState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(1, 'a', 2);

  automaton.removeNonAccessibleStates();

  EXPECT_TRUE(automaton.isLanguageEmpty());

  EXPECT_TRUE(automaton.isValid());
}

//------------testRemoveNonCoAccessibleStates-------------

TEST(testRemoveNonCoAccessibleStates, testRemoveNoState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.setStateFinal(69);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 69);

  automaton.removeNonCoAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_TRUE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 69));
}

TEST(testRemoveNonCoAccessibleStates, testRemoveNoStateNoTransitions)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateFinal(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addState(2);
  automaton.setStateFinal(2);
  automaton.addSymbol('a');

  automaton.removeNonCoAccessibleStates();

  EXPECT_TRUE(automaton.isLanguageEmpty());

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_FALSE(automaton.isStateInitial(69));
  EXPECT_TRUE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_TRUE(automaton.isStateFinal(1));
  EXPECT_TRUE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_TRUE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasSymbol('a'));
}

TEST(testRemoveNonCoAccessibleStates, testRemoveOneNonCoAccessibleStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.setStateFinal(2);
  automaton.addState(3);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(69, 'a', 3);
  automaton.addTransition(1, 'a', 2);

  automaton.removeNonCoAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_FALSE(automaton.isStateFinal(1));
  EXPECT_TRUE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_TRUE(automaton.isStateFinal(2));
  EXPECT_FALSE(automaton.hasState(3));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 1));
  EXPECT_TRUE(automaton.hasTransition(1, 'a', 2));
  EXPECT_FALSE(automaton.hasTransition(69, 'a', 3));
}

TEST(testRemoveNonCoAccessibleStates, testRemoveMultipleNonCoAccessibleStates)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.setStateFinal(69);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.addState(4);
  automaton.addSymbol('a');
  automaton.addTransition(2, 'a', 69);
  automaton.addTransition(3, 'a', 1);
  automaton.addTransition(3, 'a', 2);
  automaton.addTransition(3, 'a', 4);

  automaton.removeNonCoAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_TRUE(automaton.isStateFinal(69));
  EXPECT_FALSE(automaton.hasState(1));
  EXPECT_TRUE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_FALSE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasState(3));
  EXPECT_FALSE(automaton.isStateInitial(3));
  EXPECT_FALSE(automaton.isStateFinal(3));
  EXPECT_FALSE(automaton.hasState(4));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasTransition(2, 'a', 69));
  EXPECT_FALSE(automaton.hasTransition(3, 'a', 1));
  EXPECT_TRUE(automaton.hasTransition(3, 'a', 2));
  EXPECT_FALSE(automaton.hasTransition(3, 'a', 4));
}

TEST(testRemoveNonCoAccessibleStates, notConnexeState)
{
  fa::Automaton automaton;
  automaton.addState(69);
  automaton.setStateInitial(69);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addTransition(69, 'a', 1);
  automaton.addTransition(2, 'a', 2);

  automaton.removeNonCoAccessibleStates();

  EXPECT_TRUE(automaton.hasState(69));
  EXPECT_TRUE(automaton.isStateInitial(69));
  EXPECT_FALSE(automaton.isStateFinal(69));
  EXPECT_TRUE(automaton.hasState(1));
  EXPECT_FALSE(automaton.isStateInitial(1));
  EXPECT_TRUE(automaton.isStateFinal(1));
  EXPECT_FALSE(automaton.hasState(2));
  EXPECT_FALSE(automaton.isStateInitial(2));
  EXPECT_FALSE(automaton.isStateFinal(2));
  EXPECT_TRUE(automaton.hasSymbol('a'));
  EXPECT_TRUE(automaton.hasTransition(69, 'a', 1));
  EXPECT_FALSE(automaton.hasTransition(2, 'a', 2));
}

TEST(testRemoveNonCoAccessibleStates, noFinalState)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addSymbol('a');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, 'a', 2);

  automaton.removeNonCoAccessibleStates();

  EXPECT_TRUE(automaton.isLanguageEmpty());

  EXPECT_TRUE(automaton.isValid());
}

//------------testCreateIntersection-------------

TEST(testCreateIntersection, testPresets)
{
  // not empty langage
  fa::Automaton automaton1 = fa::Automaton::createIntersection(beginLetterA, endLetterA);
  EXPECT_TRUE(automaton1.isIncludedIn(beginAndEndLetterA));
  EXPECT_TRUE(beginAndEndLetterA.isIncludedIn(automaton1));
  fa::Automaton automaton2 = fa::Automaton::createIntersection(beginLetterA, alternateLetterAThenB);
  EXPECT_TRUE(alternateLetterAThenBBeginA.isIncludedIn(automaton2));
  EXPECT_TRUE(automaton2.isIncludedIn(alternateLetterAThenBBeginA));
  fa::Automaton automaton3 = fa::Automaton::createIntersection(endLetterA, alternateLetterAThenB);
  EXPECT_TRUE(automaton3.isIncludedIn(alternateLetterAThenBEndA));
  EXPECT_TRUE(alternateLetterAThenBEndA.isIncludedIn(automaton3));
  fa::Automaton automaton4 = fa::Automaton::createIntersection(beginLetterA, alternateLetterAThenBEndA);
  EXPECT_TRUE(automaton4.isIncludedIn(alternateLetterAThenBBeginAEndA));
  EXPECT_TRUE(alternateLetterAThenBBeginAEndA.isIncludedIn(automaton4));

  // empty langage
  fa::Automaton automaton5 = fa::Automaton::createIntersection(beginLetterA, beginLetterB);
  EXPECT_TRUE(automaton5.isLanguageEmpty());
  fa::Automaton automaton6 = fa::Automaton::createIntersection(endLetterA, endLetterB);
  EXPECT_TRUE(automaton6.isLanguageEmpty());
  fa::Automaton automaton7 = fa::Automaton::createIntersection(alternateLetterAThenB, emptyLanguage);
  EXPECT_TRUE(automaton7.isLanguageEmpty());

  // do not changes
  fa::Automaton automaton8 = fa::Automaton::createIntersection(beginLetterA, beginLetterA);
  EXPECT_TRUE(automaton8.isIncludedIn(beginLetterA));
  EXPECT_TRUE(beginLetterA.isIncludedIn(automaton8));
  fa::Automaton automaton9 = fa::Automaton::createIntersection(endLetterA, endLetterA);
  EXPECT_TRUE(automaton9.isIncludedIn(endLetterA));
  EXPECT_TRUE(endLetterA.isIncludedIn(automaton9));
  fa::Automaton automaton10 = fa::Automaton::createIntersection(alternateLetterAThenB, alternateLetterAThenB);
  EXPECT_TRUE(automaton10.isIncludedIn(alternateLetterAThenB));
  EXPECT_TRUE(alternateLetterAThenB.isIncludedIn(automaton10));
  fa::Automaton automaton11 = fa::Automaton::createIntersection(emptyLanguage, emptyLanguage);
  EXPECT_TRUE(automaton11.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton11));
  fa::Automaton automaton12 = fa::Automaton::createIntersection(allWords, allWords);
  EXPECT_TRUE(automaton12.isIncludedIn(allWords));
  EXPECT_TRUE(allWords.isIncludedIn(automaton12));

  // with the whole set of the possible words
  fa::Automaton automaton13 = fa::Automaton::createIntersection(beginLetterA, allWords);
  EXPECT_TRUE(automaton13.isIncludedIn(beginLetterA));
  EXPECT_TRUE(beginLetterA.isIncludedIn(automaton13));
  fa::Automaton automaton14 = fa::Automaton::createIntersection(endLetterA, allWords);
  EXPECT_TRUE(automaton14.isIncludedIn(endLetterA));
  EXPECT_TRUE(endLetterA.isIncludedIn(automaton14));
  fa::Automaton automaton15 = fa::Automaton::createIntersection(alternateLetterAThenB, allWords);
  EXPECT_TRUE(automaton15.isIncludedIn(alternateLetterAThenB));
  EXPECT_TRUE(alternateLetterAThenB.isIncludedIn(automaton15));
  fa::Automaton automaton16 = fa::Automaton::createIntersection(emptyLanguage, allWords);
  EXPECT_TRUE(automaton16.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton16));
}

TEST(testCreateIntersection, testInitialStateIsFinal)
{
  fa::Automaton automaton1;
  automaton1.addState(42);
  automaton1.setStateInitial(42);
  automaton1.setStateFinal(42);
  automaton1.addState(1);
  automaton1.setStateFinal(1);
  automaton1.addSymbol('a');
  automaton1.addSymbol('b');
  automaton1.addTransition(42, 'a', 1);
  automaton1.addTransition(1, 'a', 1);
  automaton1.addTransition(1, 'b', 1);

  fa::Automaton automaton2;
  automaton2.addState(42);
  automaton2.setStateInitial(42);
  automaton2.setStateFinal(42);
  automaton2.addSymbol('a');
  automaton2.addSymbol('b');
  automaton2.addTransition(42, 'b', 42);

  fa::Automaton automaton3;
  automaton3.addState(42);
  automaton3.setStateInitial(42);
  automaton3.setStateFinal(42);
  automaton3.addSymbol('a');
  automaton3.addSymbol('b');

  fa::Automaton automaton4 = fa::Automaton::createIntersection(automaton1, automaton2);
  EXPECT_TRUE(automaton4.isIncludedIn(automaton3));
  EXPECT_TRUE(automaton3.isIncludedIn(automaton4));
}

TEST(testCreateIntersection, differentAlphabet1)
{
  fa::Automaton automaton1;
  automaton1.addState(42);
  automaton1.setStateInitial(42);
  automaton1.addState(1);
  automaton1.setStateFinal(1);
  automaton1.addSymbol('a');
  automaton1.addSymbol('b');
  automaton1.addTransition(42, 'a', 1);
  automaton1.addTransition(1, 'a', 1);
  automaton1.addTransition(1, 'b', 1);

  fa::Automaton automaton2;
  automaton2.addState(42);
  automaton2.setStateInitial(42);
  automaton2.setStateFinal(42);
  automaton2.addSymbol('a');
  automaton2.addSymbol('c');
  automaton2.addTransition(42, 'c', 42);

  fa::Automaton automaton3 = fa::Automaton::createIntersection(automaton1, automaton2);
  EXPECT_TRUE(automaton3.isLanguageEmpty());
}

TEST(testCreateIntersection, differentAlphabet2)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.setStateFinal(42);
  automaton.addSymbol('a');
  automaton.addSymbol('c');
  automaton.addTransition(42, 'a', 42);
  automaton.addTransition(42, 'c', 42);

  fa::Automaton automaton2;
  automaton2.addState(69);
  automaton2.setStateInitial(69);
  automaton2.setStateFinal(69);
  automaton2.addSymbol('a');
  automaton2.addTransition(69, 'a', 69);

  fa::Automaton automaton5; // reconize only "a" and ""
  automaton5.addState(314);
  automaton5.setStateInitial(314);
  automaton5.setStateFinal(314);
  automaton5.addState(69);
  automaton5.setStateFinal(69);
  automaton5.addSymbol('a');
  automaton5.addTransition(314, 'a', 69);

  fa::Automaton automaton3 = fa::Automaton::createIntersection(automaton, allWords); // allwords contains only a and b
  fa::Automaton automaton4 = fa::Automaton::createIntersection(automaton, alternateLetterAThenB);

  EXPECT_TRUE(automaton3.isIncludedIn(automaton2));
  EXPECT_TRUE(automaton2.isIncludedIn(automaton3));
  EXPECT_TRUE(automaton5.isIncludedIn(automaton4));
  EXPECT_TRUE(automaton4.isIncludedIn(automaton5));
}

//--------------testIsIncludeIn--------------------------------
TEST(testIsIncludeIn, testPresets)
{
  EXPECT_TRUE(beginAndEndLetterA.isIncludedIn(beginLetterA));
  EXPECT_TRUE(beginAndEndLetterA.isIncludedIn(endLetterA));

  EXPECT_TRUE(alternateLetterAThenBBeginA.isIncludedIn(beginLetterA));
  EXPECT_TRUE(alternateLetterAThenBBeginA.isIncludedIn(alternateLetterAThenB));

  EXPECT_TRUE(alternateLetterAThenBEndA.isIncludedIn(endLetterA));
  EXPECT_TRUE(alternateLetterAThenBEndA.isIncludedIn(alternateLetterAThenB));

  EXPECT_TRUE(alternateLetterAThenBBeginAEndA.isIncludedIn(beginLetterA));
  EXPECT_TRUE(alternateLetterAThenBBeginAEndA.isIncludedIn(alternateLetterAThenBEndA));

  EXPECT_TRUE(beginLetterA.isIncludedIn(allWords));
  EXPECT_TRUE(endLetterA.isIncludedIn(allWords));
  EXPECT_TRUE(alternateLetterAThenB.isIncludedIn(allWords));
}

TEST(testIsIncludeIn, testIntersectionOfAlphabetsEmpty)
{
  fa::Automaton automaton1;
  automaton1.addState(42);
  automaton1.setStateInitial(42);
  automaton1.addState(69);
  automaton1.setStateFinal(69);
  automaton1.addSymbol('a');
  automaton1.addSymbol('b');
  automaton1.addTransition(42, 'a', 69);
  automaton1.addTransition(69, 'b', 69);

  fa::Automaton automaton2;
  automaton2.addState(42);
  automaton2.setStateInitial(42);
  automaton2.addState(69);
  automaton2.setStateFinal(69);
  automaton2.addSymbol('c');
  automaton2.addSymbol('d');
  automaton2.addTransition(42, 'c', 69);
  automaton2.addTransition(69, 'd', 69);

  EXPECT_FALSE(automaton1.isIncludedIn(automaton2));
}

TEST(testIsIncludeIn, testAlphabetDifferentButWorks)
{
  fa::Automaton automaton1;
  automaton1.addState(42);
  automaton1.setStateInitial(42);
  automaton1.addState(69);
  automaton1.setStateFinal(69);
  automaton1.addSymbol('a');
  automaton1.addSymbol('b');
  automaton1.addTransition(42, 'a', 69);

  fa::Automaton automaton2;
  automaton2.addState(42);
  automaton2.setStateInitial(42);
  automaton2.addState(69);
  automaton2.setStateFinal(69);
  automaton2.addSymbol('a');
  automaton2.addSymbol('d');
  automaton2.addTransition(42, 'a', 69);
  automaton2.addTransition(69, 'd', 69);

  EXPECT_TRUE(automaton1.isIncludedIn(automaton2));
}

TEST(testIsIncludeIn, testAlphabetDifferentAndDontWorks)
{
  fa::Automaton automaton1;
  automaton1.addState(42);
  automaton1.setStateInitial(42);
  automaton1.addState(69);
  automaton1.setStateFinal(69);
  automaton1.addSymbol('a');
  automaton1.addSymbol('b');
  automaton1.addTransition(42, 'a', 69);
  automaton1.addTransition(69, 'b', 69);

  fa::Automaton automaton2;
  automaton2.addState(42);
  automaton2.setStateInitial(42);
  automaton2.addState(69);
  automaton2.setStateFinal(69);
  automaton2.addSymbol('a');
  automaton2.addSymbol('d');
  automaton2.addTransition(42, 'a', 69);
  automaton2.addTransition(69, 'd', 69);

  EXPECT_FALSE(automaton1.isIncludedIn(automaton2));
}

// -------------------testCreateDeterministic-------------------

#ifdef DEVELOPMENT

TEST(testCreateDeterministic, withEpsilonTransition)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.setStateFinal(3);
  automaton.addState(4);
  automaton.setStateFinal(4);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(42, fa::Epsilon, 2);
  automaton.addTransition(1, 'a', 2);
  automaton.addTransition(1, 'a', 4);
  automaton.addTransition(1, 'b', 4);
  automaton.addTransition(1, fa::Epsilon, 3);
  automaton.addTransition(2, 'b', 3);
  automaton.addTransition(3, fa::Epsilon, 42);
  automaton.addTransition(4, 'a', 3);

  fa::Automaton automaton2 = fa::Automaton::createDeterministic(automaton);

  EXPECT_TRUE(automaton2.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(automaton2));
  EXPECT_TRUE(automaton2.isDeterministic());
}
#endif

TEST(testCreateDeterministic, testPresets)
{
  fa::Automaton automaton1 = fa::Automaton::createDeterministic(beginLetterA);
  EXPECT_TRUE(automaton1.isIncludedIn(beginLetterA));
  EXPECT_TRUE(beginLetterA.isIncludedIn(automaton1));
  EXPECT_TRUE(automaton1.isDeterministic());
  fa::Automaton automaton2 = fa::Automaton::createDeterministic(endLetterA);
  EXPECT_TRUE(automaton2.isIncludedIn(endLetterA));
  EXPECT_TRUE(endLetterA.isIncludedIn(automaton2));
  EXPECT_TRUE(automaton2.isDeterministic());
  fa::Automaton automaton3 = fa::Automaton::createDeterministic(alternateLetterAThenBBeginA);
  EXPECT_TRUE(automaton3.isIncludedIn(alternateLetterAThenBBeginA));
  EXPECT_TRUE(alternateLetterAThenBBeginA.isIncludedIn(automaton3));
  EXPECT_TRUE(automaton3.isDeterministic());
  fa::Automaton automaton4 = fa::Automaton::createDeterministic(alternateLetterAThenBEndA);
  EXPECT_TRUE(automaton4.isIncludedIn(alternateLetterAThenBEndA));
  EXPECT_TRUE(alternateLetterAThenBEndA.isIncludedIn(automaton4));
  EXPECT_TRUE(automaton4.isDeterministic());
  fa::Automaton automaton5 = fa::Automaton::createDeterministic(alternateLetterAThenB);
  EXPECT_TRUE(automaton5.isIncludedIn(alternateLetterAThenB));
  EXPECT_TRUE(alternateLetterAThenB.isIncludedIn(automaton5));
  EXPECT_TRUE(automaton5.isDeterministic());
  fa::Automaton automaton6 = fa::Automaton::createDeterministic(emptyLanguage);
  EXPECT_TRUE(automaton6.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton6));
  EXPECT_TRUE(automaton6.isDeterministic());
  fa::Automaton automaton7 = fa::Automaton::createDeterministic(allWords);
  EXPECT_TRUE(automaton7.isIncludedIn(allWords));
  EXPECT_TRUE(allWords.isIncludedIn(automaton7));
  EXPECT_TRUE(automaton7.isDeterministic());
}

TEST(testCreateDeterministic, intialStateIsFinal)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.setStateFinal(42);
  automaton.addState(1);
  automaton.setStateInitial(1);
  automaton.setStateFinal(1);
  automaton.addSymbol('a');
  automaton.addSymbol('b');

  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(1, 'a', 1);
  automaton.addTransition(1, 'b', 1);

  fa::Automaton automaton2 = fa::Automaton::createDeterministic(automaton);

  EXPECT_TRUE(automaton2.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(automaton2));
  EXPECT_TRUE(automaton2.isDeterministic());
}

//-----------------testMinimalMoore--------------------------------

TEST(testMinimalMoore, testPresets)
{
  fa::Automaton automaton1 = fa::Automaton::createMinimalMoore(beginLetterA);
  EXPECT_TRUE(automaton1.isIncludedIn(beginLetterA));
  EXPECT_TRUE(beginLetterA.isIncludedIn(automaton1));
  fa::Automaton automaton2 = fa::Automaton::createMinimalMoore(endLetterA);
  EXPECT_TRUE(automaton2.isIncludedIn(endLetterA));
  EXPECT_TRUE(endLetterA.isIncludedIn(automaton2));
  fa::Automaton automaton3 = fa::Automaton::createMinimalMoore(alternateLetterAThenBBeginA);
  EXPECT_TRUE(automaton3.isIncludedIn(alternateLetterAThenBBeginA));
  EXPECT_TRUE(alternateLetterAThenBBeginA.isIncludedIn(automaton3));
  fa::Automaton automaton4 = fa::Automaton::createMinimalMoore(alternateLetterAThenBEndA);
  EXPECT_TRUE(automaton4.isIncludedIn(alternateLetterAThenBEndA));
  EXPECT_TRUE(alternateLetterAThenBEndA.isIncludedIn(automaton4));
  fa::Automaton automaton5 = fa::Automaton::createMinimalMoore(alternateLetterAThenB);
  EXPECT_TRUE(automaton5.isIncludedIn(alternateLetterAThenB));
  EXPECT_TRUE(alternateLetterAThenB.isIncludedIn(automaton5));
  fa::Automaton automaton6 = fa::Automaton::createMinimalMoore(emptyLanguage);
  EXPECT_TRUE(automaton6.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton6));
  fa::Automaton automaton7 = fa::Automaton::createMinimalMoore(allWords);
  EXPECT_TRUE(automaton7.isIncludedIn(allWords));
  EXPECT_TRUE(allWords.isIncludedIn(automaton7));
}

TEST(testMinimalMoore, notConnexe)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(42, 'b', 2);
  automaton.addTransition(1, 'a', 1);
  automaton.addTransition(1, 'b', 1);
  automaton.addTransition(2, 'a', 2);
  automaton.addTransition(2, 'b', 2);
  automaton.addTransition(3, 'a', 3);
  automaton.addTransition(3, 'b', 3);

  fa::Automaton automaton2 = fa::Automaton::createMinimalMoore(automaton);

  EXPECT_TRUE(automaton2.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(automaton2));
  EXPECT_EQ(automaton2.countStates(), 3u);
}

//------------------testMinimalBrzozowski-----------------------

TEST(testMinimalBrzozowski, testPresets)
{
  fa::Automaton automaton1 = fa::Automaton::createMinimalBrzozowski(beginLetterA);
  EXPECT_TRUE(automaton1.isIncludedIn(beginLetterA));
  EXPECT_TRUE(beginLetterA.isIncludedIn(automaton1));
  fa::Automaton automaton2 = fa::Automaton::createMinimalBrzozowski(endLetterA);
  EXPECT_TRUE(automaton2.isIncludedIn(endLetterA));
  EXPECT_TRUE(endLetterA.isIncludedIn(automaton2));
  fa::Automaton automaton3 = fa::Automaton::createMinimalBrzozowski(alternateLetterAThenBBeginA);
  EXPECT_TRUE(automaton3.isIncludedIn(alternateLetterAThenBBeginA));
  EXPECT_TRUE(alternateLetterAThenBBeginA.isIncludedIn(automaton3));
  fa::Automaton automaton4 = fa::Automaton::createMinimalBrzozowski(alternateLetterAThenBEndA);
  EXPECT_TRUE(automaton4.isIncludedIn(alternateLetterAThenBEndA));
  EXPECT_TRUE(alternateLetterAThenBEndA.isIncludedIn(automaton4));
  fa::Automaton automaton5 = fa::Automaton::createMinimalBrzozowski(alternateLetterAThenB);
  EXPECT_TRUE(automaton5.isIncludedIn(alternateLetterAThenB));
  EXPECT_TRUE(alternateLetterAThenB.isIncludedIn(automaton5));
  fa::Automaton automaton6 = fa::Automaton::createMinimalBrzozowski(emptyLanguage);
  EXPECT_TRUE(automaton6.isIncludedIn(emptyLanguage));
  EXPECT_TRUE(emptyLanguage.isIncludedIn(automaton6));
  fa::Automaton automaton7 = fa::Automaton::createMinimalBrzozowski(allWords);
  EXPECT_TRUE(automaton7.isIncludedIn(allWords));
  EXPECT_TRUE(allWords.isIncludedIn(automaton7));
}

TEST(testMinimalBrzozowski, notConnexe)
{
  fa::Automaton automaton;
  automaton.addState(42);
  automaton.setStateInitial(42);
  automaton.addState(1);
  automaton.setStateFinal(1);
  automaton.addState(2);
  automaton.addState(3);
  automaton.addSymbol('a');
  automaton.addSymbol('b');
  automaton.addTransition(42, 'a', 1);
  automaton.addTransition(42, 'b', 2);
  automaton.addTransition(1, 'a', 1);
  automaton.addTransition(1, 'b', 1);
  automaton.addTransition(2, 'a', 2);
  automaton.addTransition(2, 'b', 2);
  automaton.addTransition(3, 'a', 3);
  automaton.addTransition(3, 'b', 3);

  fa::Automaton automaton2 = fa::Automaton::createMinimalBrzozowski(automaton);

  EXPECT_TRUE(automaton2.isIncludedIn(automaton));
  EXPECT_TRUE(automaton.isIncludedIn(automaton2));
  EXPECT_EQ(automaton2.countStates(), 3u);
}

//------------------testCreateWithoutEpsilon-----------------------

#ifdef DEVELOPMENT

TEST(testCreateWithoutEpsilon, td3_exo10)
{
  fa::Automaton automaton = fa::Automaton::createWithoutEpsilon(td3_exo10);
  EXPECT_FALSE(automaton.hasEpsilonTransition());
  EXPECT_TRUE(automaton.isValid());
}

TEST(testCreateWithoutEpsilon, td3_exo11)
{
  fa::Automaton automaton = fa::Automaton::createWithoutEpsilon(td3_exo11);
  EXPECT_FALSE(automaton.hasEpsilonTransition());
  EXPECT_TRUE(automaton.isValid());
}

#endif

//============================================================================
//============================ Main function =================================

int main(int argc, char **argv)
{
  //============================================================================
  //======================== Initiate the presets automata =====================

  // automaton that recognizes the langage of the words that begin with the lettre 'a'
  beginLetterA.addState(42);
  beginLetterA.setStateInitial(42);
  beginLetterA.addState(1);
  beginLetterA.setStateFinal(1);
  beginLetterA.addSymbol('a');
  beginLetterA.addSymbol('b');
  beginLetterA.addTransition(42, 'a', 1);
  beginLetterA.addTransition(1, 'a', 1);
  beginLetterA.addTransition(1, 'b', 1);

  // automaton that recognizes the langage of the words that end with the lettre 'a'
  endLetterA.addState(42);
  endLetterA.setStateInitial(42);
  endLetterA.addState(1);
  endLetterA.setStateFinal(1);
  endLetterA.addSymbol('a');
  endLetterA.addSymbol('b');
  endLetterA.addTransition(42, 'a', 42);
  endLetterA.addTransition(42, 'b', 42);
  endLetterA.addTransition(42, 'a', 1);

  // automaton that recognizes the langage of the words that begin and end with the lettre 'a'
  beginAndEndLetterA.addState(42);
  beginAndEndLetterA.setStateInitial(42);
  beginAndEndLetterA.addState(1);
  beginAndEndLetterA.addState(2);
  beginAndEndLetterA.setStateFinal(2);
  beginAndEndLetterA.addSymbol('a');
  beginAndEndLetterA.addSymbol('b');
  beginAndEndLetterA.addTransition(42, 'a', 1);
  beginAndEndLetterA.addTransition(42, 'a', 2);
  beginAndEndLetterA.addTransition(1, 'a', 1);
  beginAndEndLetterA.addTransition(1, 'b', 1);
  beginAndEndLetterA.addTransition(1, 'a', 2);

  // automaton that recognizes the langage of the words that alternate the lettre 'a' and 'b'
  alternateLetterAThenB.addState(42);
  alternateLetterAThenB.setStateInitial(42);
  alternateLetterAThenB.setStateFinal(42);
  alternateLetterAThenB.addState(1);
  alternateLetterAThenB.setStateFinal(1);
  alternateLetterAThenB.addState(2);
  alternateLetterAThenB.setStateFinal(2);
  alternateLetterAThenB.addSymbol('a');
  alternateLetterAThenB.addSymbol('b');
  alternateLetterAThenB.addTransition(42, 'a', 2);
  alternateLetterAThenB.addTransition(42, 'b', 1);
  alternateLetterAThenB.addTransition(1, 'a', 2);
  alternateLetterAThenB.addTransition(2, 'b', 1);

  // automaton that recognizes the langage of the words that alternate the lettre 'a' and 'b' and begin with the lettre 'a'
  alternateLetterAThenBBeginA.addState(42);
  alternateLetterAThenBBeginA.setStateInitial(42);
  alternateLetterAThenBBeginA.addState(1);
  alternateLetterAThenBBeginA.setStateFinal(1);
  alternateLetterAThenBBeginA.addState(2);
  alternateLetterAThenBBeginA.setStateFinal(2);
  alternateLetterAThenBBeginA.addSymbol('a');
  alternateLetterAThenBBeginA.addSymbol('b');
  alternateLetterAThenBBeginA.addTransition(42, 'a', 2);
  alternateLetterAThenBBeginA.addTransition(1, 'a', 2);
  alternateLetterAThenBBeginA.addTransition(2, 'b', 1);

  // automaton that recognizes the langage of the words that alternate the lettre 'a' and 'b' and end with the lettre 'a'
  alternateLetterAThenBEndA.addState(42);
  alternateLetterAThenBEndA.setStateInitial(42);
  alternateLetterAThenBEndA.addState(1);
  alternateLetterAThenBEndA.addState(2);
  alternateLetterAThenBEndA.setStateFinal(2);
  alternateLetterAThenBEndA.addSymbol('a');
  alternateLetterAThenBEndA.addSymbol('b');
  alternateLetterAThenBEndA.addTransition(42, 'a', 2);
  alternateLetterAThenBEndA.addTransition(42, 'b', 1);
  alternateLetterAThenBEndA.addTransition(1, 'a', 2);
  alternateLetterAThenBEndA.addTransition(2, 'b', 1);

  // automaton that recognizes the langage of the words that alternate the lettre 'a' and 'b' and end with the lettre 'a' and begin with the lettre 'a'
  alternateLetterAThenBBeginAEndA.addState(42);
  alternateLetterAThenBBeginAEndA.setStateInitial(42);
  alternateLetterAThenBBeginAEndA.addState(1);
  alternateLetterAThenBBeginAEndA.setStateFinal(1);
  alternateLetterAThenBBeginAEndA.addSymbol('a');
  alternateLetterAThenBBeginAEndA.addSymbol('b');
  alternateLetterAThenBBeginAEndA.addTransition(42, 'a', 1);
  alternateLetterAThenBBeginAEndA.addTransition(1, 'b', 42);

  // automaton that recognizes the langage of the words that begin with the lettre 'a'
  beginLetterB.addState(42);
  beginLetterB.setStateInitial(42);
  beginLetterB.addState(1);
  beginLetterB.setStateFinal(1);
  beginLetterB.addSymbol('b');
  beginLetterB.addSymbol('a');
  beginLetterB.addTransition(42, 'b', 1);
  beginLetterB.addTransition(1, 'b', 1);
  beginLetterB.addTransition(1, 'a', 1);

  // automaton that recognizes the langage of the words that end with the lettre 'a'
  endLetterB.addState(42);
  endLetterB.setStateInitial(42);
  endLetterB.addState(1);
  endLetterB.setStateFinal(1);
  endLetterB.addSymbol('b');
  endLetterB.addSymbol('a');
  endLetterB.addTransition(42, 'b', 42);
  endLetterB.addTransition(42, 'a', 42);
  endLetterB.addTransition(42, 'b', 1);

  // automaton that recognizes the langage of the words that begin and end with the lettre 'a'
  beginAndEndLetterB.addState(42);
  beginAndEndLetterB.setStateInitial(42);
  beginAndEndLetterB.addState(1);
  beginAndEndLetterB.addState(2);
  beginAndEndLetterB.setStateFinal(2);
  beginAndEndLetterB.addSymbol('b');
  beginAndEndLetterB.addSymbol('a');
  beginAndEndLetterB.addTransition(42, 'b', 1);
  beginAndEndLetterB.addTransition(42, 'b', 2);
  beginAndEndLetterB.addTransition(1, 'b', 1);
  beginAndEndLetterB.addTransition(1, 'a', 1);
  beginAndEndLetterB.addTransition(1, 'b', 2);

  // automaton that recognizes the langage of the words that alternate the lettre 'a' and 'b' and begin with the lettre 'a'
  alternateLetterAThenBBeginB.addState(42);
  alternateLetterAThenBBeginB.setStateInitial(42);
  alternateLetterAThenBBeginB.addState(1);
  alternateLetterAThenBBeginB.setStateFinal(1);
  alternateLetterAThenBBeginB.addState(2);
  alternateLetterAThenBBeginB.setStateFinal(2);
  alternateLetterAThenBBeginB.addSymbol('b');
  alternateLetterAThenBBeginB.addSymbol('a');
  alternateLetterAThenBBeginB.addTransition(42, 'b', 2);
  alternateLetterAThenBBeginB.addTransition(1, 'b', 2);
  alternateLetterAThenBBeginB.addTransition(2, 'a', 1);

  // automaton that recognizes the langage of the words that alternate the lettre 'a' and 'b' and end with the lettre 'a'
  alternateLetterAThenBEndB.addState(42);
  alternateLetterAThenBEndB.setStateInitial(42);
  alternateLetterAThenBEndB.addState(1);
  alternateLetterAThenBEndB.addState(2);
  alternateLetterAThenBEndB.setStateFinal(2);
  alternateLetterAThenBEndB.addSymbol('b');
  alternateLetterAThenBEndB.addSymbol('a');
  alternateLetterAThenBEndB.addTransition(42, 'b', 2);
  alternateLetterAThenBEndB.addTransition(42, 'a', 1);
  alternateLetterAThenBEndB.addTransition(1, 'b', 2);
  alternateLetterAThenBEndB.addTransition(2, 'a', 1);

  // automaton that recognizes the langage of the words that alternate the lettre 'a' and 'b' and end with the lettre 'a' and begin with the lettre 'a'
  alternateLetterAThenBBeginBEndB.addState(42);
  alternateLetterAThenBBeginBEndB.setStateInitial(42);
  alternateLetterAThenBBeginBEndB.addState(1);
  alternateLetterAThenBBeginBEndB.setStateFinal(1);
  alternateLetterAThenBBeginBEndB.addSymbol('b');
  alternateLetterAThenBBeginBEndB.addSymbol('a');
  alternateLetterAThenBBeginBEndB.addTransition(42, 'b', 1);
  alternateLetterAThenBBeginBEndB.addTransition(1, 'a', 42);

  // complements :

  beginLetterAComplement.addState(42);
  beginLetterAComplement.setStateInitial(42);
  beginLetterAComplement.setStateFinal(42);
  beginLetterAComplement.addState(1);
  beginLetterAComplement.setStateFinal(1);
  beginLetterAComplement.addSymbol('b');
  beginLetterAComplement.addSymbol('a');
  beginLetterAComplement.addTransition(42, 'b', 1);
  beginLetterAComplement.addTransition(1, 'a', 1);
  beginLetterAComplement.addTransition(1, 'b', 1);

  endLetterAComplement.addState(42);
  endLetterAComplement.setStateInitial(42);
  endLetterAComplement.setStateFinal(42);
  endLetterAComplement.addState(1);
  endLetterAComplement.addSymbol('b');
  endLetterAComplement.addSymbol('a');
  endLetterAComplement.addTransition(42, 'b', 42);
  endLetterAComplement.addTransition(42, 'a', 1);
  endLetterAComplement.addTransition(1, 'a', 1);
  endLetterAComplement.addTransition(1, 'b', 42);

  beginAndEndLetterAComplement = beginAndEndLetterB;
  beginAndEndLetterAComplement.setStateFinal(42);

  alternateLetterAThenBComplement.addState(42);
  alternateLetterAThenBComplement.setStateInitial(42);
  alternateLetterAThenBComplement.addState(1);
  alternateLetterAThenBComplement.addState(2);
  alternateLetterAThenBComplement.addState(3);
  alternateLetterAThenBComplement.setStateFinal(3);
  alternateLetterAThenBComplement.addSymbol('a');
  alternateLetterAThenBComplement.addSymbol('b');

  alternateLetterAThenBComplement.addTransition(42, 'a', 2);
  alternateLetterAThenBComplement.addTransition(42, 'b', 1);
  alternateLetterAThenBComplement.addTransition(1, 'a', 2);
  alternateLetterAThenBComplement.addTransition(2, 'b', 1);
  alternateLetterAThenBComplement.addTransition(1, 'b', 3);
  alternateLetterAThenBComplement.addTransition(2, 'a', 3);
  alternateLetterAThenBComplement.addTransition(3, 'a', 3);
  alternateLetterAThenBComplement.addTransition(3, 'b', 3);

  alternateLetterAThenBBeginAComplement.addState(42);
  alternateLetterAThenBBeginAComplement.setStateInitial(42);
  alternateLetterAThenBBeginAComplement.setStateFinal(42);
  alternateLetterAThenBBeginAComplement.addState(1);
  alternateLetterAThenBBeginAComplement.addState(2);
  alternateLetterAThenBBeginAComplement.addState(3);
  alternateLetterAThenBBeginAComplement.setStateFinal(3);
  alternateLetterAThenBBeginAComplement.addSymbol('a');
  alternateLetterAThenBBeginAComplement.addSymbol('b');
  alternateLetterAThenBBeginAComplement.addTransition(42, 'a', 2);
  alternateLetterAThenBBeginAComplement.addTransition(42, 'b', 3);
  alternateLetterAThenBBeginAComplement.addTransition(1, 'a', 2);
  alternateLetterAThenBBeginAComplement.addTransition(1, 'b', 3);
  alternateLetterAThenBBeginAComplement.addTransition(2, 'a', 3);
  alternateLetterAThenBBeginAComplement.addTransition(2, 'b', 1);
  alternateLetterAThenBBeginAComplement.addTransition(3, 'a', 3);
  alternateLetterAThenBBeginAComplement.addTransition(3, 'b', 3);

  alternateLetterAThenBEndAComplement.addState(42);
  alternateLetterAThenBEndAComplement.setStateInitial(42);
  alternateLetterAThenBEndAComplement.setStateFinal(42);
  alternateLetterAThenBEndAComplement.addState(1);
  alternateLetterAThenBEndAComplement.setStateFinal(1);
  alternateLetterAThenBEndAComplement.addState(2);
  alternateLetterAThenBEndAComplement.addState(3);
  alternateLetterAThenBEndAComplement.setStateFinal(3);
  alternateLetterAThenBEndAComplement.addSymbol('a');
  alternateLetterAThenBEndAComplement.addSymbol('b');
  alternateLetterAThenBEndAComplement.addTransition(42, 'a', 2);
  alternateLetterAThenBEndAComplement.addTransition(42, 'b', 1);
  alternateLetterAThenBEndAComplement.addTransition(1, 'a', 2);
  alternateLetterAThenBEndAComplement.addTransition(1, 'b', 3);
  alternateLetterAThenBEndAComplement.addTransition(2, 'a', 3);
  alternateLetterAThenBEndAComplement.addTransition(2, 'b', 1);
  alternateLetterAThenBEndAComplement.addTransition(3, 'a', 3);
  alternateLetterAThenBEndAComplement.addTransition(3, 'b', 3);

  alternateLetterAThenBBeginAEndAComplement.addState(42);
  alternateLetterAThenBBeginAEndAComplement.setStateInitial(42);
  alternateLetterAThenBBeginAEndAComplement.setStateFinal(42);
  alternateLetterAThenBBeginAEndAComplement.addState(1);
  alternateLetterAThenBBeginAEndAComplement.addState(2);
  alternateLetterAThenBBeginAEndAComplement.setStateFinal(2);
  alternateLetterAThenBBeginAEndAComplement.addSymbol('a');
  alternateLetterAThenBBeginAEndAComplement.addSymbol('b');
  alternateLetterAThenBBeginAEndAComplement.addTransition(42, 'a', 1);
  alternateLetterAThenBBeginAEndAComplement.addTransition(42, 'b', 2);
  alternateLetterAThenBBeginAEndAComplement.addTransition(1, 'a', 2);
  alternateLetterAThenBBeginAEndAComplement.addTransition(1, 'b', 42);
  alternateLetterAThenBBeginAEndAComplement.addTransition(2, 'a', 2);
  alternateLetterAThenBBeginAEndAComplement.addTransition(2, 'b', 2);

  // automaton that recognizes the empty langage
  emptyLanguage.addState(42);
  emptyLanguage.setStateInitial(42);
  emptyLanguage.addSymbol('a');
  emptyLanguage.addSymbol('b');

  // automaton that recognizes the langage of all words
  allWords.addState(42);
  allWords.setStateInitial(42);
  allWords.setStateFinal(42);
  allWords.addSymbol('a');
  allWords.addSymbol('b');
  allWords.addTransition(42, 'a', 42);
  allWords.addTransition(42, 'b', 42);

  // automaton from the td
  td3_exo10.addState(0);
  td3_exo10.setStateInitial(0);
  td3_exo10.addState(1);
  td3_exo10.addState(2);
  td3_exo10.addState(3);
  td3_exo10.setStateFinal(3);
  td3_exo10.addSymbol('a');
  td3_exo10.addSymbol('b');
  td3_exo10.addTransition(0, 'a', 1);
  td3_exo10.addTransition(0, 'b', 2);
  td3_exo10.addTransition(0, fa::Epsilon, 3);
  td3_exo10.addTransition(1, fa::Epsilon, 0);
  td3_exo10.addTransition(1, 'b', 2);
  td3_exo10.addTransition(2, 'a', 3);
  td3_exo10.addTransition(2, fa::Epsilon, 1);
  td3_exo10.addTransition(3, 'a', 0);
  td3_exo10.addTransition(3, fa::Epsilon, 2);

  td3_exo11.addState(0);
  td3_exo11.setStateInitial(0);
  td3_exo11.addState(1);
  td3_exo11.addState(2);
  td3_exo11.addState(3);
  td3_exo11.setStateFinal(3);
  td3_exo11.addState(4);
  td3_exo11.setStateFinal(4);
  td3_exo11.addSymbol('a');
  td3_exo11.addSymbol('b');
  td3_exo11.addTransition(0, 'a', 1);
  td3_exo11.addTransition(0, fa::Epsilon, 2);
  td3_exo11.addTransition(1, 'b', 2);
  td3_exo11.addTransition(1, fa::Epsilon, 4);
  td3_exo11.addTransition(2, 'b', 3);
  td3_exo11.addTransition(2, fa::Epsilon, 4);
  td3_exo11.addTransition(3, 'b', 0);
  td3_exo11.addTransition(4, 'a', 2);
  td3_exo11.addTransition(4, 'b', 3);

  //=====================================================================
  //================== Initiate other global variables =================

  generateWords("", 5, wordsToTest);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}