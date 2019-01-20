#include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address, if it's not working try 0x27.   

int trashDepth;
boolean isClean;
int startCount; //where we begin counting from for timing
boolean firstRun = true;

void setup() {
  // put your setup code here, to run once:
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
  trashDepth = 100;
  startCount = 0;

}

void loop() {
  // put your main code here, to run repeatedly:
  float duration, distance;
  int percentFull;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(TRIG_DELAY);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN,HIGH);
  distance = duration / 58;
  if(firstRun == true){
    trashDepth = distance ; 
    lcd.clear();
   }
  distance =distance -3;  
  if(distance > trashDepth){
    distance = trashDepth;
  }
  else if (distance < 0) {
    distance = 0;
  }
  
  if(firstRun == false){
    lcd.setCursor(0, 0); 
    percentFull = trashDepth - distance;
    percentFull = (int)(percentFull * 100 / trashDepth);
    lcd.print("                   ");

    if(percentFull > 80){
      lcd.setCursor(0, 0); 
      lcd.print("Change the trash");
    }
    else{
    lcd.setCursor(0, 0); 
    lcd.print(percentFull);
    lcd.print(" % full");
    }

//    lcd.setCursor(0,1);
//    lcd.print("full");
  }


firstRun=false;

//  Serial.print(distance);
//  Serial.println(" cm");
delay(2000);
}
