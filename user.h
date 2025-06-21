//#pragma once
#include <string>
#include <vector>
enum class Role { USER, ADMIN };
class User {
public:
    std::string username, password_hash, full_name, phone;
    Role role;
    bool auto_generated_pw;
    User();
    User(const std::string&, const std::string&, const std::string&, const std::string&, Role, bool);
    static bool registerUser(std::vector<User>&);
    static User* login(std::vector<User>&);
    bool changePassword(std::vector<User>& users);
    static void loadUsers(std::vector<User>&);
    static void saveUsers(const std::vector<User>&);
    static User* findUser(std::vector<User>&, const std::string&);
    static bool phoneExists(const std::vector<User>&, const std::string&);
    static std::string uniqueUsername(const std::vector<User>&, const std::string&);
    static void ensureAdminExists(std::vector<User>&);
    static bool passwordHashExists(const std::vector<User>&, const std::string&);
};
