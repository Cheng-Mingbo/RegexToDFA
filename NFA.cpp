//
// Created by Cheng MingBo on 2023/3/12.
//

#include "NFA.h"
#include <stack>
#include <vector>

std::vector<NFAState> NFA::NFAStates(256);
static int NFAStateNum = 0;

void add(NFAState* node_1, NFAState* node_2, char ch) {
    node_1->input_ = ch;
    node_1->chTrans = node_2->index_;
}

void add(NFAState* node_1, NFAState* node_2) {
    node_1->epsilonTrans_.insert(node_2->index_);
}

bool notOperator(char ch) {
    return ch != '*' && ch != '|' && ch != '.' && ch != '(' && ch != ')';
}

NFA createNFA() {
    NFA node;
    
    node.head = &NFA::NFAStates[NFAStateNum];
    node.tail = &NFA::NFAStates[NFAStateNum + 1];
    NFAStateNum += 2;
    return node;
}

NFA NFA::translateToNFA() {
    std::stack<NFA> NFAStack;
    auto getTop = [&] {
        NFA node = NFAStack.top();
        NFAStack.pop();
        return node;
    };
    for (const auto& ch : postfix_) {
        if (notOperator(ch)) {
            NFA node = createNFA();
            add(node.head, node.tail, ch);
            NFAStack.push(node);
        } else if (ch == '*') {
            NFA node_1 = createNFA();
            NFA node_2 = getTop();
            add(node_2.tail, node_2.head);
            add(node_2.tail, node_1.tail);
            add(node_1.head, node_2.head);
            add(node_1.head, node_1.tail);
            NFAStack.push(node_1);
        } else if (ch == '|') {
            auto node_1 = getTop();
            auto node_2 = getTop();
            auto node = createNFA();
            
            add(node.head, node_1.head);
            add(node.head, node_2.head);
            add(node_1.tail, node.tail);
            add(node_2.tail, node.tail);
            
            NFAStack.push(node);
        } else if (ch == '.') {
            NFA node;
            auto node_2 = getTop();
            auto node_1 = getTop();
            add(node_1.tail, node_2.head);
            node.head = node_1.head;
            node.tail = node_2.tail;
            
            NFAStack.push(node);
        }
    }
    return NFAStack.top();
}

void NFA::printNFA(const NFA& nfa) {
    std::cout << "***************     NFA     ***************\n\n";
    std::cout << "NFA总共有" << NFAStateNum << "个状态\n";
    std::cout << "初态为" << nfa.head->index_ << "，终态为" << nfa.tail->index_ << std::endl;
    
    for (int i = 0; i < NFAStateNum; i++) {
        if (NFAStates[i].input_ != '#') {
            std::cout << NFAStates[i].index_ << "->" << NFAStates[i].input_ << "->" << NFAStates[i].chTrans << '\n';
        }
        for (const auto& it : NFAStates[i].epsilonTrans_) {
            std::cout << NFAStates[i].index_ << "->" << "\u03B5" << "->" << it << '\n';
        }
    }
    std::cout << std::endl;
}

void NFA::init() {
    for (int i = 0; i < 256; i++) {
        NFAStates[i].index_ = i;
    }
}
