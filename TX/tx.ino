#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint64_t pipe = 0xE8E8F0F0E1LL;
RF24 radio(9,10);// vмодуль на пинах 9 и 10

byte pinLeftJoystickX = 14;
byte pinRightJoystickX = 15;
byte pinLeftJoystickSwitch = 4;
byte pinRightJoystickSwitch = 3;

boolean leftJoystickSwitch = 0;
boolean rightJoystickSwitch = 0;

boolean leftJoystickSwitchState;
boolean rightJoystickSwitchState;

int transmitData[4]; // массив, хранящий передаваемые данные
int latestData[4]; // массив, хранящий последние переданные данные
boolean flag = 0;
boolean leftJoystickSwitchFlag = 0;
boolean rightJoystickSwitchFlag = 0;

unsigned long lastPressLeftJoystickSwitch;
unsigned long lastPressRightJoystickSwitch;

void setup() {
  pinMode(pinLeftJoystickSwitch, INPUT_PULLUP);
  pinMode(pinRightJoystickSwitch, INPUT_PULLUP);
 
  radio.begin(); //активировать модуль

  radio.openWritingPipe(pipe);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(9);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening(); //не слушаем радиоэфир, мы передатчик
}

void loop() {
  leftJoystickSwitchState = !digitalRead(pinLeftJoystickSwitch);
  rightJoystickSwitchState = !digitalRead(pinRightJoystickSwitch);

  if(leftJoystickSwitchState == 1 && leftJoystickSwitchFlag == 0 && millis() - lastPressLeftJoystickSwitch > 50){
    leftJoystickSwitchFlag = 1;
    leftJoystickSwitch = !leftJoystickSwitch;
    lastPressLeftJoystickSwitch = millis();
  }
  if(leftJoystickSwitchState == 0 && leftJoystickSwitchFlag == 1){
    leftJoystickSwitchFlag = 0;
  }
  if(rightJoystickSwitchState == 1 && rightJoystickSwitchFlag == 0 && millis() - lastPressRightJoystickSwitch > 50){
    rightJoystickSwitchFlag = 1;
    rightJoystickSwitch = !rightJoystickSwitch;
    lastPressRightJoystickSwitch = millis();
  }
  if(rightJoystickSwitchState == 0 && rightJoystickSwitchFlag == 1){
    rightJoystickSwitchFlag = 0;
  }
  
  transmitData[0] = analogRead(pinLeftJoystickX);
  transmitData[1] = analogRead(pinRightJoystickX);
  transmitData[2] = leftJoystickSwitch;
  transmitData[3] = rightJoystickSwitch;

  for (int i = 0; i < 4; i++) { // в цикле от 0 до числа каналов
    if (transmitData[i] != latestData[i]) { // если есть изменения в transmit_data
      flag = 1; // поднять флаг отправки по радио
      latestData[i] = transmitData[i]; // запомнить последнее изменение
    }
  }

  if (flag == 1) {
    radio.powerUp(); // включить передатчик
    radio.write(&transmitData, sizeof(transmitData)); // отправить по радио
    flag = 0; //опустить флаг
    radio.powerDown(); // выключить передатчик
  }
}
