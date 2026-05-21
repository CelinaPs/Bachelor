// This code is all AI generated
// Link to the AI conversation: https://claude.ai/share/c8f22a11-e3df-4f2f-92bc-3d8085a3306d 

#include <Keypad.h>
#include <Servo.h>

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3};

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

Servo lockServo;
const int SERVO_PIN = 12;
const String password = "573";
String input_password;
bool isUnlocked = false;

void setup() {
  Serial.begin(9600);
  input_password.reserve(32);
  lockServo.attach(SERVO_PIN);
  lockServo.write(90); // locked position
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '*') {
      input_password = "";
      if (isUnlocked) {
        lockServo.write(90); // re-lock on *
        isUnlocked = false;
        Serial.println("locked");
      }
    } else if (key != '#') {
      input_password += key;

      if (input_password.length() == password.length()) {
        if (password == input_password) {
          Serial.println("password is correct");
          lockServo.write(0); // rotates the other way
          isUnlocked = true;
        } else {
          Serial.println("password is incorrect, try again");
        }
        input_password = "";
      }
    }
  }
}