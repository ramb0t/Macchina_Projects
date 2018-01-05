#include <OBD2.h>
#include <DueTimer.h>

//Serial Tx 
#include <EasyTransfer.h>
/********************************************************************
This example is built upon the CANAcquisition class and the OBDParmameter class using 11bit (non-extended) OBD2 ID's

This example shows how to set up periodic data acquisition of OBD2 paramters based upon
standard PID's. If you'd like to add another paramter,simply copy one of the definitions and modify it accordingly. 
You may also need to add a new PID to the "OBD_PID" enum in the header file. 

Note as of 8/24/15 this has only been tested on one Toyota vehicle!
As of July 24, 2017 it has also been tested on a Buick Enclave. Works fine!
/********************************************************************/

//create the CANport acqisition schedulers
cAcquireCAN CANport0(CAN_PORT_0);

/***** DEFINITIONS FOR OBD MESSAGES ON CAN PORT 0, see https://en.wikipedia.org/wiki/OBD-II_PIDs to add your own ***************/
//char _name[10], char _units[10], OBD_PID pid,  uint8_t OBD_PID_SIZE size, bool _signed, OBD_MODE_REQ mode, float32 slope, float32 offset, cAcquireCAN *, extended ID;

  //cOBDParameter OBD_Speed(      "Speed "        , " KPH"		,  SPEED       , _8BITS,   false,   CURRENT,  1,      0,  &CANport0, false);
  //cOBDParameter OBD_EngineSpeed("Engine Speed " , " RPM"		,  ENGINE_RPM  , _16BITS,  false,   CURRENT,  0.25,   0,  &CANport0, false);
 //cOBDParameter OBD_Throttle(   "Throttle "     , " %"  		,  THROTTLE_POS, _8BITS,   false,   CURRENT,  0.3922, 0,  &CANport0, false);
  //cOBDParameter OBD_Coolant(    "Coolant "      , " C"  		,  COOLANT_TEMP, _8BITS,   false ,  CURRENT,  1,    -40,  &CANport0, false);
  //cOBDParameter OBD_EngineLoad( "Load "         , " %"  		,  ENGINE_LOAD , _8BITS,   false,   CURRENT,  0.3922, 0,  &CANport0, false);
  //cOBDParameter OBD_MAF(        "MAF "          , " grams/s",  ENGINE_MAF  , _16BITS,  false,   CURRENT,  0.01,   0,  &CANport0, false);
  cOBDParameter OBD_MAP(        "MAP "          , " kPa"    ,  ENGINE_MAP  , _8BITS,   false,   CURRENT,  1,      0,  &CANport0, false);
  //cOBDParameter OBD_IAT(        "IAT "          , " C"  		,  ENGINE_IAT  , _8BITS,   false ,  CURRENT,  1,    -40,  &CANport0, false);


// Serial Tx Struct setup
//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  uint8_t boost;
  uint8_t throttle;
  uint8_t injector;
  uint8_t timing;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup()
{
  delay(500); //allow USB time to settle
	//output pin that can be used for debugging purposes
	pinMode(RGB_GREEN, OUTPUT);  
  pinMode(GPIO1    , OUTPUT);
  pinMode(I_SENSE_EN, OUTPUT);
  digitalWrite(GPIO1 , HIGH);
  digitalWrite(I_SENSE_EN, HIGH);      

	//start serial port 
	Serial.begin(115200);

  //start the second serial
  Serial3.begin(115200);

	//debugging message for monitor to indicate CPU resets are occuring
	Serial.println("System Reset");
  Serial3.println("System Reset");

  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &Serial3);
       
  //start CAN ports,  enable interrupts and RX masks, set the baud rate here
	CANport0.initialize(_500K);

  //set up the transmission/reception of messages to occur at 500Hz (2mS) timer interrupt
  Timer3.attachInterrupt(PrintScreen).setFrequency(20).start();

}


UINT8 i;
UINT32 maxTime;

void loop()
{
  CANport0.run(POLLING);

}

//this is our timer interrupt handler, called at XmS interval
void PrintScreen()
{
  digitalWrite(RGB_GREEN, LOW);

	//print out our latest OBDII data
//	Serial.print(OBD_Speed.getName()); 
//	Serial.print(OBD_Speed.getData());
//	Serial.println(OBD_Speed.getUnits()); 	

//    Serial.print(OBD_EngineSpeed.getName()); 
//	Serial.print(OBD_EngineSpeed.getData());
//	Serial.println(OBD_EngineSpeed.getUnits()); 
//
//	Serial.print(OBD_Throttle.getName()); 
//	Serial.print(OBD_Throttle.getData());
//	Serial.println(OBD_Throttle.getUnits()); 
//    
//	Serial.print(OBD_Coolant.getName()); 
//	Serial.print(OBD_Coolant.getData());
//	Serial.println(OBD_Coolant.getUnits()); 
//
//	Serial.print(OBD_EngineLoad.getName()); 
//	Serial.print(OBD_EngineLoad.getData());
//	Serial.println(OBD_EngineLoad.getUnits()); 
//
//	Serial.print(OBD_MAF.getName()); 
//	Serial.print(OBD_MAF.getData());
//	Serial.println(OBD_MAF.getUnits()); 

  Serial.print(OBD_MAP.getName()); 
  Serial.print(OBD_MAP.getData());
  Serial.println(OBD_MAP.getUnits()); 
  byte byt = OBD_MAP.getIntData();
  //Serial3.write(byt);
    //this is how you access the variables. [name of the group].[variable name]
  mydata.boost = byt;
  //send the data
  digitalWrite(RGB_BLUE, LOW);
  ET.sendData();
  digitalWrite(RGB_BLUE, HIGH);
//
//	Serial.print(OBD_IAT.getName()); 
//	Serial.print(OBD_IAT.getData());
//	Serial.println(OBD_IAT.getUnits());         	

  Serial.println();
  Serial.println();
  
  digitalWrite(RGB_GREEN, HIGH);
}

