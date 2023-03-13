//
// Created by Cheng MingBo on 2023/3/12.
//

#include "RegexToDFA.h"
#include <stack>

std::string RegexToDFA::insert_concat() {
    char c_1, c_2;
    std::string ret;
    for (int i = 0; i < regex_.size(); i++) {
        c_1 = regex_[i];
        if (i + 1 < regex_.size()) {
            c_2 = regex_[i + 1];
            ret.push_back(c_1);
            if (c_1 != '(' && c_2 != ')' && c_1 != '|' && c_2 != '|' && c_2 != '*') {
                ret.push_back('.');
            }
        }
    }
    ret.push_back(regex_[ regex_.size() - 1 ]);
    return ret;
}

int RegexToDFA::precedence(char c) {
    switch (c) {
        case '|':
            return 1;
        case '.':
            return 2;
        case '*':
            return 3;
        default:
            return 0;
    }
}

std::string RegexToDFA::convert() {
    std::string postfix;
    std::stack<char> opstack;
    
    regex_ = insert_concat();
    
    for (const char& c : regex_) {
        switch (c) {
            case '(':
                opstack.push(c);
                break;
            case ')':
                while (!opstack.empty() && opstack.top() != '(') {
                    postfix += opstack.top();
                    opstack.pop();
                }
                opstack.pop();
                break;
            case '|':
            case '.':
            case '*':
                while (!opstack.empty() && opstack.top() != '(' && precedence(opstack.top()) >= precedence(c)) {
                    postfix += opstack.top();
                    opstack.pop();
                }
                opstack.push(c);
                break;
            default:
                postfix += c;
                break;
        }
    }
    while (!opstack.empty()) {
        postfix += opstack.top();
        opstack.pop();
    }
    return postfix;
}