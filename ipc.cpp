#include "ipc.h"
#include <windows.h>
#include <string>
#include <iostream>

extern const std::string PIPE_NAME = R"(\\.\pipe\otp_pipe)";

bool sendOTPAndWaitForResponse(const std::string& otp, std::string& response) {
    HANDLE hPipe = CreateNamedPipeA(
        PIPE_NAME.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, 512, 512, 0, NULL
    );
    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Tao pipe that bai!\n";
        return false;
    }
    std::cout << "Dang doi cua so xac nhan ket noi...\n";
    ConnectNamedPipe(hPipe, NULL);

    DWORD dwWritten, dwRead;
    WriteFile(hPipe, otp.c_str(), otp.size() + 1, &dwWritten, NULL);

    char buffer[128] = {0};
    ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL);
    response = buffer;

    CloseHandle(hPipe);
    return true;
}

void receiveOTPAndSendResponse() {
    HANDLE hPipe;
    while (true) {
        hPipe = CreateFileA(
            PIPE_NAME.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING, 0, NULL
        );
        if (hPipe != INVALID_HANDLE_VALUE) break;
        Sleep(100);
    }
    char buffer[128] = {0};
    DWORD dwRead, dwWritten;
    ReadFile(hPipe, buffer, sizeof(buffer), &dwRead, NULL);
    std::string otp = buffer;
    std::cout << "Ma OTP nhan duoc: " << otp << std::endl;
    std::cout << "Nhap lai OTP de xac nhan giao dich: ";
    std::string input;
    std::getline(std::cin, input);
    std::string result = (input == otp) ? "OK" : "CANCEL";
    WriteFile(hPipe, result.c_str(), result.size() + 1, &dwWritten, NULL);
    CloseHandle(hPipe);
}
