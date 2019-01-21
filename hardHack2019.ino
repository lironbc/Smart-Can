/*
 * Author: Liron Borwick-Cooper
 * Date: January 20th, 2019
 * Resources: http://www.circuitbasics.com/how-to-set-up-an-ultrasonic-range-finder-on-an-arduino/
 * https://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay
 */
  
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.   


  int trashDepth;
  boolean isClean;
  int startCount; //where we begin counting from for timing
  float duration, distance;

void setup() {
  
  #define TRIG_PIN 3 
  #define ECHO_PIN 4
  #define TRIG_DELAY 10
  #define SPEED_OF_SOUND 340
  #define SIZE_PERCENTILES 10

  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  lcd.backlight();   // Turn on the backligt (try lcd.noBaklight() to turn it off)
  lcd.setCursor(0,0); //First line
  lcd.print("Initializing");
  

   isClean = true;
   
    //Reads initial trash depth
    digitalWrite(TRIG_PIN, LOW); //Resets the trigger
    delayMicroseconds(2);

   //Read ultrasonic range finder documentation for explanation on this
   digitalWrite(TRIG_PIN, HIGH);
   delayMicroseconds(TRIG_DELAY);
   digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN,HIGH);
  trashDepth = duration / 58;
  startCount = 0;

}

void loop() {
  int percentFull;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(TRIG_DELAY);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN,HIGH);
  distance = duration / 58;

  //Set max a min distances because rangefinder occasionally gives garbage values
    if (distance < 0) {
      distance = 0;
    }
    else if(distance > trashDepth){
    distance = trashDepth;
    }
 
    lcd.clear();
    lcd.setCursor(0, 0); 
    percentFull = trashDepth - distance;
    percentFull = (int)(percentFull * 100 / trashDepth);
    lcd.print("                   "); //Clears the first line

    if(percentFull > 80){
      lcd.setCursor(0, 0); 
      lcd.print("Change the trash");
    }
    else{
    lcd.setCursor(0, 0); 
    lcd.print(percentFull);
    lcd.print(" % full");
    }
delay(10000);
}
