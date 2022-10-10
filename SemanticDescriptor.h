#ifndef SEMANTICDESCRIPTOR_H_INCLUDED
#define SEMANTICDESCRIPTOR_H_INCLUDED

#include <iostream>
#include <map>
#include <cmath>
using namespace std;

class SemanticDescriptor
{
    private:
        string targetWord;
        map<string,int> contextWords;

    public:
        SemanticDescriptor(string _targetWord) : targetWord(_targetWord) {}
        string getTargetWord() { return targetWord; }
        void processContextWord(string s);
        int operator*(const SemanticDescriptor& desc);
        friend ostream& operator<<(ostream& os, const SemanticDescriptor& desc);
        friend istream& operator>>(istream& is, SemanticDescriptor& desc);
};

#endif // SEMANTICDESCRIPTOR_H_INCLUDED