//sensor parameters

#define echoPin1  6  // controlled by car/app
#define trigPin1  5
#define echoPin2  11 // controlled by cube
#define trigPin2  10


// sum for computing the average raw sensor value
float d1 = 0;
float d2 = 0;
float d3 = 0;
float distance = 0;

// motor parameters
#include <Servo.h>
Servo servo_motor; 
int servoMotorPin = 9;        // the servo motor is attached to the 9th Pulse Width Modulation (PWM)Arduino output




float setpoint = 20;
int period = 10;
float elapsedTime, time, timePrev;        //Variables for time control
float distance_previous_error, distance_error;
float PID_p, PID_i, PID_d, PID_total;

float kp=10;            //0.00001 0.00001 25     0.03 0.000001 0       // proportional control 
float ki=0;                         // integral control
float kd=0;                        // derivative control
     
       // discretization constant, that is equal to the total control loop delay, the number 32 is obtained by measuring the time it takes to execute a single loop iteration                         
float servoAngle;



float measure_1 (void);

void setup() 
{
  Serial.begin(9600);

  time = millis();

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  servo_motor.attach(servoMotorPin);
  servo_motor.write(90);
  delay(200);

}

void loop() 
{
//  unsigned long startTime = micros(); // this is used to measure the time it takes for the code to execute
  // obtain the sensor measurements
  if (millis() > time+period)
  {
    time = millis();    

    d1 = d2;
    d2 = d3;
    d3 = measure_1();
    distance = (d3 + d2 + d1) / 3;  


    // distance = 0;
    // for(int i = 0; i < 10; i ++){
    //   distance += measure_1();
    //   delayMicroseconds(10);
    // }
    // distance /= 10;

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
  
    PID_total = PID_p;  
    PID_total = map(PID_total, -150, 150, 0, 150);
  
    if(PID_total < 40){PID_total = 40;}
    if(PID_total > 90) {PID_total = 90; } 
  
    servo_motor.write(PID_total+30);  
    distance_previous_error = distance_error;

    Serial.println(distance_error);
    Serial.print(",");
    Serial.println(PID_total + 30);
  }
  


  
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