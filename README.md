| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-C61 | ESP32-H2 | ESP32-H21 | ESP32-H4 | ESP32-P4 | ESP32-S2 | ESP32-S3 | Linux |
| ----------------- | ----- | -------- | -------- | -------- | -------- | --------- | -------- | --------- | -------- | -------- | -------- | -------- | ----- |

# ESP32-Edge-Vision

> **Note:** This is a work-in-progress project combining **ESP32-CAM firmware** with **AI modules** for real-time streaming, face detection, and recognition. This is **not yet a full drone OS**, but lays the foundation for edge vision capabilities.  

---

## Overview

ESP32-Edge-Vision combines:  

- **ESP32-CAM firmware** for real-time video streaming via HTTP  
- **FreeRTOS tasks** for concurrency, queues, and event groups  
- **Python AI modules** for face detection using **YOLOv8n-face**  
- ONNX support for exporting models to optimized inference  
- **OpenCV** for capturing the ESP32 webserver stream  

The goal is a **modular, scalable system** capable of handling multiple cameras, face recognition, and AI-based vision tasks.

---

## Features

- Real-time video streaming from ESP32-CAM  
- Ultrasonic object detection with HC-SR04  
- UART-based communication to send uart wakeup signal when motion/object is detected  
- Modular FreeRTOS architecture (capture, stream, power management)  
- Queue-based frame buffer to prevent lost frames  
- MQTT-based communication and notifications
- Event group flags to synchronize WiFi, streaming, and capture  
- Python-based face detection and recognition using YOLOv8n-face  
- ONNX export for optimized inference  
- OpenCV fetches HTTP stream directly from ESP32-CAM  
- Easily extendable for multiple cameras and AI models  

---

## System Architecture

[ESP32-CAM] --> [Capture Task] --> [Frame Queue] --> [Stream Task] --> [HTTP Clients / Python AI]
[WiFi] --> [Event Group] --> Signals readiness
[Motion Detection / HC-SR04] --> (via Arduino) --> UART Wakeup --> ESP32 Light Sleep Management
[Python AI Server] --> Face Detection & Recognition (YOLOv8n-face / ONNX)


- FreeRTOS tasks run concurrently  
- Event groups signal WiFi status and streaming readiness (`WIFI_CONNECTED_BIT`, `STREAM_READY_BIT`)  
- Queues store camera frames between capture and streaming tasks  

---

## Hardware

- **ESP32-CAM** (AI-Thinker)  
- **PSRAM** for higher resolution frames  
- Arduino Uno (for motion detection / sensors)  
- Optional: Level shifter for UART wakeup  
- Server for AI processing  

---

## Software Setup

### ESP32 Firmware

1. Install **ESP-IDF** and set up your environment  
2. Clone the repository:  
   ```bash
   git clone https://github.com/SamuelMacharia/esp32-edge-vision.git
   cd esp32-edge-ai/face_recognition
3. Configure WiFi in menuconfig (`CONFIG_WIFI_SSID` & `CONFIG_WIFI_PASSWORD`)

4. Build and flash:
    ```bash
    $HOME/esp/esp-idf/export.sh
    idf.py build
    idf.py flash
    idf.py monitor
    ```
5. Access the stream in a browser:
    ```bash
    http://<ESP32_IP>/stream
    ```

    ## Python Edge AI
1. Go to the AI folder:
    ```bash
    cd ../ai
    ```
2. Create and activate a Python virtual environment:
    ```bash
    python -m venv venv
    source venv/bin/activate
    ```
3. Install dependencies:
    ```bash
    pip install -r requirements.txt

4. Run the face detection script:
    ```bash
    python face_detect.py
    ```
---
- Captures frames from the ESP32 HTTP stream

- Performs face detection and recognition using YOLOv8n-face

- Extracts ROI for downstream processing

---

# ESP32-Edge-Vision

