#include <ESP8266WiFi.h>
#include <Wire.h>
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11

#define dht_dpin D2
#include "Ubidots.h"
DHT dht(dht_dpin, DHTTYPE);

//GAS sensor
const int gasPin = A0;

//Ultrasonic sensor
const int trigPin = D5;
const int echoPin = D6;
//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
float UltraSensor();
long duration;
float distanceCm;

//buzzer
const int buzzer = D1;

//WiFiClient client;

const char* UBIDOTS_TOKEN = "BBFF-Nlb8QukD6Ncw470MaH2ifHBrTpy8oO";
const char* ssid     = "MAJOR PROJECT"; // Your ssid
const char* password = "12345678"; // Your Password
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);


void setup()
{

  Serial.begin(115200); // Starts the serial communication

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  pinMode(buzzer, OUTPUT);

  pinMode(gasPin, INPUT);
  //Serial.begin(9600); //Initialize serial port - 9600 bps
  dht.begin();
  Serial.println("Humidity and temperature\n\n");
  delay(700);

  ubidots.wifiConnect(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi is connected");
}

void loop()
{

  float a = UltraSensor();
  Serial.println(a);

  float A = (a*4.88);

  if (A <= 25) {

    //buzzer
    tone(buzzer, 100); // Send 1KHz sound signal...
    delay(3000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);
    tone(buzzer, 100); // Send 1KHz sound signal...
    delay(3000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);
    tone(buzzer, 100); // Send 1KHz sound signal...
    delay(3000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...
    delay(1000);
  }

  int B = gasSensor();
  Serial.println(B);

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  ubidots.add("Percentage", A);// Change for your variable name
  ubidots.add("Gas", B);
  ubidots.add("Humidity", h);
  ubidots.add("Temperature", t);
  //ubidots.add("Motor", output_value);


  bool bufferSent = false;
  bufferSent = ubidots.send(); // Will send data to a device label that matches the device Id
  if (bufferSent) {
    Serial.println("Values sent by the device");
  }
  delay(3000);


}


int gasSensor() {

  Serial.println(analogRead(gasPin));
  int G = analogRead(gasPin); 
  return G;
}

float UltraSensor() {

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY / 2;

  // Prints the distance on the Serial Monitor
  //Serial.print("Distance (cm): ");
  //Serial.println(distanceCm);
  delay(1000);
  return distanceCm;
}
