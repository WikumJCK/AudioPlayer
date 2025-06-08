# 🔊 Smart Audio Scheduler with PIR Motion Detection

This project uses an **ESP32**, **DFPlayer Mini**, and **RTC module (DS1307)** to play specific audio files at scheduled times (morning and evening) and in response to motion detection. It ensures audio playback is not interrupted or retriggered unnecessarily.

---

## 📦 Features

- ⏰ Plays a **morning audio** every day at 07:00 AM
- 🌙 Plays an **evening audio** every day at 07:00 PM
- 🕵️ Plays a **motion-triggered audio** when movement is detected via a PIR sensor
- 🔒 Prevents motion audio from interrupting scheduled audio
- 🔁 Avoids replaying the same track if it's already playing
- 🕘 RTC-based timekeeping with DS1307
- 🎵 Audio playback using DFPlayer Mini module and microSD card

---

## 🧰 Hardware Used

- ESP32 (DevKit)
- DFPlayer Mini MP3 Player
- DS1307 RTC Module
- PIR Motion Sensor
- Speaker (connected to DFPlayer)
- microSD card (formatted to FAT32)

---

## 🔌 Wiring Diagram

> ![AudioPlayer](https://github.com/user-attachments/assets/759b841b-b931-4b99-bfbb-650b7c94717f)

## 🖼️ Product Images

![image2](https://github.com/user-attachments/assets/8f23aae5-4f1a-4fe6-bd7e-32eb4b4f51f5)
![image1](https://github.com/user-attachments/assets/df28b1ea-7517-4877-ad57-9fa9d6f71cf6)

---

## 🚀 Getting Started

1. Format a microSD card to **FAT32**
2. Place audio files in the root directory:
   - `0001.mp3` → Morning audio
   - `0002.mp3` → Evening audio
   - `0003.mp3` → Motion audio
3. Flash the code to the ESP32
4. Power the board and monitor serial output (baud: 115200)
5. Ensure the RTC is running correctly (auto set on first boot)

---

## 📁 Code Highlights

- Uses `DFRobotDFPlayerMini` library for MP3 control
- Uses `RTClib` for time management
- Interrupt-driven PIR detection
- Playback control using `readState()` to avoid interrupting active audio

---
