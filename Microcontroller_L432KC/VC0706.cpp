
#include "VC0706.h"

  
 camera::camera()
{
      frameptr  = 0;
      bufferLen = 0;
      serialNum = 0;
    pc=new Serial(USBTX, USBRX);
    device= new Serial(D5, D4);// tx, rx
    pc->baud(38400);
    device->baud(38400);
}
uint8_t camera::getImageSize() 
{
  uint8_t args[] = {0x4, 0x4, 0x1, 0x00, 0x19};
  if (! runCommand(VC0706_READ_DATA, args, sizeof(args), 6))
    return -1;

  return camerabuff[5];
}
 char * camera::getVersion(void) 
{
  uint8_t args[] = {0x01};
  
  sendCommand(VC0706_GEN_VERSION, args, 1);
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
    
        //pc->printf("----------------------------\n");
        //pc->printf("Version :\n");
        //pc->printf((char *)camerabuff);
    
  return (char *)camerabuff;  // return it!
}   


/***************** baud rate commands */

char* camera::setBaud9600() 
{
  uint8_t args[] = {0x03, 0x01, 0xAE, 0xC8};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!

}

char* camera::setBaud19200() 
{
  uint8_t args[] = {0x03, 0x01, 0x56, 0xE4};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!
}

char* camera::setBaud38400() 
{
  uint8_t args[] = {0x03, 0x01, 0x2A, 0xF2};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!
}

char* camera::setBaud57600() 
{
  uint8_t args[] = {0x03, 0x01, 0x1C, 0x1C};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!
}

char* camera::setBaud115200() 
{
  uint8_t args[] = {0x03, 0x01, 0x0D, 0xA6};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!
}

/****************** high level photo comamnds */

void camera::OSD(uint8_t x, uint8_t y, char *str) 
{
  if (strlen(str) > 14) { str[13] = 0; }

  uint8_t args[17] = {strlen(str), strlen(str)-1, (y & 0xF) | ((x & 0x3) << 4)};

  for (uint8_t i=0; i<strlen(str); i++) {
    char c = str[i];
    if ((c >= '0') && (c <= '9')) {
      str[i] -= '0';
    } else if ((c >= 'A') && (c <= 'Z')) {
      str[i] -= 'A';
      str[i] += 10;
    } else if ((c >= 'a') && (c <= 'z')) {
      str[i] -= 'a';
      str[i] += 36;
    }

    args[3+i] = str[i];
  }

   runCommand(VC0706_OSD_ADD_CHAR, args, strlen(str)+3, 5);
   printBuff();
}

bool camera::setCompression(uint8_t c) 
{
  uint8_t args[] = {0x5, 0x1, 0x1, 0x12, 0x04, c};
  return runCommand(VC0706_WRITE_DATA, args, sizeof(args), 5);
}

uint8_t camera::getCompression(void) 
{
  uint8_t args[] = {0x4, 0x1, 0x1, 0x12, 0x04};
  runCommand(VC0706_READ_DATA, args, sizeof(args), 6);
  printBuff();
  return camerabuff[5];
}

bool camera::setPTZ(uint16_t wz, uint16_t hz, uint16_t pan, uint16_t tilt) 
{
  uint8_t args[] = {0x08, wz >> 8, wz, 
            hz >> 8, wz, 
            pan>>8, pan, 
            tilt>>8, tilt};

  return (! runCommand(VC0706_SET_ZOOM, args, sizeof(args), 5));
}


bool camera::getPTZ(uint16_t &w, uint16_t &h, uint16_t &wz, uint16_t &hz, uint16_t &pan, uint16_t &tilt) 
{
  uint8_t args[] = {0x0};
  
  if (! runCommand(VC0706_GET_ZOOM, args, sizeof(args), 16))
    return false;
  printBuff();

  w = camerabuff[5];
  w <<= 8;
  w |= camerabuff[6];

  h = camerabuff[7];
  h <<= 8;
  h |= camerabuff[8];

  wz = camerabuff[9];
  wz <<= 8;
  wz |= camerabuff[10];

  hz = camerabuff[11];
  hz <<= 8;
  hz |= camerabuff[12];

  pan = camerabuff[13];
  pan <<= 8;
  pan |= camerabuff[14];

  tilt = camerabuff[15];
  tilt <<= 8;
  tilt |= camerabuff[16];

  return true;
}


bool camera::takePicture() 
{
  frameptr = 0;
  return cameraFrameBuffCtrl(VC0706_STOPCURRENTFRAME); 
}

bool camera::resumeVideo() 
{
  return cameraFrameBuffCtrl(VC0706_RESUMEFRAME); 
}

bool camera::TVon() 
{
  uint8_t args[] = {0x1, 0x1};
  return runCommand(VC0706_TVOUT_CTRL, args, sizeof(args), 5);
}
bool camera::TVoff() 
{
  uint8_t args[] = {0x1, 0x0};
  return runCommand(VC0706_TVOUT_CTRL, args, sizeof(args), 5);
}

bool camera::cameraFrameBuffCtrl(uint8_t command) 
{
  uint8_t args[] = {0x1, command};
  return runCommand(VC0706_FBUF_CTRL, args, sizeof(args), 5);
}

uint32_t camera::frameLength(void) 
{
  uint8_t args[] = {0x01, 0x00};
  if (!runCommand(VC0706_GET_FBUF_LEN, args, sizeof(args), 9))
    return 0;

  uint32_t len;
  len = camerabuff[5];
  len <<= 8;
  len |= camerabuff[6];
  len <<= 8;
  len |= camerabuff[7];
  len <<= 8;
  len |= camerabuff[8];

  return len;
}


uint8_t camera::available(void) 
{
  return bufferLen;
}


uint8_t * camera::readPicture(uint8_t n) 
{
  uint8_t args[] = {0x0C, 0x0, 0x0A, 
                    0, 0, frameptr >> 8, frameptr & 0xFF, 
                    0, 0, 0, n, 
                    CAMERADELAY >> 8, CAMERADELAY & 0xFF};

  if (! runCommand(VC0706_READ_FBUF, args, sizeof(args), 5, false))
    return 0;


  // read into the buffer PACKETLEN!
  if (readResponse(n+5, CAMERADELAY) == 0) 
      return 0;


  frameptr += n;

  return camerabuff;
}

/**************** LOW LEVEL COMMANDS *****************/


void camera::printBuff() 
{
  Serial pc(USBTX, USBRX);

  for (uint8_t i = 0; i< bufferLen; i++) {
    //Serial.print(" 0x");
    //Serial.print(camerabuff[i], HEX); 
    //pc.printf(" 0x");
    ////pc.putc(camerabuff[i]);
  }
  //Serial.println();
  //pc.printf("\n");
}

bool camera::begin()
{
    
    return sendreset();         
}
bool camera::sendreset()
{
      uint8_t args[] = {0x0};

    return (runCommand(VC0706_RESET, args, 1, 5));
};
bool camera:: runCommand(uint8_t cmd, uint8_t *args, uint8_t argn, 
               uint8_t resplen, bool flushflag)
{
     if(device->readable() && flushflag)
    {
          device->getc();
     }
     sendCommand(cmd, args, argn);
     if (readResponse(resplen, 200) != resplen) {
        printf("before false");
        return false;
    }
    
    //pc->printf("end reset recu : %x %x %x %x %x fin",camerabuff[0],camerabuff[1],camerabuff[2],camerabuff[3],camerabuff[4]);
    
     if (! verifyResponse(cmd))
    return false;
    
        //pc->printf("   okkk  ");
  return true;
}
void camera::sendCommand(uint8_t cmd, uint8_t args[] = 0, uint8_t argn = 0) {

    device->putc(0x56);
    device->putc(serialNum);
    device->putc(cmd);
     for(int i1=0;i1<argn;i1++){
        device->putc(args[i1]);
        }
}
uint8_t camera ::readResponse(uint8_t numbytes, uint8_t timeout) 

{
    Timer t;
    t.reset();
    t.start();
    int bufferLen=0;
     while ((t.read()<= (float)(timeout)/100.0) && (bufferLen != numbytes)){
        if(device->readable())
        {
           
            ////pc.putc(device.getc());
        
            camerabuff[bufferLen] = device->getc();
            bufferLen++;
            t.stop();
            t.reset();
            t.start();        // there's a byte!*/
        
        }
        //printf("c: %x\n", camerabuff[bufferLen]);
    } t.stop();
    return bufferLen;

}
bool camera::verifyResponse(uint8_t command) 
{
  if ((camerabuff[0] != 0x76) ||
      (camerabuff[1] != serialNum) ||
      (camerabuff[2] != command) ||
      (camerabuff[3] != 0x0))
      return false;
      
  return true;
  
}