#include "tournament.h"
#include <iostream>
#include <conio.h>

using namespace std;

// --- NODE ---
MatchNode::MatchNode(string name, string label, int id) {
    teamName = name;
    matchLabel = label;
    matchID = id;
    left = NULL;
    right = NULL;
}

// --- TOURNAMENT ---
Tournament::Tournament() {
    root = NULL;
    matchCounter = 0;
}

Tournament::~Tournament() {
    destroyTree(root);
}

void Tournament::destroyTree(MatchNode* node) {
    if (node == NULL) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

// FUNGSI Menangani Tim vs BYE
void Tournament::byesAutoWin(MatchNode* node) {
    if (node == NULL) return;

    // 1. Proses child dulu (Bottom-Up)
    byesAutoWin(node->left);
    byesAutoWin(node->right);

    // 2. Cek Logika BYE
    if (node->matchID > 0 && node->teamName == "TBD") {
        string leftName = (node->left) ? node->left->teamName : "";
        string rightName = (node->right) ? node->right->teamName : "";

        // Skenario 1: Kiri vs BYE -> Kiri Menang Otomatis
        if (rightName == "BYE" && leftName != "BYE" && leftName != "TBD") {
            node->teamName = leftName;
        }
        // Skenario 2: BYE vs Kanan -> Kanan Menang Otomatis
        else if (leftName == "BYE" && rightName != "BYE" && rightName != "TBD") {
            node->teamName = rightName;
        }
        // Skenario 3: BYE vs BYE -> BYE naik
        else if (leftName == "BYE" && rightName == "BYE") {
            node->teamName = "BYE";
        }
    }
}

//membuat tree nya make algoritma rekursif
MatchNode* Tournament::buildTree(vector<string>& currentTeams) {
    if (currentTeams.size() == 1) {
        return new MatchNode(currentTeams[0], "Slot Tim", 0);
    }

    //membagi tim menjadi dua bagian
    int mid = currentTeams.size() / 2;
    vector<string> leftSide(currentTeams.begin(), currentTeams.begin() + mid);
    vector<string> rightSide(currentTeams.begin() + mid, currentTeams.end());

    //rekursif untuk membangun tree kiri dan kanan
    MatchNode* leftChild = buildTree(leftSide);
    MatchNode* rightChild = buildTree(rightSide);

    //node parent untuk pertandingan
    matchCounter++;
    string label = "Match #" + to_string(matchCounter);
    
    //buat node parent dan menghubungkan dengan childnya
    MatchNode* parent = new MatchNode("TBD", label, matchCounter);
    parent->left = leftChild;
    parent->right = rightChild;

    return parent;
}

void Tournament::bikinBracket(vector<string> teams) {
    if (teams.size() < 2) {
        cout << ">> [ERROR] Minimal 2 tim untuk membuat turnamen!" << endl;
        return;
    }

    if (root != NULL) { destroyTree(root); root = NULL; }
    matchCounter = 0; 

    // --- LOGIKA UNTUK BYE ---
    // 1. Hitung target ukuran (Pangkat 2 terdekat)
    int targetSize = 1;
    while (targetSize < teams.size()) {
        targetSize *= 2;
    }

    int byeCount = targetSize - teams.size();
    if (byeCount > 0) {
        cout << ">> Info: Menambahkan " << byeCount << " slot 'BYE' agar bagan seimbang." << endl;
        // Tambahkan "BYE" ke dalam vector
        for (int i = 0; i < byeCount; i++) {
            teams.push_back("BYE");
        }
    }

    cout << ">> Membuat bracket..." << endl;
    root = buildTree(teams);
    
    if(root) root->matchLabel = "GRAND FINAL";

    // JALANKAN SISTEM AUTO-WIN UNTUK BYE
    byesAutoWin(root);

    cout << ">> Bracket Berhasil Dibuat!" << endl;
}

// --- UPDATE & ADMIN ---
void Tournament::nungguMatch(MatchNode* node, vector<MatchNode*>& list) {
    if (node == NULL) return;
    nungguMatch(node->left, list);
    nungguMatch(node->right, list);

    // Hanya masukkan match yang masih TBD
    // Match yang sudah ada nama pemenangnya (karena Auto Win BYE) tidak perlu dimasukkan
    if (node->matchID > 0 && node->teamName == "TBD") {
        list.push_back(node);
    }
}

void Tournament::showActiveMatches() {
    if (!cekBracket()) return;
    vector<MatchNode*> activeMatches;
    nungguMatch(root, activeMatches);

    if (activeMatches.empty()) {
        cout << ">> Tidak ada pertandingan yang perlu diupdate (Turnamen Selesai/Belum Mulai)." << endl;
    } else {
        cout << "\n--- LIST PERTANDINGAN AKTIF (BELUM ADA PEMENANG) ---" << endl;
        for (MatchNode* m : activeMatches) {
            string teamA = (m->left) ? m->left->teamName : "?";
            string teamB = (m->right) ? m->right->teamName : "?";
            cout << "ID [" << m->matchID << "] " << m->matchLabel << " : " << teamA << " VS " << teamB << endl;
        }
    }
}

void Tournament::updateMatchWinner(int id, string winner) {
    vector<MatchNode*> allMatches;
    nungguMatch(root, allMatches);

    bool found = false;
    for (MatchNode* m : allMatches) {
        if (m->matchID == id) {
            found = true;
            
            // --- VALIDASI 1: CEK KESIAPAN MATCH ---
            // Ambil nama tim kiri dan kanan
            string teamA = (m->left) ? m->left->teamName : "ERROR";
            string teamB = (m->right) ? m->right->teamName : "ERROR";

            // Kalau salah satu tim masih "TBD"
            if (teamA == "TBD" || teamB == "TBD") {
                cout << ">> Pertandingan belum siap! Salah satu slot masih 'TBD'." << endl;
                cout << ">> Selesaikan dulu pertandingan di babak sebelumnya (ID Child)." << endl;
                return; 
            }

            // --- VALIDASI 2: CEK INPUTAN USER ---
            // Input harus sama dengan Team A atau Team B
            if (winner != teamA && winner != teamB) {
                cout << ">> Input Salah!" << endl;
                cout << ">> Peserta match ini hanya: " << teamA << " VS " << teamB << endl;
                cout << ">> Anda tidak bisa menginput '" << winner << "'." << endl;
                return;
            }

            // Kalau lolos validasi, simpan
            m->teamName = winner;
            cout << ">> [SUKSES] Pemenang " << m->matchLabel << " adalah " << winner << endl;
            
            //Auto Win BYE
            byesAutoWin(root); 
            break;
        }
    }
    
    if (!found) cout << ">> [ERROR] Match ID tidak ditemukan di daftar aktif." << endl;
}


// --- VISUALISASI ---
void Tournament::showBracket() {
    if (!cekBracket()) {
        cout << ">> Bracket belum dibuat." << endl; return;
    }
    cout << "\n================ BAGAN TURNAMEN =====================" << endl;
    printTree(root, 0);
    cout << "=============================================================" << endl;
}

// Update fungsi visualisasi agar lebih lebar
void Tournament::printTree(MatchNode* node, int space) {
    if (node == NULL) return;

    // --- PENGATURAN JARAK ---
    int jarak = 12; 

    space += jarak;

    printTree(node->right, space);
    cout << endl;
    
    for (int i = jarak; i < space; i++) cout << " ";
    
    if (node->matchID > 0) cout << node->teamName << " [" << node->matchLabel << "]\n";
    else cout << node->teamName << " [TEAM]\n"; 
    
    printTree(node->left, space);
}

// Traversal
bool Tournament::cekBracket() { return root != NULL; }

bool Tournament::searching(MatchNode* node, string targetName) {
    if (node == NULL) return false;
    if (node->teamName == targetName) return true;
    return searching(node->left, targetName) || searching(node->right, targetName);
}
void Tournament::findTeamStatus(string teamName) {
    if(!cekBracket()){ cout<<">> Kosong."<<endl; return; }
    if(searching(root, teamName)) cout<<">> [ADA] Tim "<<teamName<<" ditemukan."<<endl;
    else cout<<">> [TIDAK ADA] Tim "<<teamName<<" tidak ditemukan."<<endl;
}

void Tournament::preOrder(MatchNode* node) {
    if(!node) return;
    cout << node->teamName << " -> ";
    preOrder(node->left); preOrder(node->right);
}
void Tournament::inOrder(MatchNode* node) {
    if(!node) return;
    inOrder(node->left);
    cout << node->teamName << " -> ";
    inOrder(node->right);
}
void Tournament::postOrder(MatchNode* node) {
    if(!node) return;
    postOrder(node->left); postOrder(node->right);
    cout << node->teamName << " -> ";
}
void Tournament::printAllTraversals() {
    if(!cekBracket()) return;
    cout << "\nPre-Order: "; preOrder(root); cout << "END";
    cout << "\nIn-Order: "; inOrder(root); cout << "END";
    cout << "\nPost-Order: "; postOrder(root); cout << "END\n";
}

// --- ADMIN ---
AdminAuth::AdminAuth() { username="admin"; password="rahasia"; }
bool AdminAuth::login() {
    string u, p=""; char c;
    system("cls"); cout<<"=== LOGIN ADMIN ==="<<endl;
    cout<<"Username: "; cin>>u; cout<<"Password: ";
    while(true){
        c=_getch(); if(c==13)break;
        else if(c==8){ if(!p.empty()){p.pop_back(); cout<<"\b \b";} }
        else{ p+=c; cout<<"*"; }
    }
    return (u==username && p==password);
}
