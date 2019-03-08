/****************************************************************************************	
**  This is example LINX firmware for use with the Arduino Uno with the serial 
**  interface enabled.
**
**  For more information see:           www.labviewmakerhub.com/linx
**  For support visit the forums at:    www.labviewmakerhub.com/forums/linx
**  
**  Written By Sam Kristoff
**
**  BSD2 License.
****************************************************************************************/

//Include All Peripheral Libraries Used By LINX
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
#include <Servo.h>

//Include Device Specific Header From Sketch>>Import Library (In This Case LinxChipkitMax32.h)
//Also Include Desired LINX Listener From Sketch>>Import Library (In This Case LinxSerialListener.h)
#include <LinxArduinoUno.h>
#include <LinxSerialListener.h>
 
//Create A Pointer To The LINX Device Object We Instantiate In Setup()
LinxArduinoUno* LinxDevice;

//Forward declaration for custom function
int clp();

//Initialize LINX Device And Listener
void setup()
{
  //Instantiate The LINX Device
  LinxDevice = new LinxArduinoUno();
  
  //The LINXT Listener Is Pre Instantiated, Call Start And Pass A Pointer To The LINX Device And The UART Channel To Listen On
  LinxSerialConnection.Start(LinxDevice, 0);
  
  //attach custom command to a LINX Listener so it can be called from LabVIEW
  LinxSerialConnection.AttachCustomCommand(0, clp);
}

void loop()
{
  //Listen For New Packets From LabVIEW
  LinxSerialConnection.CheckForCommands();
  
  //Your Code Here, But It will Slow Down The Connection With LabVIEW
}


//Custom function for paralell interface with 1602 LCD module
int clp(unsigned char numInputBytes, unsigned char* input, unsigned char* numResponseBytes, unsigned char* response)
{
#define RS 26
#define RW 27
#define E 28
#define DB0 34
#define DB1 35
#define DB2 36
#define DB3 37
#define DB4 30
#define DB5 31
#define DB6 32
#define DB7 33
  
  pinMode(RS, OUTPUT);
  pinMode(RW, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(DB0, OUTPUT);
  pinMode(DB1, OUTPUT);
  pinMode(DB2, OUTPUT);
  pinMode(DB2, OUTPUT);
  pinMode(DB3, OUTPUT);
  pinMode(DB4, OUTPUT);
  pinMode(DB5, OUTPUT);
  pinMode(DB6, OUTPUT);
  pinMode(DB7, OUTPUT);
   
int Channels[8] = {34, 35, 36, 37, 30, 31, 32, 33}; //This array contains the data bus pins from DB0 to DB7.
 
int variable[8]; //use for dummy array in order to bit shift each input byte
 
// FUNCTION SET
delay(30);
digitalWrite(E, HIGH);
digitalWrite(RS, LOW);
digitalWrite(RW, LOW);
digitalWrite(DB7, LOW);
digitalWrite(DB6, LOW);
digitalWrite(DB5, HIGH);
digitalWrite(DB4, HIGH);
digitalWrite(DB3, HIGH);
digitalWrite(DB2, HIGH);
digitalWrite(E, LOW);
delayMicroseconds(39);
 
// DISPLAY ON/OFF CONTROL
digitalWrite(E, HIGH);
digitalWrite(RS, LOW);
digitalWrite(RW, LOW);
digitalWrite(DB7, LOW);
digitalWrite(DB6, LOW);
digitalWrite(DB5, LOW);
digitalWrite(DB4, LOW);
digitalWrite(DB3, HIGH);
digitalWrite(DB2, HIGH);
digitalWrite(DB1, LOW);
digitalWrite(DB0, LOW);
digitalWrite(E, LOW);
delayMicroseconds(39);
 
// DISPLAY CLEAR
digitalWrite(E, HIGH);
digitalWrite(RS, LOW);
digitalWrite(RW, LOW);
digitalWrite(DB7, LOW);
digitalWrite(DB6, LOW);
digitalWrite(DB5, LOW);
digitalWrite(DB4, LOW);
digitalWrite(DB3, LOW);
digitalWrite(DB2, LOW);
digitalWrite(DB1, LOW);
digitalWrite(DB0, HIGH);
digitalWrite(E, LOW);
delayMicroseconds(1540);
 
// ENTRY MODE SET
digitalWrite(E, HIGH);
digitalWrite(RS, LOW);
digitalWrite(RW, LOW);
digitalWrite(DB7, LOW);
digitalWrite(DB6, LOW);
digitalWrite(DB5, LOW);
digitalWrite(DB4, LOW);
digitalWrite(DB3, LOW);
digitalWrite(DB2, HIGH);
digitalWrite(DB1, HIGH);
digitalWrite(DB0, LOW);
digitalWrite(E, LOW);
delay(2);
 
//end Initialization!!
 
 
//convert input to bytes
for(int i = 0; i < (numInputBytes); i++)
  {
    input[i] = byte(input[i]);
  }
   
//Read each byte from the input array and write each channel high or low for each bit position in the data bus.
for (int j=0; j < (numInputBytes); j++)
  {
    digitalWrite(E, HIGH); //prepare for a new letter or number by writing E HIGH and the following RS,RW values
    digitalWrite(RS, HIGH);
    digitalWrite(RW, LOW);
    for (int i=0; i<8; i++)
    {
      variable[i] = input[j] & 1;
      if (variable[i] == 1)
     {
        digitalWrite(Channels[i], HIGH);
     }
     else
     {
       digitalWrite(Channels[i], LOW);
     }
    input[j] = input [j] >> 1;
   }
  digitalWrite(E, LOW); //enable the display to show this input
  delayMicroseconds(50); //time needed for command to run
  }
  response[0]=0;
  *numResponseBytes = 1;
  return 0;
 
}
