//sensor parameters

#define echoPin1  6  // controlled by car/app
#define trigPin1  5
#define echoPin2  11 // controlled by cube
#define trigPin2  10

#define app_pin 3
#define sensor_pin 4
#define mid_pin 2


bool from_app = false; 
bool from_sensor = false;

int i = 0;

// sum for computing the average raw sensor value
float d1 = 0;
float d2 = 0;
float d3 = 0;
float distance = 0;

// motor parameters
#include <Servo.h>
Servo servo_motor; 
int servoMotorPin = 9;        



float setpoint = 22;
int period = 10;
float elapsedTime, time, timePrev;        //Variables for time control
float distance_previous_error, distance_error;
float PID_p, PID_i, PID_d, PID_total, PID_total_last;

float kp=6.3;            
float ki=0.25;                         
float kd=380;                       
     
float servoAngle;


float measure_1 (void);
float measure_2 (void);

void setup() 
{
  Serial.begin(9600);

  time = millis();

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(app_pin, OUTPUT);
  pinMode(sensor_pin, OUTPUT);
  pinMode(mid_pin, OUTPUT);
  pinMode(12, INPUT_PULLUP);
  digitalWrite(mid_pin, HIGH);

  servo_motor.attach(servoMotorPin);
  servo_motor.write(90);
  PID_total_last = 300;
  delay(200);

}

void loop() 
{
//  unsigned long startTime = micros(); // this is used to measure the time it takes for the code to execute
  // obtain the sensor measurements

  if(digitalRead(12) == LOW){
    digitalWrite(2 + i, LOW);
    i ++;
    i %= 3;
    digitalWrite(2 + i, HIGH);

    if(i == 0){
      from_app = false;
      from_sensor = false;
      setpoint = 22;
      Serial.println("Center");

    }else if(i == 1){
      Serial.println("Taking input from app");
      from_app = true;


    }else if(i == 2){
      from_app = false;
      from_sensor = true;
      Serial.println("Taking input from sensor");

    }


    while(digitalRead(12) == LOW){
      delay(200);
    }

  }



  if (millis() > time+period)
  {
    time = millis();    

    d1 = d2;
    d2 = d3;
    d3 = measure_1();
    distance = (d3 + d2 + d1) / 3;  



    if(from_sensor){
          delay(30);
            setpoint = measure_2();

    }

    if(from_app){
          if (Serial.available() > 0) {
          int serialMessage = Serial.parseInt();
          delay(10);
          Serial.print("new setpoint : ");
          Serial.println(serialMessage);
          setpoint = serialMessage;
        }
    }

    distance_error = setpoint - distance;   
    PID_p = kp * distance_error;
    float dist_diference = distance_error - distance_previous_error;     
    PID_d = kd*((distance_error - distance_previous_error)/period);
      
    if(-3 < distance_error && distance_error < 3)
    {
      PID_i = PID_i + (ki * distance_error);
    }
    else
    {
      PID_i = 0;
    }
  
    PID_total = PID_p + PID_d + PID_i;  
    PID_total = map(PID_total, -150, 150, 0, 150);
  
    if(PID_total < 25){PID_total = 25;}
    if(PID_total > 120) {PID_total = 120; } 

    if((distance_error < -1 || distance_error > 1) && (abs(PID_total-PID_total_last) > 3)){
      servo_motor.write((int) PID_total+30);
      Serial.println(PID_total+30);
      // // Serial.println((int) PID_total+30); 
      // Serial.println("->>");
      // Serial.println(i);
      
    }
    distance_previous_error = distance_error;
    PID_total_last = PID_total;
    
    
  }
  
  // Serial.println("->>");
  // Serial.println(i);
  
  delay(10); // uncomment this to introduce an additional delay
 
}

float measure_1 (void) {

long durata=0;
float distanza=0; 

digitalWrite(trigPin1, LOW); 
delayMicroseconds(10); 

digitalWrite(trigPin1, HIGH);
delayMicroseconds(10);
 
digitalWrite(trigPin1, LOW);

durata = pulseIn(echoPin1, HIGH);
distanza = (float)durata/58.2;

delay(10);

if (distanza > 42) distanza=43;
else if (distanza < 0) distanza=0;

return (distanza);   // meters   

}

float measure_2 (void) {

long durata=0;
float distanza=0; 

digitalWrite(trigPin2, LOW); 
delayMicroseconds(10); 

digitalWrite(trigPin2, HIGH);
delayMicroseconds(10);
 
digitalWrite(trigPin2, LOW);

durata = pulseIn(echoPin2, HIGH);
distanza = (float)durata/58.2;

delay(10);

if (distanza > 42) distanza=43;
else if (distanza < 0) distanza=0;

// Serial.println("->");
// Serial.println(distanza);
return (distanza);   // meters   

}
