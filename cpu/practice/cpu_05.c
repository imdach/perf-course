void func_d() {
    for (int i = 5 * 10000; i--;);
}

void func_a() {
    for (int i = 10 * 10000; i--;);
    func_d();
}

void func_b() {
    for (int i = 20 * 10000; i--;);
}

void func_c() {
    for (int i = 35 * 10000; i--;);
}

int main(void) {
    while (1) {
        for (int i = 30 * 10000; i--;);
        func_a();
        func_b();
        func_c();
    }
    return 0;
}