//
// Created by Cheng MingBo on 2023/3/12.
//

#ifndef REGEXTODFA_NFA_H
#define REGEXTODFA_NFA_H

#include <string>
#include <iostream>
#include <set>
#include <vector>

struct NFAState {
    int index_;
    char input_;
    int chTrans;
    
    std::set<int> epsilonTrans_;
    NFAState() : index_(-1), input_('#'), chTrans(-1) {}
};

class NFA {
  public:
    explicit NFA(std::string postfix) : postfix_(std::move(postfix)) {
        init();
        std::cout << postfix_ << '\n';
    }
    NFA() = default;
    
    static void init();
    NFA translateToNFA();
    static void printNFA(const NFA& nfa);
    static std::vector<NFAState> NFAStates;
    friend NFA createNFA();
    NFAState *head{ nullptr };
    NFAState *tail{ nullptr };
    std::string getPostfix() { return postfix_; }
  private:
    std::string postfix_;
};


#endif //REGEXTODFA_NFA_H
