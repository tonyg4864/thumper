/*
  Test WAV Volume.c

  Version 0.94 for use with SimpleIDE 9.40 and its Simple Libraries

  Play back a .wav file and try a few different volume settings.   
  
  http://learn.parallax.com/propeller-c-simple-devices/play-wav-files
*/

#include "simpletools.h"
#include "wavplayer.h"
#include "servo.h" 
#include "ping.h" 

int LEFT_SERVO_PIN = 3;
int RIGHT_SERVO_PIN = 4;
int HEAD_SERVO_PIN = 19;
int PINGER_PIN = 18;
int V_PANT_PIN = 14;
int H_PANT_PIN = 15;

//PWM Freq in KHz
HB25_FREQ = 9200;

int HB25_FULL_FORWARD_MS = 2200;
int HB25_NEUTRAL_MS = 1500;
int HB25_FULL_REVERSE_MS = 1000;

setupSDCard(){
  int DO = 22, CLK = 23, DI = 24, CS = 25;    // SD I/O pins
  sd_mount(DO, CLK, DI, CS);                  // Mount SD card
}

playSound(const char soundFileName[]){
  //const char techloop[] = {"techloop.wav"};   // Set up levels string
  wav_play(soundFileName);                         // Pass to wav player
  wav_volume(10);                              // Adjust volume
  pause(165900);                               
  //pause(1500000);
}

leftWheelsForward(){
  pwm_set(LEFT_SERVO_PIN,0,HB25_FULL_FORWARD_MS);
  pause(20);

}

stopLeftWheels(){
  pwm_set(LEFT_SERVO_PIN,0,HB25_NEUTRAL_MS);
  pause(20);
}

leftWheelsReverse(){

  pwm_set(LEFT_SERVO_PIN,0,HB25_FULL_REVERSE_MS);
  pause(20);
}

rightWheelsForward(){
   pwm_set(RIGHT_SERVO_PIN,1,HB25_FULL_FORWARD_MS);
   pause(20);
}

rightWheelsReverse(){
  pwm_set(RIGHT_SERVO_PIN,1,HB25_FULL_REVERSE_MS);
  pause(20);
}

stopRightWheels(){
   pwm_set(RIGHT_SERVO_PIN,1,HB25_NEUTRAL_MS);
   pause(20);
}

driveForward(){
  print("driving forward\n");
  leftWheelsForward();
  rightWheelsForward();
  pause(20);
}

driveReverse(){

  print("driving reverse\n");
  leftWheelsReverse();
  rightWheelsReverse();
  pause(20);
}

stop(){
  print("stopping...\n");
  stopLeftWheels();
  stopRightWheels();
}

printPinInfo(int pin){
  /*The pin's state. If the pin is an output, 1 = 3.3 V 
  and 0 = 0 V. If the pin is an input, 1 means V > 1.65 V, 0 means it's less.*/
  int pinState = get_state(pin);

  //I/O pin direction as seen by the cog that runs the function.
  int pinDirection = get_direction(pin);

  //In a register bit for I/O pin, either 1 or 0.
  int pinOutput = get_output(pin);

  print("pin %d state: %d\n", pin, pinState);
  print("pin %d direction: %d\n", pin, pinDirection);
  print("pin %d output: %d\n", pin, pinOutput);

}

sendLowSignal(int pin){
   set_direction(pin, 1);
  low(pin);
}

hb25Init(int pin){
  print("hb-25 init started on pin: %d\n", pin);
  sendLowSignal(pin);

  pause(5000);
  print("getting input from mc\n");
  set_direction(pin, 0);
  
  while( get_state(pin) == 0){
    //print("Wating for hb-25 to start...\n");
  } 

  sendLowSignal(pin);

  printPinInfo(pin);
  pause(20);
  printPinInfo(pin);
  
}

hb25InitAll(){
  pwm_start(HB25_FREQ);
  hb25Init(LEFT_SERVO_PIN);
  hb25Init(RIGHT_SERVO_PIN);
}

rampUp(){
  //pwm_set(RIGHT_SERVO_PIN,0,HB25_FULL_REVERSE_MS);
  
  print("ramping up...");  
  for(int i = HB25_NEUTRAL_MS; i <= HB25_FULL_FORWARD_MS; i++){
    pwm_set(RIGHT_SERVO_PIN, 0, i);
    pwm_set(LEFT_SERVO_PIN, 0, i);
    pause(150);
    print("%d\n", i);
  }
}

rampUpReverse(){
  print("ramping up reverse...");
  
  for(int i = 1070; i >= HB25_FULL_REVERSE_MS; i--){
    pwm_set(RIGHT_SERVO_PIN, 0, i);
    pause(150);
      print("%d\n", i);
  }
}

rampDown(){
  print("ramping down...");
  
  for(int i = HB25_FULL_FORWARD_MS; i >= HB25_NEUTRAL_MS; i--){
    pwm_set(RIGHT_SERVO_PIN, 0, i);
    pwm_set(LEFT_SERVO_PIN, 0, i);
    pause(150);
    print("%d\n", i);
  }

  stop();
}

spinRight(){
  rightWheelsForward();
  leftWheelsReverse();
}

testServo(int pin){

   //print("testing servo on pin %d\n", pin);
  servo_angle(pin, 400);                         // P16 servo to 0 degrees
  pause(1000);                                // ...for 3 seconds
//  servo_angle(pin, 900);
  servo_angle(pin, 1800);                       // P16 servo to 90 degrees
  pause(1000);                                // ...for 3 seconds
  //servo_angle(pin, 400);                      // P16 servo to 180 degrees
  //pause(1000);                                // ...for 3 seconds
  servo_stop();

}

testVPant(){
   for(int i = 0; i < 4; i++){
  testServo(V_PANT_PIN);
  }      
}

testHPant(){ 
  for(int i = 0; i < 4; i++){
  testServo(H_PANT_PIN);
  }
}
  
testDrive(){
  hb25InitAll();
    driveForward();    
    pause(500);
    stop();
    pause(500);
    driveReverse();
    pause(500);
    stop();
}

testPinger(int pin){
  int cmDist = ping_cm(pin);                 // Get cm distance from Ping)))
    print("cmDist = %d\n", cmDist);           // Display distance
    pause(200);      
}

int main()                                    // main function
{
  int userInput;
  setupSDCard();

  while(1){
    print("What would you like to do?");
    scan("%d/n", &userInput);
    
    if(userInput == 1){
      leftWheelsForward();
      //rightWheelsForward();
    }else if(userInput == 2){
      stop();
    }else if (userInput == 3){
      print("Playing sound\n");
     playSound("techloop.wav");
    }else if (userInput == 4){
      wav_stop();
    }else if (userInput == 5){
      //hb25Init(RIGHT_SERVO_PIN);
      hb25InitAll();
    }else if (userInput ==6){
      leftWheelsReverse();
      //rightWheelsReverse();
    }else if (userInput == 7){
      pwm_stop();
      return 0;
    }else if (userInput == 8){
      playSound("regper.wav");
    }else if (userInput == 9){
      rampUp();
      rampDown();
    }else if (userInput == 10){
      rightWheelsForward();
    }else if (userInput == 11){
      rightWheelsReverse();
    }else if (userInput == 12){
      driveForward();
    }else if (userInput == 13){
      driveReverse();
    }else if (userInput == 14){
      spinRight();
    }else if (userInput == 15){
      testServo(HEAD_SERVO_PIN);
    }else if(userInput == 16){
      testPinger(PINGER_PIN);
    }else if(userInput == 17){
        cog_run(testVPant, 128);
        cog_run(testHPant, 128);
   
    }
      //print("Playing sound\n");
      //playSound("techloop.wav");
      //playSound("regper.wav");
  }

}