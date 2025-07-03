#include <Servo.h>
#include <Arduino_LSM6DSOX.h>
Servo myservo;
#define Fan_with_speed_ctl 5 
#define Skirt_fan 7
const int trigPin = 13;
const int echoPin = 3;

// defines variables
long duration;
int distance;
int left_d, right_d, front_d;


float Ax, Ay, Az;
float Gx, Gy, Gz;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  
  myservo.attach(9);
  Intial_Hovercraft_setup();


  while(!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");
  Serial.println();

  Serial.print("Gyroscope sample rate = ");  
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println("Hz");
  Serial.println();

}

void loop() {
  distance = dist();
  Serial.print("Distance: "); // Prints the distance on the Serial Monitor
  Serial.println(distance);

  if (distance <= 15) { // Wall in front
    digitalWrite(Fan_with_speed_ctl, LOW);
    delay(2900);
    
    switch (myservo.read()) {
      case 0: 
        myservo.write(90);
        delay(1000);
        front_d = dist();
        myservo.write(180);
        delay(1000);
        right_d = dist();

        if (front_d > right_d)
          myservo.write(90);
        else
          myservo.write(180);
        delay(1000);
        speedIncrease();
        break;
      
      case 90:
        front_d = dist();
        delay(100);
        myservo.write(0);
        delay(1000);
        left_d = dist();
        myservo.write(180);
        delay(1000);
        right_d = dist();

        if (left_d > right_d) {
          myservo.write(0);
          delay(1000);
          speedIncrease2();
        } else {
          myservo.write(180);
          delay(1000);
          speedIncrease();
          if (front_d < 13) delay(750);  // Combine delay = 1500
        }
        break;

      case 180:
        myservo.write(0);
        delay(1000);
        left_d = dist();
        myservo.write(90);
        delay(1000);
        front_d = dist();

        if (left_d > front_d) {
          myservo.write(0);
          delay(1000);
          speedIncrease2();
        } else {
          myservo.write(90);
          delay(1000);
          speedIncrease();
        }
        break;
    }
  }


  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(Ax, Ay, Az);

    Serial.println("Accelerometer data: ");
    Serial.print(Ax);
    Serial.print('\t');
    Serial.print(Ay);
    Serial.print('\t');
    Serial.println(Az);
    Serial.println();
  }

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(Gx, Gy, Gz);
    
    Serial.println("Gyroscope data: ");
    Serial.print(Gx);
    Serial.print('\t');
    Serial.print(Gy);
    Serial.print('\t');
    Serial.println(Gz);
    Serial.println();
  }

delay(500);
}

int dist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void speedIncrease() {
  for (int i = 100; i < 256; i += 2) {
    analogWrite(Fan_with_speed_ctl, i);
    delay(15);
  }
  delay(1300);
  myservo.write(90);
}

void speedIncrease2() {
  for (int i = 100; i < 220; i += 2) {
    analogWrite(Fan_with_speed_ctl, i);
    delay(15);
  }
  delay(1300);
  myservo.write(90);  // Adjusted to 90
}

void Intial_Hovercraft_setup() {
  pinMode(Fan_with_speed_ctl, OUTPUT);
  pinMode(Skirt_fan, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myservo.write(90);  // Initial servo position adjusted to 90
  digitalWrite(Fan_with_speed_ctl, HIGH);
  digitalWrite(Skirt_fan, HIGH);
}
