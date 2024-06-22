
// AUTOMATIC IRRIGATION SYSTEM

/* The program for AUTOMATIC IRRIGATION SYSTEM IS Written and Tested by__

 HS Sandesh Hegde
 Harish P Nair
 HV Sampad Nayak

DISCLAIMER: This is a Open Source Software and This Software is distributed in the hope that it will be useful,
            but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

Connection Details 
ARDUINO PINS
0_________________N/C
1_________________N/C
2_________________LCD-14
3_________________LCD-13
4_________________LCD-12
5_________________LCD-11
6_________________N/C
7_________________WATER_LEVEL_STATUS_LED
8_________________N/C
9_________________SPEAKER
10________________N/C
11________________LCD-6
12________________LCD-4
13________________PUMP_STATUS_LED)_AND_TO_RELAY
A0________________SOIL_MOISTURE_SENSOR
A4________________LM35_(TEMPERATURE_SENSOR)  //Bug reported for Unstable Temperature Reading 

LCD-1_____________GND
LCD-5_____________GND
LCD-2_____________+Vcc
LCD-3_____________LCD_BRIGHTNESS


*/
 
#include <LiquidCrystal.h> //LCD Library

#define NOTE_C4 262 
#define NOTE_D4 294 
#define NOTE_E4 330 
#define NOTE_F4 349 
#define NOTE_G4 392 
#define NOTE_A4 440 
#define NOTE_B4 494 
#define NOTE_C5 523 

int temp;
//int T_Sensor = A4;
int M_Sensor = A0;
int W_led = 7;
const int W_Sensor = A2;
bool tapWater = true;
int HistoryValue = 0;
char printBuffer[45];
int P_led = 13;
int Speaker = 9;
int val;
int cel;
String analogPin;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup()
 {
    lcd.begin(16, 2);
    lcd.clear(); 
    pinMode(13,OUTPUT);
    pinMode(A2,INPUT);
    pinMode(9,OUTPUT);
    pinMode(A0,INPUT);
    Serial.begin(9600);
    analogPin = analogPinConverter(W_Sensor);
    /*
    val = analogRead(T_Sensor); //Read Temperature sensor value 
    
    int mv = ( val/1024.0)*5000; 
    cel = mv/10;
    */
    lcd.setCursor(0,1);
    lcd.print("KYLE'S");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("AUTOMATIC");
    lcd.setCursor(0,1);
    lcd.print("IRIGATION SYSTEM");
    delay(2500);
    lcd.clear(); 
   
  
    delay(1000);
}

int levelConverter(int sensorValue, bool isCleanWater = true){
  int percentage;
  static int maxValue = 350;

  if (isCleanWater){
    percentage = map(sensorValue, 0, maxValue, 0, 100);
    if (percentage > 100) { 
      maxValue = sensorValue; 
      percentage = 100;
    }
  }else{
    percentage = map(sensorValue, 0, 1023, 0, 100);
  }
  return percentage;
}

void loop()
{
  
  lcd.clear();
    /* val = analogRead(T_Sensor); //Read Temperature sensor value 
    Serial.println(val);
    int mv = ( val/1024.0)*5000; 
     cel = mv/10;
  */
  int Moisture = analogRead(M_Sensor); //Read Moisture Sensor Value 
  Serial.println(Moisture);
/*
  lcd.setCursor(0,0);
  lcd.print("TEMP:");
  lcd.setCursor(5,0);
  lcd.print(cel);
  lcd.setCursor(7,0);
  lcd.print("*C");
 */
 if (Moisture> 600)   // for dry soil
  { 
        lcd.setCursor(0,0);
        lcd.print("DRY SOIL");
        //lcd.setCursor(11,1);
        //lcd.print("SOIL");
  int currentValue = analogRead(W_Sensor);    // get sensorPin analog value
  int diffValue = HistoryValue - currentValue;
  if((abs(diffValue) >= 10)||((currentValue == 1)&&(HistoryValue != 1)))
    
       {
         int percentValue = levelConverter(currentValue, tapWater);
         sprintf(printBuffer,"PIN %s level is %03d%% (raw value is %04d)\n", analogPin.c_str(), percentValue, currentValue);
         Serial.print(printBuffer);
         HistoryValue = currentValue;
         digitalWrite(13, HIGH);
         digitalWrite(W_Sensor, HIGH);
         digitalWrite(7, LOW);
         lcd.setCursor(0,1);
         lcd.print("PUMP:ON");
         lcd.setCursor(11,0);
        lcd.print("WATER");
        lcd.setCursor(11,1);
        lcd.print("HIGH");
        delay(10000);

       }
   else
       {
         digitalWrite(13, LOW);
         digitalWrite(W_Sensor, LOW);
         digitalWrite(7, HIGH);
         lcd.setCursor(0,1);
         lcd.print("PUMP:OFF");
         lcd.setCursor(11,0);
        lcd.print("WATER");
        lcd.setCursor(11,1);
        lcd.print("LOW");
         
           
           tone(Speaker, NOTE_C4, 500); 
           delay(500); 
           tone(Speaker, NOTE_D4, 500);
           delay(500); 
           tone(Speaker, NOTE_E4, 500); 
           delay(500); 
           tone(Speaker, NOTE_F4, 500); 
           delay(500); 
           tone(Speaker, NOTE_G4, 500); 
           delay(500);
       }
    }
 
     if (Moisture>= 70 && Moisture<=600) //for Moist Soil
    { 
      lcd.setCursor(0,0);
     lcd.print("MOIST SOIL");
     //lcd.setCursor(11,1);
     //lcd.print("SOIL");
     digitalWrite(13,LOW);
     lcd.setCursor(0,1);
     lcd.print("PUMP:OFF");    
  }
 
  if (Moisture < 500)  // For Soggy soil
  { 
    lcd.setCursor(0,0);
     lcd.print("SOGGY SOIL");
     //lcd.setCursor(11,1);
     //lcd.print("SOIL");
     digitalWrite(13,LOW);
     lcd.setCursor(0,1);
     lcd.print("PUMP:OFF");
  }
 delay(1000);    
}  
String analogPinConverter(int value){
  String analogPinValue; 

  switch(value){
    case 14:                                  // D14 = A0
      analogPinValue = "A0";
      break;
    case 15:                                  // D15 = A1
      analogPinValue = "A1";
      break;
    case 16:                                  // D16 = A2
      analogPinValue = "A2";
      break;
    case 17:                                  // D17 = A3
      analogPinValue = "A3";
      break;
    case 18:                                  // D18 = A4
      analogPinValue = "A4";
      break;
    case 19:                                  // D19 = A5
      analogPinValue = "A5";
      break;
    default:
      analogPinValue = "Pin Error"; 
      break;
  }
  return analogPinValue; 
  
  }

/*References_________________________________________________
https://www.arduino.cc/en/Reference/LiquidCrystalSetCursor
http://www.instructables.com/id/Connect-A-16x2-LCD-Display-To-An-Arduino/
http://www.instructables.com/id/Soil-Moisture-Sensor/
http://www.instructables.com/id/Arduino-Temperature-Sensor-Interfacing-LM35-THE-EA/
https://www.arduino.cc/en/Reference/digitalRead
http://www.instructables.com/id/ARDUINO-TEMPERATURE-SENSOR-LM35/
http://www.electroschematics.com/6519/simple-soil-moisture-sensor-arduino-project/

*/
