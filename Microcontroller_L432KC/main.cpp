
#include "main.h"
/*------------------------------------------------------------------------------
Before to use this example, ensure that you an hyperterminal installed on your
computer. More info here: https://developer.mbed.org/handbook/Terminals

The default serial comm port uses the SERIAL_TX and SERIAL_RX pins (see their
definition in the PinNames.h file).

The default serial configuration in this case is 9600 bauds, 8-bit data, no parity

If you want to change the baudrate for example, you have to redeclare the
serial object in your code:

Serial pc(SERIAL_TX, SERIAL_RX);

Then, you can modify the baudrate and print like this:

pc.baud(115200);
pc.printf("Hello World !\n");
------------------------------------------------------------------------------*/
     PwmOut MotorA(MOTOR_A);
     PwmOut MotorB(MOTOR_B);

Serial pc(USBTX, USBRX);

DigitalOut led(LED1);
void takePhoto(Camera camera);
int main()
{    
    pc.baud(38400);
    led=0;      // 4 second period
    Moteur motor;
    Camera camera;
    wait(0.5);
    camera.begin();
    wait(1);

   

    while(1)
    {
        if(pc.readable())
        {
            led=1;
            uint8_t command=pc.getc();
            uint8_t value=(command & 0x0F);
            command= (command&0xF0);
            switch(command)
            {
                
                case COMMAND_TAKEPHOTO:
                    takePhoto(camera);
                    break;
                
                case    COMMAND_GETVERSION:
                         pc.printf(camera.getVersion());
                    break;
                
                case COMMAND_SETCOMPRESSION:
                    camera.setCompression(value<<4|value);
                    break;
                
                case COMMAND_SWITCHCOLOR:
                    camera.setColor();
                    break;
                
                case COMMAND_RESET:
                    camera.sendreset();
                    break;
                
                case COMMAND_SETBAUDRATE:
                    break;
                    
                case COMMAND_SETMOTORPWM:
                    motor.changePWM(value);
                    break;
                    
                case COMMAND_SETMOTORDIR:
                    motor.changedirection(value);
                    break;
                    
            }
        } 
    }  
}

void takePhoto(Camera camera)
{
                
                if (! camera.takePicture()) 
                    pc.printf("Failed to snap!");
                uint16_t jpglen = camera.frameLength();

           
                // Read all the data up to # bytes!
                //  uint8_t wCount = 0; // For counting # of writes
                while (jpglen > 0) {
                      // read 32 bytes at a time;
                      uint8_t *buffer;
                      uint8_t bytesToRead ;
                      if(64>jpglen)
                        bytesToRead = jpglen;
                      else bytesToRead =64; // change 32 to 64 for a speedup but may not work with all setups!
                      buffer = camera.readPicture(bytesToRead);
                      for(uint8_t i = 0; i<bytesToRead; i++){
                        pc.putc(buffer[i]);
                      }
                       jpglen -= bytesToRead;
                  }
                  camera.resumeVideo();
}
