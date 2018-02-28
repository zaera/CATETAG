/*
Flag address: sector 1 block 4

Format: XX YY ZZ  where XX (buffer[0]) - Flag, YY (buffer[1]) - block address to write, ZZ (buffer[2]) - point(1 or 2 or 3 parts of block line)

CP flag - 207(CF)     = 1
Start flag - 204(CC)  = 2
Finish flag - 255(FF) = 3
Erase flag - 238(EE)  = 4
Test flag - 221(DD)   = 5
Battery flag - 170(AA)= 6

EEPROM(0,a) bytes:
1 - CP
2 - Start
3 - Finish
4 - Erase
5 - Test (add number to cp)
EEPROM.write(0, eeprom);
eeprom=EEPROM.read(0);
Serial.println(eeprom, DEC);
*/
#include <avr/sleep.h>
#include <avr/power.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
//---------------------------------------------------------------------------------------
uint8_t eeprom;
uint8_t flagp;

int reader=0;
int eraser=0;
int dumper=0;
//int batcharge = 2;
//int buzzPin = 4;
int buzzPin = 4;
int led3 = 3;
int led2 = 6;
int led1 = 7;
int supply = 5;
int v = 6;
long flag=101;
int jj=0;
unsigned long mtime;
unsigned long delta;
int mmtime=0;
int a=0;
int incomingByte = 0; 
//---------------------------------------------------------------------------------------
tmElements_t tm;
//---------------------------------------------------------------------------------------
void setup()
{    

  
//Serial.begin(250000);
Serial.begin(2000000);


pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
//pinMode(batcharge, OUTPUT);
pinMode(supply, INPUT);
pinMode(buzzPin, OUTPUT);
//digitalWrite(batcharge, HIGH);
eeprom=EEPROM.read(0);
flagp=EEPROM.read(1);
flag=flag+flagp;
if      (eeprom==1){cp();}
else if (eeprom==2) {s();}
else if (eeprom==3) {f();}




while (!Serial);
SPI.begin();        // Init SPI bus
mfrc522.PCD_Init(); // Init MFRC522 card
 // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
}
//---------------------------------------------------------------------------------------
void(* resetFunc) (void) = 0; //declare reset function @ address 0
//---------------------------------------------------------------------------------------
void loop(){
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  digitalWrite(buzzPin, LOW);
  RTC.read(tm); 
  synctime();
     if (Serial.available() > 0) {
     incomingByte = Serial.read();     
 if (incomingByte == 73)
 {
    Serial.println();
    Serial.println("######SERVICE######");
    Serial.print("CateTag v 7    Hello, coach.");Serial.println();
    Serial.print("My cp name is: "); Serial.print(flag); Serial.println();
    Serial.print("DELTA(ms)= "); Serial.print(delta); 
    Serial.println();
    RTC.read(tm);
    Serial.print("Time = ");
    Serial.print(tm.Hour);
    Serial.write(':');
    Serial.print(tm.Minute);
    Serial.write(':');
    Serial.print(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
     if      (eeprom==1){cp(); Serial.println("CP");}
else if (eeprom==2) {s(); Serial.println("START");}
else if (eeprom==3) {f(); Serial.println("FINISH");}
else if (eeprom==4) {e(); Serial.println("ERASE");}
Serial.println("###################"); 
Serial.println();
 delay(800);
  }
      else if (incomingByte == 63)
     {
     getinfo();  
    }
      else if (incomingByte == 83)
   {Serial.println();
    Serial.println(" RESTARTING...");
    Serial.println();
   delay(800);
    resetFunc();  //call reset
    }
     else if (incomingByte == 99)
   {Serial.println();
    Serial.println(" Making CP..");
    Serial.println();
   delay(800);
   eraser=0;
   mcp();
    }    
       else if (incomingByte == 102)
   {Serial.println();
    Serial.println(" Making FINISH..");
    Serial.println();
   delay(800);
   eraser=0;
   mf();
    }
    else if (incomingByte == 68)
   {Serial.println();
    Serial.println(" DUMP..");
    Serial.println();
    delay(800);
    dumper=1;
    eraser=0;
    reader=0;
    dump();
    }
     else if (incomingByte == 101)
   {Serial.println();
    Serial.println(" Making ERASER..");
    Serial.println();
    e();
   delay(800);
   dumper=0;
   reader=0;
   eraser=1;
    }
     else if (incomingByte == 115)
   {Serial.println();
    Serial.println(" Making START..");
    Serial.println();
   delay(800);
   eraser=0;
   ms();
    }
       else if (incomingByte == 88)
   {Serial.println();
     char incomingBytes1[3];
   Serial.readBytes(incomingBytes1, 3);
   flagp=((incomingBytes1[0]-48)*100)+((incomingBytes1[1]-48)*10)+((incomingBytes1[2]-48));
  flagp=flagp-101; 
    Serial.println(" Changing CP number to ");   
   Serial.print(flagp+101);
   Serial.println();
   delay(800);
   EEPROM.write(1, flagp);
   resetFunc();  //call reset
    }
    
     else if (incomingByte == 76)
   {Serial.println();
     char incomingBytes1[3];
   Serial.readBytes(incomingBytes1, 3);
 //  flagp=((incomingBytes1[0]-48)*100)+((incomingBytes1[1]-48)*10)+((incomingBytes1[2]-48));
 // flagp=flagp-101; 
  //  Serial.println(" Changing CP number to ");   
 //  Serial.print(flagp+101);
 //  Serial.println();
 //  delay(800);
////   EEPROM.write(1, flagp);
   resetFunc();  //call reset
    }
    
else if (incomingByte == 84)
   {
   char incomingBytes[12];
    Serial.readBytes(incomingBytes, 12);
    Serial.println();
    Serial.println();
    Serial.println("My current time is:");
    RTC.read(tm);
    Serial.print(tm.Hour);
    Serial.write(':');
    Serial.print(tm.Minute);
    Serial.write(':');
    Serial.print(tm.Second);
    Serial.print("  Date  ");
    Serial.print(tm.Day);
    Serial.write('.');
    Serial.print(tm.Month);
    Serial.write('.');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
   tm.Hour =   ((incomingBytes[0]-48)*10)+ incomingBytes[1]-48;
   tm.Minute = ((incomingBytes[2]-48)*10)+ incomingBytes[3]-48;
   tm.Second = ((incomingBytes[4]-48)*10)+ incomingBytes[5] -48;
   tm.Day =    ((incomingBytes[6]-48)*10)+ incomingBytes[7]-48;
   tm.Month =  ((incomingBytes[8]-48)*10)+ incomingBytes[9]-48;
   tm.Year = CalendarYrToTm(2000 + ((incomingBytes[10]-48)*10)+ incomingBytes[11]-48);
   RTC.write(tm);  
      Serial.println("My new time will be:");
      Serial.print(tm.Hour); Serial.print(":"); Serial.print(tm.Minute); Serial.print(":"); Serial.print(tm.Second); Serial.print("  Date  "); Serial.print(tm.Day); Serial.print("."); Serial.print(tm.Month); Serial.print("."); Serial.print(tm.Year+1970);
       Serial.println();
       delay(800);
    resetFunc();  //call reset
    }

 
else if (incomingByte == 82)
{ 
  dumper=0;
  eraser=0;
  reader=1;
  rmode();
  }
}    
//---------------------------------------------------------------------------------------
      if (reader==1 && eraser==0 && dumper==0){readermode(); return;}
      if (eraser==1 && reader==0 && dumper==0){erasemode(); return;}
 else if (dumper==1 && reader==0 && eraser==0){dump(); return;}
//---------------------------------------------------------------------------------------
else{
      if (eeprom==1){reader==0; eraser==0; cpmode();    }
 else if (eeprom==2){reader==0; eraser==0; startmode(); }
 else if (eeprom==3){reader==0; eraser==0; finishmode();}
}


}
//---------------------------------------------------------------------------------------
//-----------------------------ADDITIONAL PROCEDURES-------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
void cp(){digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); delay(40); digitalWrite(buzzPin, LOW); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);  }
//-------------------------------------------------------------------------- ---------------------------------------------------------
void success(){  delay(100);      digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH); digitalWrite(buzzPin, HIGH); delay(30);digitalWrite(buzzPin, LOW); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, HIGH);}
//-----------------------------------------------------------------------------------------------------------------------------------
void s(){digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); delay(100);digitalWrite(buzzPin, LOW); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);  delay(100); digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); delay(100);digitalWrite(buzzPin, LOW); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);}
//-----------------------------------------------------------------------------------------------------------------------------------
void f(){digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); delay(100); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW); delay(100); digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); delay(100);digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW); delay(100);digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); delay(100);  digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);}
//-----------------------------------------------------------------------------------------------------------------------------------
void rmode(){digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(700);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(600);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(500);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(400);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(300);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(200);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);}
//-----------------------------------------------------------------------------------------------------------------------------------
void e(){
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(100); 
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); 
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);}
//-----------------------------------------------------------------------------------------------------------------------------------
void error(){ digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, HIGH);  

}
//-----------------------------------------------------------------------------------------------------------------------------------
void bat(){digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);delay(50); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);}
//-----------------------------------------------------------------------------------------------------------------------------------
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
void dump_byte_array1(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  
 
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
 
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both
 
  long result = (high<<8) | low;
 
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
void battery() {
long bb=readVcc();
bb=bb+637;
//Serial.println(bb);
long ret;
     if (bb<=2841)             {ret=1;}
else if (bb>2841 && bb<=2982)  {ret=2;}
else if (bb>2982 && bb<=3123)  {ret=3;}
else if (bb>3123 && bb<=3264)  {ret=4;}
else if (bb>3264 && bb<=3405)  {ret=5;}
else if (bb>3405 && bb<=3546)  {ret=6;}
else if (bb>3546 && bb<=3687)  {ret=7;}
else if (bb>3687 && bb<=3828)  {ret=8;}
else if (bb>3828 && bb<=3969)  {ret=9;}
else if (bb>3969)              {ret=10;}
//Serial.println(bb);
//Serial.println(ret);
for (int i =0; i<ret; i++){digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);delay(200); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);delay(300);}
}
//---------------------------------------------------------------------------------------
void mcp(){eeprom=1; EEPROM.write(0, eeprom);  resetFunc();}
//---------------------------------------------------------------------------------------
void mf(){eeprom=3; EEPROM.write(0, eeprom);     resetFunc();}
//---------------------------------------------------------------------------------------
void ms(){eeprom=2; EEPROM.write(0, eeprom);     resetFunc();}
//---------------------------------------------------------------------------------------
void synctime(){if(jj==0){
 
 
  if (RTC.read(tm)) {
  if (tm.Second==05 || tm.Second==10 || tm.Second==15 || tm.Second==20 || tm.Second==25 || tm.Second==30 || tm.Second==35 || tm.Second==40 || tm.Second==45 || tm.Second==50 || tm.Second==55 || tm.Second==0)
  {delta = millis();jj=1;  //Serial.println("DONE!"); Serial.println(delta); 
digitalWrite(buzzPin, HIGH);
delay(30);
digitalWrite(buzzPin, LOW); 
delay(100);
//digitalWrite(buzzPin, LOW);
return;}
  else {//Serial.println(tm.Second); 
    
   
  
  digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);delay(100);digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW);
 }
  }
  else {delay(900);}
  }}
//-----------------------------------------------------------------------------------------------------------------------------------
void getinfo(){Serial.println();
    Serial.println("INFO                   I  73");
    Serial.println("RESTART                   S 83");
    Serial.println("SETTIME                   T 84");
    Serial.println("enterREADER                   R 82");
    Serial.println("exit READER                   E 69");
    Serial.println("-----------------------");
    Serial.println("make CP                   c 99");
    Serial.println("make FINISH                   f 102");
    Serial.println("make ERASER                   e 101");
    Serial.println("make START                   s  115");
    Serial.println("-----------------------");
    Serial.println("oneSTARTtime                   N  78");
    Serial.println("change CP                   X 88");
    Serial.println();
   delay(800);}
//-----------------------------------------------------------------------------------------------------------------------------------
void cpmode(){

  byte status;
byte buffer[18];
byte size = sizeof(buffer);
  if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    // Show some details of the PICC (that is: the tag/card)
   // Serial.print(F("Card UID:"));
    //dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    //Serial.println();
   // byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Check for compatibility
 //   if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
//        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
//        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        //Serial.println(F("This sample only works with MIFARE Classic cards."));
//        return;
 //   }
     // Authenticate using key B
    //Serial.println(F("Authenticating again using key B...nnn"));
    byte trailerBlock   = 7;
  //  MFRC522::StatusCode status;
       status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
  //      Serial.print(F("PCD_Authenticate() failed: "));
  //      Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    // Read data from the sector 1 block 4 
    //Serial.print(F("Reading data from block ")); Serial.print(4);
    //Serial.println(F(" ..."));
   status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
  //     Serial.print(F("MIFARE_Read() failed: "));
  //      Serial.println(mfrc522.GetStatusCodeName(status));
        // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
    
    
    
   //dump_byte_array(buffer, 16); //Serial.println();
  if (buffer[3]==207){Serial.println(); Serial.println(" Making CP.."); Serial.println(); delay(20); mcp();}
     else if (buffer[3]==204){Serial.println(); Serial.println(" Making START.."); Serial.println(); delay(20);  ms(); }
     else if (buffer[3]==255){Serial.println();Serial.println(" Making FINISH.."); Serial.println(); delay(20);  mf(); }
     else if (buffer[3]==238){e(); dumper=0; reader=0; eraser=1; Serial.println();Serial.println(" Making ERASER.."); Serial.println();}  
     else if (buffer[3]==221){eeprom=1; cp();  flag=flag+1; Serial.println(flag);    mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
     else if (buffer[3]==170){ battery(); mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return; }
    
    
    
    
    
    
    
    
    
    mtime  = millis()-delta;
    a=mtime/1000;
    a=a*1000;
    mtime=(mtime-a)/10;
    if (mtime>100){mmtime=mtime/10; mtime=mmtime;}
    RTC.read(tm);
 
    
    
    byte address;
    byte address2;
    byte point;
    byte point2;
    //byte trailerBlock;
    byte dataBlock[16];
    byte dataBlock2[16];
    int r=0;
    int u;
    address=buffer[1];
    point  =buffer[2];
     u= (address/4); //sector
    trailerBlock   = (u*4)+3;
    if (point == 1){r=0;}
    else if (point==2){r=5;}
    else if (point==3){r=10;}
 
 
    
    if (buffer[0]==255) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else if (buffer[0]==0) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else if (buffer[0]==flag) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else if (address==62 && r==10){mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else {
  
     if ((point+1)==4){point2=1; address2=address+1;}
    else {point2 = point+1; address2=address;} 
    if (trailerBlock==(address2)){dataBlock[1]={address2=address2+1};}
    else {dataBlock[1]={address2};}
    dataBlock[0]={flag};
    dataBlock[2]={point2};
    dataBlock2[0+r]={flag};
    dataBlock2[1+r]={tm.Hour};
    dataBlock2[2+r]={tm.Minute};
    dataBlock2[3+r]={tm.Second};  
    dataBlock2[4+r]={mtime};   


    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {return;}
//---------------------------------------------------------------------------------------
//dataBlock
// -----------------------------Write (FLAG) data to the Sector 1 Block 4
 //   dump_byte_array(dataBlock, 16); 
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(4, dataBlock, 16);
// ----------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
     //   Serial.print(F("PCD_Authenticate() failed: "));
      //  Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
// --------------------------------------------------------------------------------------
//address, dataBlock2
// -----------------------------Write data to the block---------------------------------


status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(address, buffer, &size);
    
    
    
     
    
    
    
  if (point == 1)
{
    dataBlock2[0]={flag};
    dataBlock2[1]={tm.Hour};
    dataBlock2[2]={tm.Minute};
    dataBlock2[3]={tm.Second};  
    dataBlock2[4]={mtime};   
    dataBlock2[5]=0;
    dataBlock2[6]=0;
    dataBlock2[7]=0;
    dataBlock2[8]=0;  
    dataBlock2[9]=0;
    dataBlock2[10]=0;
    dataBlock2[11]=0;
    dataBlock2[12]=0;
    dataBlock2[13]=0;  
    dataBlock2[14]=0; 
}  
  if (point==2)
  {
    dataBlock2[0]=buffer[0];
    dataBlock2[1]=buffer[1];
    dataBlock2[2]=buffer[2];
    dataBlock2[3]=buffer[3];  
    dataBlock2[4]=buffer[4];
    dataBlock2[5]={flag};
    dataBlock2[6]={tm.Hour};
    dataBlock2[7]={tm.Minute};
    dataBlock2[8]={tm.Second};  
    dataBlock2[9]={mtime};  
    dataBlock2[10]=0;
    dataBlock2[11]=0;
    dataBlock2[12]=0;
    dataBlock2[13]=0;  
    dataBlock2[14]=0;  
  } 
else if (point==3)
  {
    dataBlock2[0]=buffer[0];
    dataBlock2[1]=buffer[1];
    dataBlock2[2]=buffer[2];
    dataBlock2[3]=buffer[3];  
    dataBlock2[4]=buffer[4];
    dataBlock2[5]=buffer[5];
    dataBlock2[6]=buffer[6];
    dataBlock2[7]=buffer[7];
    dataBlock2[8]=buffer[8];  
    dataBlock2[9]=buffer[9];
    dataBlock2[10]={flag};
    dataBlock2[11]={tm.Hour};
    dataBlock2[12]={tm.Minute};
    dataBlock2[13]={tm.Second};  
    dataBlock2[14]={mtime};  
  } 







  //  dump_byte_array(dataBlock2, 16);// Serial.println();
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(address, dataBlock2, 16);
    if (status != MFRC522::STATUS_OK) {
    //    Serial.print(F("MIFARE_Write() failed: "));
    //    Serial.println(mfrc522.GetStatusCodeName(status));
     mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
  
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(address, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
   
    }
 
   // dump_byte_array(buffer, 16);
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with dataBlock (= what we've written)
        if (buffer[i] == dataBlock2[i])
            count++;
    }
 //Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
 success();      
  }
      //  else {
          //Serial.println(F("Failure, no match :-(")); Serial.println(F("  perhaps the write didn't work properly..."));
    //  }
       // Serial.println();
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------   
  //  Serial.println(F("We are working until here"));  
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
// Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
 

  }
}
//-----------------------------------------------------------------------------------------------------------------------------------
void startmode(){
  
byte status;
byte buffer[18];
byte size = sizeof(buffer);
  if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    // Show some details of the PICC (that is: the tag/card)
   // Serial.print(F("Card UID:"));
    //dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    //Serial.println();
   // byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Check for compatibility
  //  if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
  //      &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
  //      &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        //Serial.println(F("This sample only works with MIFARE Classic cards."));
  //      return;
 //   }
     // Authenticate using key B
    //Serial.println(F("Authenticating again using key B...nnn"));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
//        Serial.print(F("PCD_Authenticate() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
    // Read data from the sector 1 block 4 
    //Serial.print(F("Reading data from block ")); Serial.print(4);
    //Serial.println(F(" ..."));
status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
//        Serial.print(F("MIFARE_Read() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
    
    
    
   //dump_byte_array(buffer, 16); //Serial.println();
     if (buffer[3]==207){Serial.println(); Serial.println(" Making CP.."); Serial.println(); delay(20);  mcp();}
     else if (buffer[3]==204){Serial.println(); Serial.println(" Making START.."); Serial.println(); delay(20);  ms(); }
     else if (buffer[3]==255){Serial.println();Serial.println(" Making FINISH.."); Serial.println(); delay(20);  mf(); }
     else if (buffer[3]==238){e(); dumper=0; reader=0; eraser=1; Serial.println();Serial.println(" Making ERASER.."); Serial.println();}  
     else if (buffer[3]==221){eeprom=1; cp();  flag=flag+1; Serial.println(flag);    mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
     else if (buffer[3]==170){ battery(); mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return; }
    
    
    
    mtime  = millis()-delta;
    a=mtime/1000;
    a=a*1000;
    mtime=(mtime-a)/10;
    if (mtime>100){mmtime=mtime/10; mtime=mmtime;}
    RTC.read(tm);
 
    
    
    byte address;
    byte point;
    byte trailerBlock;
    byte dataBlock[16];
    byte dataBlock2[16];
    int r=0;
    int u;
    address=buffer[1];
    point  =buffer[2];
     u= (address/4); //sector
    trailerBlock   = (u*4)+3;
    if (point == 1){r=0;}
    else if (point==2){r=5;}
    else if (point==3){r=10;}
 
 
    
    if (buffer[0]==0){
    address=5;
    point=1;
    u= (address/4); //sector
    trailerBlock   = (u*4)+3;
    dataBlock[0]={0xCC};
    dataBlock[1]={5};
    dataBlock[2]={2};
    dataBlock2[0+r]={0xCC};
    dataBlock2[1+r]={tm.Hour};
    dataBlock2[2+r]={tm.Minute};
    dataBlock2[3+r]={tm.Second};  
    dataBlock2[4+r]={mtime};  
    
    
    
    
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {return;}
//---------------------------------------------------------------------------------------
//dataBlock
// -----------------------------Write (FLAG) data to the Sector 1 Block 4
   // dump_byte_array1(dataBlock, 16); 
     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(4, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {      
} 
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {    
      }
  //  dump_byte_array1(buffer, 16); 
  //  byte count1 = 0;
  //  for (byte i = 0; i < 16; i++) {
        
  //      if (buffer[i] == dataBlock[i])
  //          count1++;}
  //  if (count1 == 16) {
  //}
  //      else {}    
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
     //   Serial.print(F("PCD_Authenticate() failed: "));
      //  Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
// ------------------------------------------------------------------------------------------------------------------------
//address, dataBlock2
// -----------------------------Write data to the block--------------------------------------------------------------------
  //  dump_byte_array(dataBlock2, 16);// Serial.println();
     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(address, dataBlock2, 16);
    if (status != MFRC522::STATUS_OK) {
    //    Serial.print(F("MIFARE_Write() failed: "));
    //    Serial.println(mfrc522.GetStatusCodeName(status));
     mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
  
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(address, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
   
    }
 
    //dump_byte_array(buffer, 16);
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with dataBlock (= what we've written)
        if (buffer[i] == dataBlock2[i])
            count++;
    }
 //Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
       // digitalWrite(led1, HIGH);digitalWrite(led2, HIGH);digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); digitalWrite(buzzPin, LOW); delay(1); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, HIGH);
      success();      
  }
      //  else {
          //Serial.println(F("Failure, no match :-(")); Serial.println(F("  perhaps the write didn't work properly..."));
    //  }
       // Serial.println();
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------   
  //  Serial.println(F("We are working until here"));  
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
// Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    
    
    
    
    
 }
else {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}    
}
//-----------------------------------------------------------------------------------------------------------------------------------
void finishmode(){

  byte status;
byte buffer[18];
byte size = sizeof(buffer);
  if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    // Show some details of the PICC (that is: the tag/card)
   // Serial.print(F("Card UID:"));
    //dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    //Serial.println();
   // byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Check for compatibility
 //   if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
 //       &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
 //       &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        //Serial.println(F("This sample only works with MIFARE Classic cards."));
 //       return;
 //   }
     // Authenticate using key B
    //Serial.println(F("Authenticating again using key B...nnn"));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
   //     Serial.print(F("PCD_Authenticate() failed: "));
    //    Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
    // Read data from the sector 1 block 4 
    //Serial.print(F("Reading data from block ")); Serial.print(4);
    //Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
   //     Serial.print(F("MIFARE_Read() failed: "));
   //     Serial.println(mfrc522.GetStatusCodeName(status));
        // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
    
    
   //dump_byte_array(buffer, 16); //Serial.println();
     if (buffer[3]==207){Serial.println(); Serial.println(" Making CP.."); Serial.println(); delay(20);  mcp();}
     else if (buffer[3]==204){Serial.println(); Serial.println(" Making START.."); Serial.println(); delay(20);  ms(); }
     else if (buffer[3]==255){Serial.println();Serial.println(" Making FINISH.."); Serial.println(); delay(20);  mf(); }
     else if (buffer[3]==238){e(); dumper=0; reader=0; eraser=1; Serial.println();Serial.println(" Making ERASER.."); Serial.println();}  
     else if (buffer[3]==221){eeprom=1; cp();  flag=flag+1; Serial.println(flag);    mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
     else if (buffer[3]==170){ battery(); mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return; }
    
    
    
    
    
    mtime  = millis()-delta;
    a=mtime/1000;
    a=a*1000;
    mtime=(mtime-a)/10;
    if (mtime>100){mmtime=mtime/10; mtime=mmtime;}
    RTC.read(tm);
 
    
    
    byte address;
    byte address2;
    byte point;
    byte point2;
    byte trailerBlock;
    byte dataBlock[16];
   // byte dataBlockCheck[16];
    byte dataBlock2[16];   
    int u;
    
 
    
    
    
    address=buffer[1];
    point  =buffer[2];
     u= (address/4); //sector
    trailerBlock   = (u*4)+3;
       
    if (buffer[0]==0) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else if (buffer[0]==255) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else {    
    if ((point+1)==4){point2=1; address2=address+1;}
    else {point2 = point+1; address2=address;} 
    if (trailerBlock==(address2)){dataBlock[1]={address2=address2+1};}
    else {dataBlock[1]={address2};}
   
    dataBlock[0]={0xFF};
    dataBlock[2]={point2};
    
  //  dataBlockCheck[0]=dataBlock[0];
   // dataBlockCheck[1]=dataBlock[1];
  //  dataBlockCheck[2]=dataBlock[2];
  //  dataBlockCheck[4]=dataBlock[0];
    
    
    
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {return;}
//---------------------------------------------------------------------------------------
//dataBlock
// -----------------------------Write (FLAG) data to the Sector 1 Block 4
 //   dump_byte_array(dataBlock, 16); 
     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(4, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {      
} 
  //  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(4, buffer, &size);
  //  if (status != MFRC522::STATUS_OK) {    
  //    }
  //  dump_byte_array(buffer, 16); 
  //  byte count1 = 0;
  //  for (byte i = 0; i < 16; i++) {
        
  //      if (buffer[i] == dataBlock[i])
  //          count1++;}
  //  if (count1 == 16) {
 // }
           
// ----------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// Authenticate to needed block using key B
   // Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
     //   Serial.print(F("PCD_Authenticate() failed: "));
      //  Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
// --------------------------------------------------------------------------------------
//address, dataBlock2
// -----------------------------Write data to the block---------------------------------
status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(address, buffer, &size);
    
  if (point == 1)
{
    dataBlock2[0]={0xFF};
    dataBlock2[1]={tm.Hour};
    dataBlock2[2]={tm.Minute};
    dataBlock2[3]={tm.Second};  
    dataBlock2[4]={mtime};
    dataBlock2[5]=0;
    dataBlock2[6]=0;
    dataBlock2[7]=0;
    dataBlock2[8]=0;  
    dataBlock2[9]=0;
    dataBlock2[10]=0;
    dataBlock2[11]=0;
    dataBlock2[12]=0;
    dataBlock2[13]=0;  
    dataBlock2[14]=0; 
}  
  if (point==2)
  {
    dataBlock2[0]=buffer[0];
    dataBlock2[1]=buffer[1];
    dataBlock2[2]=buffer[2];
    dataBlock2[3]=buffer[3];  
    dataBlock2[4]=buffer[4];
    dataBlock2[5]={0xFF};
    dataBlock2[6]={tm.Hour};
    dataBlock2[7]={tm.Minute};
    dataBlock2[8]={tm.Second};  
    dataBlock2[9]={mtime}; 
    dataBlock2[10]=0;
    dataBlock2[11]=0;
    dataBlock2[12]=0;
    dataBlock2[13]=0;  
    dataBlock2[14]=0;  
  } 
else if (point==3)
  {
    dataBlock2[0]=buffer[0];
    dataBlock2[1]=buffer[1];
    dataBlock2[2]=buffer[2];
    dataBlock2[3]=buffer[3];  
    dataBlock2[4]=buffer[4];
    dataBlock2[5]=buffer[5];
    dataBlock2[6]=buffer[6];
    dataBlock2[7]=buffer[7];
    dataBlock2[8]=buffer[8];  
    dataBlock2[9]=buffer[9];
    dataBlock2[10]={0xFF};
    dataBlock2[11]={tm.Hour};
    dataBlock2[12]={tm.Minute};
    dataBlock2[13]={tm.Second};  
    dataBlock2[14]={mtime};  
  } 









  //  dump_byte_array(dataBlock2, 16);// Serial.println();
     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(address, dataBlock2, 16);
    if (status != MFRC522::STATUS_OK) {
    //    Serial.print(F("MIFARE_Write() failed: "));
    //    Serial.println(mfrc522.GetStatusCodeName(status));
     mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
  
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(address, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
   
    }
 
  //  dump_byte_array(buffer, 16);
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with dataBlock (= what we've written)
        if (buffer[i] == dataBlock2[i])
            count++;
    }
 //Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
 success();      
  }
     //   else {
          //Serial.println(F("Failure, no match :-(")); Serial.println(F("  perhaps the write didn't work properly..."));
    //  }
           // Serial.println();
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------   
  //  Serial.println(F("We are working until here"));  
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
// Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    
    
    
    
    
 
}
}
//-----------------------------------------------------------------------------------------------------------------------------------
void erasemode(){
  
byte status;
byte buffer[18];
byte size = sizeof(buffer);
  if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    // Show some details of the PICC (that is: the tag/card)
   // Serial.print(F("Card UID:"));
    //dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    //Serial.println();
    //byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Check for compatibility
 //   if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
 //       &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
 //       &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
 //       //Serial.println(F("This sample only works with MIFARE Classic cards."));
 //       return;
 //   }
     // Authenticate using key B
    //Serial.println(F("Authenticating again using key B...nnn"));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
    // Read data from the sector 1 block 4 
    //Serial.print(F("Reading data from block ")); Serial.print(4);
    //Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
  //dump_byte_array(buffer, 16); //Serial.println();
     if (buffer[3]==207){Serial.println(); Serial.println(" Making CP.."); Serial.println(); delay(20);  mcp();}
     else if (buffer[3]==204){Serial.println(); Serial.println(" Making START.."); Serial.println(); delay(20);  ms(); }
     else if (buffer[3]==255){Serial.println();Serial.println(" Making FINISH.."); Serial.println(); delay(20);  mf(); }
     else if (buffer[3]==238){e(); dumper=0; reader=0; eraser=1; Serial.println();Serial.println(" Making ERASER.."); Serial.println();}  
     else if (buffer[3]==221){eeprom=1; cp();  flag=flag+1; Serial.println(flag);    mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
     else if (buffer[3]==170){ battery(); mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return; }
  
  
    byte sectorE         = 1;
    byte blockAddrE      = 4;
    byte dataBlockE[]    = {
        0x00, 0x00, 0x00, 0x00, //  1,  2,   3,  4,
        0x00, 0x00, 0x00, 0x00, //  5,  6,   7,  8,
        0x00, 0x00, 0x00, 0x00, //  9, 10, 255, 12,
        0x00, 0x00, 0x00, 0x00  // 13, 14,  15, 16
    };
    
    byte trailerBlockE   = 7;
    byte statusE;
  //  byte buffer[18];
   // byte size = sizeof(buffer);   
    for (byte cc = 0; cc < 15; cc++) {
     Serial.println(cc);
    // РџРѕРєР°Р·Р°С‚СЊ РїРѕР»РЅРѕСЃС‚СЊСЋ С‚РµРєСѓС‰РёР№ СЃРµРєС‚РѕСЂ
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sectorE+cc);
    Serial.println();
    // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlockE, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }  
    for (byte bb = 0; bb < 3; bb++) {

  // -----------------------------Write data to the block
    Serial.print(F("Writing data into block ")); Serial.print(blockAddrE+bb);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlockE, 16); Serial.println();
     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddrE+bb, dataBlockE, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();
       
    // Read data from the block (again, should now be what we have written)
    Serial.print(F("Reading data from block ")); Serial.print(blockAddrE+bb);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddrE+bb, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddrE+bb); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
        
    // Check that data in block is what we have written
    // by counting the number of bytes that are equal
    Serial.println(F("Checking result..."));
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with dataBlock (= what we've written)
        if (buffer[i] == dataBlockE[i])
            count++;
    }
    Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {Serial.println(F("Success :-)")); digitalWrite(led1, HIGH); digitalWrite(led2, HIGH); digitalWrite(led3, HIGH);digitalWrite(buzzPin, HIGH); delay(1); digitalWrite(led1, LOW); digitalWrite(led2, LOW); digitalWrite(led3, LOW); digitalWrite(buzzPin, LOW);//success();       
  } else {Serial.println(F("Failure, no match :-(")); Serial.println(F("  perhaps the write didn't work properly..."));
      }
        Serial.println();}  
    if (blockAddrE+4==62){mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
    else
    blockAddrE=blockAddrE+4;
    trailerBlockE=trailerBlockE+4;
    }    
// Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
}
//-----------------------------------------------------------------------------------------------------------------------------------
void readermode(){
byte status;
byte buffer[18];
byte size = sizeof(buffer);
  if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    // Show some details of the PICC (that is: the tag/card)
//Serial.print(F("Card UID:"));
//  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
 //   Serial.println();
   // byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Check for compatibility
 //   if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
//        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
 //       &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
 //       Serial.println(F("This sample only works with MIFARE Classic cards."));
 //       return;
 //   }
     // Authenticate using key B
//    Serial.println(F("Authenticating again using key B...nnn"));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
    // Read data from the sector 1 block 4 
 //   Serial.print(F("Reading data from block ")); Serial.print(4);
 //   Serial.println(F(" ..."));
 //Serial.println();
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
    
    
   //dump_byte_array(buffer, 16); //Serial.println();
     if (buffer[3]==207){Serial.println(); Serial.println(" Making CP.."); Serial.println(); delay(20);  mcp();}
     else if (buffer[3]==204){Serial.println(); Serial.println(" Making START.."); Serial.println(); delay(20);  ms(); }
     else if (buffer[3]==255){Serial.println();Serial.println(" Making FINISH.."); Serial.println(); delay(20);  mf(); }
     else if (buffer[3]==238){e(); dumper=0; reader=0; eraser=1; Serial.println();Serial.println(" Making ERASER.."); Serial.println();}  
     else if (buffer[3]==221){eeprom=1; cp();  flag=flag+1; Serial.println(flag);    mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
     else if (buffer[3]==170){ battery(); mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return; }
    
    
//    Serial.print(F("Data in block ")); Serial.print(4); Serial.println(F(":"));
//    dump_byte_array(buffer, 16); Serial.println();
//Serial.println("-----------------");
   //Serial.println(buffer[3]);
   byte address;
   byte address0;
   byte address2=5;
   long flag;
   byte point;
   byte trailerBlock;
   byte timing;
   int u;
   int r=0;
   flag=     buffer[0];
   address=  buffer[1];
   address0=buffer[1];
   point   = buffer[2];
//   Serial.println(address);
 /*  if (buffer[2]==1 || buffer[2]==2){address=address;}
   else {address=address+1;}
   
if ((point-1)==0){point=3;}
   else {point=point-1;}
   if (point == 1){r=16;}
else if (point==2){r=11;}
else if (point==3){r=6;} */
   
     if (buffer[2]==1){address=address-1; point = 3;}
     else if (buffer[2]==2){address=address; point = 1;}
     else if (buffer[2]==3){address=address; point = 2;}
//   else {address=address+1;}
   
//Serial.println(address);
//Serial.println(point);


   
   
   
  // Serial.print(F("Card:"));
 //String uid;
 
 //uid = String(mfrc522.uid.uidByte, mfrc522.uid.size);
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    //Serial.print("|");
   // Serial.println();
   
   
     for (byte bb = 0; bb < address-4; bb++) {
       address2=5;
       address2=address2+bb;
       u= (address2/4);
       trailerBlock   = (u*4)+3;
      // Serial.println();
       //Serial.println(address2);

if (address2==trailerBlock){  continue;}
//   Serial.println(address2);
//       Serial.println(bb);



   // Authenticate using key B
//    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
    }
  
 // Read data from the sector 1 block 4 
//    Serial.print(F("Reading data from block ")); Serial.print(address2);
//    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(address2, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
//    Serial.print(F("Data in block ")); Serial.print(address2); Serial.println(F(":"));
 //   dump_byte_array(buffer, 16); Serial.println();
       
       unsigned long qqq;
       unsigned long a;
       unsigned long b;
       unsigned long c;
       unsigned long d;
       
       unsigned long x;
       String xx;
   if (address2==address0){
    for (byte bbb = 0; bbb <point; bbb++) {
     // if(buffer[4+(b*5)]>99){buffer[4+(b*5)]=buffer[4+(b*5)]-100;}
     //Serial.println("----------------------------------------------");
    // Serial.println(bbb*5);
       qqq=buffer[0+(bbb*5)];
        a =buffer[1+(bbb*5)];
        b =buffer[2+(bbb*5)];
        c =buffer[3+(bbb*5)];
        d =buffer[4+(bbb*5)];
        
        if (qqq <101){qqq=999;}
     
        unsigned long x= (a*360000)+(b*6000)+(c*100)+d;
        xx=String(x);
       // Serial.println();
       // Serial.println(x);
       // Serial.println();
        if      (xx.length()==1){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}    
        else if (xx.length()==2){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==3){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==4){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==5){Serial.print(qqq);Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==6){Serial.print(qqq);Serial.print("0"); Serial.print(x);}
        else                    {Serial.print(qqq); Serial.print(x);}
      // Serial.println();
     }  
 }  
     else{
      // if(buffer[4]>99){buffer[4]=buffer[4]-100;}
       //if(buffer[9]>99){buffer[9]=buffer[9]-100;}
       //if(buffer[14]>99){buffer[14]=buffer[14]-100;}
       qqq=buffer[0];
        a =buffer[1];
        b =buffer[2];
        c =buffer[3];
        d =buffer[4];
         if (qqq <101){qqq=999;}
        unsigned long x= (a*360000)+(b*6000)+(c*100)+d;
        xx=String(x);
        if      (xx.length()==1){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}    
        else if (xx.length()==2){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==3){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==4){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==5){Serial.print(qqq);Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==6){Serial.print(qqq);Serial.print("0"); Serial.print(x);}
        else                    {Serial.print(qqq); Serial.print(x);}
      // Serial.println();
      qqq=buffer[5];
        a =buffer[6];
        b =buffer[7];
        c =buffer[8];
        d =buffer[9];
         if (qqq <101){qqq=999;}
        x= (a*360000)+(b*6000)+(c*100)+d;
        xx=String(x);
        if      (xx.length()==1){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}    
        else if (xx.length()==2){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==3){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==4){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==5){Serial.print(qqq);Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==6){Serial.print(qqq);Serial.print("0"); Serial.print(x);}
        else                    {Serial.print(qqq); Serial.print(x);}
    // Serial.println();
    qqq=buffer[10];
        a =buffer[11];
        b =buffer[12];
        c =buffer[13];
        d =buffer[14];
         if (qqq <101){qqq=999;}
        x= (a*360000)+(b*6000)+(c*100)+d;
        xx=String(x);
        if      (xx.length()==1){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}    
        else if (xx.length()==2){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==3){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==4){Serial.print(qqq);Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==5){Serial.print(qqq);Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==6){Serial.print(qqq);Serial.print("0"); Serial.print(x);}
        else                    {Serial.print(qqq); Serial.print(x);}
    // Serial.println();
     
 }

  }
 Serial.print("***");
//Serial.println("-----------------");
// Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
}

void servicetag(){

if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F("PICC type: "));
   // byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Check for compatibility
 //   if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
 //       &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
 //       &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
 //       Serial.println(F("This sample only works with MIFARE Classic cards."));
 //       return;
 //   }

    // In this sample we use the second sector,
    // that is: sector #1, covering block #4 up to and including block #7
   
//CP flag - 207(CF)     = 1
//Start flag - 204(CC)  = 2
//Finish flag - 255(FF) = 3
//Erase flag - 238(EE)  = 4
//Test flag - 221(DD)   = 5
//Battery flag - 170(AA)= 6
   
    byte sector         = 1;
    byte blockAddr      = 4;
    byte dataBlock[]    = {
        0x00, 0x00, 0x00, 0x00, //  1,  2,   3,  4,
        0x00, 0x00, 0x00, 0x00, //  5,  6,   7,  8,
        0x00, 0x00, 0x00, 0x00, //  9, 10, 255, 12,
        0x00, 0x00, 0x00, 0x00  // 13, 14,  15, 16
    };
    byte trailerBlock   = 7;
    byte status;
    byte buffer[18];
    byte size = sizeof(buffer);
//MFRC522::StatusCode status;
    // Authenticate using key A
    Serial.println(F("Authenticating using key A..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Show the whole sector as it currently is
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    // Read data from the block
    Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
    Serial.println();

    // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }

    // Write data to the block
    Serial.print(F("Writing data into block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlock, 16); Serial.println();
     status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();

    // Read data from the block (again, should now be what we have written)
   Serial.print(F("Reading data from block ")); Serial.print(blockAddr);
    Serial.println(F(" ..."));
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
//        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(blockAddr); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();
        
    // Check that data in block is what we have written
    // by counting the number of bytes that are equal
    Serial.println(F("Checking result..."));
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with dataBlock (= what we've written)
        if (buffer[i] == dataBlock[i])
            count++;
    }
    Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
        Serial.println(F("Success :-)"));
    } else {
        Serial.println(F("Failure, no match :-("));
        Serial.println(F("  perhaps the write didn't work properly..."));
    }
    Serial.println();
        
    // Dump the sector data
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
    Serial.println();

    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

}


void dump(){
// Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}


