# EnvironmentMonitoring
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

# Contributing
Contributions are welcome! Feel free to submit bug reports, feature requests, or contribute code. Please follow our contribution guidelines.

# License
This project is licensed under the MIT License.
