#include <Keypad.h>
#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>

int reset();
int opassword();
int enter = 1, code = 4000, rpass = 0, opass = 0;

Servo servo1;
int servoPin = 11;
int pos;

Adafruit_LiquidCrystal lcd_1(0);

int indicator = 12;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int pass = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Enter the password");
  Serial.println("Enter A for Reset");
  Serial.println("Enter B for Save");
  Serial.println("Enter C for Clear");
  Serial.println("Enter D for Lock");

  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Enter the passwo");
  lcd_1.setCursor(0, 1);
  lcd_1.print("rd");
  delay(1000);
  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Enter A to Reset   ");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Enter B to Save    ");
  delay(1000);
  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Enter C to Clear   ");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Enter D to Lock    ");

  servo1.attach(servoPin);
  servo1.write(0);
  pinMode(indicator, OUTPUT);
  digitalWrite(indicator, 0);
}

void loop() {
  char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.println(customKey);

    lcd_1.begin(16, 2);
    lcd_1.setCursor(0, 0);
    lcd_1.print(customKey);
    delay(50);
    lcd_1.begin(16, 2);

    switch (customKey)
    {
      case '0': pass = 10;  break;
      case '1': pass += 200;  break;
      case '2': pass = pass * pass; break;
      case '3': pass = pass * pass + 1; break;
      case '4': pass += 432; break;
      case '5': pass += 5; break;
      case '6': pass = pass + 100; break;
      case '7': pass = pass * 10; break;
      case '8': pass += 20; break;
      case '9': pass += 15; break;
      case 'A':
        code = opassword();
        break;
      case 'C': Serial.println("Clear password");
        lcd_1.setCursor(0, 0);
        lcd_1.print("Clear password");
        pass = 0; break;
      case 'D' : pass = 0; servo1.write(0);
        delay(500);
        digitalWrite(indicator, 0);
        Serial.println("Locked");
        lcd_1.setCursor(0, 0);
        lcd_1.print("    Locked  ");
        break;
      default: break;
    }
    // Serial.println(pass);
    if (pass == code)
    {
      digitalWrite(indicator, 1);
      for (pos = 0; pos <= 180; pos++)
      {
        servo1.write(pos);
        delay(10);
      }
      Serial.println("Unlocked");
      lcd_1.setCursor(0, 0);
      lcd_1.print("   Unlocked   ");
      lcd_1.setCursor(0, 1);
      lcd_1.print("Enter D to lock");

    }
  }
}

int opassword()
{
  Serial.println("Enter original password to Reset");
  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Enter Original   ");
  lcd_1.setCursor(0, 1);
  lcd_1.print("pin to Reset");
  delay(500);
  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Enter C to clear");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Enter B to Proceed");
  opass = 0;
  enter = 1;
  while (enter)
  {
    char customKey = customKeypad.getKey();
    if (customKey) {
      Serial.println(customKey);

      lcd_1.begin(16, 2);
      lcd_1.setCursor(0, 0);
      lcd_1.print(customKey);
      delay(50);
      lcd_1.begin(16, 2);

      switch (customKey)
      {
        case '0': opass = 10;  break;
        case '1': opass += 200;  break;
        case '2': opass = opass * opass; break;
        case '3': opass = opass * opass + 1; break;
        case '4': opass += 432; break;
        case '5': opass += 5; break;
        case '6': opass = opass + 100; break;
        case '7': opass = opass * 10; break;
        case '8': opass += 20; break;
        case '9': opass += 15; break;
        case 'C': Serial.println("Pin cleared, Re-Enter");
          lcd_1.setCursor(0, 0);
          lcd_1.print("Pin cleared");
          lcd_1.setCursor(0, 1);
          lcd_1.print("Re-Enter");
          rpass = 0; break;
        case 'B' : Serial.println("Proceeding");
          lcd_1.begin(16, 2);
          lcd_1.setCursor(0, 0);
          lcd_1.print("Proceeding");
          enter = 0;
          break;
        default: break;
      }
    }
  }
  if (opass == code)
  {
    opass = 0;
    code = reset();
    Serial.println("Enter the password to unlock");
    lcd_1.begin(16, 2);
    lcd_1.setCursor(0, 0);
    lcd_1.print("Enter the passwo");
    lcd_1.setCursor(0, 1);
    lcd_1.print("rd to unlock");
  }
  else
  {
    lcd_1.begin(16, 2);
    lcd_1.setCursor(0, 0);
    lcd_1.print("Failed!");
    lcd_1.setCursor(0, 1);
    lcd_1.print("Re-Enter A");
    Serial.println("Failed!, Re-Enter A");
    loop();
  }
  return code;
}

int reset()
{
  Serial.println("Enter new pin");
  Serial.println("Enter B to Save");
  Serial.println("Enter C to Clear");

  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Enter new pin");
  delay(1000);
  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("Enter B to Save   ");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Enter C to Clear    ");
  rpass = 0;
  enter = 1;
  while (enter)
  {
    char customKey = customKeypad.getKey();
    if (customKey) {
      Serial.println(customKey);

      lcd_1.begin(16, 2);
      lcd_1.setCursor(0, 0);
      lcd_1.print(customKey);
      delay(50);
      lcd_1.begin(16, 2);

      switch (customKey)
      {
        case '0': rpass = 10;  break;
        case '1': rpass += 200;  break;
        case '2': rpass = rpass * rpass; break;
        case '3': rpass = rpass * rpass + 1; break;
        case '4': rpass += 432; break;
        case '5': rpass += 5; break;
        case '6': rpass = rpass + 100; break;
        case '7': rpass = rpass * 10; break;
        case '8': rpass += 20; break;
        case '9': rpass += 15; break;
        case 'C': Serial.println("Pin cleared, Re-Enter");
          lcd_1.setCursor(0, 0);
          lcd_1.print("Pin cleared");
          lcd_1.setCursor(0, 1);
          lcd_1.print("Re-Enter");
          rpass = 0; break;
        case 'B' : Serial.print("New pin saved");
          Serial.println(rpass);
          lcd_1.begin(16, 2);
          lcd_1.setCursor(0, 0);
          lcd_1.print("New pin saved");
          lcd_1.print(rpass);
          enter = 0;
          break;
        default: break;
      }
    }
  }
  return rpass;
}