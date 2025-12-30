/* 
  SISTEM MONITOR JANTUNG BERBASIS MAX30102 & OLED 
  Mikrokontroler: Arduino Uno 
  Sensor: MAX30102 (I2C) 
  Display: OLED SSD1306 (I2C) 
*/ 
 
#include <Wire.h> 
#include "MAX30105.h"        // Library inti sensor 
#include "spo2_algorithm.h"  // Algoritma perhitungan 
#include <Adafruit_GFX.h>    // Library grafis 
#include <Adafruit_SSD1306.h> // Library OLED 
 
MAX30105 particleSensor; 
 
// Konfigurasi OLED 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 
OLED_RESET); 
 
// Variabel Data 
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__) 
  uint16_t irBuffer[100]; // 16-bit buffer untuk Uno 
  uint16_t redBuffer[100]; 
#else 
  uint32_t irBuffer[100]; 
  uint32_t redBuffer[100]; 
#endif 
 
int32_t bufferLength; 
int32_t spo2; 
int8_t validSPO2; 
int32_t heartRate; 
int8_t validHeartRate; 
 
// Variabel Filter Moving Average 
const int BPM_HISTORY_SIZE = 5; 
int bpmHistory[BPM_HISTORY_SIZE]; 
int bpmHistoryIndex = 0; 
long bpmTotal = 0; 
int bpmAverage = 0; 
 
void setup() { 
  Serial.begin(9600); 
 
  // Inisialisasi OLED 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Gagal OLED")); 
    while(1); 
  } 
  display.clearDisplay(); 
  display.setTextColor(WHITE); 
  
  // Inisialisasi Sensor 
 
FAKULTAS ILMU KOMPUTER 
UNIVERSITAS BRAWIJAYA 
 
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) { 
    display.setCursor(0,0); 
    display.println(F("Sensor Gagal")); 
    display.display(); 
    while (1); 
  } 
 
  // --- KALIBRASI SENSOR (HASIL ANALISIS) --- 
  byte ledBrightness = 30; // Diturunkan ke 30 agar tidak saturasi 
  byte sampleAverage = 4; 
  byte ledMode = 2; 
  byte sampleRate = 50;    // 50Hz untuk filter noise listrik 
  int pulseWidth = 411; 
  int adcRange = 4096; 
 
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, 
sampleRate, pulseWidth, adcRange); 
  
  // Inisialisasi nilai awal filter 
  for (int i = 0; i < BPM_HISTORY_SIZE; i++) { 
    bpmHistory[i] = 70; 
    bpmTotal += 70; 
  } 
  bpmAverage = 70; 
} 
 
void loop() { 
  bufferLength = 100; // Ambil 100 sampel data 
 
  // Loop pengumpulan data 
  for (byte i = 0 ; i < bufferLength ; i++) { 
    while (particleSensor.available() == false) 
      particleSensor.check(); 
    redBuffer[i] = particleSensor.getRed(); 
    irBuffer[i] = particleSensor.getIR(); 
    particleSensor.nextSample(); 
  } 
 
  // Hitung BPM & SpO2 
  maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, 
redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate); 
  
  // Tampilkan Hasil 
  if (validHeartRate && validSPO2) { 
    // Filter Moving Average 
    if (heartRate < 180 && heartRate > 40) { 
      bpmTotal = bpmTotal - bpmHistory[bpmHistoryIndex]; 
      bpmHistory[bpmHistoryIndex] = heartRate; 
      bpmTotal = bpmTotal + heartRate; 
      bpmHistoryIndex = (bpmHistoryIndex + 1) % BPM_HISTORY_SIZE; 
      bpmAverage = bpmTotal / BPM_HISTORY_SIZE; 
    } 
 
    // Output ke OLED 
    display.clearDisplay(); 
    display.setTextSize(1); 
    display.setCursor(0,0); 
    display.println(F("Status: Stabil")); 
    
    display.setTextSize(2); 
    display.setCursor(0,20); 
    display.print(F("BPM: ")); display.println(bpmAverage); 
    display.setCursor(0,45); 
    display.print(F("O2 : ")); display.print(spo2); 
display.println(F("%")); 
    display.display(); 
  } else { 
    display.clearDisplay(); 
    display.setTextSize(1); 
    display.setCursor(0,0); 
    display.println(F("Mengambil Data...")); 
    display.setCursor(0,20); 
    display.println(F("Pastikan jari diam")); 
    display.display(); 
  } 
}
