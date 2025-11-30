#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <string>
#include <vector>

using namespace std;

// STRUKTUR NODE
struct MatchNode {
    string teamName;      
    string matchLabel;    
    int matchID;          
    MatchNode* left;
    MatchNode* right;

    MatchNode(string name, string label, int id);
};

// CLASS TOURNAMENT
class Tournament {
private:
    MatchNode* root;
    int jumlahMatch; 


    MatchNode* buildTree(vector<string>& currentTeams);
    void printRecursive(MatchNode* node, int space);
    void destroyTree(MatchNode* node);
    void byesAutoWin(MatchNode* node); 
    void nungguMatch(MatchNode* node, vector<MatchNode*>& list);
    bool searching(MatchNode* node, string targetName);
    
    // Traversals
    void preOrder(MatchNode* node);
    void inOrder(MatchNode* node);
    void postOrder(MatchNode* node);


    // Function Bracket
    void bikinBracket(vector<string> teams); 
    void showBracket();                            
    
    // Function Admin
    void showActiveMatches();                      
    void updateMatchWinner(int id, string winner); 
    
    
    void findTeamStatus(string teamName);
    void printAllTraversals();
    bool cekBracket();
};

// CLASS ADMIN AUTH
class AdminAuth {
private:
    string username;
    string password;
public:
    AdminAuth();
    bool login();
};

#endif