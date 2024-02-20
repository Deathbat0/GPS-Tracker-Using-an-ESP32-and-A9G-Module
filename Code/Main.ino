//By Daniel Torres (DeathbatO) - 2022
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//Enter character for control option:
//d : to send a message
//c : to receive a message
//a : to read location
//b : to disconnect gps

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

String incomingData;
String gpsData;
String sms;
boolean m1 = false;
boolean m2 = false;



void SendMessage(){
    Serial.println("AT+CMGF=1");       // Sets the GSM Module into text mode
    delay(500);                        // Delay of one second
    Serial.println("AT+CMGS=\"3002297439\"\r");  // Replace your mobile number here
    delay(500);
    sms = "http://maps.google.com/maps?q=" + gpsData;  // Create the SMS location string
    Serial.println(sms);
    delay(100);
    Serial.println((char)26);          // ASCII code of CTRL+Z
    delay(1000);
}


void DisconnectGps(){
    Serial.println("AT+GPS=0");
    SerialBT.println("Disconnect GPS");
    delay(1000);
}

void ReadLocation(){
    if (Serial.available() > 0){
    incomingData = Serial.readString();  // Get the data from the serial port
    gpsData = incomingData.substring(33, 52);
    SerialBT.print(gpsData); 
    delay(100); 
    }
    Serial.println("AT+GPS=1");
    delay(1000);
    Serial.println("AT+LOCATION=2");   // Check location every two seconds
    delay(2000);
}


void setup() {
    Serial.begin(115200);
    SerialBT.begin("GPS_Traker"); //Bluetooth device name

    // Set SMS mode to text mode
    Serial.print("AT+CMGF=1\r");
    delay(100);

    // Set GSM module to TP show the output on serial out
    Serial.print("AT+CNMI=2,2,0,0,0\r");
    delay(100);
}

void loop() {
  
    while(m1){
        ReadLocation();
        delay(200);
        break;
    }
    while(m2){
        SendMessage();
        delay(1000);
        break;
    }

if(SerialBT.available() > 0){

    switch(SerialBT.read()){
        case 'a':
          m1 = true;
        break; 
    
        case 'b':
          m1 = false;
          DisconnectGps();
        break;

        case 'c':
          m2 = true;
        break;
      
        case 'd':
          m2 = false;
        break;
        }       
    }
}