#include "ipc.h"
#include <iostream>
int main() {
    std::cout << "=== CUA SO XAC THUC OTP ===\n";
    while (true) {
        receiveOTPAndSendResponse();
        std::cout << "Xac nhan xong mot giao dich. Nhan Enter de cho giao dich tiep theo...\n";
        std::cin.get();
    }
    return 0;
}
