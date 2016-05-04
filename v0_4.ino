/*
Web client+
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
created 18 Dec 2009 by David A. Mellis
modified 9 Apr 2012 by Tom Igoe, based on work by Adrian McEwen
modified 4 May 2019 by Alexander Kovalenko
 */

#include <SPI.h>
#include <TimerOne.h>
#include <UIPEthernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x13, 0x13, 0x13, 0x13, 0x13, 0x13 };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)
//char server[] = "www.timeapi.org";
char server[] = "edis.pp.ua";
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192,168,200,89);
IPAddress gw(192,168,200,223);
IPAddress dnsr(8,8,8,8);

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
int i=0;
EthernetClient client;
void setup() {
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
readnet();
Timer1.initialize(); // 1 секунда по умолчанию - то, что надо
Timer1.attachInterrupt(tmr);
}
void readnet()
{
//Serial.print("static..."); 
Ethernet.begin(mac,ip,dnsr,gw); 
//Serial.print("static OK"); 
delay(1000);
 Serial.println("...");
//  Serial.println("connect to server");
    if (client.connect(server,80)) {
  //  Serial.println("connected");
    // Make a HTTP request:
    //   Serial.println("---------------");
    // Создаем HTTP-запрос
    client.println("GET /project+/button/ardu.php HTTP/1.1");
    client.println("Host: edis.pp.ua");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  } 
i=0;
//  Timer1.restart();
}
void tmr()
{ 
  ++i;
Serial.print(".");
if (i==30) {i=0; 
//    if (!client.connected()) {
//    client.stop();
//readnet();
//}
//Serial.println();
}
}



void loop()
{
      char c;
  if (client.available()) {
    c = client.read();
//    Serial.print(c);
}
  if(c=='$'){c = client.read(); if(c=='1') Serial.println("OK+");else Serial.println("OK-");}
  //  if (!client.connected()) {client.stop();}
  if (i>=25) readnet();
}
