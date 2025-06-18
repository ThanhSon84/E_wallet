#include "otp.h"
#include <ctime>
#include <cstdlib>
#include <sstream>

std::string generateOTP(int length) {
    std::string chars = "0123456789";
    std::string otp;
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < length; ++i) {
        otp += chars[rand() % chars.size()];
    }
    return otp;
}
