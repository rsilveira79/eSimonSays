#include <Wire.h>
#include "TimerOne.h"

int sensorPinGreen = A0;        // select the input pin for the potentiometer
int sensorPinYellow = A1;       // select the input pin for the potentiometer
int sensorPinRed = A2;        // select the input pin for the potentiometer
int sensorPinBlue = A3;       // select the input pin for the potentiometer

int ledDebug = 13;          // select the pin for the LED
int ledGreen = 10;          // select the pin for the LED
int ledYellow = 9;         // select the pin for the LED
int ledRed = 8;        // select the pin for the LED
int ledBlue = 7;            // select the pin for the LED

int sensorValueGreen = 0;         // variable to store the value coming from the sensor Green
int sensorValueYellow = 0;        // variable to store the value coming from the sensor Yellow
int sensorValueRed = 0;         // variable to store the value coming from the sensor Red
int sensorValueBlue = 0;      // variable to store the value coming from the sensor Blue

int MaxValueGreen=0;          // Maximum value for variable sensor Green
int CountKeyGreen=0;          // Counter for variable sensor Green
int MaxValueYellow=0;         // Maximum value for variable sensor Yellow
int CountKeyYellow=0;         // Counter for variable sensor Yellow
int MaxValueRed=0;            // Maximum value for variable sensor Red
int CountKeyRed=0;            // Counter for variable sensor Red
int MaxValueBlue=0;           // Maximum value for variable sensor Blue
int CountKeyBlue=0;           // Counter for variable sensor Blue

byte LedBlinkStatus = 0;       // variable to store LEDs blink status ------- Green = bit 0, Yellow = bit 1, Red = bit 2, Blue = bit 3
byte flagLedGreen = 1;      
byte flagLedYellow = 2;
byte flagLedRed = 3;
byte flagLedBlue = 4;

byte KeyStatus = 0;           // variable to store Key status ------- Green = bit 0, Yellow = bit 1, Red = bit 2, Blue = bit 3
byte flagKeyGreen = 1;      
byte flagKeyYellow = 2;
byte flagKeyRed = 3;
byte flagKeyBlue = 4;

int oldValueG = 0;
int counterLED = 0;

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  
  // Initialize LED states
  pinMode(ledDebug, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledRed, OUTPUT);

// Initialize Timer Interrrupt
  Timer1.initialize(500000);                 // initialize timer1, and set a 1 second period
  Timer1.attachInterrupt(timer_callback);     // attaches callback() as a timer overflow interrupt
}

void loop() {

}


void timer_callback()
{

  sensorValueGreen = analogRead(sensorPinGreen);
  if (sensorValueGreen > MaxValueGreen) MaxValueGreen=sensorValueGreen;

  sensorValueYellow = analogRead(sensorPinYellow);
  if (sensorValueYellow > MaxValueYellow) MaxValueYellow=sensorValueYellow;

  sensorValueRed = analogRead(sensorPinRed);
  if (sensorValueRed > MaxValueRed) MaxValueRed=sensorValueRed;

  sensorValueBlue = analogRead(sensorPinBlue);
  if (sensorValueBlue > MaxValueBlue) MaxValueBlue=sensorValueBlue;
  
  Serial.print("Sensor Green: ");
  Serial.println(sensorValueGreen);
  Serial.print("Max Green: ");
  Serial.println(MaxValueGreen);
  Serial.println();
  // identifying keypress on green sensor
  if (sensorValueGreen < MaxValueGreen/2)
  {
    CountKeyGreen=CountKeyGreen+1;
    if (CountKeyGreen == 3)
    {
      bitSet(KeyStatus,flagKeyGreen);
      bitSet(LedBlinkStatus,flagLedGreen);
      CountKeyGreen=0;
      }
  }

  // identifying keypress on yellow sensor
  if (sensorValueYellow < MaxValueYellow/2)
  {
    CountKeyYellow=CountKeyYellow+1;
    if (CountKeyYellow == 3)
    {
      bitSet(KeyStatus,flagKeyYellow);
      bitSet(LedBlinkStatus,flagLedYellow);
      CountKeyYellow=0;
      }
  }

  // identifying keypress on red sensor
  if (sensorValueRed < MaxValueRed/2)
  {
    CountKeyRed=CountKeyRed+1;
    if (CountKeyRed == 3)
    {
      bitSet(KeyStatus,flagKeyRed);
      bitSet(LedBlinkStatus,flagLedRed);
      CountKeyRed=0;
      }
  }

  // identifying keypress on blue sensor
  if (sensorValueBlue < MaxValueBlue/2)
  {
    CountKeyBlue=CountKeyBlue+1;
    if (CountKeyBlue == 3)
    {
      bitSet(KeyStatus,flagKeyBlue);
      bitSet(LedBlinkStatus,flagLedBlue);
      CountKeyBlue  =0;
      }
  }
  
  if (LedBlinkStatus==0)
    {
     // bitSet(LedBlinkStatus,flagLedGreen);
    }
  
  if (bitRead(LedBlinkStatus,flagLedGreen))
    {
      digitalWrite(ledGreen, !digitalRead(ledGreen));       // Toggle green led on Timer Interrupt
      counterLED=counterLED+1;
      if (counterLED==10)
        {
      //    bitSet(LedBlinkStatus,flagLedYellow);
          bitClear(LedBlinkStatus,flagLedGreen);
          digitalWrite(ledGreen, LOW);
          counterLED=0;
        }
    }

  if (bitRead(LedBlinkStatus,flagLedYellow))
    {
      digitalWrite(ledYellow, !digitalRead(ledYellow));       // Toggle green led on Timer Interrupt
      counterLED=counterLED+1;
      if (counterLED==10)
        {
       //   bitSet(LedBlinkStatus,flagLedRed);
          bitClear(LedBlinkStatus,flagLedYellow);
          digitalWrite(ledYellow, LOW);
          counterLED=0;
        }
    }

    if (bitRead(LedBlinkStatus,flagLedRed))
    {
      digitalWrite(ledRed, !digitalRead(ledRed));       // Toggle green led on Timer Interrupt
      counterLED=counterLED+1;
      if (counterLED==10)
        {
       //   bitSet(LedBlinkStatus,flagLedBlue);
          bitClear(LedBlinkStatus,flagLedRed);
          digitalWrite(ledRed, LOW);
          counterLED=0;
        }
    }

  if (bitRead(LedBlinkStatus,flagLedBlue))
    {
      digitalWrite(ledBlue, !digitalRead(ledBlue));       // Toggle green led on Timer Interrupt
      counterLED=counterLED+1;
      if (counterLED==10)
        {
   //       bitSet(LedBlinkStatus,flagLedGreen);
          bitClear(LedBlinkStatus,flagLedBlue);
          digitalWrite(ledBlue, LOW);
          counterLED=0;
        }
    }
}


