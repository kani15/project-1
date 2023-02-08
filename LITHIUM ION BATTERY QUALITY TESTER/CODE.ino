#include <LiquidCrystal.h>  //Default Arduino LCD Librarey is included 
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8; //Mention the pin number for LCD connection
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const float BAT_LOW = 3.0;    //to define the low voltage limit of the Li Ion cell
const float BAT_HIGH = 4.5;   //to define the high voltage limit of the cell
const int MOSFET_Pin=9;
const int PWM_VALUE=150;
unsigned long previousMillis = 0; // Previous time in ms
unsigned long millisPassed = 0;  // Current time in ms
float Capacity=0;     //Variable to define the battery Capacity
float Resistor=2.2;   // Load Resistor Value is 2.5ohms
float mA;

void setup() 
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  lcd.begin(20, 4); //Initialise 16*2 LCD
  lcd.setCursor(3,1); // Set the cursor on the first column and first row.
  lcd.print("BATTERY QUALITY");
  lcd.setCursor(3,2);
  lcd.print("TESTER CIRCUIT");
  delay(2000);
  lcd.clear();
}

void loop() 
{
  analogWrite(MOSFET_Pin, PWM_VALUE);
  // read the input on analog pin 0:
  int sensorValue_voltage_Cell = analogRead(A0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  delay(10);
  float voltage = sensorValue_voltage_Cell * (5.00 / 1023.0);
  Serial.print("VOLTAGE= ");
  Serial.println(voltage);      // Here the voltage is being printed on Serial Monitor
  lcd.setCursor(0,0); // Set the cursor on the first column and first row.
  lcd.print("VOLTAGE= "); // Print the voltage reading on the screen
  lcd.setCursor(9,0);
  lcd.print(voltage);
  lcd.setCursor(13,0);
  lcd.print(" Volts");

  int sensorValue_Shunt_Resistor= analogRead(A1);
  delay(10);
  float voltage1= sensorValue_Shunt_Resistor *(5.00 / 1023.0);
  float current= voltage1/Resistor;
  Serial.print("CURRENT= ");
  Serial.println(current);
  lcd.setCursor(0, 1); //Set the cursor on the first column and the second row (counting starts at 0!).
  lcd.print("CURRENT= ");
  lcd.setCursor(9,1);  
  lcd.print(current); 
  lcd.setCursor(13,1);
  lcd.print(" Amps");

  float power= voltage*current;
  Serial.print("POWER = ");
  Serial.println(power);
  lcd.setCursor(0, 2);
  lcd.print("POWER  = ");
  lcd.setCursor(9, 2);
  lcd.print(power);
  lcd.setCursor(13, 2);
  lcd.print(" Watt");

  float temp = analogRead(A2) * (5.0/1024.0) * 100;
  delay(10);
  Serial.print("OP TEMP=");
  Serial.println(temp);
  lcd.setCursor(0, 3); //Set the cursor on the first column and the second row (counting starts at 0!).
  lcd.print("OP TEMP=");
  lcd.setCursor(9,3);
  lcd.print(temp); 
  lcd.setCursor(13,3);
  lcd.print(" DEG.C");


  if ( voltage > BAT_HIGH)
  {
    digitalWrite(MOSFET_Pin, LOW);    // Turned Off the MOSFET , No discharge 
    Serial.println( "Warning High-V! ");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("HIGH VOLTAGE!!");
    delay(2000);
    lcd.clear();
  }
  else if(voltage < BAT_LOW)
  {
      digitalWrite(MOSFET_Pin, LOW);      // Turned Off the MOSFET , No discharge 
      Serial.println( "Warning Low-V! ");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Low Voltage!!!");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CAPACITY:");
      lcd.setCursor(0,1);
      lcd.print(Capacity);
      delay(10000);
  }
  else if(voltage > BAT_LOW && voltage < BAT_HIGH  ) 
  { // Check if the battery voltage is within the safe limit
      millisPassed = millis() - previousMillis;     
      mA = current * 1000.0 ;
      Capacity = Capacity + (mA * (millisPassed / 3600000.0)); // 1 Hour = 3600000ms to convert it into mAh units
      previousMillis = millis();
      //Serial.print("DATA,TIME,"); Serial.print(voltage); Serial.print(","); Serial.println(Capacity);       //uncomment this line to diplay on serial monitor
      delay(1000); 
      lcd.clear();
     }
}
