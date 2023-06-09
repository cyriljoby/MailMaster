// SimpleRx - the slave or the receiver
//References - Inspiredd by Transciever Demo (https://forum.arduino.cc/t/simple-nrf24l01-2-4ghz-transceiver-demo/405123) (Robin2, 2016)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

struct dataReceived{
  int distance;
  bool state;
}; // this must match dataToSend in the TX
dataReceived data;
bool newData = false;

//===========

void setup() {

    Serial.begin(9600);

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setAutoAck(false);
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

//=============

void loop() {
    getData();
    showData();
}

//==============

void getData() {
    if ( radio.available() ) {
        radio.read( &data, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    if (newData == true) {
      if (data.state==true){
        Serial.println("open");
      }
        Serial.println("MailSpace Left: ");
        Serial.print(data.distance);
        Serial.println("cm");
        newData = false;
    }
}
