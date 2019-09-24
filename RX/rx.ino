#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(2, 9);

int data[4];
int reserve=0;
unsigned long motorOnTime;
byte count = 0;

void setup() {
  delay(50);
  radio.begin();
  radio.setChannel(9);
  radio.setDataRate(RF24_250KBPS);        // Установка минимальной скорости;
  radio.setPALevel(RF24_PA_HIGH);         // Установка максимальной мощности;
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
}

void loop()  {
  if ( radio.available() ){
    bool done = false;
    while (!done){
      done = radio.read(data, sizeof(data));    
      if(data[0]>450 && data[0]<598){
        analogWrite(5, 0);
        analogWrite(6, 0);
        digitalWrite(17,LOW);
        digitalWrite(10,LOW);
        digitalWrite(18,LOW);
        digitalWrite(19,LOW);
      }
      if(data[0]>598)
      {   
        analogWrite(5,255);
        analogWrite(6,255);
        digitalWrite(17,LOW);
        digitalWrite(10,HIGH);
        digitalWrite(18,HIGH);
        digitalWrite(19,LOW);
      }
      if(data[0]< 450)
      {   
        analogWrite(5,255);
        analogWrite(6,255);
        digitalWrite(17,HIGH);
        digitalWrite(10,LOW);
        digitalWrite(18,LOW);
        digitalWrite(19,HIGH);
      }  

      if(data[1]>450 && data[1]<598){
        digitalWrite(3, LOW);
        digitalWrite(8, LOW);
        digitalWrite(4,LOW);
        digitalWrite(7,LOW);
        digitalWrite(14,LOW);
        digitalWrite(15,LOW);
      }
      if(data[1]>598)
      {   
        digitalWrite(3, HIGH);
        digitalWrite(8,HIGH);
        digitalWrite(4,LOW);
        digitalWrite(7,HIGH);
        digitalWrite(14,HIGH);
        digitalWrite(15,LOW);
      }
      if(data[1]< 450)
      {   
        digitalWrite(3,HIGH);
        digitalWrite(8,HIGH);
        digitalWrite(4,HIGH);
        digitalWrite(7,LOW);
        digitalWrite(14,LOW);
        digitalWrite(15,HIGH);
      }  
      if(data[2] == 1){ 
         if(count < 1){
            digitalWrite(16, HIGH);
            delay(500);
            digitalWrite(16, LOW);
            count++;
         }
      }
      else{
        if(count >= 1){
            digitalWrite(16, HIGH);
            delay(500);
            digitalWrite(16, LOW);
            count--;
         }
      }
      reserve = !data[3];
    }
  }
}
