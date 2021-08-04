#include <SPI.h>
#include<nRF24L01.h>
#include <RF24.h>


RF24 radio(9,10); // CNS, CE <-WARNING MAY BE REVERSE!!!! CE CNS INSTEAD

const byte address[6] = "00001";
const int L_X_pin = 7;//X to analog 7
const int L_Y_pin =6; //Y to analog 6

//joystick right
const int R_X_pin =2 ;//X to analog 2
const int R_Y_pin =1; //Y to analog 1

//fire buttons
const int W1 = 2;
const int W2 = 3;
const int W3 = 4;
const int W4 = 5;
const int W5 = 6;
const int W6 = 7;
const int W7 = 8;
const int W8 = A4; //NOTE: this is analog pin because ran out of digital ones


const int SFpin = A5; //Safety pin
const int rLED = A3;//safety light???
int shootVal=0;

void setup() {

  //pinMode(SW_pin, INPUT);
  //digitalWrite(SW_pin, HIGH);
  int ThresLow = 362;
  int ThresHigh = 662;
  int shootVal = 0;

   pinMode(W1, INPUT);
   pinMode(W2, INPUT);
   pinMode(W3, INPUT);
   pinMode(W4, INPUT);
   pinMode(W5, INPUT);
   pinMode(W6, INPUT);
   pinMode(W7, INPUT);
   pinMode(W8, INPUT);

   pinMode(SFpin, INPUT);
   pinMode(rLED, OUTPUT);
   
 
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
  
  Serial.begin(115200);
}

void loop() {
  /*
  Serial.print("X-axis:");
  Serial.print(analogRead(L_X_pin));
  Serial.print("\n");
  Serial.print("Y-axis:");
  Serial.print(analogRead(L_Y_pin));
  Serial.print("\n\n");
  float x_val = analogRead(L_X_pin);
  Serial.print("X_val:");
  Serial.print(x_val);
  */
  if(digitalRead(SFpin)==HIGH){
    Serial.print("???");
    shootVal=1;
  }else{
    shootVal=0;
  }
  
  if(shootVal==1){
    digitalWrite(rLED, HIGH);
  }else{
    digitalWrite(rLED, LOW);
  }
  
  Serial.print("Button sf:");
  Serial.println(digitalRead(SFpin));
  Serial.println(shootVal);
  

  
  // put your main code here, to run repeatedly:
  //const char text[] = "Hello World";
  int test[13] = {digitalRead(SFpin),analogRead(L_X_pin), analogRead(L_Y_pin), analogRead(R_X_pin), analogRead(R_Y_pin),
  digitalRead(W1), digitalRead(W2), digitalRead(W3), digitalRead(W4),digitalRead(W5), digitalRead(W6), digitalRead(W7), digitalRead(W8) };

   
  
//  for(int i=0; i<(sizeof(test)/sizeof(int)); i++){
//    Serial.print(test[i]);
//    Serial.print("-");
//  }
//  Serial.print('\n');

  //Serial.println(digitalRead(W1));
  
  //Serial.println(sizeof(test)/sizeof(int));
  /*
  if(test[0]==1){
    Serial.print("OK");
  }
  */
  radio.write(&test, sizeof(test));
  
}
