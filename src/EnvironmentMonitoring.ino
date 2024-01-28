#include "TCA9548A.h"
#include "Seeed_TMG3993.h"
#include "SCD30.h"
#include "Seeed_HM330X.h"
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <Wire.h>

TFT_eSPI tft;
TCA9548A TCA;  // Mux
TMG3993 TMG;
SCD30 SCD;
HM330X HM330;

//wio terminal display design: https://www.hackster.io/Salmanfarisvp/air-quality-monitoring-station-with-wio-terminal-6ef85f
/*
const char *str[] = {
  "sensor num: ",
 "PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
 "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
 "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
 "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
 "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
 "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
};
*/

void printResult(const char *label, float value, const char *unit, int line) {
  int lineHeight = 20;
  int x = 0;
  int y = line * lineHeight;
  int charWidth = 12; // Adjust this based on your font size

  // Clear the region where the values will be displayed
  tft.fillRect(x, y, tft.width(), lineHeight, TFT_BLUE);

  // Print the label
  tft.setCursor(x, y);
  tft.setTextColor(TFT_WHITE);
  tft.print(label);

  // Print the value
  tft.setCursor(x + strlen(label) * charWidth, y);
  tft.print(": ");
  tft.print(value, 1);
  tft.print(" ");
  tft.print(unit);
}


void initializeSensors() {
  TCA.begin(Wire);
  Serial.println("--- Opening multiple channels --- ");
  for(uint8_t x = 0; x < 8; x++)
  {
    Serial.print("Opening << Channel: ");   
    Serial.println(x); 
    TCA.openChannel(x);   // Open channel
    //delay(500);
    Serial.print("Register = Value: ");
    //Serial.println(TCA.readRegister());
    //delay(500);
  }
/*
  TCA.openChannel(TCA_CHANNEL_0);
  TCA.openChannel(TCA_CHANNEL_1);
  TCA.openChannel(TCA_CHANNEL_2);
  TCA.openChannel(TCA_CHANNEL_3);
  TCA.openChannel(TCA_CHANNEL_4);
  TCA.openChannel(TCA_CHANNEL_5);
  TCA.openChannel(TCA_CHANNEL_6);
  TCA.openChannel(TCA_CHANNEL_7);
*/
  Serial.println("Sensor Initialization Complete");
}

void readAndPrintTestData(){
  printResult("TestData", 0.0, "", 7);
}

void readAndPrintTMGData() {
  Serial.println("----------------------TMG----------------------");
    if (TMG.initialize() == false) {
        Serial.println("Device not found. Check wiring.");
        while (1);
    }
    TMG.setADCIntegrationTime(0xdb); // the integration time: 103ms
    TMG.enableEngines(ENABLE_PON | ENABLE_AEN | ENABLE_AIEN);

  if (TMG.getSTATUS() & STATUS_AVALID) {
    uint16_t proximityRaw = TMG.getProximityRaw();
    Serial.print("TMG Proximity Raw: ");
    Serial.println(proximityRaw);

    uint16_t red, green, blue, clear;
    int32_t lux, ctemp;
    TMG.getRGBCRaw(&red, &green, &blue, &clear);
      ctemp = TMG.getCCT(red, green, blue, clear);
      lux = TMG.getLux(red, green, blue, clear);

    Serial.print("TMG Red: ");
    Serial.println(red);
    Serial.print("TMG Green: ");
    Serial.println(green);
    Serial.print("TMG Blue: ");
    Serial.println(blue);
    Serial.print("TMG Clear: ");
    Serial.println(clear);
    Serial.print("TMG Lux: ");
    Serial.println(lux);
    Serial.print("TMG C Temp: ");
    Serial.println(ctemp);

    // Display on TFT_eSPI Screen
    printResult("Proximity", proximityRaw, "", 7);
    /*
    printResult("Red", red, "", 1);
    printResult("Green", green, "", 2);
    printResult("Blue", blue, "", 3);
    printResult("Clear", clear, "", 4);
    */
    printResult("Lux", lux, "Lux", 5);
    printResult("C Temp", ctemp, "K", 6);
  }
  // don't forget to clear the interrupt bits
  TMG.clearALSInterrupts();
  Serial.println("----------------------TMG----------------------");
  Serial.println("");
 //delay(500);
}

void readAndPrintSCDData() {
  Serial.println("----------------------SCD----------------------");
  float result[3] = {0};
  if (SCD.isAvailable()) {
    SCD.getCarbonDioxideConcentration(result);
    Serial.print("SCD Carbon Dioxide Concentration: ");
    Serial.print(result[0]);
    Serial.println(" ppm");
    Serial.print("SCD Temperature: ");
    Serial.print(result[1]);
    Serial.println(" °C");
    Serial.print("SCD Humidity: ");
    Serial.print(result[2]);
    Serial.println(" %");

    // Display on TFT_eSPI Screen
    printResult("CO2", result[0], "ppm", 2);
    printResult("Temperature", result[1], "C", 3);
    printResult("Humidity", result[2], "%", 4);
  }
  Serial.println("----------------------SCD----------------------");
  Serial.println("");
}

void readAndPrintHM330Data() {
  Serial.println("----------------------HM330----------------------");
  uint8_t buf[30];
  if (HM330.read_sensor_value(buf, 29)) {
    Serial.println("HM330X read result failed!!!");
  } else {
    uint16_t dustConcentration = (uint16_t)buf[11] << 8 | buf[12];
    Serial.print("HM330 Dust Concentration: ");
    Serial.println(dustConcentration);

    // Display on TFT_eSPI Screen
    printResult("Dust", dustConcentration, "ug/m3", 1);
  }

  Serial.println("----------------------HM330----------------------");
  Serial.println("");
}

void setup() {
  Serial.begin(115200);
  
  //Serial.begin(9600);
  /*
  while (!Serial) {
  }
  */

  tft.begin();
  tft.setRotation(3); // Adjust the screen orientation if needed
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLUE);  // Set initial background color

  initializeSensors();
}

void loop() {
  Serial.println("----------------------Reading Sensors----------------------");
//  readAndPrintTestData();
  readAndPrintHM330Data();
  readAndPrintSCDData();
  readAndPrintTMGData();
  delay(1000);
 // tft.fillScreen(TFT_BLUE);
}
