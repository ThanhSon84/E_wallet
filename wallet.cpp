#include "wallet.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <climits>
using namespace std;

Wallet::Wallet() {}
Wallet::Wallet(const string& id, const string& uname, int bal) : wallet_id(id), username(uname), balance(bal) {}

void Wallet::loadWallets(vector<Wallet>& wallets) {
    wallets.clear();
    ifstream fin("data/wallets.txt");
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string id, uname, bal_str;
        getline(ss, id, '|');
        getline(ss, uname, '|');
        getline(ss, bal_str, '|');
        wallets.emplace_back(id, uname, stoi(bal_str));
    }
    fin.close();
}

void Wallet::saveWallets(const vector<Wallet>& wallets) {
    ofstream fout("data/wallets.txt");
    for (const auto& w : wallets) {
        fout << w.wallet_id << "|" << w.username << "|" << w.balance << "\n";
    }
    fout.close();
    backupFile("data/wallets.txt");
}

Wallet* Wallet::findWalletByUsername(vector<Wallet>& wallets, const string& uname) {
    for (auto& w : wallets) if (w.username == uname) return &w;
    return nullptr;
}
Wallet* Wallet::findWalletById(vector<Wallet>& wallets, const string& id) {
    for (auto& w : wallets) if (w.wallet_id == id) return &w;
    return nullptr;
}
void Wallet::ensureTotalWallet(vector<Wallet>& wallets) {
    bool found = false;
    for (const auto& w : wallets)
        if (w.username == "TOTAL") found = true;
    if (!found) {
        wallets.push_back(Wallet("WALLET_TOTAL", "TOTAL", 1000000));
        cout << "Da tao vi tong mac dinh 1.000.000 diem\n";
    }
}
