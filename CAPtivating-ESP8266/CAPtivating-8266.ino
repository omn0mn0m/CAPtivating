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

WiFiServer server(80);

// ISR for display refresh
void display_updater() {
  display.display(70);
}

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 1, 1);
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

int last_image_displayed = -2;
int image_to_display = -1;

void loop() {
  // Check for WiFi client
  WiFiClient client = server.available();

  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/pic/twitch") != -1) {
      image_to_display = 0;
    } else if (request.indexOf("/pic/robotics") != -1) {
      image_to_display = 1;
    } else if (request.indexOf("/pic/matlab") != -1) {
      image_to_display = 2;
    } else if (request.indexOf("/pic/off") != -1) {
      image_to_display = -1;
    } else {
//      image_to_display = -1;
    }

    // JSON response
    String s = "HTTP/1.1 200 OK\r\n";
    s += "Content-Type: application/json\r\n\r\n";
    s += "{\"data\":{\"message\":\"success\",\"value\":\"";
    s += value;
    s += "\"}}\r\n";
    s += "\n";
  
    // Send the response to the client
    client.print(s);
  }

  if (last_image_displayed != image_to_display) {
    switch (image_to_display) {
      case 0: 
        drawFrame(twitch);
        break;
      case 1: 
        drawFrame(robotics);
        break;
      case 2: 
        drawFrame(matlab);
        break;
      default: 
        display.clearDisplay();
        break;
    }

    last_image_displayed = image_to_display;
  }
  
  yield();
}
