#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include "myLib.hpp"

#define PORT 9090

void handleConnection(int client_socket) {
    char buffer[1024] = {0};
    while (true) {
        int valread = read(client_socket, buffer, 1024);
        if (valread <= 0) {
            std::cout << "Соединение разорвано. Ожидание нового подключения..." << std::endl;
            break;
        }

        std::string received_data(buffer);

        bool res = analyzeData(received_data);
        if (res)
            std::cout << "Данные получены и обработаны успешно. Ожидание новых данных." << std::endl;
        else
            std::cout << "Ошибка получения данных. Ожидание новых данных." << std::endl;
        memset(buffer, 0, sizeof(buffer)); 
    }
    close(client_socket);
}

void startServer() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Ошибка сокета");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Ошибка установки сокета");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Ошибка бинда");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Ошибка прослушивания");
        exit(EXIT_FAILURE);
    }

    std::cout << "Сервер работает. Ожидание подключения..." << std::endl;

    while (true) {
        if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Ошибка подключения");
            continue;
        }

        std::cout << "Клиент подключён! Ожидание данных..." << std::endl;
        handleConnection(client_socket);
    }

    close(server_fd);
}

int main() {
    startServer();
    return 0;
}
