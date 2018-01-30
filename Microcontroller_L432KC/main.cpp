#include "VX0706.h"
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


DigitalOut led(LED1);

Timer t;
int main()
{
   
    led=0;  

    
    camera Camera;
    if(Camera.begin()) led=1;
    //Camera.pc->printf("test");
    char *reply = Camera.getVersion();
    if (reply == 0) {
        //Camera.pc->printf("Failed to get version");
    } 
    else {
        //Camera.pc->printf("----------------------------\n");
      // Camera.pc->printf("Version :\n");
       // Camera.pc->printf(reply);
    }
    int imgsize=Camera.getImageSize();
   /*Camera.pc->printf("Image size: ");
  if (imgsize == VC0706_640x480)  Camera.pc->printf("640x480");
  if (imgsize == VC0706_320x240)  Camera.pc->printf("320x240");
  if (imgsize == VC0706_160x120)  Camera.pc->printf("160x120");
 */if (! Camera.takePicture()) 
    Camera.pc->printf("Failed to snap!");

  //Camera.pc->printf("Storing ");
  //Camera.pc->printf("%d",jpglen);
  //Camera.pc->printf(" byte image.");
    while(1)
    {
        if(Camera.pc->readable())
        {
            if(Camera.pc->getc()=='c')
            {
   
uint16_t jpglen = Camera.frameLength();

           
            // Read all the data up to # bytes!
          //  uint8_t wCount = 0; // For counting # of writes
            while (jpglen > 0) {
              // read 32 bytes at a time;
              uint8_t *buffer;
              uint8_t bytesToRead ;
              if(64>jpglen)
               bytesToRead = jpglen;
               else bytesToRead =64; // change 32 to 64 for a speedup but may not work with all setups!
              buffer = Camera.readPicture(bytesToRead);
              event_callback_t callback;
             //Camera.pc->write(buffer, bytesToRead,&callback);
              for(uint8_t i = 0; i<bytesToRead; i++){
                Camera.pc->putc(buffer[i]);
              }
              //Camera.pc->write(buffer, bytesToRead);
              
                //Camera.pc->print("Read ");  Camera.pc->print(bytesToRead, DEC); Camera.pc->println(" bytes");
              jpglen -= bytesToRead;
              //Camera.pc->printf("taille restante %d",jpglen);
            }

   
      }}}

    
    
}