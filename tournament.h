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
    int matchCounter; 


    MatchNode* buildTree(vector<string>& currentTeams);
    void printRecursive(MatchNode* node, int space);
    void destroyTree(MatchNode* node);
    void processAutoWinByes(MatchNode* node); 
    void getOpenMatches(MatchNode* node, vector<MatchNode*>& list);
    bool searchRecursive(MatchNode* node, string targetName);
    
    // Traversals
    void preOrderRec(MatchNode* node);
    void inOrderRec(MatchNode* node);
    void postOrderRec(MatchNode* node);

public:
    Tournament();
    ~Tournament();

    // Function Bracket
    void initBracket(vector<string> teams); 
    void showBracket();                            
    
    // Function Admin
    void showActiveMatches();                      
    void updateMatchWinner(int id, string winner); 
    
    
    void findTeamStatus(string teamName);
    void printAllTraversals();
    bool isReady();
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