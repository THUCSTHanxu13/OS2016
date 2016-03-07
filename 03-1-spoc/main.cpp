#include <iostream>
#include <map>
#include <cstdio>
#include "mem.h"

std::map<std::string, void*> mems;

int main() {
    std::cout << "Use:\n"
        << "\tnew [name] [size]\n"
        << "\tdel [name]\n";

    std::string op, name;
    int len;
    while (std::cin >> op) {
        if (op == "new") {
            std::cin >> name >> len;
            void *ptr = malloc_local(len);
            printf("0x%08X\n", (int)ptr);
            mems[name] = ptr;
        } else if (op == "del") {
            std::cin >> name;
            if (mems.find(name) == mems.end()) {
                std::cout << "name not found!\n";
            } else {
                free_local(mems[name]);
                mems.erase(name);
            }
        } else
            std::cout << "Op not found!\n";
    }
}
