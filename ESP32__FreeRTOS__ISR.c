#include <Wire.h>
#include <esp_sleep.h>
#include <task.h>


// Variables
int Max_Value = 12;
int Min_Value = 6;
float data;


// LED Pinout
int LED_Yellow = 5;
int LED_Green = 4;
int LED_Red = 6;

// Interrupt Pin
int ISRinterrupt_Pin = 2;

void setup() {
    Serial.begin(9600);
    pinMode(LED_Yellow, OUTPUT);
    pinMode(LED_Green, OUTPUT);
    pinMode(LED_Red, OUTPUT);
    pinMode(ISRinterrupt_Pin, INPUT);

    xTaskCreate(Read_Sensor_Data, "Read Sensor data", 400, NULL, 1, NULL);
    xTaskCreate(ConsolDisplay_Sensor_Data, "Display Sensor Information on the console", 128, (void*)&data, 1, NULL);
    xTaskCreate(Notify_Output, "Notify With LEDs", 128, (void*)&Sensor_data, 1, NULL);

    esp_sleep_enable_ext0_wakeup(ISRinterrupt_Pin, LOW); // Enable wake-up by external interrupt
    attachInterrupt(digitalPinToInterrupt(ISRinterrupt_Pin), ISRinterrupt, FALLING);
    vTaskStartScheduler();
}

void Read_Sensor_Data(void *pvParameters) {
    while(1) {
        Sensor_data = random(5, 20);
        if (isnan(Sensor_data)) {
            Serial.println("Failed to read from sensor!");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void ConsolDisplay_Sensor_Data(void *pvParameters) {
    while(1) {
        float V = *((float*)pvParameters);
        Serial.print("Sensor Data: ");
        Serial.println(V);
        vTaskDelay(pdMS_TO_TICKS(500)); // Add a delay to balance task load
    }
}

void Notify_Output(void *pvParameters) {
    while(1) {
        float Vn = *((float*)pvParameters);
        if (Vn <= Min_Value) {
            toggleLED(LED_Yellow);
        } else if (Vn > Min_Value && Vn < Max_Value) {
            toggleLED(LED_Green);
        } else {
            toggleLED(LED_Red);
        }
    }
}

void toggleLED(int pin) {
    digitalWrite(pin, HIGH);
    vTaskDelay(pdMS_TO_TICKS(200));
    digitalWrite(pin, LOW);
    vTaskDelay(pdMS_TO_TICKS(200));}

void ISRinterrupt() {
    esp_deep_sleep_start(); // Put ESP32 into deep sleep mode
    digitalWrite(LED_Green,LOW);
    digitalWrite(LED_Red,LOW);
    digitalWrite(LED_Yellow,LOW);
    // Note: ESP32 will reset upon waking, re-running setup()
}

void loop() {}
