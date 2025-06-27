#include "transaction.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <string>
using namespace std;

// Ham chuyen timestamp sang hh:mm dd/mm/yyyy
std::string formatDate(const std::string& timestamp_str) {
    time_t t = std::stoll(timestamp_str);
    struct tm* tm_info = localtime(&t);
    char buffer[25];
    strftime(buffer, sizeof(buffer), "%H:%M %d/%m/%Y", tm_info);
    return std::string(buffer);
}

void Transaction::logTransaction(const Transaction& t) {
    ofstream fout("data/transactions.txt", ios::app);
    fout << t.from_wallet << "|" << t.to_wallet << "|" << t.amount << "|" << t.status << "|" << t.timestamp << "\n";
    fout.close();
    backupFile("data/transactions.txt");
}

void Transaction::showHistory(const string& wallet_id) {
    ifstream fin("data/transactions.txt");
    string line;
    cout << "Lich su giao dich cua vi " << wallet_id << ":\n";
    while (getline(fin, line)) {
        stringstream ss(line);
        string from, to, amount_str, status, time;
        getline(ss, from, '|');
        getline(ss, to, '|');
        getline(ss, amount_str, '|');
        getline(ss, status, '|');
        getline(ss, time, '|');
        int amount = stoi(amount_str);
        if (from == wallet_id || to == wallet_id) {
            cout << "Tu: " << from
                 << ", Den: " << to
                 << ", So diem: " << amount
                 << ", Trang thai: " << status
                 << ", Thoi gian: " << formatDate(time) << endl;
        }
    }
    fin.close();
}
