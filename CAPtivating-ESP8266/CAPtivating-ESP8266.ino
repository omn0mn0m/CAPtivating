/*******************************************************************
    Using a 32 * 32 RGB Matrix
 *                                                                 *
    Based on the great work of Brian Lough
    Written by Leonid Rusnac (@lrusnac github)
    Modified by Nam Tran
    https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
 *******************************************************************/

// ----------------------------
// Standard Libraries - Already Installed if you have ESP8266 set up
// ----------------------------

#include<ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Ticker.h>

#include <PxMatrix.h>
// The library for controlling the LED Matrix
// Needs to be manually downloaded and installed
// https://github.com/2dom/PxMatrix

#include "pics.h"

Ticker display_ticker;

// LED MATRIX size
#define MATRIX_LENGTH 32
#define MATRIX_WIDTH  32

// Pins for LED MATRIX
#define P_LAT 16
#define P_A 5
#define P_B 4
#define P_C 15
#define P_OE 2
#define P_D 12
#define P_E 0

PxMATRIX display(MATRIX_LENGTH, MATRIX_WIDTH,P_LAT, P_OE,P_A,P_B,P_C,P_D);

// WiFi Variables
const char* ssid = "NamGradCap";
const char* password = "guccigangguccigang";
const char* value = "";

ESP8266WebServer server(80);

// ISR for display refresh
void display_updater() {
  display.display(70);
}

void create_routes() {
  server.on("/pic", handlePic);
  server.on("/off", handleOff);
  server.on("/print", handlePrint);
}

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 1, 1);

  create_routes();
  server.begin();
  
  display.begin(16);
  display.clearDisplay();
  
  display_ticker.attach(0.002, display_updater);
  yield(); 
}

void drawFrame(uint16_t *frame) {
  display.clearDisplay();
  int imageHeight = 32;
  int imageWidth = 32;
  int counter = 0;
  for (int yy = 0; yy < imageHeight; yy++) {
    for (int xx = 0; xx < imageWidth; xx++) {
      display.drawPixel(xx, yy, frame[counter]);
      counter++;
    }
  }
  delay(100);
}

void loop() {
  server.handleClient();
}

void handlePic() {
  String image_name = server.arg("name");
  
  if (image_name == "twitch") {
    drawFrame(twitch);
  } else if (image_name == "robotics") {
    drawFrame(robotics);
  } else if (image_name == "matlab") {
    drawFrame(matlab);
  } else {
    display.clearDisplay();
  }
}

void handleOff() {
  display.clearDisplay();
}

String last_message = "";

void handlePrint() {
  String to_print = server.arg("text");

  if (last_message != to_print) {
    display.clearDisplay();
    display.setCursor(2,0);
    display.print(to_print);

    last_message = to_print;
  }
}
