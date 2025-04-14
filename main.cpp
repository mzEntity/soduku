#include "debug_utils/debug_utils.h"

void demo(int x) {
    LOG_INFO("进入 demo()");
    ASSERT(x != 0, "x 不可以为 0");
    if (x < 0) LOG_WARN("x 是负数");
    if (x == 42) PANIC("发现神秘值 42，程序中止");
}

int main() {
    demo(42);
    return 0;
}
