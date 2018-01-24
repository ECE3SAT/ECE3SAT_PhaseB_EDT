#include "mbed.h"

DigitalOut myled(LED1);

Serial s(USBTX, USBRX);

int main() {
    while(1) {
        myled = 1; // LED is ON
        wait(1);
        myled = 0; // LED is OFF
        wait(1);
        s.printf("test");
    }
}

