#include "SemanticDescriptor.h"

void SemanticDescriptor::processContextWord(string s)
{
    auto it = contextWords.find(s);
    if (it == contextWords.end())
        contextWords.insert({s,1});
    else    
        contextWords[s]++;
}

int SemanticDescriptor::operator*(const SemanticDescriptor& desc)
{
    int sum;                    // create sum var to store running sum
    for (auto it = this->contextWords.begin(); it != this->contextWords.end(); ++it)    // iterate through this->contextWords
    {
        auto it2 = desc.contextWords.find(it->first);   // search through desc contextWords with this->contextWords key
        if (it2 != desc.contextWords.end())             // if key is found
            sum += (it->second)*(it2->second);        // multiply values and add to running sum
    }
    return sum;                 // return sum once loop is finished
}

ostream& operator<<(ostream& os, const SemanticDescriptor& desc)
{
    os << desc.targetWord << " { ";                    // output targetWord in desc object
    // Loop through contextWords using iterators
    for (auto it = desc.contextWords.begin(); it != desc.contextWords.end(); ++it)
    {
        os << it->first << " " << it->second << " ";   // output each pair in map (word count) 
    }
    os << "}";
    return os;
}

istream& operator>>(istream& is, SemanticDescriptor& desc)
{
    desc.targetWord = "";
    desc.contextWords.clear();
    string targetWord;
    is >> targetWord;                   // extract targetWord
    desc.targetWord = targetWord;       // assign desc's targetWord to extracted targetWord

    char ch;                            
    is >> ch;                           // read one char
    if (ch != '{')                      
    {
        is.unget();
        is.clear(ios_base::failbit);
        return is;
    }
    string word;                        // temp local var for word and count
    int count;
    while (is)                          // while stream does not fail
    {
        is >> ch;
        if (ch =='}') return is;
        else
        {
            is.unget();
            is >> word >> count;        // extract word and count
            desc.contextWords.insert({word,count}); // insert extracted pair into contextWords map
        }
    }
    return is;
}
