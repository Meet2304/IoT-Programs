#include<SoftwareSerial.h>
#define Tx 10
#define Rx 11

SoftwareSerial sim(Tx, Rx);

int timeout;
String bufr;
String number = "+91";

void setup() {
  Serial.begin(9600);
  bufr.reserve(50);
  Serial.println("System Started");
  sim.begin(9600);
  delay(1000);
  Serial.println("Type s to send, c to call and r to recieve");  
}

void loop() {
  if(Serial.available()>0)
  {
    switch(Serial.read())
    {
      case 's':
        sendMessage();
        break;
      case 'c':
        recieveMessage();
        break;
      case 'r':
        callNumber();
        break;
    }
  }
  if(sim.available()>0)
  {
    Serial.write(sim.read());
  }
}

void sendMessage()
{
  Serial.println("Sending Message: ");
  sim.println("AT+CMGF=1"); // Set the GSm module in text mode
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(200);
  String sms = "HOLA!..Test";
  sim.println((char)26);
  delay(200);
  bufr=readSerial();
}

void recieveMessage()
{
  Serial.println("SIM800L read an SMS");
  sim.println("AT+CMGF=1");
  delay(200);
  sim.println("AT+CNMI=1,2,0,0,0"); // AT command to read live message
  delay(200);
  Serial.write("Unread Message Done");
}

String readSerial()
{
  timeout=0;
  while(!sim.available() && timeout < 12000)
  {
    delay(13);
    timeout++;
  }
  if (sim.available())
  {
    return sim.readString();
  }
}

void callNumber()
{
  sim.print(F("ATD"));
  sim.print(number);
  sim.print(F(";\r\n"));
  bufr=readSerial();
  Serial.println(bufr);
}

