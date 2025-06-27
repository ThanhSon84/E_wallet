#include <string>
struct Transaction {
    std::string from_wallet, to_wallet;
    int amount;
    std::string status, timestamp;
    static void logTransaction(const Transaction&);
    static void showHistory(const std::string& wallet_id);
};
