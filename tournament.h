#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <string>

using namespace std;

// STRUKTUR NODE
struct MatchNode {
    string teamName;      
    string matchLabel;    
    MatchNode* left;      
    MatchNode* right;     

    MatchNode(string name, string label);
};

#endif