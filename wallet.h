#include <string>
#include <vector>
class Wallet {
public:
    std::string wallet_id, username;
    int balance;
    Wallet();
    Wallet(const std::string&, const std::string&, int);
    static void loadWallets(std::vector<Wallet>&);
    static void saveWallets(const std::vector<Wallet>&);
    static Wallet* findWalletByUsername(std::vector<Wallet>&, const std::string&);
    static Wallet* findWalletById(std::vector<Wallet>&, const std::string&);
    static void ensureTotalWallet(std::vector<Wallet>&);
};
