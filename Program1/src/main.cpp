
#include <mutex>
#include <string>
#include <queue>
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "myLib.hpp"

namespace {
    std::queue<std::string> buffer;
    std::mutex buf_mutex;
    bool cInFlag = false;
    bool flag = false;

    const char* server_ip = "127.0.0.1"; // для 2 программы
    const int server_port = 9090;
}

int connectToServer() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Ошибка создания сокета");
        return -1;
    }

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("Неправильный адрес");
        close(sock);
        return -1;
    }

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        close(sock);
        return -1;
    }

    return sock;
}

bool isDisconnected(int sock) {
    char buffer;
    int result = recv(sock, &buffer, 1, MSG_PEEK | MSG_DONTWAIT);
    if (result == 0) {
        return true;
    }
    if (result < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
        return true;
    }
    return false;
}

void threadOne() {
    while (true) {
        if (cInFlag) continue;
        std::string str;
        std::cout << "Введите строку из цифр (не более 64 символов):" << std::endl;
        std::cin >> str;

        if (str.size() > 64) {
            std::cout << "Вышли за предел символов. Попробуйте ещё раз." << std::endl;
            continue;
        }
        else if (str.find_first_not_of("0123456789") != std::string::npos) {
            std::cout << "Обнаружен символ, отличающийся от цифр. Попробуйте ещё раз." << std::endl;
            continue;
        }

        sortString(str);

        {
            std::lock_guard<std::mutex> lock(buf_mutex);
            buffer.push(str);
            flag = true;
            cInFlag = true;
        }
    }
}

void threadTwo() {
    int sock = -1;
    while (true) {
        if (!flag || buffer.empty()) continue;

        std::string res;     
   
        {
            std::lock_guard<std::mutex> lock(buf_mutex);
            while (!buffer.empty()) {
                res += buffer.front();
                buffer.pop();
            }
            std::cout << "Полученные данные: " << res << std::endl;
        }

        cInFlag = false;

        if (sock < 0) {
            sock = connectToServer();
            if (sock < 0) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
        }

        if (isDisconnected(sock)) {
            close(sock);
            sock = -1;
            continue;
        }

        std::string data = sumString(res);
        if (send(sock, data.c_str(), data.size(), 0) < 0) {
            close(sock);
            sock = -1;
        }
        else {
            flag = false;
        }
    }
}
int main() {
    setlocale(0, "LC_ALL");
    std::thread t1(threadOne);
    std::thread t2(threadTwo);
    t1.join();
    t2.join();
    return 0;
}
