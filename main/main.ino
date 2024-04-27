#include "DHTStable.h" //Install Rob Tillaart's DHTStable library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHT11_PIN 10
#define LED 4
#define FAN 5
#define SoundSensor 8
#define SWITCH 13

boolean LEDStatus = false;
int Threshold_Temperature = 30;  //Change Temperature based on ambient temperature value and when you want the fan to switch on

LiquidCrystal_I2C lcd(0x27, 16, 2);  //Change Address 0x27 to the address of your I2C module, Change 16 and 2 based on the dimensions of your LCD Display 
DHTStable DHT;

void setup() {
  pinMode(SoundSensor, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(FAN, OUTPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
    int check = digitalRead(SWITCH);
    
    lcd.setCursor(0, 0);
    lcd.print("T=");
    lcd.print(DHT.getHumidity()); 
    lcd.print(" ");
    lcd.print("H=");
    lcd.print(DHT.getTemperature());
    lcd.print("%");
    if (DHT.getTemperature() >= Threshold_Temperature)  //If Ambient Temperature higher than the Threshold Temperature
    {
      Serial.println(DHT.getTemperature(), 1);
      digitalWrite(FAN, HIGH);
      lcd.setCursor(1,1);
      lcd.print("Today is warm!");
    } 
    else                                        //If Ambient Temperature is lower than the Threshold Temperature
    {
      digitalWrite(FAN, LOW);
      lcd.setCursor(1,1);
      lcd.print("Today is cold!");
    }

    if (check == 0) { // Check == 0 indicates that the Sound Sensor is on and the LDR is Off
        int SensorData = digitalRead(SoundSensor);
        if (SensorData == 1) 
        {
            if (LEDStatus == false) 
            {
                LEDStatus = true;
                digitalWrite(LED, HIGH);
            } else if (LEDStatus == true)
            {
                LEDStatus = false;
                digitalWrite(LED, LOW);
            }
        }
    } 
    else if (check == 1) //Check == 1 indicates that the LDR is on and the Sound Sensor is Off
    {
      { 
        int analogValue = analogRead(A1); //LDR Connected to Analogue port A1
        Serial.print("Analog reading = ");
        Serial.print(analogValue);
        if (analogValue < 500) {          //High Analogue Value indicates Low light
            digitalWrite(LED, LOW);
        } 
        else {
            digitalWrite(LED, HIGH);
        }
      }
    }
}