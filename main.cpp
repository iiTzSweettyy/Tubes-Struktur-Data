#include <iostream>
#include <vector>
#include <string>
#include "tournament.h"

using namespace std;

void printTeamList(const vector<string>& t) {
    cout << "\n   [ DAFTAR TIM (" << t.size() << " Tim) ]" << endl;
    if (t.empty()) cout << "   (Kosong)" << endl;
    else for (size_t i=0; i<t.size(); i++) cout << "   " << i+1 << ". " << t[i] << endl;
    
    cout << "   STATUS: ";
    if (t.size() >= 2) cout << "[SIAP] Bracket bisa dibuat" << endl;
    else cout << "[BELUM SIAP] Minimal 2 tim." << endl;
    cout << "   ----------------------" << endl;
}

int main() {
    Tournament tree;
    AdminAuth admin;
    vector<string> teams; 
    bool isAdmin = false;
    int pil;

    do {
        system("cls");
        cout << "\n==============================================" << endl;
        cout << "   SISTEM TURNAMEN PRO (AUTO BYE SYSTEM)      " << endl;
        cout << "==============================================" << endl;
        cout << "Role: " << (isAdmin ? "[ADMINISTRATOR]" : "[GUEST]") << endl;
        
        cout << "1. Login Admin" << endl;
        cout << "2. Lihat Bagan (Visual)" << endl;
        cout << "3. Cari Tim" << endl;
        cout << "4. Lihat Traversal" << endl;
        
        if (isAdmin) {
            cout << "\n--- ADMIN MENU ---" << endl;
            cout << "5. Kelola Tim (Sekarang: " << teams.size() << " Tim)" << endl;
            cout << "6. GENERATE BRACKET" << endl;
            cout << "7. Update Pemenang" << endl;
            cout << "8. Logout" << endl;
        }
        
        cout << "0. Keluar" << endl;
        cout << ">> Pilih: "; cin >> pil;

        switch(pil) {
            case 1: 
                if (!isAdmin) {
                    if(admin.login()) isAdmin=true;
                    else { cout<<"\nGagal!"; system("pause"); }
                }
                break;
            
            case 2: tree.showBracket(); system("pause"); break;
            case 3: { string c; cout<<"Nama: "; cin.ignore(); getline(cin, c); tree.findTeamStatus(c); system("pause"); } break;
            case 4: tree.printAllTraversals(); system("pause"); break;
            
            case 5: if(isAdmin) {
                int sm;
                do {
                    system("cls");
                    cout << "--- KELOLA TIM ---" << endl;
                    printTeamList(teams);
                    cout << "1. Add  2. Edit  3. Delete  0. Back\n>> "; cin >> sm;
                    if (!(cin >> sm)) {
                        cin.clear();
                        cin.ignore(1000, '\n');
                        sm = -1; 
                        continue; 
                    }

                    if(sm == 1) { 
                        string n; 
                        cout << "Nama: "; 
                        
                        cin.ignore();
                        getline(cin, n);

                        if (n.empty()) {
                            cout << ">> [ERROR] Nama tim tidak boleh kosong" << endl;
                            system("pause");
                            continue;
                        }
                        
                        teams.push_back(n);
                    }
                    
                    else if(sm == 2 && !teams.empty()) { 
                        int i; 
                        cout << "No: "; 
                        if(cin >> i) {
                            if(i>0 && i<=teams.size()){ 
                                string nBaru;
                                cout << "Nama Baru: "; 
                                cin.ignore();
                                getline(cin, nBaru);
                                
                                if(nBaru.empty()) {
                                    cout << ">> [ERROR] Nama tim tidak boleh kosong" << endl;
                                    system("pause");
                                    continue;
                                }

                                teams[i-1] = nBaru;
                            } else {
                                cout << ">> No tim tidak valid." << endl; system("pause");
                            }
                        } else {
                            cin.clear(); cin.ignore(1000, '\n');
                        }
                    }
                    
                    else if(sm == 3 && !teams.empty()) { 
                        int i; 
                        cout << "No: "; 
                        if(cin >> i) {
                            if(i>0 && i<=teams.size()) teams.erase(teams.begin()+i-1); 
                        } else {
                            cin.clear(); cin.ignore(1000, '\n');
                        }
                    }

                } while(sm != 0);
            } break;

            case 6: if(isAdmin) {
                tree.bikinBracket(teams);
                system("pause");
            } break;

            case 7: if(isAdmin) {
                if(!tree.cekBracket()) { cout<<"Bracket belum ada."; system("pause"); }
                else {
                    tree.showActiveMatches();
                    cout << "\nMasukkan ID Match yang mau diupdate (0 Batal): ";
                    int id; cin >> id;
                    if(id > 0) {
                        string w; cout << "Siapa Pemenangnya? ";
                        cin.ignore();
                        getline(cin, w);
                        tree.updateMatchWinner(id, w);
                        system("pause");
                    }
                }
            } break;

            case 8: isAdmin=false; break;
        }
    } while (pil != 0);

    return 0;
}