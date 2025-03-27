String op = ": ";
const int Recieve_From_PC = 0;     // Connected to RX pin (NOT CONNECTED)
const int Transmit_To_PC = 1;      // Connected to TX pin (NOT CONNECTED)
const int ManualInput = 2;         // Manual switch position
const int WashInput = 3;           //  WASH switch position
const int AutoInput_1 = 4;           // Input for Program 1
const int GoInput = 5;             // Push GO button
const int WorkingOutput = 13;       // Turn WORKING light
const int SwitchA = 7;             // State of switch A - 1 or 0
const int SwitchB = 8;             // State of switch B - 1 or 0
const int SwitchC = 9;             // State of switch C - 1 or 0
const int SwitchD = 21;            // State of switch D - 1 or 0
const int SwitchE = 20;            //solution for switching with 11 digital
const int WashOutput = 12;         //Green LED for end of WASH
const int AutoInput_2 = 10;         // Input for Program 2
const int Recieve_From_Pump = 14;  //Recieve codes from pump
const int Transmit_To_Pump = 15;   //Transmit codes to pump
const int LED_Done = 16;              // DONE LED 
const int AutoInput_3 = 17;         // Input for program 3
const int AutoInput_4 = 18;         // Input for program 4       
const int Heating  = 11;
const int Limit_Switch = 19;

//PIN 6 unused

// _____Pump Commands:_____

// Query:
byte Query[] = {0xCC, 0x00, 0x4A, 0x00, 0x00, 0xDD, 0xF3, 0x01 };

// Valve Movements:
byte Valve_to_1[] = { 0xCC, 0x00, 0x44, 0x01, 0x00, 0xDD, 0xEE, 0x01 };
byte Valve_to_2[] = { 0xCC, 0x00, 0x44, 0x02, 0x00, 0xDD, 0xEF, 0x01 };
byte Valve_to_3[] = { 0xCC, 0x00, 0x44, 0x03, 0x00, 0xDD, 0xF0, 0x01 };
byte Valve_to_4[] = { 0xCC, 0x00, 0x44, 0x04, 0x00, 0xDD, 0xF1, 0x01 };
byte Valve_to_5[] = { 0xCC, 0x00, 0x44, 0x05, 0x00, 0xDD, 0xF2, 0x01 };
byte Valve_to_6[] = { 0xCC, 0x00, 0x44, 0x06, 0x00, 0xDD, 0xF3, 0x01 };

// Syringe Speed:
byte Speed_003_mms[] = { 0xCC, 0x00, 0x4B, 0x01, 0x00, 0xDD, 0xF5, 0x01 };
byte Speed_006_mms[] = { 0xCC, 0x00, 0x4B, 0x02, 0x00, 0xDD, 0xF6, 0x01 };
byte Speed_009_mms[] = { 0xCC, 0x00, 0x4B, 0x03, 0x00, 0xDD, 0xF7, 0x01 };
byte Speed_01_mms[] = { 0xCC, 0x00, 0x4B, 0x03, 0x00, 0xDD, 0xF7, 0x01 };
byte Speed_013_mms[] = { 0xCC, 0x00, 0x4B, 0x04, 0x00, 0xDD, 0xF8, 0x01 };
byte Speed_05_mms[] = { 0xCC, 0x00, 0x4B, 0x0F, 0x00, 0xDD, 0x03, 0x02 };
byte Speed_08_mms[] = { 0xCC, 0x00, 0x4B, 0x18, 0x00, 0xDD, 0x0C, 0x02 };
byte Speed_09_mms[] = { 0xCC, 0x00, 0x4B, 0x1B, 0x00, 0xDD, 0x0F, 0x02 };
byte Speed_1_mms[] = { 0xCC, 0x00, 0x4B, 0x1E, 0x00, 0xDD, 0x12, 0x02 };
byte Speed_1_5_mms[] = { 0xCC, 0x00, 0x4B, 0x2D, 0x00, 0xDD, 0x21, 0x02 };
byte Speed_2_mms[] = { 0xCC, 0x00, 0x4B, 0x3C, 0x00, 0xDD, 0x30, 0x02 };
byte Speed_3_mms[] = { 0xCC, 0x00, 0x4B, 0x5A, 0x00, 0xDD, 0x4E, 0x02 };

// Syringe Intake:
byte intake_10[] = { 0xCC, 0x00, 0x43, 0x18, 0x00, 0xDD, 0x04, 0x02 };
byte intake_15[] = { 0xCC, 0x00, 0x43, 0x24, 0x00, 0xDD, 0x10, 0x02 };
byte intake_20[] = { 0xCC, 0x00, 0x43, 0x30, 0x00, 0xDD, 0x1C, 0x02 };
byte intake_30[] = { 0xCC, 0x00, 0x43, 0x48, 0x00, 0xDD, 0x34, 0x02 };
byte intake_31[] = { 0xCC, 0x00, 0x43, 0x4B, 0x00, 0xDD, 0x37, 0x02 };
byte intake_40[] = { 0xCC, 0x00, 0x43, 0x60, 0x00, 0xDD, 0x4C, 0x02 };
byte intake_50[] = { 0xCC, 0x00, 0x43, 0x78, 0x00, 0xDD, 0x64, 0x02 };
byte intake_51[] = { 0xCC, 0x00, 0x43, 0x7B, 0x00, 0xDD, 0x67, 0x02 };
byte intake_55[] = { 0xCC, 0x00, 0x43, 0x84, 0x00, 0xDD, 0x70, 0x02 };
byte intake_60[] = { 0xCC, 0x00, 0x43, 0x90, 0x00, 0xDD, 0x7C, 0x02 };
byte intake_62[] = { 0xCC, 0x00, 0x43, 0x96, 0x00, 0xDD, 0x82, 0x02 };
byte intake_75[] = { 0xCC, 0x00, 0x43, 0xB4, 0x00, 0xDD, 0xA0, 0x02 };
byte intake_80[] = { 0xCC, 0x00, 0x43, 0xC0, 0x00, 0xDD, 0xAC, 0x02 };
byte intake_93[] = { 0xCC, 0x00, 0x43, 0xE0, 0x00, 0xDD, 0xCC, 0x02 };
byte intake_100[] = { 0xCC, 0x00, 0x43, 0xF0, 0x00, 0xDD, 0xDC, 0x02 };
byte intake_102[] = { 0xCC, 0x00, 0x43, 0xF6, 0x00, 0xDD, 0xE2, 0x02 };
byte intake_110[] = { 0xCC, 0x00, 0x43, 0x08, 0x01, 0xDD, 0xF5, 0x01 };
byte intake_120[] = { 0xCC, 0x00, 0x43, 0x21, 0x01, 0xDD, 0x0E, 0x02 };
byte intake_124[] = { 0xCC, 0x00, 0x43, 0x2B, 0x01, 0xDD, 0x18, 0x02 };
byte intake_140[] = { 0xCC, 0x00, 0x43, 0x50, 0x01, 0xDD, 0x3D, 0x02 };
byte intake_150[] = { 0xCC, 0x00, 0x43, 0x68, 0x01, 0xDD, 0x55, 0x02 };
byte intake_153[] = { 0xCC, 0x00, 0x43, 0x70, 0x01, 0xDD, 0x5D, 0x02 };
byte intake_155[] = { 0xCC, 0x00, 0x43, 0x75, 0x01, 0xDD, 0x62, 0x02 };
byte intake_160[] = { 0xCC, 0x00, 0x43, 0x81, 0x01, 0xDD, 0x6E, 0x02 };
byte intake_165[] = { 0xCC, 0x00, 0x43, 0x8C, 0x01, 0xDD, 0x79, 0x02 };
byte intake_180[] = { 0xCC, 0x00, 0x43, 0xB1, 0x01, 0xDD, 0x9E, 0x02 };
byte intake_200[] = { 0xCC, 0x00, 0x43, 0xE0, 0x01, 0xDD, 0xCD, 0x02 };
byte intake_204[] = { 0xCC, 0x00, 0x43, 0xEB, 0x01, 0xDD, 0xD8, 0x02 };
byte intake_220[] = { 0xCC, 0x00, 0x43, 0x08, 0x02, 0xDD, 0xF6, 0x01 };
byte intake_225[] = { 0xCC, 0x00, 0x43, 0x1C, 0x02, 0xDD, 0x0A, 0x02 };
byte intake_240[] = { 0xCC, 0x00, 0x43, 0x41, 0x02, 0xDD, 0x2F, 0x02 };
byte intake_250[] = { 0xCC, 0x00, 0x43, 0x59, 0x02, 0xDD, 0x47, 0x02 };
byte intake_255[] = { 0xCC, 0x00, 0x43, 0x65, 0x02, 0xDD, 0x53, 0x02 };
byte intake_275[] = { 0xCC, 0x00, 0x43, 0x96, 0x02, 0xDD, 0x84, 0x02 };
byte intake_300[] = { 0xCC, 0x00, 0x43, 0xD2, 0x02, 0xDD, 0xC0, 0x02 };
byte intake_350[] = { 0xCC, 0x00, 0x43, 0x49, 0x03, 0xDD, 0x38, 0x02 };
byte intake_375[] = { 0xCC, 0x00, 0x43, 0x86, 0x03, 0xDD, 0x75, 0x02 };
byte intake_470[] = { 0xCC, 0x00, 0x43, 0x6A, 0x04, 0xDD, 0x5A, 0x02 };
byte intake_500[] = { 0xCC, 0x00, 0x43, 0xB1, 0x04, 0xDD, 0xA1, 0x02 };
byte intake_660[] = { 0xCC, 0x00, 0x43, 0x33, 0x06, 0xDD, 0x25, 0x02 };
byte intake_680[] = { 0xCC, 0x00, 0x43, 0x62, 0x06, 0xDD, 0x54, 0x02 };
byte intake_700[] = { 0xCC, 0x00, 0x43, 0x93, 0x06, 0xDD, 0x85, 0x02 };
byte intake_1400[] = { 0xCC, 0x00, 0x43, 0x25, 0x0D, 0xDD, 0x1E, 0x02 };
byte intake_2400[] = { 0xCC, 0x00, 0x43, 0x89, 0x16, 0xDD, 0x8B, 0x02 };

// Syringe Discharge:
byte push_1_25[] = { 0xCC, 0x00, 0x42, 0x03, 0x00, 0xDD, 0xEE, 0x01 };
byte push_1_66[] = { 0xCC, 0x00, 0x42, 0x04, 0x00, 0xDD, 0xEF, 0x01 };
byte push_2_5[] = { 0xCC, 0x00, 0x42, 0x06, 0x00, 0xDD, 0xF1, 0x01 };
byte push_4_15[] = { 0xCC, 0x00, 0x42, 0x0A, 0x00, 0xDD, 0xF5, 0x01 };
byte push_6_25[] = { 0xCC, 0x00, 0x42, 0x0E, 0x00, 0xDD, 0xF9, 0x01 };
byte push_7_5[] = { 0xCC, 0x00, 0x42, 0x11, 0x00, 0xDD, 0xFC, 0x01 };
byte push_8[] = { 0xCC, 0x00, 0x42, 0x13, 0x00, 0xDD, 0xFE, 0x01 };
byte push_9[] = { 0xCC, 0x00, 0x42, 0x15, 0x00, 0xDD, 0x00, 0x02 };
byte push_9_5[] = { 0xCC, 0x00, 0x42, 0x17, 0x00, 0xDD, 0x02, 0x02 };
byte push_10[] = { 0xCC, 0x00, 0x42, 0x18, 0x00, 0xDD, 0x03, 0x02 };
byte push_10_5[] = { 0xCC, 0x00, 0x42, 0x19, 0x00, 0xDD, 0x04, 0x02 };
byte push_11[] = { 0xCC, 0x00, 0x42, 0x1A, 0x00, 0xDD, 0x05, 0x02 };
byte push_11_5[] = { 0xCC, 0x00, 0x42, 0x1B, 0x00, 0xDD, 0x06, 0x02 };
byte push_12[] = { 0xCC, 0x00, 0x42, 0x1C, 0x00, 0xDD, 0x07, 0x02 };
byte push_17[] = { 0xCC, 0x00, 0x42, 0x29, 0x00, 0xDD, 0x14, 0x02 };
byte push_20[] = { 0xCC, 0x00, 0x42, 0x30, 0x00, 0xDD, 0x1B, 0x02 };
byte push_23[] = { 0xCC, 0x00, 0x42, 0x38, 0x00, 0xDD, 0x23, 0x02 };
byte push_25[] = { 0xCC, 0x00, 0x42, 0x3C, 0x00, 0xDD, 0x27, 0x02 };
byte push_31[] = { 0xCC, 0x00, 0x42, 0x4B, 0x00, 0xDD, 0x36, 0x02 };
byte push_4_150[] = { 0xCC, 0x00, 0x42, 0x60, 0x00, 0xDD, 0x4B, 0x02 };
byte push_50[] = { 0xCC, 0x00, 0x42, 0x78, 0x00, 0xDD, 0x63, 0x02 };
byte push_55[] = { 0xCC, 0x00, 0x42, 0x84, 0x00, 0xDD, 0x6F, 0x02 };
byte push_60[] = { 0xCC, 0x00, 0x42, 0x90, 0x00, 0xDD, 0x7B, 0x02 };
byte push_75[] = { 0xCC, 0x00, 0x42, 0xB4, 0x00, 0xDD, 0x9F, 0x02 };
byte push_100[] = { 0xCC, 0x00, 0x42, 0xF0, 0x00, 0xDD, 0xDB, 0x02 };
byte push_130[] = { 0xCC, 0x00, 0x42, 0x38, 0x01, 0xDD, 0x24, 0x02 };
byte push_133[] = { 0xCC, 0x00, 0x42, 0x3A, 0x01, 0xDD, 0x26, 0x02 };
byte push_135[] = { 0xCC, 0x00, 0x42, 0x45, 0x01, 0xDD, 0x31, 0x02 };
byte push_140[] = { 0xCC, 0x00, 0x42, 0x50, 0x01, 0xDD, 0x3C, 0x02 };
byte push_150[] = { 0xCC, 0x00, 0x42, 0x68, 0x01, 0xDD, 0x54, 0x02 };
byte push_200[] = { 0xCC, 0x00, 0x42, 0xE0, 0x01, 0xDD, 0xCC, 0x02 };
byte push_250[] = { 0xCC, 0x00, 0x42, 0x59, 0x02, 0xDD, 0x46, 0x02 };
byte push_650[] = { 0xCC, 0x00, 0x42, 0x1B, 0x06, 0xDD, 0x0C, 0x02 };
byte push_725[] = { 0xCC, 0x00, 0x42, 0xCF, 0x06, 0xDD, 0xC0, 0x02 };
byte push_750[] = { 0xCC, 0x00, 0x42, 0x0B, 0x07, 0xDD, 0xFD, 0x01 };
byte push_800[] = { 0xCC, 0x00, 0x42, 0x83, 0x07, 0xDD, 0x75, 0x02 };
byte push_875[] = { 0xCC, 0x00, 0x42, 0x37, 0x08, 0xDD, 0x2A, 0x02 };
byte push_2400[] = { 0xCC, 0x00, 0x42, 0x89, 0x16, 0xDD, 0x8A, 0x02 };

// Pump reset:
byte reset[] = { 0xCC, 0x00, 0x45, 0x00, 0x00, 0xDD, 0xEE, 0x01 };

// Pump stop:
byte stop[] = { 0xCC, 0x00, 0x49, 0x00, 0x00, 0xDD, 0xF2, 0x01 };

// ______Pump Messages______:
byte Cannot_Push[] = { 0xCC, 0x00, 0x08, 0x00, 0x00, 0xDD, 0xB1, 0x01 };
byte busy[] = { 0xCC, 0x00, 0x04, 0x00, 0x00, 0xDD, 0xAD, 0x01 };
