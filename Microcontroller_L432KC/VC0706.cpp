#include "main.h"

Serial pc(USBTX, USBRX);

// Initialization code used by all constructor types
void Adafruit_VC0706::common_init(void) 
{
  ser  = NULL;
  frameptr  = 0;
  bufferLen = 0;
  serialNum = 0;
}

Adafruit_VC0706::Adafruit_VC0706(Serial *serial) 
{
  common_init();  // Set everything to common state, then...
  ser = serial; 
}

bool Adafruit_VC0706::begin(uint16_t baud) 
{
  ser->baud(baud);
  return reset();
}

bool Adafruit_VC0706::reset() 
{
  uint8_t args[] = {0x0};
  return runCommand(VC0706_RESET, args, 1, 5);
}

bool Adafruit_VC0706::motionDetected() 
{
  if (readResponse(4, 200) != 4) {
    return false;
  }
  if (! verifyResponse(VC0706_COMM_MOTION_DETECTED))
    return false;

  return true;
}


bool Adafruit_VC0706::setMotionStatus(uint8_t x, uint8_t d1, uint8_t d2) 
{
  uint8_t args[] = {0x03, x, d1, d2};
  return runCommand(VC0706_MOTION_CTRL, args, sizeof(args), 5);
}


uint8_t Adafruit_VC0706::getMotionStatus(uint8_t x)
{
  uint8_t args[] = {0x01, x};
  return runCommand(VC0706_MOTION_STATUS, args, sizeof(args), 5);
}


bool Adafruit_VC0706::setMotionDetect(bool flag) 
{
  if (! setMotionStatus(VC0706_MOTIONCONTROL, VC0706_UARTMOTION, VC0706_ACTIVATEMOTION))
    return false;

  uint8_t args[] = {0x01, flag};
  runCommand(VC0706_COMM_MOTION_CTRL, args, sizeof(args), 5);
}


bool Adafruit_VC0706::getMotionDetect(void) 
{
  uint8_t args[] = {0x0};

  if (! runCommand(VC0706_COMM_MOTION_STATUS, args, 1, 6))
    return false;

  return camerabuff[5];
}

uint8_t Adafruit_VC0706::getImageSize() 
{
  uint8_t args[] = {0x4, 0x4, 0x1, 0x00, 0x19};
  if (! runCommand(VC0706_READ_DATA, args, sizeof(args), 6))
    return -1;

  return camerabuff[5];
}

bool Adafruit_VC0706::setImageSize(uint8_t x) 
{
  uint8_t args[] = {0x05, 0x04, 0x01, 0x00, 0x19, x};

  return runCommand(VC0706_WRITE_DATA, args, sizeof(args), 5);
}

/****************** downsize image control */

uint8_t Adafruit_VC0706::getDownsize(void) 
{
  uint8_t args[] = {0x0};
  if (! runCommand(VC0706_DOWNSIZE_STATUS, args, 1, 6)) 
    return -1;

   return camerabuff[5];
}

bool Adafruit_VC0706::setDownsize(uint8_t newsize) 
{
  uint8_t args[] = {0x01, newsize};

  return runCommand(VC0706_DOWNSIZE_CTRL, args, 2, 5);
}

/***************** other high level commands */

char * Adafruit_VC0706::getVersion(void) 
{
  uint8_t args[] = {0x01};
  
  sendCommand(VC0706_GEN_VERSION, args, 1);
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!
}


/***************** baud rate commands */

char* Adafruit_VC0706::setBaud9600() 
{
  uint8_t args[] = {0x03, 0x01, 0xAE, 0xC8};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!

}

char* Adafruit_VC0706::setBaud19200() 
{
  uint8_t args[] = {0x03, 0x01, 0x56, 0xE4};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!
}

char* Adafruit_VC0706::setBaud38400() 
{
  uint8_t args[] = {0x03, 0x01, 0x2A, 0xF2};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!
}

char* Adafruit_VC0706::setBaud57600() 
{
  uint8_t args[] = {0x03, 0x01, 0x1C, 0x1C};

  sendCommand(VC0706_SET_PORT, args, sizeof(args));
  // get reply
  if (!readResponse(CAMERABUFFSIZ, 200)) 
    return 0;
  camerabuff[bufferLen] = 0;  // end it!
  return (char *)camerabuff;  // return it!
}

char* Adafruit_VC0706::setBaud115200() 
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

void Adafruit_VC0706::OSD(uint8_t x, uint8_t y, char *str) 
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

bool Adafruit_VC0706::setCompression(uint8_t c) 
{
  uint8_t args[] = {0x5, 0x1, 0x1, 0x12, 0x04, c};
  return runCommand(VC0706_WRITE_DATA, args, sizeof(args), 5);
}

uint8_t Adafruit_VC0706::getCompression(void) 
{
  uint8_t args[] = {0x4, 0x1, 0x1, 0x12, 0x04};
  runCommand(VC0706_READ_DATA, args, sizeof(args), 6);
  printBuff();
  return camerabuff[5];
}

bool Adafruit_VC0706::setPTZ(uint16_t wz, uint16_t hz, uint16_t pan, uint16_t tilt) 
{
  uint8_t args[] = {0x08, wz >> 8, wz, 
            hz >> 8, wz, 
            pan>>8, pan, 
            tilt>>8, tilt};

  return (! runCommand(VC0706_SET_ZOOM, args, sizeof(args), 5));
}


bool Adafruit_VC0706::getPTZ(uint16_t &w, uint16_t &h, uint16_t &wz, uint16_t &hz, uint16_t &pan, uint16_t &tilt) 
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


bool Adafruit_VC0706::takePicture() 
{
  frameptr = 0;
  return cameraFrameBuffCtrl(VC0706_STOPCURRENTFRAME); 
}

bool Adafruit_VC0706::resumeVideo() 
{
  return cameraFrameBuffCtrl(VC0706_RESUMEFRAME); 
}

bool Adafruit_VC0706::TVon() 
{
  uint8_t args[] = {0x1, 0x1};
  return runCommand(VC0706_TVOUT_CTRL, args, sizeof(args), 5);
}
bool Adafruit_VC0706::TVoff() 
{
  uint8_t args[] = {0x1, 0x0};
  return runCommand(VC0706_TVOUT_CTRL, args, sizeof(args), 5);
}

bool Adafruit_VC0706::cameraFrameBuffCtrl(uint8_t command) 
{
  uint8_t args[] = {0x1, command};
  return runCommand(VC0706_FBUF_CTRL, args, sizeof(args), 5);
}

uint32_t Adafruit_VC0706::frameLength(void) 
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


uint8_t Adafruit_VC0706::available(void) 
{
  return bufferLen;
}


uint8_t * Adafruit_VC0706::readPicture(uint8_t n) 
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

bool Adafruit_VC0706::runCommand(uint8_t cmd, uint8_t *args, uint8_t argn, 
               uint8_t resplen, bool flushflag) 
{
  // flush out anything in the buffer?
  if (flushflag) {
    readResponse(100, 10); 
  }
  
  sendCommand(cmd, args, argn);;
  
    printf("before  read respond length : %d ", resplen);
  if (readResponse(resplen, 200) != resplen) {
    printf("before false");
    return false;
    }
    
    printf("before verify response");
  if (! verifyResponse(cmd))
    return false;
    
    printf("after verify response");
    
  return true;
}

void Adafruit_VC0706::sendCommand(uint8_t cmd, uint8_t args[] = 0, uint8_t argn = 0) {

    /*ser->write(0x56, 2);
    ser->write((byte)serialNum);
    ser->write((byte)cmd);*/
    
    ser->printf("%c",0x56);
    ser->printf("%c",serialNum);
    ser->printf("%c",cmd);
    
    /*ser->putc(0x56);
    ser->putc(serialNum);
    ser->putc(cmd);*/
    
    

    /*for (uint8_t i=0; i<argn; i++) {
      ser->write((byte)args[i]);
    }*/
    for (uint8_t i=0; i<argn; i++) {
        //ser->printf(args[i]);
        ser->putc(args[i]);
    }
}

uint8_t Adafruit_VC0706::readResponse(uint8_t numbytes, uint8_t timeout) 
{
    uint8_t counter = 0;
    bufferLen = 0;
    int avail;
    
     printf("before loop\n");
     
    while ((timeout != counter) && (bufferLen != numbytes)){
        
        avail = ser->readable();
        
        //printf("%d\n", counter);
        
        if (avail <= 0) {
            wait(1);
            counter++;
            continue;
        }
        counter = 0;
        // there's a byte!
        //camerabuff[bufferLen++] = ser->read();
        camerabuff[bufferLen++] = ser->getc();
        //printf("c: %x\n", camerabuff[bufferLen]);
    }
    printf("\nfinish loop: %d\n", bufferLen);
    return bufferLen;
}

bool Adafruit_VC0706::verifyResponse(uint8_t command) 
{
  
  if ((camerabuff[0] != 0x76) ||
      (camerabuff[1] != serialNum) ||
      (camerabuff[2] != command) ||
      (camerabuff[3] != 0x0))
      return false;
      
  return true;
  
}

void Adafruit_VC0706::printBuff() 
{
  Serial pc(USBTX, USBRX);

  for (uint8_t i = 0; i< bufferLen; i++) {
    //Serial.print(" 0x");
    //Serial.print(camerabuff[i], HEX); 
    pc.printf(" 0x");
    pc.putc(camerabuff[i]);
  }
  //Serial.println();
  pc.printf("\n");
}
