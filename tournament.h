#ifndef TOURNAMENT_H
#define TOURNAMENT_H

#include <string>
#include <vector>

using namespace std;

struct MatchNode {
    string teamName;      
    string matchLabel;    
    int matchID;
    MatchNode* left;
    MatchNode* right;

    MatchNode(string name, string label, int id);
};

class Tournament {
private:
    MatchNode* root;
    int matchCounter; 

    MatchNode* buildTree(vector<string>& currentTeams);
    void printTree(MatchNode* node, int space);    
    void printRecursive(MatchNode* node, int space);
    void destroyTree(MatchNode* node);
    void byesAutoWin(MatchNode* node); 
    void nungguMatch(MatchNode* node, vector<MatchNode*>& list);
    
    void getFinishedMatches(MatchNode* node, vector<string>& dataList);
    // Di bagian private:
    MatchNode* searching(MatchNode* node, string targetName);

    void preOrder(MatchNode* node);
    void inOrder(MatchNode* node);
    void postOrder(MatchNode* node);

public:
    Tournament();
    ~Tournament();

    void bikinBracket(vector<string> teams); 
    void showBracket();                            
    
    // Fitur Admin  
    void showActiveMatches();                      
    void updateMatchWinner(int id, string winner); 
    
    // Fitur Umum
    void findTeamStatus(string teamName);
    void printAllTraversals();
    bool cekBracket();

    // Fitur Database
    void saveToCSV(vector<string>& teams);
    void loadFromCSV(vector<string>& teams);
};

class AdminAuth {
private:
    string username;
    string password;
public:
    AdminAuth();
    bool login();
};

#endif