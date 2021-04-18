#include <SoftwareSerial.h>

SoftwareSerial espSerial(2,3); // Rx, Tx
String wifiname = "Girinath R";
String pass = "#girinath07";
String recvdata = "GET /channels/1091619/feeds.json?api_key=CMSV35528WSBBGOE&results=2";
String output ;

#define relay 21

void setup()
{
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(9600);
  espSerial.begin(9600);
  espSerial.println("AT+RST");
  espSerial.println("AT+CWMODE = 1");
  espSerial.println("AT+CWJAP = \"" + wifiname + "\", \"" + pass + "\"");
  while(!espSerial.find("OK"))
  {
    //wait for connection
  }
  Serial.println("Connected...");
  delay(500);
}

void loop()
{
  espSerial.println("AT+CIPMUX = 0");
  espSerial.println("AT+CIPSTART = \"TCP\", \"api.thingspeak.com\", 80");
  espSerial.println("AT+CIPSEND = " + String(recvdata.length()));
  espSerial.find(">");
  espSerial.println(recvdata);
  output=""; //clear the string
  while (espSerial.available()>0)
    {
      output = espSerial.readStringUntil('\n');  
    }
  Serial.print("Received: ");
  Serial.println(output);
  char light = output[output.length()-5];
  Serial.println(light);
  Serial.print("The value is...");
  Serial.println(light);
  if (light == '0')
  {
    digitalWrite(relay, LOW);
    Serial.println("Light is turned off!");
  }
  if (light == '1')
  {
    digitalWrite(relay, HIGH);
    Serial.println("Light is turned on!");
  }
  espSerial.println("AT+CIPCLOSE = 0");
  delay(1000);
}
