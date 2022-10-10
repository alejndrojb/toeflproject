#include "SemanticDictionary.h"

vector<vector<string>> getSentenceLists(string text) 
{

    vector<vector<string>> sentenceLists;
    vector<string> sentenceList;
    string word = "";

    for(size_t i = 0; i < text.size(); i++)                         // loop through each char in text
    {
        if (isalpha(text[i]))
            word += tolower(text[i]);                               // if the ith char is alphabetical, change to lowercase and concatenate it onto word 
        else {
            if ((word.size() > 0) && (!isStopWord(word))) {         // if word size > 0 and not a stop word 
                sentenceList.push_back(word);                       // push back into sentenceList
                word = "";                                          // clear word
            }                                                        
            else if ((word.size() > 0) && (isStopWord(word))) {     // if word is a stopWord
                word = "";                                          // clear word
            }
            if ((text[i] == '?' || text[i] == '.' || text[i] == '!') && sentenceList.size() > 0) { 
                sentenceLists.push_back(sentenceList);              // push back sentenceList into sentenceLists and clear sentenceList
                sentenceList.clear();
            }
        }
    }
    if ((word.size() > 0) && (!isStopWord(word))) sentenceList.push_back(word);
    if (sentenceList.size() > 0) sentenceLists.push_back(sentenceList);

    return sentenceLists;
}

bool isStopWord(string s)
{
    return stopWords.find(s) != stopWords.end();
}

void SemanticDictionary::processTargetWord(string targetWord, vector<string> sentenceList)
{
    auto it = semanticDescriptors.find(targetWord);                     // search for targetWord using find

    if (it == semanticDescriptors.end()) {                              // if targetWord is not found
        SemanticDescriptor desc(targetWord);
        it = (semanticDescriptors.insert({targetWord, desc})).first;    // insert a new pair into semanticDescriptors map
        
    }
    for (string word : sentenceList) 
    {                                                               // loop through each word in sentence
        if (word != targetWord)                                     // if word does not equal targetword
            it->second.processContextWord(word);                    // process word
    }
}

double SemanticDictionary::getSimilarity(string targetWord1, string targetWord2)
{
    auto it1 = semanticDescriptors.find(targetWord1);               // search for targetWords in semanticDescriptor
    auto it2 = semanticDescriptors.find(targetWord2);

    if (it1 == semanticDescriptors.end() || it2 == semanticDescriptors.end())
        throw runtime_error("Target word(s) unknown");
    
    SemanticDescriptor a = it1->second; 
    SemanticDescriptor b = it2->second;                  
    double result = (a * b) / ((sqrt(a*a)) * (sqrt(b*b)));            // use cosine similarity formula to assign result                                       
             
    return result;                                               
}

string SemanticDictionary::mostSimilarWord(string word, vector<string> choices)
{   
    double max = 0.0;                                               // create var to store max num of getSimilarity
    string mostSimilar;                                             // create var to store choice that is most similar

    for (size_t i = 0; i < choices.size(); i++) {                   // iterate through each choice
        if (getSimilarity(word,choices[i]) >= max)                  // if getSimilarity of choice[i] is greater than current max
            max = getSimilarity(word,choices[i]);                   // max gets assigned to new max
    }

    for (size_t i = 0; i < choices.size(); i++) {                   // iterate through each choice
        if (getSimilarity(word,choices[i]) == max)                  // if getSimilarity of choice[i] equals max
            mostSimilar = choices[i];                               // assign mostSimilar to choice[i]
    }
    return mostSimilar;                                             // return choice that is most similar
}

ostream& operator<<(ostream& os, const SemanticDictionary& dict)
{
    os << "Semantic Descriptors:\n";
    os << "{" << endl;                                              
    for (auto it = dict.semanticDescriptors.begin(); it != dict.semanticDescriptors.end(); ++it)    // iterate through semanticDescriptors and output each one
    {
        os << it->second << endl;
    }
    os << "}" << endl;

    return os;
}


ostream& operator<<(ostream& os, const vector<vector<string>>& sentenceLists)
{   
    os << "Sentence Lists:\n";
    os << "[" << endl;
    for (vector<string> sentence : sentenceLists) {             // iterate through each sentence in sentenceLists
        os << "[ ";                                             
        for (string word : sentence) {                          // iterate through each word in each sentence
            os << word << " ";                                  // output each word in sentence
        }                                                       // seperate sentences by a new line
        os << "]" << endl;
    }
    os << "]" << endl;

    return os;
}

istream& operator>>(istream& is, SemanticDictionary& d)
{
    d.semanticDescriptors.clear();
    SemanticDescriptor desc("");
    char ch;
    is >> ch;
    while (ch != '{')
    {
        is >> ch;
    }

    if (ch == '{')
    {
        while (is >> desc)
            d.semanticDescriptors.insert({desc.getTargetWord(), desc});
        is >> ch;
        if (ch != '}')
        {
            is.unget();
            is.clear(ios_base::failbit);
        }
    }

    return is;
}