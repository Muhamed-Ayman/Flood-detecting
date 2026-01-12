<p align="center">
  <img src="https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white"/>
  <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white"/>
  <img src="https://img.shields.io/badge/ESP32-IoT-red?style=for-the-badge"/>
  <img src="https://img.shields.io/badge/Node--RED-8F0000?style=for-the-badge&logo=nodered&logoColor=white"/>
  <img src="https://img.shields.io/badge/MQTT-Protocol-blue?style=for-the-badge"/>
</p>

# 🌊 Advanced Flood Detection System using ESP32, IoT & Real-Time Analytics

An IoT-based Flood Early Warning System designed to monitor environmental and hydraulic parameters in real time.  
The system integrates multi-sensor data, scientific analysis, and cloud-based communication to provide early flood detection and alerts.  
**Node-RED is used for mobile dashboard visualization and real-time notifications/messages to the user’s phone.**

---

## 📑 Table of Contents
- [Project Overview](#project-overview)
- [Problem Statement](#problem-statement)
- [Project Objectives](#project-objectives)
- [Project Structure](#project-structure)
- [System Architecture](#system-architecture)
- [Hardware Components](#hardware-components)
- [Component Connections](#component-connections)
- [Measured Parameters](#measured-parameters)
- [Danger Thresholds](#danger-thresholds)
- [Flowchart (System Logic)](#flowchart-system-logic)
- [Analysis and Scientific Basis](#analysis-and-scientific-basis)
- [Network & MQTT Configuration](#network--mqtt-configuration)
- [Recommendations](#recommendations)
  

---

## 📌 Project Overview
Flooding is one of the most dangerous climate-related disasters, especially in flood-prone regions such as Egypt.  
This project presents an advanced flood detection system that continuously monitors water behavior and environmental conditions to provide early warning alerts.



- [Project Portfolio ](https://github.com/Muhamed-Ayman/Flood-detecting/blob/main/docs/Portfolio.pdf)
- [Project Poster](https://github.com/Muhamed-Ayman/Flood-detecting/blob/main/docs/Poster.pdf)
- [Project Presentation](https://github.com/Muhamed-Ayman/Flood-detecting/blob/main/docs/presentaion.ppsx)


---

## ⚠️ Problem Statement
Egypt faces increasing challenges in mitigating the effects of climate change, particularly the rising risk of sudden floods in vulnerable areas.  
These floods threaten public safety, infrastructure, and agriculture, highlighting the urgent need for real-time flood detection solutions.

---

## 🎯 Project Objectives
- Detect flood risks at early stages  
- Monitor water level and discharge velocity  
- Analyze environmental parameters contributing to floods  
- Provide real-time alerts on mobile devices  
- Visualize data through an interactive dashboard  
- Enhance public safety using cost-effective technology  

---

## 🧱 Project Structure
The project is structured into **three main layers**:

### 1️⃣ Sensing Layer
- DHT11 (Temperature & Humidity)
- BMP280 (Atmospheric Pressure)
- Soil Moisture Sensor (Water Level)
- Water Flow Sensor (Flow Rate & Velocity)

### 2️⃣ Processing & Communication Layer
- ESP32 processes sensor data
- Performs calculations and threshold checks
- Publishes data using MQTT over Wi-Fi

### 3️⃣ Application & Alert Layer
- Node-RED mobile dashboard
- Real-time data visualization
- Notifications and warnings
- Local buzzer alerts

---

## 🏗 System Architecture
The ESP32 collects data from all sensors and transmits it wirelessly using MQTT.  
Node-RED receives the data, analyzes it, visualizes it on a mobile dashboard, and triggers alerts when danger conditions occur.

<img width="1867" height="1079" alt="image" src="https://github.com/user-attachments/assets/7173572b-4ccb-48ba-b1c7-9e5e594cc05f" />

---

## 🧰 Hardware Components

| Component | Function |
|--------|---------|
| ESP32-WROOM-32U | Data processing & wireless communication |
| DHT11 | Temperature & humidity |
| BMP280 | Atmospheric pressure |
| Soil Moisture Sensor | Water level |
| YF-S201C Flow Sensor | Flow rate & velocity |
| Magnetic Buzzer | Audible alert |
| Node-RED | Mobile dashboard & notifications |
<p align="center">
<img width="819" height="579" alt="image" src="https://github.com/user-attachments/assets/09b225d0-eb98-47bb-995e-935a97cf8a85" />
</p>
<img width="1630" height="1030" alt="image" src="https://github.com/user-attachments/assets/6e8d0bf3-439a-4d41-851b-2530d8ccda86" />

---

## 🔌 Component Connections

<table>
<tr>
<td>

| Component      | ESP32 Pin | Connection           |
|---------------|-----------|--------------------|
| DHT11          | GPIO 4    | Digital             |
| Soil Moisture  | GPIO 36   | Analog              |
| Flow Sensor    | GPIO 23   | Digital (Interrupt) |
| Buzzer         | GPIO 26   | Digital Output      |
| BMP280         | I2C (0x76)| SDA / SCL           |

</td>
<td>

<img width="476" height="286" alt="image" src="https://github.com/user-attachments/assets/ffe7e1f3-eec4-42ed-866c-b1e0e40cea60" />


</td>
</tr>
</table>

---

## 📊 Measured Parameters
- Water level  
- Water flow rate  
- Water velocity  
- Temperature  
- Humidity  
- Atmospheric pressure  

---

## 🚨 Danger Thresholds

| Parameter | Danger Level |
|--------|--------------|
| Water Level | ≥ 100% |
| Water Velocity | ≥ 150 cm/s |
| Humidity | > 80% |
| Pressure | < 1010 hPa |
| Temperature | < 20°C or > 30°C |

---

## 🔁 Flowchart (System Logic)
1. Initialize ESP32 and sensors  
2. Read sensor data periodically  
3. Process and calibrate values  
4. Calculate water velocity  
5. Compare values with thresholds  
6. Trigger buzzer and notifications if danger detected  
7. Publish data via MQTT  
8. Display data on Node-RED dashboard  
9. Repeat continuously  

<img width="855" height="934" alt="image" src="https://github.com/user-attachments/assets/5c9883dd-a091-432f-a152-9c049bde8597" />

---

## 🔍 Analysis and Scientific Basis 🔬
Egypt faces significant challenges due to climate change, especially increased flood risks.  
The proposed design integrates IoT, multi-sensor systems, real-time monitoring, Node-RED mobile visualization, and energy-efficient hardware.

<img width="739" height="773" alt="image" src="https://github.com/user-attachments/assets/ac4a20d0-4b60-4562-9e1b-ff027780dd8f" />
<img width="739" height="750" alt="image" src="https://github.com/user-attachments/assets/ae8806fd-a4f3-410a-a58c-f2e26e9158ef" />





## 🌐 Network & MQTT Configuration

### Wi-Fi
The ESP32 connects to a Wi-Fi network to transmit sensor data in real time:  

```text
SSID: ESP32
Password: 12345678
```
---

## 📝 Recommendations
- Use real-time data to model flood spread for better predictions and early warnings.  
- Upgrade sensors for faster, more accurate data (e.g., GFS131-WC2 for flow, SHT35 for temp & humidity).  
- Use GIS tools to create detailed flood maps for efficient planning and response.  
- Harness high water flow to drive turbines for clean energy generation.

---

## 📧 Contact

You can reach us via the following emails:
| Name                                  | Email                                  |
|--------------------------------------|----------------------------------------|
| Mohamed Ayman Abd-elhakim Ibrahim     | [mohammed.a.ibrahim@gmail.com](mailto:mohammed.a.ibrahim@gmail.com)  |
| Omar Bahaa Eldin Elsayed              | [omarbahaa013@gmail.com](mailto:omarbahaa013@gmail.com)              |
