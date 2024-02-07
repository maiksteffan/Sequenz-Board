#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include <Adafruit_CAP1188.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// CAP1188 touch sensor setup
Adafruit_CAP1188 cap = Adafruit_CAP1188();
byte previousTouchStatus = 0;

// Define pin connections & constants for the LED strip
const int buttonPin = 2;  
#define LED_PIN     5      
#define LED_COUNT  130   

// LED mapping array 
const int LED_MAPPING[7] = {111, 18, 98, 30, 87, 42, 75}; // LED positions for A-G

// Create an Adafruit_NeoPixel object
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// OLED display setup
U8G2_SSD1309_128X64_NONAME0_2_4W_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* DC=*/ 9, /* RESET=*/ 8);


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  strip.begin();
  strip.show(); 
  u8g2.begin(); // Initialize the OLED display
  // Initialize the CAP1188 sensor
  if (!cap.begin(0x29)) {
    Serial.println(F("Could not connect to CAP1188"));
  } else {
    Serial.println(F("CAP1188 initialized"));
  }
  // Set sensitivity level and debounce
  cap.writeRegister(0x1F, 0x3F); // Medium sensitivity
  cap.writeRegister(0x2A, 0x01); 
  cap.writeRegister(0x2B, 0x01); 
}

void displaySequence(const String& sequence) {
  u8g2.clearBuffer();                  
  u8g2.setFont(u8g2_font_ncenB08_tr);  
  u8g2.drawStr(0,10, sequence.c_str()); // Display the received data
  u8g2.sendBuffer();                   
}

void turnOffAllLEDs() {
  for (int i = 0; i < LED_COUNT; i++) {
  strip.setPixelColor(i, strip.Color(0, 0, 0)); // Turn off each LED
  }
  strip.show();
}

void lightUpLED(char led, char color) {
    // Map the character 'A' - 'G' to the LED index
    int ledIndex = LED_MAPPING[led - 'A'];
    // Determine the color
    uint32_t ledColor;
    switch (color) {
        case 'g': // Green
            ledColor = strip.Color(0, 255, 0);
            break;
        case 'r': // Red
            ledColor = strip.Color(255, 0, 0);
            break;
        case 'o': // off
            ledColor = strip.Color(0, 0, 0);
            break;
        // Add more colors as needed
        default:
            ledColor = strip.Color(0, 0, 0); // Default to off if color not recognized
    }
    // Set the LED color and show it
    strip.setPixelColor(ledIndex, ledColor);
    strip.show();
}

// Variables
String receivedData = "";
bool sequencePlayed = false;
void playSequence() {
    turnOffAllLEDs();
    for (int i = 0; i+1 < receivedData.length(); i++) {
        char currentChar = receivedData.charAt(i);
        lightUpLED(currentChar, 'g');
        delay(1500);
        lightUpLED(currentChar, 'o');
        strip.show();
    }
    sequencePlayed = true; // Set the flag to indicate the sequence has been played
}

int currentSequenceIndex = 0;
bool sequenceCompleted = false;
void repeatSequence() {
    byte touchStatus = cap.touched();

    for (byte sensorIdx = 0; sensorIdx < 7; sensorIdx++) {
        if (touchStatus & (1 << sensorIdx)) {
            if ((previousTouchStatus & (1 << sensorIdx)) == 0) {
                turnOffAllLEDs(); 
                char touchedLetter = 'A' + sensorIdx;
                Serial.println(touchedLetter);

                // Check if the touch matches the current sequence
                if (currentSequenceIndex < receivedData.length() &&
                    touchedLetter == receivedData.charAt(currentSequenceIndex)) {
                    lightUpLED(touchedLetter, 'g');
                    currentSequenceIndex++; 

                    // Check if this was the last character in the sequence
                    if (currentSequenceIndex + 1 >= receivedData.length()) {
                        sequenceCompleted = true;
                        currentSequenceIndex = 0; // Reset for the next sequence
                        break;
                    }
                } else {
                    lightUpLED(touchedLetter, 'r');
                    currentSequenceIndex = 0;
                    break;
                }
            }
        }
    }
    previousTouchStatus = touchStatus;
    delay(100);
    // Play the victtory animation if the sequence was completed
    if (sequenceCompleted) {
        victoryAnimation(2000);
        sequenceCompleted = false;
    }
}


void createSequence() {
    displaySequence("Create Sequence");
    byte touchStatus = cap.touched();
    for (byte sensorIdx = 0; sensorIdx < 7; sensorIdx++) { // Loop for 7 sensors (A-G)
        if (touchStatus & (1 << sensorIdx)) {
            if ((previousTouchStatus & (1 << sensorIdx)) == 0) { // New touch detected
                char touchedLetter = 'A' + sensorIdx; //FEHLER?
                Serial.println(touchedLetter); // Print the touched letter
                turnOffAllLEDs();
                // Light up the corresponding LED in green
                lightUpLED(touchedLetter, 'g');
            }
        }
    }
    previousTouchStatus = touchStatus; // Update the last touch state
    delay(100); // Optional delay for debouncing
}

char REPEAT_MODE = '!';
char  CREATE_MODE = '?';
char currentMode = REPEAT_MODE;

void loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    playSequence();
  }
    while (Serial.available() > 0) {
        char receivedChar = Serial.read();
        if (receivedChar == '!') {
            currentMode = REPEAT_MODE;
            receivedData = "";
            sequencePlayed = false; // Reset the flag
        } else if (receivedChar == '?') {
            currentMode = CREATE_MODE;
            receivedData = "";
            sequencePlayed = false; // Reset the flag
        } else {
            receivedData += receivedChar;
        }
        displaySequence(receivedData);
    }
    if (currentMode == CREATE_MODE) {
        createSequence();
    }
    if (currentMode == REPEAT_MODE) {
        if (!sequencePlayed) {
            playSequence();
        }
        repeatSequence();
    }
}

void victoryAnimation(int duration) {
    int waveLength = LED_COUNT / 4; // Length of the smaller wave
    int waveSpeed = 50; // Speed of each wave step in milliseconds
    int totalTimeSteps = duration / waveSpeed; // Total steps in the animation

    for (int t = 0; t < totalTimeSteps; t++) {
        for (int i = 0; i < LED_COUNT; i++) {
            float wave = (sin(4 * PI * (i - t) / waveLength) + 1) / 2; // Normalized to range 0-1
            int intensity = 5 + wave * (100); // Range between 5 and 105
            // Set the color with varying intensity
            strip.setPixelColor(i, strip.Color(0, 0, intensity));
        }
        strip.show();
        delay(waveSpeed);
    }
    turnOffAllLEDs();
}




