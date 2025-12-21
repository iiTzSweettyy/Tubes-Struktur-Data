#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include "tournament.h"

using namespace std;

int getValidNumber(string prompt) {
    int number;
    while (true) {
        cout << prompt;
        cin >> number;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "   [ERROR] Harap masukkan angka!" << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        }
    }
}

string getValidString(string prompt) {
    string input;
    cout << prompt;
    getline(cin, input); 
    return input;
}

void printTeamList(const vector<string>& t) {
    cout << "\n   [ DAFTAR TIM (" << t.size() << " Tim) ]" << endl;
    if (t.empty()) cout << "   (Kosong)" << endl;
    else for (size_t i=0; i<t.size(); i++) cout << "   " << i+1 << ". " << t[i] << endl;
    
    cout << "   STATUS: ";
    if (t.size() >= 2) cout << "[SIAP] Auto-Bye System Aktif." << endl;
    else cout << "[BELUM SIAP] Minimal 2 tim." << endl;
    cout << "   ----------------------" << endl;
}

int main() {
    Tournament tree;
    AdminAuth admin;
    vector<string> teams; 
    bool isAdmin = false;
    int pil;

    //LOAD DATA OTOMATIS SAAT MULAI
    tree.loadFromCSV(teams);

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
            cout << "5. Kelola Tim" << endl;
            cout << "6. GENERATE BRACKET" << endl;
            cout << "7. Update Pemenang" << endl;
            cout << "8. Logout" << endl;
        }
        
        cout << "0. Keluar" << endl;
        
        pil = getValidNumber(">> Pilih: ");

        switch(pil) {
            case 1: 
                if (!isAdmin) {
                    if(admin.login()) isAdmin=true;
                    else { cout<<"\nGagal!"; system("pause"); }
                }
                break;
            
            case 2: tree.showBracket(); system("pause"); break;
            
            case 3: { 
                string c = getValidString("Nama Tim yang dicari: ");
                tree.findTeamStatus(c); 
                system("pause"); 
            } break;
            
            case 4: tree.printAllTraversals(); system("pause"); break;
            
            case 5: if(isAdmin) {
                int sm;
                bool dataBerubah = false; 
                do {
                    system("cls");
                    cout << "--- KELOLA TIM ---" << endl;
                    printTeamList(teams);
                    cout << "1. Add  2. Edit  3. Delete  0. Back" << endl;
                    
                    sm = getValidNumber(">> Pilih Menu: ");

                    if(sm == 1) { 
                        string n = getValidString("Nama Tim Baru: ");
                        if (!n.empty()) { teams.push_back(n); dataBerubah = true; }
                        else cout << ">> Nama tidak boleh kosong!" << endl;
                    }
                    else if(sm == 2 && !teams.empty()) { 
                        int i = getValidNumber("Nomor Tim yg diedit: ");
                        if(i>0 && i<=teams.size()) {
                            string nBaru = getValidString("Nama Baru: ");
                            if(!nBaru.empty()) { teams[i-1] = nBaru; dataBerubah = true; }
                        } else {
                            cout << ">> Nomor tidak valid!" << endl; system("pause");
                        }
                    }
                    else if(sm == 3 && !teams.empty()) { 
                        int i = getValidNumber("Nomor Tim yg dihapus: ");
                        if(i>0 && i<=teams.size()) { teams.erase(teams.begin()+i-1); dataBerubah = true; }
                        else {
                            cout << ">> Nomor tidak valid!" << endl; system("pause");
                        }
                    }
                } while(sm != 0);

                if(dataBerubah) {
                    tree.saveToCSV(teams); 
                }
            } break;

            case 6: if(isAdmin) {
                tree.bikinBracket(teams);
                tree.saveToCSV(teams); 
                system("pause");
            } break;

            case 7: if(isAdmin) {
                if(!tree.cekBracket()) { cout<<"Bracket belum ada."; system("pause"); }
                else {
                    tree.showActiveMatches();
                    cout << endl;
                    int id = getValidNumber("Masukkan ID Match (0 Batal): ");
                    
                    if(id > 0) {
                        string w = getValidString("Siapa Pemenangnya? ");
                        tree.updateMatchWinner(id, w);
                        tree.saveToCSV(teams); 
                        
                        system("pause");
                    }
                }
            } break;

            case 8: isAdmin=false; break;
        }
    } while (pil != 0);

    return 0;
}