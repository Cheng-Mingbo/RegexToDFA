//
// Created by Cheng MingBo on 2023/3/12.
//

#include "DFA.h"
#include "NFA.h"
#include <stack>
#include <queue>

DFAState DFAStates[256];
int DFAStateNum = 0;

std::set<int> DFA::epcloure(std::set<int> set) {
    std::stack<int> epStack;
    
    for (const auto& e : set) {
        epStack.push(e);
    }
    
    while (!epStack.empty()) {
        int temp = epStack.top();
        epStack.pop();
        
        for (const auto& e : NFA::NFAStates[temp].epsilonTrans_) {
            if (!set.count(e)) {
                set.insert(e);
                epStack.push(e);
            }
        }
    }
    
    return set;
}

std::set<int> DFA::moveEpCloure(std::set<int> set, char ch) {
    std::set<int> temp;
    
    for (const auto& e : set) {
        if (NFA::NFAStates[e].input_ == ch) {
            temp.insert(NFA::NFAStates[e].chTrans);
        }
    }
    
    temp = epcloure(temp);
    return temp;
}

bool isFinal(const NFA& node, const std::set<int>& set) {
    for (const auto& e : set) {
        if (e == node.tail->index_) {
            return true;
        }
    }
    return false;
}

DFA DFA::NFAToDFA() {
    std::cout << "***************     DFA     ***************" << '\n';
    
    DFA dfa;
    std::set<std::set<int>> states;
    
    for (const auto& e : postfix_) {
        if (e >= 'a' && e <= 'z') {
            dfa.terminator_.insert(e);
        }
    }
    dfa.startState_ = 0;
    std::set<int> tempSet;
    tempSet.insert(nfa_.head->index_);
    DFAStates[0].closure_ = epcloure(tempSet);
    DFAStates[0].isFinal_ = isFinal(nfa_, DFAStates[0].closure_);
    
    DFAStateNum++;
    std::queue<int> que;
    que.push(dfa.startState_);
    
    while (!que.empty()) {
        int num = que.front();
        que.pop();
        
        for (const auto& e : dfa.terminator_) {
            std::set<int> temp = moveEpCloure(DFAStates[num].closure_, e);
            if(!states.count(temp) && !temp.empty()) {
                states.insert(temp);
                DFAStates[DFAStateNum].closure_ = temp;
                DFAStates[num].edges[DFAStates[num].edgeNum_].input_ = e;
                DFAStates[num].edges[DFAStates[num].edgeNum_].trans_ = DFAStateNum;
                DFAStates[num].edgeNum_++;
                dfa.trans_[num][e - 'a'] = DFAStateNum;
                DFAStates[DFAStateNum].isFinal_ = isFinal(nfa_, DFAStates[DFAStateNum].closure_);
                que.push(DFAStateNum);
                DFAStateNum++;
            } else {
                for (int i = 0; i < DFAStateNum; i++) {
                    if (temp == DFAStates[i].closure_) {
                        DFAStates[num].edges[DFAStates[num].edgeNum_].input_ = e;		/*该状态弧的输入即为当前终结符*/
                        DFAStates[num].edges[DFAStates[num].edgeNum_].trans_ = i;			/*该弧转移到的状态即为i*/
                        DFAStates[num].edgeNum_++;										/*该状态弧的数目加一*/
    
                        dfa.trans_[num][e - 'a'] = i;		/*更新转移矩阵*/
                        break;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < DFAStateNum; i++) {
        if (DFAStates[i].isFinal_) {
            dfa.endStates_.insert(i);
        }
    }
    return dfa;
}

void DFA::printDFA(DFA d) {
    int i, j;
    std::cout<<"DFA总共有"<<DFAStateNum<<"个状态，"<<"初态为"<<d.startState_<<std::endl<<std::endl;
    
    std::cout<<"有穷字母表为｛ ";
    std::set<char>::iterator it;
    for(it = d.terminator_.begin(); it != d.terminator_.end(); it++)
    {
        std::cout<<*it<<' ';
    }
    std::cout<<'}'<<std::endl<<std::endl;
    
    std::cout<<"终态集为｛ ";
    std::set<int>::iterator iter;
    for(iter = d.endStates_.begin(); iter != d.endStates_.end(); iter++)
    {
        std::cout<<*iter<<' ';
    }
    std::cout<<'}'<<std::endl<<std::endl;
    
    std::cout<<"转移函数为："<<std::endl;
    for(i = 0; i < DFAStateNum; i++)
    {
        for(j = 0; j < DFAStates[i].edgeNum_; j++)
        {
            
            if(DFAStates[DFAStates[i].edges[j].trans_].isFinal_)
            {
                std::cout<<DFAStates[i].index_<<"-->'"<<DFAStates[i].edges[j].input_;
                std::cout<<"'--><"<<DFAStates[i].edges[j].trans_<<">\t";
            }
            else
            {
                std::cout<<DFAStates[i].index_<<"-->'"<<DFAStates[i].edges[j].input_;
                std::cout<<"'-->"<<DFAStates[i].edges[j].trans_<<'\t';
            }
        }
        std::cout<<std::endl;
    }
    
    std::cout<<std::endl<<"转移矩阵为："<<std::endl<<"     ";
    std::set<char>::iterator t;
    for(t = d.terminator_.begin(); t != d.terminator_.end(); t++)
    {
        std::cout<<*t<<"   ";
    }
    std::cout<<std::endl;
    
    for(i = 0; i < DFAStateNum; i++)
    {
        
        if(d.endStates_.count(i))
        {
            std::cout<<'<'<<i<<">  ";
        }
        else
        {
            std::cout<<' '<<i<<"   ";
        }
        
        for(j = 0; j < 26; j++)
        {
            if(d.terminator_.count(j + 'a'))
            {
                if(d.trans_[i][j] != -1)
                {
                    std::cout<<d.trans_[i][j]<<"   ";
                }
                else
                {
                    std::cout<<"    ";
                }
            }
        }
        
        std::cout<<std::endl;
    }
}

void DFA::init() {
    for (int i = 0; i < 256; i++) {
        DFAStates[i].index_ = i;
        DFAStates[i].isFinal_ = false;
        for (auto & edge : DFAStates[i].edges) {
            edge.input_ = '#';
            edge.trans_ = -1;
        }
    }
}
