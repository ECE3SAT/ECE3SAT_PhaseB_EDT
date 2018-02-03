#ifndef MAIN_H_
#define MAIN_H_
#include "mbed.h"

#include "Camera.h"
#include "Moteur.h"

#define MOTOR_A A1
#define MOTOR_B A6

#define COMMAND_TAKEPHOTO 0x10
#define COMMAND_GETVERSION 0x20
#define COMMAND_SETCOMPRESSION 0x30
#define COMMAND_SWITCHCOLOR 0x40
#define COMMAND_RESET 0x50
#define COMMAND_SETBAUDRATE 0x60
#define COMMAND_SETMOTORPWM 0x70
#define COMMAND_SETMOTORDIR 0x80

#endif