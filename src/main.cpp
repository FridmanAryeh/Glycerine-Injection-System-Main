//// END CODE MUST TURN WASH CONDITION BACK ON

#include <SoftwareSerial.h>
#include <Arduino.h>
#include "parameters.h"

bool preheat_required;
long time_of_last_closed;
long time_of_last_off;

SoftwareSerial Pump(Recieve_From_Pump, Transmit_To_Pump);  //Defining pins for pump serial input and ouput

void Clean_Pump_Serial()   // function which reads buffer and by that empties it
{
  for (int i = 0; i < 65; i++){
    Pump.read();
  }
}

void gen_function(const byte command[], int size_t)// function verifies each pump command is only given once previous command is completed
{
  int isBusy;
  Clean_Pump_Serial();
  Pump.write(command, size_t);
  do {
    delay(500);
    Clean_Pump_Serial();
    Pump.write(Query, 8);
    byte response[8];
    Pump.readBytes(response, 8);
    isBusy = memcmp(response, busy, size_t) == 0 ? 1 : 0;
  } while (isBusy);
}

void Wash_cycle(byte valve[]) // cycle of wash - change to tank, intake of glycerine, switch to designated port, push
{
    gen_function(Valve_to_1, 8);
    gen_function(Speed_1_mms, 8);
    gen_function(intake_2400, 8);
    delay(20000); 
    gen_function(valve, 8);
    gen_function(Speed_05_mms, 8);
    gen_function(push_2400, 8);
}

void Wash_function()  // Replace all old glycerine in pipes with new glycerine
{  
    digitalWrite(LED_Done, LOW);
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

void Check_Switch(int SwitchMode, int option, byte valve[], byte push[]) // Checking if the switch is on. if on - aspirate to applicator. if off - wait the amount of time it would take to aspirate
{ 
  if (SwitchMode)
  {
    gen_function(valve, 8);
    gen_function(push, 8);
  }
  else if(!SwitchMode && (option == 1 || option == 2)) //if there is no applicator, and the program is needle or flex, delay using amount of pushing time. if loading device, skip delay.
  {
    if (option == 1)
    {
      delay(332); // aprx time it would take to push 1.66 ul
    }
    if (option == 2)
    {
      delay(554); // aprx time it would take to push 4.16 ul
    }
  }
}

void Auto(byte intake[], byte push[], byte speed[], int reps, long end_delay, int SwitchAMode, int SwitchBMode, int SwitchCMode, int SwitchDMode, int SwitchEMode, int option){ // general program without parameters 
    digitalWrite(WorkingOutput, HIGH);  //Turn on WORKING light
    digitalWrite(LED_Done, LOW);
    gen_function(Speed_1_mms, 8);
    gen_function(Valve_to_1, 8);
    gen_function(intake, 8);
    delay(6000); // short delay in order to stabilize pressure
    gen_function(speed, 8);
    for (int t = 0; t < reps; t++)  // repeat sequence 
    { 
      Check_Switch(SwitchAMode, option, Valve_to_2, push);
      Check_Switch(SwitchBMode, option, Valve_to_3, push);
      Check_Switch(SwitchCMode, option, Valve_to_4, push);
      Check_Switch(SwitchDMode, option, Valve_to_5, push);
      Check_Switch(SwitchEMode, option, Valve_to_6, push);
    }
    if (SwitchAMode && end_delay != 0)// PATCH (NEEDLE, FLEX)! - after a lot of experiments, port A specifically is usually missing fluid. Code adds a little more assuming port is open
    {
      gen_function(Valve_to_2, 8);
      gen_function(push_4_15, 8);
    }
    if (SwitchAMode && end_delay == 0)// PATCH (LOADING DEVICE)! - after a lot of experiments, port A specifically is usually missing fluid. Code adds a little more assuming port is open
    {
      gen_function(Valve_to_2, 8);
      gen_function(push_10, 8);
    }
    delay(end_delay);
    digitalWrite(LED_Done, HIGH);
    digitalWrite(WorkingOutput, LOW); 
}

void Auto_function(int option, int SwitchAMode, int SwitchBMode, int SwitchCMode, int SwitchDMode, int SwitchEMode) // parameters for each program. Intake has always a little extra in case pump malfunctions on the steps
{
  switch (option)
  {
  case 1: // NEEDLE - 50 ul to each applicator
    Auto(intake_255,push_4_15,Speed_009_mms,12,40000, SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode, 1);
    break;
  case 2: // FLEX - 43 ul to each applicator with more delay time to equalize pressure
    Auto(intake_225,push_1_66,Speed_006_mms,26,60000,  SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode, 2);
    break;
  case 3: // LOADING DEVICE 10, 130 ul to each applicator
    Auto(intake_680,push_130,Speed_1_mms,1,0, SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode, 3);
    break;
  case 4: // LOADING DEVICE 20, 135 ul to each applicator
    Auto(intake_700,push_135,Speed_1_mms,1,0, SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode, 4);
    break;  
  }
}

void Manual_function() // function which "manually" adds 4 ul to applicator which is inserted into port A
{
  digitalWrite(WorkingOutput, HIGH); 
  digitalWrite(LED_Done, LOW);
  gen_function(Valve_to_1, 8);
  gen_function(Speed_1_mms, 8);
  gen_function(intake_10, 8);
  delay(2000);
  gen_function(Valve_to_2, 8);
  gen_function(Speed_003_mms, 8);
  for (int i = 0; i < 1; i++) //loop in case want to change amount of iterations
  {
    gen_function(push_6_25,8);
    delay(45000);
  }
  digitalWrite(LED_Done, HIGH);
  digitalWrite(WorkingOutput, LOW);
}
 
void Reset_function()  // Command to bring pump back to port 1. Reset command empties syringe and resest pump.
{  
  digitalWrite(WorkingOutput, HIGH); 
  digitalWrite(LED_Done, LOW);
  gen_function(Valve_to_1, 8);
  gen_function(Speed_1_mms, 8);
  gen_function(reset, 8);
  digitalWrite(WorkingOutput, LOW); 
}

void Check_Preheat(bool Preheat_required)
{
  if (Preheat_required)
  {
    digitalWrite(Heating, HIGH); /// turn on heat
    for (int i = 0; i < 240; i++) /// flash working light
    {
      digitalWrite(WorkingOutput, HIGH);
      delay(500);
      digitalWrite(WorkingOutput, LOW);
      delay(500);
    }
  }
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
  pinMode(Heating, OUTPUT);            //11
  pinMode(WashOutput, OUTPUT);         //12
  pinMode(AutoInput_2, INPUT_PULLUP);  //10
  pinMode(AutoInput_3, INPUT_PULLUP);  //17
  pinMode(Recieve_From_Pump, INPUT);   //14
  pinMode(Transmit_To_Pump, OUTPUT);   //15
  pinMode(LED_Done, OUTPUT);           //16
  pinMode(AutoInput_4, INPUT_PULLUP);  //18
  pinMode(Limit_Switch, INPUT_PULLUP); //19
  Reset_function();        
  digitalWrite(WorkingOutput, LOW);    //program starts with WORKING light off
  digitalWrite(WashOutput, LOW);       //program starts with Washed light off
  digitalWrite(LED_Done, LOW);
  digitalWrite(Heating, HIGH); // Machine turns on, heat starts
  
}

void loop() // Constant code, running in loops as long as Arduino is on
{
// ___Definitions of Variables___
  int WashSignal = digitalRead(WashInput);
  int ManualSignal = digitalRead(ManualInput);
  int WashOutputSignal = digitalRead(WashOutput);
  int GoSignal = digitalRead(GoInput);
  int Auto_1_Signal = digitalRead(AutoInput_1);
  int Auto_2_Signal = digitalRead(AutoInput_2);
  int Auto_3_Signal = digitalRead(AutoInput_3);
  int Auto_4_Signal = digitalRead(AutoInput_4);
  int SwitchAMode = digitalRead(SwitchA);            //Define presence applicator for switch A
  int SwitchBMode = digitalRead(SwitchB);            //Define presence applicator for switch B
  int SwitchCMode = digitalRead(SwitchC);            //Define presence applicator for switch C
  int SwitchDMode = analogRead(SwitchD) / 1023;      //Define presence applicator for switch D  NOTE - using analog pin!
  int SwitchEMode = analogRead(SwitchE) / 1023;      //Define presence applicator for switch E  NOTE - using analog pin!
  bool Bottom_panel_open = ((analogRead(Limit_Switch) / 1023.0) > 0.5); //checking if panel is open. 

///////  BEFORE DELIVERY - CHANGE BACK WASHOUTPUT SIGNAL CONDITION /////// CHANGE TIMES OF MICROSWITCH

//___Main Code___

// 1. Heating Control //
  if (!Bottom_panel_open && time_of_last_closed == 0) //// if box is closed and has just been closed, start timer of how long box is closed
  {
    time_of_last_closed = millis();
  }

// If heating just turned off, store the time once
  if (digitalRead(Heating) == LOW && time_of_last_off == 0) 
  {
    time_of_last_off = millis();
  }

  long duration_box_closed = millis() - time_of_last_closed; //defining variable of how long box was closed
  long duration_heating_off = millis() - time_of_last_off;    //defining variable of how long heating was off

// If the box is open, reset timers
  if(Bottom_panel_open) 
  {
    time_of_last_closed = 0;
    time_of_last_off = 0;
    duration_box_closed = 0;
    duration_heating_off = 0;
    digitalWrite(Heating, HIGH);
  }

// Turn heating off after 2 min 
  if (duration_box_closed > 120000) 
  {
  digitalWrite(Heating, LOW);
  duration_box_closed = 0;  // Reset duration tracking
  time_of_last_off = millis(); // Store when heating turned off
  }

// If heating was off for 3 min, require reheating
  if (duration_heating_off > 180000) 
  {
    preheat_required = true;
    duration_heating_off = 0; // Reset duration tracking
  }
  
 // 2. Glycerin filling programs // 
  if (WashSignal == LOW && GoSignal == LOW) // GO button is pressed, Switch at WASH
  { 
    Reset_function();
    Check_Preheat(preheat_required);
    preheat_required = false;
    Wash_function();  
  }
  if (/*WashOutputSignal == HIGH &&*/ Bottom_panel_open && Auto_1_Signal == LOW && GoSignal == LOW) // GO button is pressed, Switch at AUTO 1 - NEEDLE
  {
    Reset_function();
    Check_Preheat(preheat_required);
    preheat_required = false;
    Auto_function(1, SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode); 
  }
  if (/*WashOutputSignal == HIGH &&*/  Bottom_panel_open &&  Auto_2_Signal == LOW && GoSignal == LOW) // GO button is pressed, Switch at AUTO 2 - FLEX 
  {
    Reset_function();
    Check_Preheat(preheat_required);
    preheat_required = false;
    Auto_function(2,  SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode); 
  } 
  if (/*WashOutputSignal == HIGH &&*/Auto_3_Signal == LOW && GoSignal == LOW) // GO button is pressed, Switch at AUTO 4 - LOADING DEVICE 10
  {
    Reset_function();
    Auto_function(3,  SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode); 
  }
  if (/*WashOutputSignal == HIGH &&*/Auto_4_Signal == LOW && GoSignal == LOW) // GO button is pressed, Switch at AUTO 5 - LOADING DEVICE 20
  {
    Reset_function();
    Auto_function(4,  SwitchAMode, SwitchBMode, SwitchCMode, SwitchDMode, SwitchEMode); 
  }
  if (/*WashOutputSignal == HIGH && */ManualSignal == LOW && GoSignal == LOW && SwitchAMode == HIGH) // GO button is pressed, Switch at MANUAL
  {
    Reset_function();
    Manual_function();  
  }
}
