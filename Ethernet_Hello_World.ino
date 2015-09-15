/*************************************************************** 
 *  File: Ethernet_Hello_World
 *  Description: This project sets up a webserver using the EtherCard
 *  library and the ENC28J60 breakout board. 
 *  Author: Mr. Reeses
 *  Reference: 
 *    http://www.tweaking4all.com/hardware/arduino/arduino-enc28j60-ethernet/
 *  
 ***************************************************************/

#include <EtherCard.h>

/***************************************************************
 * Be sure to select an IP address that is not already being used.
 * You can arbitrarily pick the MAC address, but be careful so that
 * it doesn't conflict with any other device. You can use to this 
 * default MAC address as long as it's contained within your LAN. 
 ***************************************************************/

// Ethernet interface MAC address. Must be unique on the LAN
static byte mymac[] = {0x74,0x69,0x69,0x2D,0x30,0x31};
static byte gwip[] = { 192,168,1,1 };
static byte myip[] = {192,168,1,203}; 

byte Ethernet::buffer[500];

/***************************************************************
 * This variable stores the HTML code that sets up the home page 
 ***************************************************************/
const char page[] PROGMEM =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
  "<head><title>"
    "Hello World!"
  "</title></head>"
  "<body>"
    "<h3>Hello World! This is your Arduino speaking on behalf of Mr. Reeses!</h3>"
    "<A HREF=' ?cad=off'>Turn off</>"
    "<A HREF=' ?cad=on'>Turn on</>"
  "</body>"
"</html>";

/***************************************************************
 * This function is used to setup the pins for the program. 
 ***************************************************************/
void setup() 
{
  Serial.begin(57600);
  Serial.println("\n[Hello World]");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
  ether.staticSetup(myip, gwip);

  // Prints the IP in the serial port
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);
}

/***************************************************************
 * This is the main function used in the Arduino. For this 
 * function, it passes the variable that sets up the home page to 
 * the functions that help set it up.
 ***************************************************************/
void loop() 
{
  // Wait for an incoming TCP packet, but ignore its contents
  if (ether.packetLoop(ether.packetReceive())) {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
  }
}
