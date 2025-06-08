#include <Arduino.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "RTClib.h"

#define FPSerial Serial1

// Pin Definitions
#define LED_BUILTIN 2 // Built-in LED on ESP32
#define PIR_PIN 15    // PIR Motion Sensor pin

#define DFPLAYER_RX_PIN 27
#define DFPLAYER_TX_PIN 26

#define RTC_SCL_PIN 22 // RTC SCL pin
#define RTC_SDA_PIN 21 // RTC SDA pin

// SoftwareSerial softwareSerial(DFPLAYER_RX_PIN, DFPLAYER_TX_PIN);

DFRobotDFPlayerMini player;
RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

boolean is_motion_detected = false;
bool is_audio_playing = false;

bool morning_audio_played_today = false; // Flag to check if morning audio has been played today
bool evening_audio_played_today = false; // Flag to check if evening audio has been played today

uint8_t MORNING_HOUR = 6;  // Hour to play morning audio
uint8_t EVENING_HOUR = 18; // Hour to play evening audio

unsigned long last_audio_check = 0; // Last time audio was checked
unsigned long last_motion_time = 0; // Last time motion was detected

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement()
{
  if (!is_audio_playing)
  {
    is_motion_detected = true;
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void setup()
{

  Serial.begin(115200); // Serial port for debugging purposes
  // softwareSerial.begin(9600); // Software serial for DFPlayer Mini
  FPSerial.begin(9600, SERIAL_8N1, DFPLAYER_RX_PIN, DFPLAYER_TX_PIN); // Hardware serial for DFPlayer Mini

  Serial.println();
  Serial.println(F("Audio Player"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!player.begin(FPSerial, /*isACK = */ true, /*doReset = */ true))
  {
    Serial.println("DFPlayer Mini initialization failed.");
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true)
    {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  player.volume(15); // Set volume to a comfortable level
  // player.play(1);    // Play the first track to test

  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1)
      delay(10);
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is NOT running, setting the time!");
    // Set the date and time to compile time
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  else
  {
    Serial.println("RTC is running.");
  }
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIR_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIR_PIN), detectsMovement, RISING);

  // Set initial state

  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  DateTime now = rtc.now();

  // Reset flags at midnight
  if (now.hour() == 0 && now.minute() == 0 && now.second() == 0)
  {
    morning_audio_played_today = false;
    evening_audio_played_today = false;
  }

  // Check once per second
  if (millis() - last_audio_check >= 1000)
  {
    last_audio_check = millis();

    // Morning audio
    if (now.hour() == MORNING_HOUR && now.minute() == 0 && !morning_audio_played_today && !is_audio_playing)
    {
      playAudio(1);
      morning_audio_played_today = true;
    }

    // Evening audio
    if (now.hour() == EVENING_HOUR && now.minute() == 0 && !evening_audio_played_today && !is_audio_playing)
    {
      playAudio(2);
      evening_audio_played_today = true;
    }
  }

  // Motion audio
  if (is_motion_detected && !is_audio_playing && millis() - last_motion_time > 10000)
  { // 10s debounce
    playAudio(3);
    last_motion_time = millis();
    is_motion_detected = false;
  }

  // Monitor if audio stopped
  if (is_audio_playing && !(player.readState() == 513))
  {
    Serial.println("Audio finished.");
    is_audio_playing = false;
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void playAudio(uint8_t trackNum)
{
  if (is_audio_playing)
    return;

  Serial.print("Playing track: ");
  Serial.println(trackNum);
  player.play(trackNum);
  is_audio_playing = true;
}
