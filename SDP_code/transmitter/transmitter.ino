/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#define sensorPin2 A5 // for capactive moisure sensor
const int index = 2; // id of moisture sensor, change for each arduino deployed 0-4


RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.stopListening();
}

void loop() {
  int moisture_val = analogRead(sensorPin2);
  char text[32]; // set text buffer length
  sprintf(text, "%d,%d",index, moisture_val); // Append the plant value to the text
  radio.write(text, strlen(text)); // Send the modified text via radio
  Serial.print("message sent: ");
  Serial.println(text);
  delay(6000);
}