//sensor parameters

#define echoPin1  6  // controlled by car/app
#define trigPin1  5
#define echoPin2  11 // controlled by cube
#define trigPin2  10


// sum for computing the average raw sensor value
float distance;

// motor parameters
#include <Servo.h>
Servo servo_motor; 
int servoMotorPin = 3;        // the servo motor is attached to the 9th Pulse Width Modulation (PWM)Arduino output


// control parameters
float desiredPosition=20;     // desired position of the ball
float errorK;                 // position error at the time instant k
float errorKm1=0;             // position error at the time instant k-1
float errorKm2=0;             // position error at the time instant k-2
float controlK=0;             // control signal at the time instant k
float controlKm1=0;           // control signal at the time instant k-1
int delayValue=0;             // additional delay in [ms]

float Kp=0.005;            //0.00001 0.00001 25            // proportional control 
float Ki=0.0000001;                         // integral control
float Kd=0;                        // derivative control
float h=(delayValue+32)*0.00001;       // discretization constant, that is equal to the total control loop delay, the number 32 is obtained by measuring the time it takes to execute a single loop iteration                         

float keK=Kp*(1+h/Ki+Kd/h);               // parameter that multiplies the error at the time instant k
float keKm1=-Kp*(1+2*Kd/h);               // parameter that multiplies the error at the time instant k-1                    
float keKm2=Kp*Kd/h;                      // parameter that multiplies the error at the time instant k-2

float measure_1 (void);

void setup() 
{
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  servo_motor.attach(servoMotorPin);
}

void loop() 
{
//  unsigned long startTime = micros(); // this is used to measure the time it takes for the code to execute
  // obtain the sensor measurements
  

  distance = measure_1();

  
  errorK=desiredPosition-distance; // error at the time instant k;

  // compute the control signal
  controlK=controlKm1+keK*errorK+keKm1*errorKm1+keKm2*errorKm2;

  // update the values for the next iteration
  controlKm1=controlK;
  errorKm2=errorKm1;
  errorKm1=errorK;

  servo_motor.write(94+controlK); // the number 94 is the control action necessary to keep the ball in the horizontal position
 // Serial.println((String)"Control:"+controlK+(String)"---Error:"+errorK);

 // these three lines are used to plot the data using the Arduino serial plotter 
   //Serial.println("error ");
int ff=94+controlK;
// Serial.println("error ");

//   Serial.println("controlk ");
    Serial.println(errorK);
    Serial.print(",");
    Serial.println(controlK);
//   Serial.println("distace ");
//   Serial.println(distance);
//   Serial.println("servo angle ");
//   Serial.println(ff % 180);
//   delay(500);

  //unsigned long endTime = micros();
 // unsigned long deltaTime=endTime-startTime;
 // Serial.println(deltaTime);
  
  delay(100); // uncomment this to introduce an additional delay
 
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

delay(30);

if (distanza > 42) distanza=43;
else if (distanza < 0) distanza=0;

return (distanza);   // meters   

}
