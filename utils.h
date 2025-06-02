//#pragma once
#include <string>
std::string hashPassword(const std::string& password);
bool checkPassword(const std::string& password, const std::string& hash);
void backupFile(const std::string& filename);
void restoreFile(const std::string& filename);
std::string randomString(int length);
