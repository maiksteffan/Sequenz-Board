# Arduino LED Sequence Game

This project is an Arduino-based LED sequence game where users can create and repeat sequences using touch sensors.

## Table of Contents
- [Introduction](#introduction)
- [Components](#components)
- [Setup](#setup)
- [Usage](#usage)
- [Modes](#modes)
- [Victory Animation](#victory-animation)

## Introduction

This Arduino project utilizes touch sensors and an LED strip to create a sequence game. Users can either create sequences or repeat pre-defined sequences by touching designated sensors.

## Components

- Arduino board
- Adafruit NeoPixel LED strip
- U8g2 SSD1309 128x64 OLED display
- Adafruit CAP1188 touch sensor

## Setup

1. Connect the components as follows:

    - **LED Strip**:
        - Connect the data pin (DI) of the LED strip to pin 5 (LED_PIN) on the Arduino.
        - Connect the power (+5V) and ground (GND) pins of the LED strip to the corresponding pins on the Arduino.

    - **Button**:
        - Connect one terminal of the button to pin 2 (buttonPin) on the Arduino.
        - Connect the other terminal of the button to ground (GND) on the Arduino.

    - **OLED Display**:
        - Connect the CS (chip select) pin of the OLED display to pin 10 on the Arduino.
        - Connect the DC (data/command) pin of the OLED display to pin 9 on the Arduino.
        - Connect the RESET pin of the OLED display to pin 8 on the Arduino.
        - Connect the MOSI (Master Out Slave In) pin of the OLED display to pin 11 (MOSI) on the Arduino.
        - Connect the SCK (serial clock) pin of the OLED display to pin 13 (SCK) on the Arduino.
        - Connect the power (+5V) and ground (GND) pins of the OLED display to the corresponding pins on the Arduino.

    - **CAP1188 Touch Sensor**:
        - Connect the SDA pin of the CAP1188 to the SDA (data) pin on the Arduino.
        - Connect the SCL pin of the CAP1188 to the SCL (clock) pin on the Arduino.
        - Connect the power (+5V) and ground (GND) pins of the CAP1188 to the corresponding pins on the Arduino.

2. Install the necessary libraries (`Adafruit_NeoPixel`, `U8g2lib`, `Adafruit_CAP1188`) in your Arduino IDE.
3. Upload the provided code to your Arduino board.


## Usage

- Press the button connected to pin 2 to start playing a sequence.
- Use touch sensors A-G to either create or repeat sequences.

## Modes

This project supports two modes:
- **Repeat Mode (`!`)**: In this mode, the Arduino repeats pre-defined sequences.
- **Create Mode (`?`)**: In this mode, users can create their own sequences by touching sensors.

## Victory Animation
Upon successfully completing a sequence, the Arduino performs a victory animation using the LED strip.

