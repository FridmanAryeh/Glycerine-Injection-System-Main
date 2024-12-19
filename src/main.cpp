#include <SoftwareSerial.h>
#include <Arduino.h>
#include "parameters.h"

SoftwareSerial Pump(Recieve_From_Pump, Transmit_To_Pump);  //Defining pins for pump serial input and ouput

void Green_LEDs_out() // turns off all green LEDs
{
  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(LED_C, LOW);
  digitalWrite(LED_D, LOW);
  digitalWrite(LED_E, LOW);
}

void Clean_Pump_Serial()   // function which reads buffer and by that empties it
{
  for (int i = 0; i < 65; i++){
    Pump.read();
  }
}

void gen_function(byte command[], int size_t) // function verifies each pump command is only given once previous command is completed
{  
  int flag;
  Clean_Pump_Serial();
  Pump.write(command, size_t);
  do {
    delay(500);
    Clean_Pump_Serial();
    Pump.write(Query,8);
    byte Data[8];
    Pump.readBytes(Data, 8);
    int comparison = memcmp(Data, busy, size_t);  //compare response with defined busy response. if the same, function will return 0
    if (comparison == 0) {
      flag = 1;
    } else if (comparison != 0) {
      flag = 0;
    }
  } while (flag == 1); // As long as pump returns "busy" response, keep on sending command
}

void Wash_cycle(byte valve[]) // cycle of wash - change to tank, take glycerine, switch to valve, push
{
    gen_function(Valve_to_1, 8);
    gen_function(Speed_1_mms, 8);
    gen_function(intake_2400, 8);
    delay(20000);
    gen_function(valve, 8);
    gen_function(Speed_08_mms, 8);
    gen_function(push_2400, 8);
}

void Wash_function()  // Replace all old glycerine in pipes with new glycerine
{  
    Green_LEDs_out();
    digitalWrite(WorkingOutput, HIGH);  
    Clean_Pump_Serial();
    Wash_cycle(Valve_to_2);
    Wash_cycle(Valve_to_3);
    Wash_cycle(Valve_to_4);
    Wash_cycle(Valve_to_5);
    Wash_cycle(Valve_to_6);
    digitalWrite(WorkingOutput, LOW);  
    digitalWrite(WashOutput, HIGH);   
}

void Check_Switch(int SwitchMode, byte valve[], byte push[]) // Checking if the switch is on. if on - aspirate to applicator. if off - wait the amount of time it would take to aspirate
{ 
  if (SwitchMode == HIGH){
    gen_function(valve, 8);
    gen_function(push, 8);
  }
  else
  {
    delay(4500);
  }
  delay(4000);
}

void Auto(byte intake[], byte push[], int reps, long end_delay, int SwitchAMode, int SwitchBMode, int SwitchCMode, int SwitchDMode, int SwitchEMode){ // general program without parameters 
    digitalWrite(WorkingOutput, HIGH);  //Turn on WORKING light
    Green_LEDs_out();
    gen_function(Speed_1_mms, 8);
    gen_function(Valve_to_1, 8);
    gen_function(intake, 8);
    delay(5000); // short delay in order to stabilize pressure
    gen_function(Speed_003_mms, 8);
    for (int t = 0; t < reps; t++)  // repeat sequence 
    {
      Check_Switch(SwitchAMode, Valve_to_2, push);
      Check_Switch(SwitchBMode, Valve_to_3, push);
      Check_Switch(SwitchCMode, Valve_to_4, push);
      Check_Switch(SwitchDMode, Valve_to_5, push);
      Check_Switch(SwitchEMode, Valve_to_6, push);
    }
    delay(end_delay);
    Green_LEDs_on();
    digitalWrite(WorkingOutput, LOW); 
}

void Auto_function(int option, int SwitchAMode, int SwitchBMode, int SwitchCMode, int SwitchDMode, int SwitchEMode){ // parameters for each program
  switch (option)
  {
  case 1: // NEEDLE, FLEX 1/2 seeds - 48 ul to each applicator
    Auto(intake_240,push_12,4,80000, SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode); 
    break;
  case 2: // FLEX 3/4 seeds - 44 ul to each applicator
    Auto(intake_220,push_11,4,95000,  SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode);
    break;
  case 3: // FLEX 5/6 seeds - 40 ul to each applicator
    Auto(intake_200,push_10,4,105000,  SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode);
    break;  
  }
}

void Manual_function() // Specific function which adds 30 ul to applicator which is inserted into port A
{
  digitalWrite(WorkingOutput, HIGH);  //Turn on WORKING light
  Green_LEDs_out();
  gen_function(Valve_to_1, 8);
  gen_function(Speed_1_mms, 8);
  gen_function(intake_30, 8);
  gen_function(Valve_to_2, 8);
  gen_function(Speed_003_mms, 8);
  delay(2000);
  for (int i = 0; i < 3; i++)
  {
    gen_function(push_10,8);
    delay(10000);
  }
  digitalWrite(LED_A, HIGH);
  digitalWrite(WorkingOutput, LOW);  // Turn off WORKING light
}
 
void Reset_function()  // Command to bring pump back to port 1. Reset command empties syringe and resest pump.
{  
  digitalWrite(WorkingOutput, HIGH);  //Turn on WORKING light
  Green_LEDs_out();
  gen_function(Valve_to_1, 8);
  gen_function(Speed_1_mms, 8);
  gen_function(reset, 8);
  digitalWrite(WorkingOutput, LOW);  // Turn off WORKING light
}

void Green_LEDs_on() // Turns on all green LEDs
{
  digitalWrite(LED_A, HIGH);
  digitalWrite(LED_B, HIGH);
  digitalWrite(LED_C, HIGH);
  digitalWrite(LED_D, HIGH);
  digitalWrite(LED_E, HIGH);  
}

void setup() // General setup - happens each time machine turns on and off (every time Arduino's power turns on after being off)
{
  Serial.begin(9600);
  Pump.begin(9600);
  pinMode(Recieve_From_PC, INPUT);     //no wire attached physically; used for PC connection
  pinMode(Transmit_To_PC, OUTPUT);     //no wire attached physically; used for PC connection
  pinMode(ManualInput, INPUT_PULLUP);  //2;
  pinMode(WashInput, INPUT_PULLUP);    //3
  pinMode(AutoInput_1, INPUT_PULLUP);  //4
  pinMode(GoInput, INPUT_PULLUP);      //5
  pinMode(WorkingOutput, OUTPUT);      //6
  pinMode(SwitchA, INPUT);             //7
  pinMode(SwitchB, INPUT);             //8
  pinMode(SwitchC, INPUT);             //9
  pinMode(SwitchD, INPUT);             //21 Analog!
  pinMode(SwitchE, INPUT);             //20 Analog!
  pinMode(WashOutput, OUTPUT);         //12
  pinMode(AutoInput_2, INPUT_PULLUP);  //10
  pinMode(AutoInput_3, INPUT_PULLUP);  //13
  pinMode(Recieve_From_Pump, INPUT);   //14
  pinMode(Transmit_To_Pump, OUTPUT);   //15
  pinMode(LED_A, OUTPUT);              //16
  pinMode(LED_B, OUTPUT);              //17
  pinMode(LED_C, OUTPUT);              //18
  pinMode(LED_D, OUTPUT);              //19
  pinMode(LED_E, OUTPUT);              //11         
  gen_function(reset, 8);              // Reset pump every time machine turns on
  digitalWrite(WorkingOutput, LOW);    //program starts with WORKING light off
  digitalWrite(WashOutput, LOW);       //program starts with Washed light off
  Green_LEDs_out();
}

void loop() // Constant code, running over and again as long as Arduino is on
{
// ___Definitions of Variables___
  int WashSignal = digitalRead(WashInput);
  int ManualSignal = digitalRead(ManualInput);
  int WashOutputSignal = digitalRead(WashOutput);
  int Auto_1_Signal = digitalRead(AutoInput_1);
  int GoSignal = digitalRead(GoInput);
  int Auto_2_Signal = digitalRead(AutoInput_2);
  int Auto_3_Signal = digitalRead(AutoInput_3);
  int SwitchAMode = digitalRead(SwitchA);                                                //Define presence applicator for switch A
  int SwitchBMode = digitalRead(SwitchB);                                                //Define presence applicator for switch B
  int SwitchCMode = digitalRead(SwitchC);                                                //Define presence applicator for switch C
  int SwitchDMode = analogRead(SwitchD) / 1023;                                      //Define presence applicator for switch D  NOTE - using analog pin!
  int SwitchEMode = analogRead(SwitchE) / 1023;                                      //Define presence applicator for switch E. NOTE - using analog pin!

//___Main Code___
  if (WashSignal == LOW && GoSignal == LOW) // GO button is pressed, Switch at WASH
  { 
    Reset_function();
    Wash_function(); 
  }
  if (/*WashOutputSignal == HIGH && */Auto_1_Signal == LOW && GoSignal == LOW) // GO button is pressed, Switch at AUTO 1 - NEEDLE, FLEX 1,2
  {
    Reset_function();
    Auto_function(1, SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode); 
  }
  if (/*WashOutputSignal == HIGH && */Auto_2_Signal == LOW && GoSignal == LOW) // GO button is pressed, Switch at AUTO 2 - FLEX 3,4
  {
    Reset_function();
    Auto_function(2,  SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode); 
  }
    if (/*WashOutputSignal == HIGH && */Auto_3_Signal == LOW && GoSignal == LOW) // GO button is pressed, Switch at AUTO 3 - FLEX 5,6
  {
    Reset_function();
    Auto_function(3,  SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode); 
  } 
  
  if (WashOutputSignal == HIGH && ManualSignal == LOW && GoSignal == LOW) // GO button is pressed, Switch at MANUAL
  {
    Reset_function();
    Manual_function();  
  }
}
