# Environment Monitoring using Wio Terminal and Grove Sensors
 This project utilizes Seeed Studio's Wio Terminal and various sensors to monitor air quality, real-time data on dust concentration, carbon dioxide levels, temperature, humidity, illuminance, color temperature and proximity.
 
Environmental Monitoring with Wio Terminal
Monitor air quality using Seeed Studio's Wio Terminal with multiple sensors.

Table of Contents
Introduction
Features
Requirements
Installation
Usage
Contributing
License
Introduction
This project utilizes Seeed Studio's Wio Terminal and various sensors to monitor air quality. It provides real-time data on dust concentration, carbon dioxide levels, temperature, humidity, and proximity.

# Features
Real-time monitoring of dust concentration, carbon dioxide levels, temperature, humidity, and proximity.
Integration with Seeed Studio sensors, including HM330 for dust, SCD30 for CO2, and TMG3993 for proximity and ambient light.
Requirements
Wio Terminal
Seeed Studio HM330 Dust Sensor
Seeed Studio SCD30 CO2 Sensor
Seeed Studio TMG3993 Proximity and Ambient Light Sensor
Additional dependencies specified in the code

# Installation

1. Clone this repository:

```bash
git clone [https://github.com/your-username/your-repo.git](https://github.com/asediga/EnvironmentMonitoring.git)
cd EnvironmentMonitoring.git
```
2. Connect the Wio Terminal and required sensors.
3. Upload the code to your Wio Terminal.
4. Install Arduiono IDE

# Material List
* https://www.seeedstudio.com/Grove-8-Channel-I2C-Hub-TCA9548A-p-4398.html
* https://www.seeedstudio.com/Grove-CO2-Temperature-Humidity-Sensor-SCD30-p-2911.html
* https://www.seeedstudio.com/Grove-Light-Color-Proximity-Sensor-TMG39931-p-2879.html
* https://www.seeedstudio.com/Grove-Laser-PM2-5-Sensor-HM3301.html
* https://www.seeedstudio.com/Wio-Terminal-p-4509.html
* https://www.seeedstudio.com/Grove-Universal-4-Pin-20cm-Unbuckled-Cable-5-PCs-Pack-p-749.html

![image](https://github.com/asediga/EnvironmentMonitoring/assets/92961504/579c947b-c504-4c85-a66a-d5a42c241494)


# Prototype Design
<note> O2 sensor though seen in this diagram , was later removed from the code in this repo 
![image](https://github.com/asediga/EnvironmentMonitoring/assets/92961504/42e19646-7537-4ba0-a4f5-518f43603f83)

# Usage
1. Power up the Wio Terminal with connected sensors by  Connect Wio Terminal to a USB port on your computer
4. Open the Arduino Serial Monitor to view real-time sensor data.
5. Customize the code or sensor connections based on your requirements.

# Code

```cpp
#include "TCA9548A.h"
#include "Seeed_TMG3993.h"
#include "SCD30.h"
#include "Seeed_HM330X.h"
#include <TFT_eSPI.h>
#include <Arduino.h>
#include <Wire.h>

// Initialize TFT_eSPI display
TFT_eSPI tft;
// Initialize I2C Mux
TCA9548A TCA;
// Initialize TMG3993 sensor for proximity and ambient light
TMG3993 TMG;
// Initialize SCD30 sensor for CO2, temperature, and humidity
SCD30 SCD;
// Initialize HM330X sensor for dust concentration
HM330X HM330;

// Function to print sensor results on TFT_eSPI display
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

// Function to initialize I2C sensors
void initializeSensors() {
  TCA.begin(Wire);
  Serial.println("--- Opening multiple channels --- ");
  for (uint8_t x = 0; x < 8; x++) {
    Serial.print("Opening << Channel: ");
    Serial.println(x);
    TCA.openChannel(x);   // Open channel
  }
  Serial.println("Sensor Initialization Complete");
}

// Function to read and print test data
void readAndPrintTestData() {
  printResult("TestData", 0.0, "", 7);
}

// Function to read and print TMG3993 sensor data
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
    printResult("Lux", lux, "Lux", 5);
    printResult("C Temp", ctemp, "K", 6);
  }
  // Don't forget to clear the interrupt bits
  TMG.clearALSInterrupts();
  Serial.println("----------------------TMG----------------------");
  Serial.println("");
}

// Function to read and print SCD30 sensor data
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

// Function to read and print HM330X sensor data
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
  tft.begin();
  tft.setRotation(3); // Adjust the screen orientation if needed
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLUE);  // Set initial background color

  initializeSensors();
}

void loop() {
  Serial.println("----------------------Reading Sensors----------------------");
  readAndPrintHM330Data();
  readAndPrintSCDData();
  readAndPrintTMGData();
  delay(1000);
}

```

# Contributing
Contributions are welcome! Feel free to submit bug reports, feature requests, or contribute code. Please follow our contribution guidelines.

# License
This project is licensed under the MIT License.
