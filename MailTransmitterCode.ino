//Imporrt all necessary libraries
#include <ezButton.h>
#include <SPI.h>
#include <nRF24L01.h>        /* to handle this particular modem driver*/
#include <RF24.h>            /* the library which helps us to control the radio modem*/


ezButton limitSwitch(2);  // create ezButton object that attach to pin 2tr;
#define CE_PIN   8
#define CSN_PIN 9

const byte slaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

struct MailBoxData {
 int distance;
 bool state;
};

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second

bool door = false;
const int trigPin = 6;
const int echoPin = 7;
// defines variables
long duration;
int distance;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  limitSwitch.setDebounceTime(50); // set debounce time to 50 milliseconds
  pinMode(3,OUTPUT);
  Serial.println("SimpleTx Starting");
  radio.begin();
  radio.setAutoAck(false); 
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3,5); // delay, count
  radio.openWritingPipe(slaveAddress);
}
void loop() {
  // Clears the trigPin

  
  limitSwitch.loop(); // MUST call the loop() function first
  int state = limitSwitch.getState();

  if(state == HIGH){
    door=true;
    digitalWrite(3,HIGH);}
  else{
    door=false;
    // Serial.println("The limit switch: TOUCHED");
    digitalWrite(3,LOW);}

  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.println(distance);

  currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
        prevMillis = millis();
    }
  delay(1000);
}


void send() {
    bool rslt;
    MailBoxData mystruct;
    mystruct.distance=distance;
    mystruct.state=door;
    rslt=radio.write( &mystruct, sizeof(MailBoxData) );


    Serial.print("Data Sent ");
    Serial.print(mystruct.distance);
    Serial.print(mystruct.state);
    if (rslt) {
        Serial.println("  Acknowledge received");
        
    }
    else {
        Serial.println("  Tx failed");
    }
}