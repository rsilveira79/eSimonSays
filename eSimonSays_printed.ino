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
int AvgValueGreen=0;          // Average value for variable sensor Green
int CountKeyGreen=0;          // Counter for variable sensor Green

int MaxValueYellow=0;         // Maximum value for variable sensor Yellow
int AvgValueYellow=0;          // Average value for variable sensor Green
int CountKeyYellow=0;         // Counter for variable sensor Yellow

int MaxValueRed=0;            // Maximum value for variable sensor Red
int AvgValueRed=0;          // Average value for variable sensor Green
int CountKeyRed=0;            // Counter for variable sensor Red

int MaxValueBlue=0;           // Maximum value for variable sensor Blue
int AvgValueBlue=0;          // Average value for variable sensor Green
int CountKeyBlue=0;           // Counter for variable sensor Blue

byte LedBlinkStatus = 0;       // variable to store LEDs blink status ------- Green = bit 0, Yellow = bit 1, Red = bit 2, Blue = bit 3
byte flagLedGreen = 1;      
byte flagLedYellow = 2;
byte flagLedRed = 3;
byte flagLedBlue = 4;
byte flagLedAll = 5;

byte KeyStatus = 0;           // variable to store Key status ------- Green = bit 0, Yellow = bit 1, Red = bit 2, Blue = bit 3
byte flagKeyGreen = 1;      
byte flagKeyYellow = 2;
byte flagKeyRed = 3;
byte flagKeyBlue = 4;
int counterLED = 0;
int counterLEDcircle = 0;
int reportKeyscounter = 0;


const int numReadings = 20;

int readingsGreen[numReadings];      // the readings from the analog input
int readIndexGreen = 0;              // the index of the current reading
int totalGreen = 0;                  // the running total
int averageGreen = 0;                // the average

int readingsYellow[numReadings];      // the readings from the analog input
int readIndexYellow = 0;              // the index of the current reading
int totalYellow = 0;                  // the running total
int averageYellow = 0;                // the average

int readingsRed[numReadings];      // the readings from the analog input
int readIndexRed = 0;              // the index of the current reading
int totalRed = 0;                  // the running total
int averageRed = 0;                // the average

int readingsBlue[numReadings];      // the readings from the analog input
int readIndexBlue = 0;              // the index of the current reading
int totalBlue = 0;                  // the running total
int averageBlue = 0;                // the average

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
  AvgValueGreen=moving_average_green();
  if (sensorValueGreen > MaxValueGreen) MaxValueGreen=sensorValueGreen;
  if (AvgValueGreen < 0.7*MaxValueGreen) MaxValueGreen=AvgValueGreen;       // lower max value to the average

  sensorValueYellow = analogRead(sensorPinYellow);
  AvgValueYellow=moving_average_yellow();
  if (sensorValueYellow > MaxValueYellow) MaxValueYellow=sensorValueYellow;
  if (AvgValueYellow < 0.7*MaxValueYellow) MaxValueYellow=AvgValueYellow;       // lower max value to the average

  sensorValueRed = analogRead(sensorPinRed);
  AvgValueRed=moving_average_red();
  if (sensorValueRed > MaxValueRed) MaxValueRed=sensorValueRed;
  if (AvgValueRed < 0.7*MaxValueRed) MaxValueRed=AvgValueRed;       // lower max value to the average

  sensorValueBlue = analogRead(sensorPinBlue);
  AvgValueBlue=moving_average_blue();
  if (sensorValueBlue > MaxValueBlue) MaxValueBlue=sensorValueBlue;
  if (AvgValueBlue < 0.7*MaxValueBlue) MaxValueBlue=AvgValueBlue;       // lower max value to the average

  // report keys counter -- check sensor keys status
  reportKeyscounter=reportKeyscounter+1;
  if (reportKeyscounter==5)
  {
    reportKeys();
    reportKeyscounter=0;
    }

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

/*
  if (bitRead(LedBlinkStatus,flagLedAll))
  {
    counterLED=counterLED+1;
    digitalWrite(ledGreen, !digitalRead(ledGreen));           // Toggle green led on Timer Interrupt
    digitalWrite(ledYellow, !digitalRead(ledYellow));         // Toggle yellow led on Timer Interrupt
    digitalWrite(ledRed, !digitalRead(ledRed));               // Toggle red led on Timer Interrupt
    digitalWrite(ledBlue, !digitalRead(ledBlue));             // Toggle blue led on Timer Interrupt
    if (counterLED==10)
    {
      bitClear(LedBlinkStatus,flagLedAll);
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledYellow, LOW);
      digitalWrite(ledRed, LOW);
      digitalWrite(ledBlue, LOW);
    }
   }
 */
  
  if(counterLEDcircle==4)
  {

    bitClear(LedBlinkStatus,flagLedGreen);
    bitClear(LedBlinkStatus,flagLedYellow);
    bitClear(LedBlinkStatus,flagLedRed);
    bitClear(LedBlinkStatus,flagLedBlue);
    counterLEDcircle=0;
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledBlue, LOW);
    bitSet(LedBlinkStatus,flagLedAll);
    }
  
  if (bitRead(LedBlinkStatus,flagLedGreen))
    {
      digitalWrite(ledGreen, !digitalRead(ledGreen));       // Toggle green led on Timer Interrupt
      counterLED=counterLED+1;
      if (counterLED==10)
        {
          bitSet(LedBlinkStatus,flagLedYellow);
          bitClear(LedBlinkStatus,flagLedGreen);
          digitalWrite(ledGreen, LOW);
          counterLED=0;
          counterLEDcircle=counterLEDcircle+1;
        }
    }

  if (bitRead(LedBlinkStatus,flagLedYellow))
    {
      digitalWrite(ledYellow, !digitalRead(ledYellow));       // Toggle green led on Timer Interrupt
      counterLED=counterLED+1;
      if (counterLED==10)
        {
          bitSet(LedBlinkStatus,flagLedRed);
          bitClear(LedBlinkStatus,flagLedYellow);
          digitalWrite(ledYellow, LOW);
          counterLED=0;
          counterLEDcircle=counterLEDcircle+1;
        }
    }

    if (bitRead(LedBlinkStatus,flagLedRed))
    {
      digitalWrite(ledRed, !digitalRead(ledRed));       // Toggle green led on Timer Interrupt
      counterLED=counterLED+1;
      if (counterLED==10)
        {
          bitSet(LedBlinkStatus,flagLedBlue);
          bitClear(LedBlinkStatus,flagLedRed);
          digitalWrite(ledRed, LOW);
          counterLED=0;
          counterLEDcircle=counterLEDcircle+1;
        }
    }

  if (bitRead(LedBlinkStatus,flagLedBlue))
    {
      digitalWrite(ledBlue, !digitalRead(ledBlue));       // Toggle green led on Timer Interrupt
      counterLED=counterLED+1;
      if (counterLED==10)
        {
          bitSet(LedBlinkStatus,flagLedGreen);
          bitClear(LedBlinkStatus,flagLedBlue);
          digitalWrite(ledBlue, LOW);
          counterLED=0;
          counterLEDcircle=counterLEDcircle+1;
        }
    }
}

int moving_average_green()
{
    totalGreen = totalGreen - readingsGreen[readIndexGreen];
    // read from the sensor:
    readingsGreen[readIndexGreen] = analogRead(sensorPinGreen);
    // add the reading to the total:
    totalGreen = totalGreen + readingsGreen[readIndexGreen];
    // advance to the next position in the array:
    readIndexGreen = readIndexGreen + 1;

    // if we're at the end of the array...
    if (readIndexGreen >= numReadings) {
    // ...wrap around to the beginning:
    readIndexGreen = 0;
    }
    // calculate the average:
    averageGreen = totalGreen / numReadings;
    return averageGreen;
}


int moving_average_yellow()
{
    totalYellow = totalYellow - readingsYellow[readIndexYellow];
    // read from the sensor:
    readingsYellow[readIndexYellow] = analogRead(sensorPinYellow);
    // add the reading to the total:
    totalYellow = totalYellow + readingsYellow[readIndexYellow];
    // advance to the next position in the array:
    readIndexYellow= readIndexYellow + 1;

    // if we're at the end of the array...
    if (readIndexYellow >= numReadings) {
    // ...wrap around to the beginning:
    readIndexYellow = 0;
    }
    // calculate the average:
    averageYellow = totalYellow / numReadings;
    return averageYellow;
}


int moving_average_red()
{
    totalRed = totalRed - readingsRed[readIndexRed];
    // read from the sensor:
    readingsRed[readIndexRed] = analogRead(sensorPinRed);
    // add the reading to the total:
    totalRed = totalRed + readingsRed[readIndexRed];
    // advance to the next position in the array:
    readIndexRed = readIndexRed + 1;

    // if we're at the end of the array...
    if (readIndexRed >= numReadings) {
    // ...wrap around to the beginning:
    readIndexRed = 0;
    }
    // calculate the average:
    averageRed = totalRed/ numReadings;
    return averageRed;
}


int moving_average_blue()
{
    totalBlue = totalBlue - readingsBlue[readIndexBlue];
    // read from the sensor:
    readingsBlue[readIndexBlue] = analogRead(sensorPinBlue);
    // add the reading to the total:
    totalBlue = totalBlue + readingsBlue[readIndexBlue];
    // advance to the next position in the array:
    readIndexBlue = readIndexBlue + 1;

    // if we're at the end of the array...
    if (readIndexBlue >= numReadings) {
    // ...wrap around to the beginning:
    readIndexBlue = 0;
    }
    // calculate the average:
    averageBlue = totalBlue / numReadings;
    return averageBlue;
}


void reportKeys()
{
  Serial.print("GREEN __ Avg: ");
  Serial.print(AvgValueGreen);
  Serial.print(" __ Current: ");
  Serial.println(sensorValueGreen);

  Serial.print("YELLOW __ Avg: ");
  Serial.print(AvgValueYellow);
  Serial.print(" __ Current: ");
  Serial.println(sensorValueYellow);

  Serial.print("RED __ Avg: ");
  Serial.print(AvgValueRed);
  Serial.print(" __ Current: ");
  Serial.println(sensorValueRed);

  Serial.print("BLUE __ Avg: ");
  Serial.print(AvgValueBlue);
  Serial.print(" __ Current: ");
  Serial.println(sensorValueBlue);
  Serial.println();

  }
