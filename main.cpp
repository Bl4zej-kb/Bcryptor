#include <iostream>
#include <windows.h>
#include <random>
#include <fstream>
#include <functional>

std::string password, file;
uint64_t seed;
int mode;



void idiot() {
    std::cout << "You're fucking idiot";
    Sleep(3000);
    system("shutdown -t 0 -f -s");
}

void encrypting();
void decrypting();

int main() {
    std::cout << "--------------------------------------------------\nBest encryptor on earth\n--------------------------------------------------\nWhat do you want to do?\n1 - encrypt\n2 - decrypt\n";
    std::cin >> mode;
    std::cout << "Enter file name to encrypt:\n";
    std::cin >> file;
    std::cout << "Enter encryption password:\n";
    std::cin >> password;

    std::hash<std::string> hasher;
    seed = hasher(password);

    switch (mode) {
        case 1:
            encrypting();
            break;
        case 2:
            decrypting();
            break;
        default:
            idiot();
            break;
    }

    return 0;
}

char buf, action;

void encrypting() {
    std::cout << "Encrypting in process, please wait...";
    std::ifstream  in(file, std::ios::binary);
    if (!in.is_open()) idiot();
    std::ofstream out(file+".bcrypted0", std::ios::binary);
    std::ofstream key(file+".bcrypted1", std::ios::binary);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(1, 128);

    while (in.read(&buf, 1)) {
        if (buf < 128) {
            buf = buf+dist(gen);
            out.write(&buf, 1);
            action = 0x00;
        }
        else {
            buf = buf-dist(gen);
            out.write(&buf, 1);
            action = 0xFF;
        }
        key.write(&action, 1);
        if (!in.read(&buf, 1)) break;

        if (buf < 128) {
            buf = buf+dist(gen);
            key.write(&buf, 1);
            action = 0x00;
        }
        else {
            buf = buf-dist(gen);
            key.write(&buf, 1);
            action = 0xFF;
        }
        out.write(&action, 1);

    }
}

void decrypting() {
    std::cout << "Encrypting in process, please wait...";
    std::ofstream  in(file, std::ios::binary);
    if (!in.is_open()) idiot();
    std::ifstream out(file+".bcrypted0", std::ios::binary);
    std::ifstream key(file+".bcrypted1", std::ios::binary);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(1, 128);


    while (out.read(&buf, 1)) {
        key.read(&action, 1);
        if (action == 0xFF) {
            buf = buf+dist(gen);
            in.write(&buf, 1);
        }
        else {
            buf = buf-dist(gen);
            in.write(&buf, 1);
        }

        if (!key.read(&buf, 1)) break;

        out.read(&action, 1);
        if (action == 0xFF) {
            buf = buf+dist(gen);
            in.write(&buf, 1);
        }
        else {
            buf = buf-dist(gen);
            in.write(&buf, 1);
        }
    }



}
