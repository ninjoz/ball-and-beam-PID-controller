
#include <Servo.h>
Servo servo;
    // ********HERE******* //
#define Umax 66 // maximum angle of the servomotr in degrees
#define Umin -66 // minimun angle
#define Umax_rad 1.151  // maximum angle of the servomotr in radiants
#define Umin_rad -1.151 // minimun angle
#define T 0.09 // sampling time  // time microcontroller needs to complete an entire loop

#define echoPin1  6  // controlled by car/app
#define trigPin1  5
#define echoPin2  11 // controlled by cube
#define trigPin2  10

double setpoint, setpoint_prev;  // (position of the cube)  // In meters : 30cm --> 0.3m
double y, y_prev;  // output variable (position of the car)
double error;
double P, I, D, U;  // the three components of the PID algorithm, the control variable U
double I_prev = 0, U_prev = 0, D_prev = 0;        
boolean Saturation = false;
boolean appflag = false;
char controlSignal;

double Kp = 8.6;
double Ki = 1.1;
double Kd = 6.3;

float measure_1 ();
float measure_2 ();
void move_servo(int);
void cube_controlled();
void app_controlled();

void setup() {
  
  Serial.begin(9600);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  servo.attach(9);

  delay(1000);
  move_servo(90);
  delay(2000);
  setpoint_prev = measure_2(); // cube  (the previous value to measure the error)
  delay(1000);
  y_prev = measure_1(); // car  (the previous value to measure the error)
  delay(1000);

}

void loop() {
  
  cube_controlled();

  while(appflag){
    app_controlled();
  }

}

float measure_1(){ // it measures the distance of the car from sensor number one


  long duration = 0;
  float distance = 0;

  digitalWrite(trigPin1, LOW); 
  delayMicroseconds(10); 

  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin1, LOW);

  duration = pulseIn(echoPin1, HIGH);
  distance = (float)duration/58.2;

  delay(30);
      // ********HERE******* //
  if (distance > 42) distance = 43; // range of the distance the car can go
  else if (distance < 0) distance = 0;
                // ********HERE******* //
  return 0.01*(distance-1.5+0.5); // meters

}

float measure_2(){ // it measures the distance of the cube from sensor number two

  long duration = 0;
  float distance = 0;

  digitalWrite(trigPin2, LOW); 
  delayMicroseconds(10); 

  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin2, LOW);

  duration = pulseIn(echoPin2, HIGH);
  distance = (float)duration/58.2;
             // ********HERE******* //                     
  if(distance > 42) distance = 43;
  else if(distance < 0) distance =0;
                                                        // ********HERE******* //
  return 0.01*(distance+2); // meters ( +2 to measure the center of the cube)
  
}

void move_servo(int u){
              // from min max, to min max
  servo.write(u-map(u, 30, 150, 14, 3));

}

void cube_controlled(){
  
  setpoint = measure_2();  // distance of the cube from the sensor
  setpoint = 0.53*setpoint + 0.47*setpoint_prev; // for filtering from noises

  delay(3);

  y = measure_1();
  y = 0.53*y + 0.47*y_prev;  // for filtering from noises

  delay(3);

  error = round( 100*(y - setpoint) )*0.01;  // meters 

  // ** The start of computing the PID algorithm three components ** //

  P = Kp*error;  // the proportional action

  if( !Saturation ) I = I_prev + T*Ki*error;  // the integral action

  D = 0.56*D + 0.44*D_prev;  // the derivative action  // filtering D

  U = P + I + round(100*D)*0.01 ;  // the total control action in radiants

  // ** If we are outside the admissble limits of the control variable, we have to limite it ** //

  if ( U < Umin_rad)  {
                        U=Umin_rad; 
                        Saturation = true;
                       }
                   
   else if ( U > Umax_rad)  {
                             U=Umax_rad; 
                             Saturation = true;
                            }

   else     Saturation = false;

   U=round(U*180/M_PI);     // Transform U in degrees. Now I have :   -63° < U < 63°

   U=map(U, Umin, Umax, 24, 156); // I map the computed value of U to the corresponding value of the servomotor
   
   if (U < 83 || U > 95 || abs(error) > 0.02 ) move_servo( round(U) );   // I continue until I have error and the control action U is greater than a threshold.
   
   delay (24);

   I_prev = I;
   y_prev = y;
   D_prev = D;
   setpoint_prev = setpoint;

   while(Serial.available()){
    controlSignal = Serial.read();
    delay(5);
    if(controlSignal == 'A'){
      appflag = !appflag;
    }
  }

}

void app_controlled(){
  double new_y;
  while(Serial.available()){
    new_y = Serial.read();
  }

  setpoint = measure_2();  // distance of the cube from the sensor
  setpoint = 0.53*setpoint + 0.47*setpoint_prev; // for filtering from noises

  delay(3);

  /*y = measure_1();
  y = 0.53*y + 0.47*y_prev;  // for filtering from noises

  delay(3);*/

  error = round( 100*(new_y - setpoint) )*0.01;  // meters 

  // ** The start of computing the PID algorithm three components ** //

  P = Kp*error;  // the proportional action

  if( !Saturation ) I = I_prev + T*Ki*error;  // the integral action

  D = 0.56*D + 0.44*D_prev;  // the derivative action  // filtering D

  U = P + I + round(100*D)*0.01 ;  // the total control action in radiants

  // ** If we are outside the admissble limits of the control variable, we have to limite it ** //

  if ( U < Umin_rad)  {
                        U=Umin_rad; 
                        Saturation = true;
                       }
                   
   else if ( U > Umax_rad)  {
                             U=Umax_rad; 
                             Saturation = true;
                            }

   else     Saturation = false;

   U=round(U*180/M_PI);     // Transform U in degrees. Now I have :   -63° < U < 63°

   U=map(U, Umin, Umax, 24, 156); // I map the computed value of U to the corresponding value of the servomotor
   
   if (U < 83 || U > 95 || abs(error) > 0.02 ) move_servo( round(U) );   // I continue until I have error and the control action U is greater than a threshold.
   
   delay (24);

   I_prev = I;
   //y_prev = y;
   D_prev = D;
   setpoint_prev = setpoint;

}
