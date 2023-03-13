//
// Created by Cheng MingBo on 2023/3/12.
//

#ifndef REGEXTODFA_H
#define REGEXTODFA_H
#include <string>
#include <memory>
#include "NFA.h"
#include "DFA.h"

class RegexToDFA {
  public:
    explicit RegexToDFA(std::string regex) : regex_(std::move(regex)) {
        postfix_ = convert();
        nfa_ = std::make_shared<NFA>(postfix_);
        NFA nfa = nfa_->translateToNFA();
        nfa_->printNFA(nfa);
        dfa_ = std::make_shared<DFA>(nfa, nfa_->getPostfix());
        dfa_->printDFA(dfa_->NFAToDFA());
    }
    
    
  private:
    std::string insert_concat();
    std::string convert();
    static int precedence(char c);
    
    std::string regex_;
    std::string postfix_;
    std::shared_ptr<NFA> nfa_;
    std::shared_ptr<DFA> dfa_;
};


#endif //REGEXTODFA_H
