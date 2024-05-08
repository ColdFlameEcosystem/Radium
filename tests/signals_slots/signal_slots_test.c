#include <r_signals_slots>
#include <stdio.h>

int integer1 = 10;

r_signal* integer1changed;

void int1slot1() {
    printf("V1 Changed to %u\n",integer1);
};

void int2slot2(char* message) {
    printf("%s", message);
}
void changeV1() {
    integer1 = 30;
    r_emit_signal(integer1changed);
};
int main() {
    integer1changed = r_create_signal();
    r_connect_signal(integer1changed, int1slot1, NULL);
    r_connect_signal(integer1changed, int2slot2, "Damn!\n");
    changeV1();
    r_destroy_signal(integer1changed);
    return 0;
}
