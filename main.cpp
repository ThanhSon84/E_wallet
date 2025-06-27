#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include "user.h"
#include "wallet.h"
#include "transaction.h"
#include "utils.h"
#include "ipc.h"
#include "otp.h"

using namespace std;

vector<User> users;
vector<Wallet> wallets;

void napDiemTuViTong() {
    Wallet* total = Wallet::findWalletByUsername(wallets, "TOTAL");
    if (!total) { cout << "Khong tim thay vi tong!\n"; return; }
    string uname;
    cout << "Nhap ten dang nhap nguoi nhan: "; getline(cin, uname);
    Wallet* member = Wallet::findWalletByUsername(wallets, uname);
    if (!member) { cout << "Khong tim thay vi nhan!\n"; return; }
    int amount;
    cout << "Nhap so diem muon nap: "; cin >> amount; cin.ignore();
    if (amount <= 0) { cout << "Chi nhan so nguyen duong!\n"; return; }
    if (total->balance < amount) { cout << "Vi tong khong du diem!\n"; return; }
    total->balance -= amount;
    member->balance += amount;
    Transaction t{total->wallet_id, member->wallet_id, amount, "success", to_string(time(0))};
    Transaction::logTransaction(t);
    Wallet::saveWallets(wallets);
    cout << "Nap diem thanh cong!\n";
}

void transferPoints(User& user) {
    Wallet* from = Wallet::findWalletByUsername(wallets, user.username);
    if (!from) { cout << "Khong tim thay vi!\n"; return; }
    string to_wallet_id;
    cout << "Nhap ID vi nguoi nhan: "; getline(cin, to_wallet_id);
    Wallet* to = Wallet::findWalletById(wallets, to_wallet_id);
    if (!to) { cout << "Khong tim thay vi nhan!\n"; return; }
    int amount;
    cout << "Nhap so diem can chuyen: "; cin >> amount; cin.ignore();
    if (amount <= 0) {
        cout << "So diem giao dich phai la so nguyen duong!\n"; return;
    }
    if (from->balance < amount) {
        cout << "So du khong du!\n"; return;
    }

    string otp = generateOTP();
    cout << "Dang gui ma OTP sang cua so xac nhan (chay otp_confirm.exe)...\n";
    string response;
    if (!sendOTPAndWaitForResponse(otp, response)) {
        cout << "Khong the giao tiep voi tien trinh xac thuc OTP!\n";
        return;
    }
    if (response == "OK") {
        from->balance -= amount;
        to->balance += amount;
        Transaction t{from->wallet_id, to->wallet_id, amount, "success", to_string(time(0))};
        Transaction::logTransaction(t);
        cout << "Chuyen diem thanh cong!\n";
    } else {
        Transaction t{from->wallet_id, to->wallet_id, amount, "failed", to_string(time(0))};
        Transaction::logTransaction(t);
        cout << "Giao dich bi tu choi hoac OTP sai!\n";
    }
    Wallet::saveWallets(wallets);
}

void menuUser(User& user) {
    Wallet* w = Wallet::findWalletByUsername(wallets, user.username);
    cout << "Thong tin vi cua ban: Wallet ID: " << (w ? w->wallet_id : "N/A") << endl;
    while (true) {
        cout << "\n===== MENU NGUOI DUNG =====\n";
        cout << "1. Xem so du\n2. Chuyen diem\n3. Xem lich su giao dich\n4. Doi mat khau\n5. Thoat\nChon: ";
        int ch; cin >> ch; cin.ignore();
        if (ch == 1) {
            Wallet* w = Wallet::findWalletByUsername(wallets, user.username);
            cout << "So du: " << (w ? w->balance : 0) << endl;
        } else if (ch == 2) {
            transferPoints(user);
        } else if (ch == 3) {
            Wallet* w = Wallet::findWalletByUsername(wallets, user.username);
            if (w) Transaction::showHistory(w->wallet_id);
        } else if (ch == 4) {
            user.changePassword(users);
            User::saveUsers(users);
        } else break;
    }
}

int main() {
    User::loadUsers(users);
    Wallet::loadWallets(wallets);
    User::ensureAdminExists(users);
    Wallet::ensureTotalWallet(wallets);

    User* currentUser = nullptr; // Luu user đang đang nhap

    while (true) {
        cout << "\n===== HE THONG QUAN LY VI DIEM =====\n";
        cout << "1. Dang ky\n2. Dang nhap\n";
        if (currentUser && currentUser->role == Role::ADMIN) {
            cout << "3. Nap diem tu vi tong (admin)\n";
        }
        cout << "4. Phuc hoi du lieu\n5. Thoat\nChon: ";
        int ch; cin >> ch; cin.ignore();

        if (ch == 1) {
            if (User::registerUser(users)) {
                User::saveUsers(users);
                Wallet* w = new Wallet("WALLET_" + users.back().username, users.back().username, 0);
                wallets.push_back(*w);
                Wallet::saveWallets(wallets);
                cout << "Wallet ID cua ban la: " << w->wallet_id << endl;
            }
        } else if (ch == 2) {
            currentUser = User::login(users);
            if (currentUser) menuUser(*currentUser);
        } else if (ch == 3) {
            if (currentUser && currentUser->role == Role::ADMIN) {
                napDiemTuViTong();
            } else {
                cout << "Ban phai dang nhap voi tai khoan admin moi su dung duoc chuc nang nay!\n";
            }
        } else if (ch == 4) {
            restoreFile("data/users.txt");
            restoreFile("data/wallets.txt");
            restoreFile("data/transactions.txt");
            cout << "Phuc hoi du lieu xong!\n";
            User::loadUsers(users);
            Wallet::loadWallets(wallets);
        } else break;
    }
    User::saveUsers(users);
    Wallet::saveWallets(wallets);
    return 0;
}
