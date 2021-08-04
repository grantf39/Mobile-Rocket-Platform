#include <Wire.h>
//def I2C slave address
#define SLAVE_ADDR 9
#define ANSWERSIZE 11

#include <Servo.h>

Servo servo1;//left and right
Servo servo2;//up and down
const int s1=2;
const int s2=3;

void setup() {
  // put your setup code here, to run once:

  Wire.begin();
  //Wire.onRequest(requestEvent);

  //Wire.onReceive(receiveEvent);


  for(int i=0;i<8;i++){
    pinMode(i+4,OUTPUT);
    digitalWrite(i+4,HIGH);
  }
  servo1.attach(s1);
  servo2.attach(s2);
  Serial.begin(115200);
}
/*
void receiveEvent() {
  Serial.print("Dicks");
  while(Wire.available()) {
    char b = Wire.read();
    Serial.print(b);
    //dick[sizeof(dick)/sizeof(int)+1] += b;
  }
  Serial.println("");
}
*/

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
  char Command[11] = {'s','s','s','0','0','0','0','0','0','0','0'};
  
  //Wire.beginTransmission(SLAVE_ADDR);
  //Wire.write(0);
  //Wire.endTransmission();
  Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);

  int cnt = 0;
  while(Wire.available()) {
    char b = Wire.read();
    Serial.print(b);
    //dick[sizeof(dick)/sizeof(int)+1] += b;
    Command[cnt] = b;
    
    cnt++;
  }
  Serial.println("");


  //action logic
  //servo1.attach(s1);
  if(Command[1]=='l'){//horizontal aim
    //servo1.detach();
    servo1.attach(s1);
    servo1.write(0);
    Serial.println("left");
  }else if(Command[1] == 'r'){
    //servo1.detach();
    servo1.attach(s1);
    servo1.write(180);
    Serial.println("right");
  }else{
    servo1.write(90); 
    servo1.detach();
    //servo1.attach(s1);
    //servo1.write(90);
    Serial.println("middle");
  }

  //servo2.attach(s2);
  if(Command[2]=='u'){//vertical aim
    //servo2.detach();
    servo2.attach(s2);
    servo2.write(180);
  }else if(Command[2] == 'd'){
    //servo2.detach();
    servo2.attach(s2);
    servo2.write(0);
  }else{
    servo2.detach();
  }

  //fire or not fire

  if(Command[0]=='f'){//first check if is set to fire; HIGH is Nc(normally Closed ) and LOW is NO(normally open) because some retard decided that was gonna be the relay logic.
    //pints3-11?
    for(int i=0; i<8; i++){
      if(Command[i+3]=='1'){
        digitalWrite(i+4,LOW);
        //Serial.println(i+4);
      }else{
        digitalWrite(i+4,HIGH);
      }
    }
    
  }else{
    for(int i=0; i<8; i++){ //set all to HIGH for NC so ignites are not used.
      digitalWrite(i+4,HIGH);
    }
  }
  
  
}
