# Arduino LED Sequence Game

This project is an Arduino-based LED sequence game where users can create and repeat sequences using touch sensors.

## Table of Contents
- [Introduction](#introduction)
- [Components](#components)
- [Setup](#setup)
- [Usage](#usage)
- [Modes](#modes)
- [Victory Animation](#victory-animation)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This Arduino project utilizes touch sensors and an LED strip to create a sequence game. Users can either create sequences or repeat pre-defined sequences by touching designated sensors.

## Components

- Arduino board
- Adafruit NeoPixel LED strip
- U8g2 SSD1309 128x64 OLED display
- Adafruit CAP1188 touch sensor

## Setup

1. Connect the components as per the wiring diagram.
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

## Contributing

Contributions are welcome! Fork this repository, make your changes, and submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
