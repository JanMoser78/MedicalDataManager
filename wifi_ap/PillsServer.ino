/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>
#include "WString.h"

#define POS_0 0
#define POS_1 22
#define POS_2 45
#define POS_3 67
#define POS_4 90
#define POS_5 112
#define POS_6 135
#define POS_7 157
#define POS_OPEN 160
#define POS_CLOSE 50


/* Set these to your desired credentials. */
const char *ssid = "Drugs delivering Station";
const char *password = "42isTheAnswer";

ESP8266WebServer server(80);
WiFiUDP udp_server;
WiFiUDP token_server;
Servo myservo;
Servo servo_door;

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  String value = server.arg("step");
  char buf[10] = "";
  value.toCharArray(buf, 10);
  if(*buf >= '0' && *buf <= '7')
  {
    server.send(200, "text/html", "1");
    Serial.print("$AUTH,");
    Serial.print(buf);
    Serial.println("*");
  }
  else
  {
    server.send(200, "text/html", "0");
  }
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  
  myservo.attach(2);
  servo_door.attach(0);
  servo_door.write(POS_CLOSE);
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  if(udp_server.begin(6666))
    Serial.println("UDP server started on port 6666");
  if(token_server.begin(6665))
    Serial.println("UDP server started on port 6665");
}

void loop() {
  server.handleClient();

  while(udp_server.available())
  {
    static int count = 0;
    static int temp, hum, pres;
    char value = udp_server.read();
    if(value == 0xCC && count == 0)
    {
      count++;
    }
    else if(count == 1 && value == 0x04)
    {
      count++;
    }
    else if(count >= 2)
    {
      switch(count){
        case 3:
          temp = ((int) value) << 8;
        break;

        case 2:
          temp += value;
        break;

        case 5:
          hum = ((int) value) << 8;
        break;

        case 4:
          hum += value;
        break;

        case 7:
          pres = ((int) value) << 8;
        break;

        case 6:
          pres += value;
        break;

        default:
          count = 0;
        break;
      }
      count++;
    }
    if(!udp_server.available())
    {
      udp_server.begin(6666);
      Serial.print("$ENV,");
      Serial.print(temp, DEC);
      Serial.print(",");
      Serial.print(hum, DEC);
      Serial.print(",");
      Serial.print(pres, DEC);
      Serial.println("*");
    }
  }

  while(token_server.available())
  {
    unsigned char value = token_server.read();
    if(value != 0)
    {
      Serial.print("$AUTH,");
      Serial.print(value, DEC);
      Serial.println("*");
    }
    if(!token_server.available())
      token_server.begin(6665);
  }
  
  while(Serial.available())
  {
    decode_message(Serial.read());
  }
}

void decode_message(char c)
{
  serv_drug(c-'0');
}

void serv_drug(char drug_nb)
{
  switch(drug_nb){
    case 0:
      myservo.write(POS_0);
    break;
    case 1:
      myservo.write(POS_1);
    break;
    case 2:
      myservo.write(POS_2);
    break;
    case 3:
      myservo.write(POS_3);
    break;
    case 4:
      myservo.write(POS_4);
    break;
    case 5:
      myservo.write(POS_5);
    break;
    case 6:
      myservo.write(POS_6);
    break;
    case 7:
      myservo.write(POS_7);
    break;
  }
  delay(1000);
  servo_door.write(POS_OPEN);
  delay(10000);
  servo_door.write(POS_CLOSE);
}

