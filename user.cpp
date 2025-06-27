#include "user.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

User::User() {}
User::User(const string& uname, 
            const string& hash, 
            const string& fname, 
            const string& phone, 
            Role r, 
            bool auto_pw)
    : username(uname), password_hash(hash), full_name(fname), phone(phone), role(r), auto_generated_pw(auto_pw) {}

void User::loadUsers(vector<User>& users) {
    users.clear();
    ifstream fin("data/users.txt");
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string uname, hash, fname, phone, role_str, auto_pw_str;
        getline(ss, uname, '|');
        getline(ss, hash, '|');
        getline(ss, fname, '|');
        getline(ss, phone, '|');
        getline(ss, role_str, '|');
        getline(ss, auto_pw_str, '|');
        Role r = (role_str == "ADMIN") ? Role::ADMIN : Role::USER;
        bool auto_pw = (auto_pw_str == "1");
        users.emplace_back(uname, hash, fname, phone, r, auto_pw);
    }
    fin.close();
}

void User::saveUsers(const vector<User>& users) {
    ofstream fout("data/users.txt");
    for (const auto& u : users) {
        fout << u.username << "|" << u.password_hash << "|" << u.full_name << "|"
             << u.phone << "|"
             << (u.role == Role::ADMIN ? "ADMIN" : "USER") << "|"
             << (u.auto_generated_pw ? "1" : "0") << "\n";
    }
    fout.close();
    backupFile("data/users.txt");
}

bool User::phoneExists(const vector<User>& users, const string& phone) {
    for (const auto& u : users)
        if (u.phone == phone) return true;
    return false;
}

bool User::passwordHashExists(const std::vector<User>& users, const std::string& hash) {
    for (const auto& u : users)
        if (u.password_hash == hash) return true;
    return false;
}

std::string User::uniqueUsername(const std::vector<User>& users, const std::string& base) {
    std::string uname = base;
    int suffix = 1;
    while (findUser(const_cast<std::vector<User>&>(users), uname)) {
        uname = base + std::to_string(suffix++);
    }
    return uname;
}

bool User::registerUser(vector<User>& users) {
    string uname, pass, fname, phone, role_str;
    cout << "Nhap ten dang nhap: "; getline(cin, uname);
    cout << "Nhap so dien thoai: "; getline(cin, phone);
    if (phoneExists(users, phone)) {
        cout << "So dien thoai da ton tai! Moi so chi duoc dang ky 1 vi.\n"; 
        return false;
    }
    uname = uniqueUsername(users, uname);
    cout << "Nhap ho ten: "; getline(cin, fname);

    bool auto_pw = false;
    string hash;
    do {
        cout << "Nhap mat khau (bo trong de tu sinh): ";
        getline(cin, pass);
        if (pass.empty()) {
            pass = randomString(8);
            cout << "Mat khau tu sinh: " << pass << endl;
            auto_pw = true;
        } else {
            auto_pw = false;
        }
        hash = hashPassword(pass);
        if (passwordHashExists(users, hash)) {
            cout << "Mat khau nay da duoc su dung. Vui long nhap lai.\n";
            pass.clear();
        } else {
            break;
        }
    } while (true);

    cout << "Phan quyen (USER/ADMIN): "; getline(cin, role_str);
    Role r = (role_str == "ADMIN") ? Role::ADMIN : Role::USER;
    
    // Them user truoc khi luu
    users.emplace_back(uname, hash, fname, phone, r, auto_pw);
    // Lưu file SAU khi them user
    User::saveUsers(users);
    
    cout << "Dang ky thanh cong! Username cua ban la: " << uname << endl;
    return true;
}


User* User::login(vector<User>& users) {
    string uname, pass;
    cout << "Ten dang nhap: "; getline(cin, uname);
    cout << "Mat khau: "; getline(cin, pass);
    for (auto& u : users) {
        if (u.username == uname && checkPassword(pass, u.password_hash)) {
            if (u.auto_generated_pw) {
                cout << "Ban can doi mat khau tu sinh.\n";
                u.changePassword(users);
                u.auto_generated_pw = false;
                saveUsers(users);
            }
            cout << "Dang nhap thanh cong!\n";
            cout << "Thong tin vi cua ban: Username: " << u.username << ", So dien thoai: " << u.phone << endl;
            return &u;
        }
    }
    cout << "Dang nhap that bai!\n";
    return nullptr;
}

bool User::changePassword(std::vector<User>& users) {
    std::string pass1, pass2;
    do {
        std::cout << "Nhap mat khau moi: ";
        getline(std::cin, pass1);
        std::cout << "Nhap lai: ";
        getline(std::cin, pass2);
        if (pass1 != pass2 || pass1.empty()) {
            std::cout << "Mat khau khong hop le!\n";
            continue;
        }
        std::string hash = hashPassword(pass1);
        if (passwordHashExists(users, hash)) {
            std::cout << "Mat khau nay da duoc su dung. Vui long chon mat khau khac.\n";
            continue;
        }
        password_hash = hash;
        std::cout << "Doi mat khau thanh cong!\n";
        return true;
    } while (true);
}

User* User::findUser(vector<User>& users, const string& uname) {
    for (auto& u : users) 
        if (u.username == uname) return &u;
    return nullptr;
}

void User::ensureAdminExists(vector<User>& users) {
    bool found = false;
    for (const auto& u : users)
        if (u.role == Role::ADMIN) found = true;
    if (!found) {
        users.emplace_back("admin", hashPassword("admin123"), "Admin", "0000000000", Role::ADMIN, false);
        cout << "Da tao tai khoan admin mac dinh (admin/admin123)\n";
    }
}
