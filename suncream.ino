#define PAN 8
#define TILT 7
#define SHOOT 6
#define PAN_MIN 50
#define PAN_MAX 130
#define TILT_MIN 40
#define TILT_MAX 120
#define SHOOT_LENGTH 10

#include <ArduinoJson.h>
#include <Servo.h>

Servo panServo;  // create servo object to control a servo
Servo tiltServo;  

int pan = 90;    // variable to store the servo position
int tilt = 90;    // variable to store the servo position

void setPosition(StaticJsonDocument<300> &doc) {
  int newPan = doc["data"]["pan"].as<int>();
  int newTilt = doc["data"]["tilt"].as<int>();

  if (newTilt > TILT_MAX) {
    tilt = TILT_MAX;
  } 
  else if (newTilt < TILT_MIN) {
    tilt = TILT_MIN;
  } 
  else {
    tilt = newTilt;
  }

  if (newPan > PAN_MAX) {
    pan = PAN_MAX;
  } 
  else if (newPan < PAN_MIN) {
    pan = PAN_MIN;
  } 
  else {
    pan = newPan;
  }

  panServo.write(pan);              // tell servo to go to position in variable 'pos'
  tiltServo.write(tilt);
  
}

void setup() {
  Serial.begin(57600);
  panServo.attach(PAN);  // attaches the servo on pin 9 to the servo object
  tiltServo.attach(TILT);  // attaches the servo on pin 9 to the servo object    
  pinMode(SHOOT, OUTPUT);
  analogWrite(SHOOT, 0);
}

void loop() {
  panServo.write(pan);              // tell servo to go to position in variable 'pos'
  tiltServo.write(tilt); 
  
  if (Serial.available()) {
    // Allocate the JSON document
    StaticJsonDocument<300> doc;
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok) {
      String event = doc["event"];

      if (event == "position") {
        setPosition(doc);
      }

      if (event == "shoot") {
        setPosition(doc);
        delay(200);  // let it get to the position

        analogWrite(SHOOT, 255);
        delay(SHOOT_LENGTH);
        analogWrite(SHOOT, 0);
    
      }

    }
    else {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Flush all bytes in the "link" serial port buffer
      while (Serial.available() > 0)
        Serial.read();
    }
  }
  
  
  delay(10);
}
