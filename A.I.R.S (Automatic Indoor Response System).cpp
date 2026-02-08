#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>  

#define DHTPIN 7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);  

#define GasSensor A0

#define GAS_THRESHOLD 400 // Adjust based on your sensor's calibration
#define TEMP_THRESHOLD 37.0 // Adjust based on your requirements 

#define RLED 3
#define GLED 5
#define FAN_PIN 9
#define buzz 11

#define RESET_BTN 2 // Main purpose was to reset the system       

LiquidCrystal_I2C lcd(0x27, 16, 2);

void(* resetFunc) (void) = 0; // Function pointer to reset Arduino  

void setup() {
  pinMode(RLED, OUTPUT);
  pinMode(GLED, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(buzz, OUTPUT); 

  pinMode(RESET_BTN, INPUT_PULLUP);

  digitalWrite(FAN_PIN, LOW);
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, HIGH);
  digitalWrite(buzz, LOW);

  Serial.begin(9600);
  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Starting");
  delay(1000);
  lcd.clear();
}

void loop() {

  // RESET
  if (digitalRead(RESET_BTN) == LOW) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" System Reset ");
    digitalWrite(FAN_PIN, LOW);
    digitalWrite(RLED, LOW);
    digitalWrite(GLED, LOW);
    digitalWrite(buzz, LOW);
    delay(1000);
    resetFunc();   // restart Arduino
  }

  int gasValue = analogRead(GasSensor);
  float temp = dht.readTemperature();

  if (isnan(temp)) { // Check if temperature reading is valid
    lcd.setCursor(0, 0);
    lcd.print(" Temp Error     ");
    delay(1000);
    return;
  }

  bool gasDanger  = gasValue > GAS_THRESHOLD;
  bool tempDanger = temp >= TEMP_THRESHOLD;

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print((char)223);
  lcd.print("C   ");  

  lcd.setCursor(0, 1);
  lcd.print("Gas: ");
  lcd.print(gasValue);
  lcd.print("   ");

 if (gasDanger || tempDanger) {
  digitalWrite(FAN_PIN, HIGH);
  digitalWrite(RLED, HIGH);       
  digitalWrite(GLED, LOW);
  digitalWrite(buzz, HIGH);
 } else {
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(RLED, LOW);
  digitalWrite(GLED, HIGH);
  digitalWrite(buzz, LOW);
 }

  delay(1000); 
}
