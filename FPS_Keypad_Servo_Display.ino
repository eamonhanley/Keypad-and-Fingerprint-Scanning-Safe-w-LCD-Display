#include "FPS_GT511C3.h"
#include "SoftwareSerial.h"
#include <Servo.h>
#include "Keypad.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Password.h>


 //Password
Password password = Password( "1234" ); //password to unlock, can be changed
 


//boolean
bool nm = false;
bool kp = false;
bool fn = false;
bool key1 = false;
bool key2 = false;
bool key3 = false;
bool key4 = false;
bool dp = false;
bool dpkey1 = false;
bool dpkey2 = false;
bool dpkey3 = false;
bool dpkey4 = false;

//keypad section

const byte ROWS = 4; // number of rows
const byte COLS = 3; // number of columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {8, 7, 6, 5}; // row pinouts of the keypad R1 = D8, R2 = D7, R3 = D6, R4 = D5
byte colPins[COLS] = {4, 3, 2};    // column pinouts of the keypad C1 = D4, C2 = D3, C3 = D2
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//FPS Section

FPS_GT511C3 fps(12, 13);

//Servo section

Servo myservo;  // create servo object to control a servo
// a maximum of eight servo objects can be created

//Liquid crystal display

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


void setup() {

  //keypad/Password

  Serial.begin(9600);
  Serial.write(254);
  Serial.write(0x01);
  delay(200); 

  keypad.addEventListener(keypadEvent); //add an event listener for this keypad

  //FPS

  delay(100);
  fps.Open();
  fps.SetLED(true);

  //Servo
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  //Liquid crystal display
  lcd.begin(16, 2);

  //Password
  
}

void loop() {


///////////NEW CODE

char key = keypad.getKey();
if (nm == false){
  lcd.print("Enter: ");
  delay(200);
  lcd.clear();
}


    if (kp == true && fps.IsPressFinger())
  {
     dp = true;
     checkFinger();
  }

    if (kp == true && dp == false){
      //Serial.print("Press Finger");
      //Serial.write(254);
      //delay(1000);

      lcd.print("Press Finger");
      delay(200);
      lcd.clear();
      
    }
    

   //if (kp == false && dp == false){
    //  Serial.print("Press Keypad");
   // }
}

///////////////////////////////////////////////////////////////
//Functions

void unlock () {
  myservo.write(180);
}

void lock () {
  myservo.write(90);
}

void checkPassword(){
  
if (password.evaluate() && kp == false){  //if password is right open
    
    Serial.println("Accepted");
    Serial.write(254);delay(1000);

    lcd.print("Accepted");
      delay(1000);
      lcd.clear();
    
    kp = true;
    //fn = true;
    //dp = true;
    
}else if (kp == false){
    Serial.println("Denied"); //if passwords wrong keep locked
    Serial.write(254);delay(100);

    lcd.print("Denied");
      delay(1000);
      lcd.clear();
    //add code to run if it did not work
    
    
}
}

void checkFinger(){
   {

    fps.CaptureFinger(false);
    int id = fps.Identify1_N();

    if (id < 200)
    {
      fn = true;
      Serial.print("Verified ID:");
      Serial.println(id);

      lcd.print("Verified ID:");
      lcd.print(id);
      delay(3000);
      lcd.clear();

      //Serial.print(myservo.read());
      unlock();
      //dp = true;
      lcd.write("Open");
      delay(2000);
      lcd.clear();
      Serial.println("Open");

    
  }
  else if (kp == true && dp == true && fn == false) {
      Serial.println("Finger not found");
      Serial.write(254); delay(200);
 
      lcd.print("Finger not found");
      delay(1000);
      lcd.clear();
      kp = false;
      dp = false;
      fn = false;
      nm = false;

    }
}
}

void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
  
  case PRESSED:
  
  nm = true;

  lcd.print("*");
      delay(1000);
      
  Serial.print("Enter:");
  Serial.println(eKey);
  delay(10);
  
  Serial.write(254);
  
  switch (eKey){
    case '*': lcd.clear(); checkPassword(); delay(1); break;
    
    case '#': password.reset(); delay(1); break;
    
    case '0': lock(); kp = false; dp = false; fn = false; nm = false;
     
     default: password.append(eKey); delay(1);
}
}
}



