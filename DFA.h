//
// Created by Cheng MingBo on 2023/3/12.
//

#ifndef REGEXTODFA_DFA_H
#define REGEXTODFA_DFA_H
#include <set>
#include <utility>
#include <vector>
#include <memory>
#include "NFA.h"

struct Edge {
    char input_;
    int trans_;
};

struct DFAState {
    bool isFinal_;
    int index_;
    std::set<int> closure_;
    int edgeNum_;
    Edge edges[10];
};

class DFA {
  public:
    explicit DFA(NFA nfa, std::string postfix) : nfa_(std::move(nfa)), postfix_(std::move(postfix)) {
        init();
        trans_ = std::move(std::vector<std::vector<int>>(256, std::vector<int>(26, -1)));
    }
    explicit DFA() {
        init();
        trans_ = std::move(std::vector<std::vector<int>>(256, std::vector<int>(26, -1)));
    }
    static std::set<int> epcloure(std::set<int> set);
    static std::set<int> moveEpCloure(std::set<int> set, char ch);
    
    DFA NFAToDFA();
    void printDFA(DFA d);
    void init();
  private:
    int startState_{};
    NFA nfa_;
    std::set<int> endStates_;
    std::set<char> terminator_;
    std::string postfix_;
    std::vector<std::vector<int>> trans_;
};


#endif //REGEXTODFA_DFA_H
