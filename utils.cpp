#include "utils.h"
#include "picosha2.h"
#include <fstream>
#include <ctime>
using namespace std;

std::string hashPassword(const std::string& password) {
    return picosha2::hash256_hex_string(password);
}

bool checkPassword(const std::string& password, const std::string& hash) {
    return hashPassword(password) == hash;
}

void backupFile(const std::string& filename) {
    std::ifstream src(filename, std::ios::binary);
    std::ofstream dst(filename + ".bak", std::ios::binary);
    dst << src.rdbuf();
}

void restoreFile(const std::string& filename) {
    std::ifstream src(filename + ".bak", std::ios::binary);
    std::ofstream dst(filename, std::ios::binary);
    dst << src.rdbuf();
}

std::string randomString(int length) {
    std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string res = "";
    srand((unsigned)time(0) + rand());
    for (int i = 0; i < length; ++i)
        res += chars[rand() % chars.size()];
    return res;
}
