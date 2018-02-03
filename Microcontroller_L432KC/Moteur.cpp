#include "Moteur.h"

Moteur::Moteur()
{
    MotorA.period(0.005f);      // 5 ms period
    MotorA.write(0.0f);
    MotorB.period(0.005f);      // 5ms period
    MotorB.write(0.0f);
    direction=0;
}
void Moteur ::changePWM(uint8_t value)
{
     PWM=(float)value/16;
    setMotorTurn();
   
}
void Moteur ::changedirection(uint8_t value)
{
    direction=value;
    
    setMotorTurn();
    
}
void Moteur :: setMotorTurn()
{
    if(direction==0)
    {
        MotorA.write(0.0f);
        MotorB.write(PWM);
    }   
    else if (direction==1)
    {
        MotorA.write(PWM);
        MotorB.write(0.0f);
    }
    else 
    {
        MotorA.write(0.0f);
        MotorB.write(0.0f);
    }
}
