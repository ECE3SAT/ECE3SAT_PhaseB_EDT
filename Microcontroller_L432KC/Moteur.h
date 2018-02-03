#ifndef MOTEUR_H_
#define MOTEUR_H_
#include "mbed.h"
#include "main.h"
extern PwmOut MotorA;
extern PwmOut MotorB;
 class Moteur
 {
    private : 
        uint8_t direction;
        float PWM;
        void setMotorTurn();
    public:
        void changePWM(uint8_t value);
        void changedirection(uint8_t value);
        Moteur();
};
#endif