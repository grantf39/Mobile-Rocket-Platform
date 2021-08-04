#include <SPI.h>
#include<nRF24L01.h>
#include <RF24.h>

#include <Wire.h>
//def I2C slave address
#define SLAVE_ADDR 9
//define slave answer size
//#define ANSWERSIZE 5

#include <Servo.h>




RF24 radio(9,10); // CSN, CE <-WARNING MAY BE REVERSE!!!! CE CNS INSTEAD

Servo servo1;//R,R,L,L
Servo servo2;
Servo servo3;
Servo servo4;

const byte address[6] = "00001";

int ThresLow = 362;
int ThresHigh = 662;
int shootVal = 0;
//input vals
//int shootVal = 0;

int L_X_val=500; //cuz 500 is about the middle val
int L_Y_val=500;

int R_X_val=500;
int Y_X_val=500;

//fire buttons
int W1=0;
int W2=0;
int W3=0;
int W4=0;
int W5=0;
int W6=0;
int W7=0;
int W8=0;

int x_val;
int y_val;

//servo pins
const int s1=5;//R
const int s2=6;//R
const int s3=7;//L
const int s4=8;//L


//Wire char array setup

char gResp[11] = {};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();

  //setup servo wheels
  servo1.attach(5);//L
  servo1.detach();
  servo2.attach(6);//L
  servo2.detach();
  servo3.attach(7);//L
  servo3.detach();
  servo4.attach(8);//L
  servo4.detach();
  //initialize to be stationary just in case
  x_val=500;
  y_val=500;


  Wire.begin(SLAVE_ADDR);

  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
}

void receiveEvent(){
  while (0<Wire.available()){
    byte x = Wire.read();
  }

  Serial.println("Received Gun Req");
}

void requestEvent(){
  //gResp is char array response
  byte cSig[11]; //int size is 2?
  for( byte i=0; i<11; i++){
    cSig[i] = (byte)gResp[i];
  }
  
  //Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(cSig, sizeof(cSig));
  //Wire.endTransmission();
  
}


void loop() {
  /*
  int test[13] = {digitalRead(SFpin),analogRead(L_X_pin), analogRead(L_Y_pin), analogRead(R_X_pin), analogRead(R_Y_pin),
  digitalRead(W1), digitalRead(W2), digitalRead(W3), digitalRead(W4),digitalRead(W5), digitalRead(W6), digitalRead(W7), digitalRead(W8) };
  */
  int rInput[13] = {0,500,500,500,500,0,0,0,0,0,0,0,0};
  //read in radio data
  delay(50);

  
  if (radio.available()) {//if signal read, overwrite
    //int rDirect[13] = {};
    radio.read(&rInput, sizeof(rInput));
  }

 
  for(int i=0; i<(sizeof(rInput)/sizeof(int));i++){
      Serial.print(rInput[i]);
      Serial.print("-");
    }
    Serial.println("");

  //Send rest of data through I2C to slave arduino
  //define data wanted to be sent
  /*
  int gSig[11] = {rInput[0], rInput[3], rInput[4], rInput[5], rInput[6], rInput[7], rInput[8], rInput[9], rInput[10], rInput[11], rInput[12]};
  byte cSig[11]; //int size is 2?
  for( byte i=0; i<11; i++){
    cSig[i] = (byte)gSig[i];
  }
  */
  //initialize char commands to be safe
  char gChar[11] = {'s','s','s', '0', '0','0', '0', '0', '0', '0','0'};

  if(rInput[0]==1){//safe or fire
    gChar[0]= 'f';
  }
  int rX = 1023-rInput[4];
  int rY = 1023-rInput[3];

  /*
  ThresLow = 362;
  ThresHigh = 662;
  */
  if(rX<ThresLow){//left or right
    gChar[1]='l';
  }else if(rX>ThresHigh){
    gChar[1]='r';
  }

  if(rY<ThresLow){//up or down
    gChar[2]='d';
  }else if(rY>ThresHigh){
    gChar[2]='u';
  }

  for(int i=0; i<8; i++){//Check values for all rocket batteries
    if(rInput[i+5]==1){
      gChar[i+3]='1';
    }
  }

  for(int i=0;i<11;i++){//copy gChar to global char array, gResp
    gResp[i]=gChar[i];
  }
  /*
  
  byte cSig[11]; //int size is 2?
  for( byte i=0; i<11; i++){
    cSig[i] = (byte)gChar[i];
  }
  
  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(cSig, sizeof(cSig));
  Wire.endTransmission();

    Serial.println("Byte?:");
  for(int i =0; i<(sizeof(cSig)/sizeof(byte));i++){  
    Serial.print(cSig[i]);
    Serial.print("-");
  }
    Serial.println("");

  Serial.println("Back 2 int?:");
  for(int i =0; i<(sizeof(cSig)/sizeof(byte));i++){  
    Serial.print((int)cSig[i]);
    Serial.print("-");
  }
    Serial.println("");
  */

  //translate those radio values into our shit.
  x_val= (1023-rInput[2]);
  y_val=(1023-rInput[1]);

  Serial.print("Xval:");
  Serial.println(x_val);

  Serial.print("Yval:");
  Serial.println(y_val);
  Serial.println("");

  
  //control servo wheels
  if (x_val>ThresLow && x_val <ThresHigh && y_val>ThresLow && y_val <ThresHigh){//stationary
    servo1.detach();
    servo2.detach();
    servo3.detach();
    servo4.detach();
    //float x_norm = (((x_val)/(362))*(89));
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm);
    //Serial.print("servo_val: N/A");
  }
  else if(x_val>ThresLow && x_val <ThresHigh && y_val>ThresHigh ){//move forward
    servo1.detach();
    servo1.attach(s1);    
    servo2.detach();
    servo2.attach(s2);
    servo3.detach();
    servo3.attach(s3);
    servo4.detach();
    servo4.attach(s4);
    //float x_norm = (((x_val)/(362))*(89));
    
    servo1.write(0);
    servo2.write(0);
    servo3.write(180);
    servo4.write(180);
    
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm); 
    //Serial.print("servo_val: 0");
    
  }
  else if(x_val>ThresLow && x_val <ThresHigh && y_val<ThresLow ){//move backwards
    servo1.detach();
    servo2.detach();
    servo1.attach(s1);
    servo2.attach(s2);
    servo3.detach();
    servo3.attach(s3);
    servo4.detach();
    servo4.attach(s4);
    //float x_norm = (90+((x_val-662)/(362))*(90));
    servo1.write(180);
    servo2.write(180);
    servo3.write(0);
    servo4.write(0);

    //Serial.print(x_norm);
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm);

    //Serial.print("servo_val: 180");
  }


  else if(x_val<ThresLow && y_val>ThresLow && y_val <ThresHigh ){//turn left
    servo1.detach();
    servo2.detach();
    servo1.attach(s1);
    servo2.attach(s2);
    servo3.detach();
    servo3.attach(s3);
    servo4.detach();
    servo4.attach(s4);
    //float x_norm = (90+((x_val-662)/(362))*(90));
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    servo4.write(0);

    //Serial.print(x_norm);
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm);

    //Serial.print("servo_val: 180");
  }

  else if(x_val>ThresHigh && y_val>ThresLow && y_val <ThresHigh ){//turn right
    servo1.detach();
    servo2.detach();
    servo1.attach(s1);
    servo2.attach(s2);
    servo3.detach();
    servo3.attach(s3);
    servo4.detach();
    servo4.attach(s4);
    //float x_norm = (90+((x_val-662)/(362))*(90));
    servo1.write(180);
    servo2.write(180);
    servo3.write(180);
    servo4.write(180);

    //Serial.print(x_norm);
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm);

    //Serial.print("servo_val: 180");
  }
  //R,R,L,L
  else if(x_val<ThresLow && y_val>ThresHigh ){//Forward left
    servo1.detach();
    servo2.detach();
    servo1.attach(s1);
    servo2.attach(s2);
    
    servo3.detach();
    //servo3.attach(s3);
    servo4.detach();
    //servo4.attach(s4);
    //float x_norm = (90+((x_val-662)/(362))*(90));
    servo1.write(0);
    servo2.write(0);
    //servo3.write(0);
    //servo4.write(0);

    //Serial.print(x_norm);
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm);

    //Serial.print("servo_val: 180");
  }
  else if(x_val>ThresLow && y_val>ThresHigh ){//Forward right
    servo1.detach();
    servo2.detach();
    //servo1.attach(s1);
    //servo2.attach(s2);
    servo3.detach();
    servo3.attach(s3);
    servo4.detach();
    servo4.attach(s4);
    //float x_norm = (90+((x_val-662)/(362))*(90));
    //servo1.write(180);
    //servo2.write(180);
    servo3.write(180);
    servo4.write(180);

    //Serial.print(x_norm);
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm);

    //Serial.print("servo_val: 180");
  }

  else if(x_val<ThresLow && y_val<ThresLow ){//Backward left
    servo1.detach();
    servo2.detach();
    servo1.attach(s1);
    servo2.attach(s2);
    servo3.detach();
    //servo3.attach(s3);
    servo4.detach();
    //servo4.attach(s4);
    //float x_norm = (90+((x_val-662)/(362))*(90));
    servo1.write(180);
    servo2.write(180);
    //servo3.write(180);
    //servo4.write(180);

    //Serial.print(x_norm);
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm);

    //Serial.print("servo_val: 180");
  }

  else if(x_val>ThresHigh && y_val<ThresLow ){//Backward right
    servo1.detach();
    servo2.detach();
    //servo1.attach(s1);
    //servo2.attach(s2);
    servo3.detach();
    servo3.attach(s3);
    servo4.detach();
    servo4.attach(s4);
    //float x_norm = (90+((x_val-662)/(362))*(90));
    //servo1.write(0);
    //servo2.write(0);
    servo3.write(0);
    servo4.write(0);

    //Serial.print(x_norm);
    //Serial.print("X_norm:");
    //Serial.print((int)x_norm);

    //Serial.print("servo_val: 180");
   }
  
}
