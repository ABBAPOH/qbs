#include <cstdlib>

void foo() {
    std::abort();
}

int main() {
    foo();
    return 0;
}