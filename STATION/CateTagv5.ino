/*
Flag address: sector 1 block 4

Format: XX YY ZZ  where XX (buffer[0]) - Flag, YY (buffer[1]) - block address to write, ZZ (buffer[2]) - point(1 or 2 or 3 parts of block line)

CP flag - 207(CF)     = 1
Start flag - 204(CC)  = 2
Finish flag - 255(FF) = 3
Erase flag - 238(EE)  = 4
Test flag - 221(DD)   = 5


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
int pin2 = 2;
int buzzPin = 4;
int led = 3;
int supply = 5;
int v = 6;
long flag=101;

unsigned long mtime;
int mmtime=0;
int a=0;
//---------------------------------------------------------------------------------------
void setup()
{ 



//Serial.begin(115200);
pinMode(led, OUTPUT);
 pinMode(pin2, INPUT);
 pinMode(supply, INPUT);
pinMode(buzzPin, OUTPUT);


  eeprom=EEPROM.read(0);
Serial.print(eeprom);






if      (eeprom==1){cp(); Serial.println("we are CP. CP number is: "); Serial.print(flag);}
else if (eeprom==2) {s(); Serial.println("we are START");}
else if (eeprom==3) {f(); Serial.println("we are FINISH");}
else if (eeprom==4) {e(); Serial.println("we are ERASE");}
else if (eeprom==5) {cp(); Serial.println("we are TEST");}

while (!Serial);
SPI.begin();        // Init SPI bus
mfrc522.PCD_Init(); // Init MFRC522 card
// Prepare the key (used both as key A and as key B) using FFFFFFFFFFFFh which is the default at chip delivery from the factory
for (byte i = 0; i < 6; i++) {key.keyByte[i] = 0xFF;}
dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
}
//---------------------------------------------------------------------------------------
void loop(){ digitalWrite(buzzPin, LOW);
//---------------------------------------------------------------------------------------
byte status;
byte buffer[18];
byte size = sizeof(buffer);
  if ( ! mfrc522.PICC_IsNewCardPresent())
        return;
    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;
    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F("Card UID:"));
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return;
    }
     // Authenticate using key B
    Serial.println(F("Authenticating again using key B...nnn"));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
    // Read data from the sector 1 block 4 
    Serial.print(F("Reading data from block ")); Serial.print(4);
    Serial.println(F(" ..."));
    status = mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.print(F("Data in block ")); Serial.print(4); Serial.println(F(":"));
    dump_byte_array(buffer, 16); Serial.println();

   Serial.println(buffer[3]);
//eeprom reprogram  
//Start flag - 204(CC)  = 2
//Finish flag - 255(FF) = 3
//Erase flag - 238(EE)  = 4
//CP flag - 207(CF)     = 1
        if (buffer[3]==207){eeprom=1; EEPROM.write(0, eeprom); cp();  mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
   else if (buffer[3]==204){eeprom=2; EEPROM.write(0, eeprom); s();   mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
   else if (buffer[3]==255){eeprom=3; EEPROM.write(0, eeprom); f();  mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
   else if (buffer[3]==238){eeprom=4; EEPROM.write(0, eeprom); e();   mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}  
   else if (buffer[3]==221){eeprom=1; EEPROM.write(0, eeprom); cp();  flag=flag+1;    mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
   //else if (buffer[3]==205){Serial.println("SLEEP card. GOING TO SLEEP!... "); mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); delay(200); success(); success(); success();  success(); enterSleep(); return;}
   
   
//flag=flag+1; Serial.println("Control point is now: "); Serial.print(flag);   
   
   //eeprom reprogram 
//time

  mtime  = millis();
  a=mtime/1000;
  a=a*1000;
  mtime=(mtime-a)/10;
  
  if (mtime>100){mmtime=mtime/10; mtime=mmtime;}
  
  tmElements_t tm;
  RTC.read(tm);
//time
// Authenticate using key B
  //  Serial.println(F("Authenticating again using key B..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
   //     Serial.print(F("PCD_Authenticate() failed: "));
     //   Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }

//WRITE FLAG
byte trailerBlock;
byte address;
byte address2;
byte point;
byte point2;
byte dataBlock[16];
byte dataBlock2[16];
int u;


address=buffer[1];
point   = buffer[2];
 
u= (address/4); //sector
trailerBlock   = (u*4)+3;

int r=0;
if (point == 1){r=0;}
else if (point==2){r=5;}
else if (point==3){r=10;}

//Serial.println(address);
//Serial.println(trailerBlock);
//Serial.println(point);

//START
if      (eeprom==2){
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
 }
else {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}   
}

//FINISH
else if (eeprom==3){
if (buffer[0]==0) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else if (buffer[0]==255) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else {    
    if ((point+1)==4){point2=1; address2=address+1;}
    else {point2 = point+1; address2=address;} 
    if (trailerBlock==(address2)){dataBlock[1]={address2=address2+1};}
    else {dataBlock[1]={address2};}
    dataBlock[0]={0xFF};
    //dataBlock[1]={address2};
    dataBlock[2]={point2};
  
    dataBlock2[0+r]={0xFF};
    dataBlock2[1+r]={tm.Hour};
    dataBlock2[2+r]={tm.Minute};
    dataBlock2[3+r]={tm.Second};  
    dataBlock2[4+r]={mtime};
}
}
//ERASE
else if (eeprom==4){ 
  
  
  
    byte sectorE         = 1;
    byte blockAddrE      = 4;
    byte dataBlockE[]    = {
        0x00, 0x00, 0x00, 0x00, //  1,  2,   3,  4,
        0x00, 0x00, 0x00, 0x00, //  5,  6,   7,  8,
        0x00, 0x00, 0x00, 0x00, //  9, 10, 255, 12,
        0x00, 0x00, 0x00, 0x00  // 13, 14,  15, 16
    };
    /** byte dataBlockE[]    = {
        0x11, 0x11, 0x11, 0x11, //  1,  2,   3,  4,
        0x11, 0x11, 0x11, 0x11, //  5,  6,   7,  8,
        0x11, 0x11, 0x11, 0x11, //  9, 10, 255, 12,
        0x11, 0x11, 0x11, 0x11  // 13, 14,  15, 16
    }; */
    byte trailerBlockE   = 7;
    byte statusE;
    byte buffer[18];
    byte size = sizeof(buffer); 
    
    for (byte cc = 0; cc < 15; cc++) {
     Serial.println(cc);
    // Показать полностью текущий сектор
    Serial.println(F("Current data in sector:"));
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sectorE+cc);
    Serial.println();
    // Authenticate using key B
    Serial.println(F("Authenticating again using key B..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlockE, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
      
    for (byte bb = 0; bb < 3; bb++) {

  // -----------------------------Write data to the block
    Serial.print(F("Writing data into block ")); Serial.print(blockAddrE+bb);
    Serial.println(F(" ..."));
    dump_byte_array(dataBlockE, 16); Serial.println();
    status = mfrc522.MIFARE_Write(blockAddrE+bb, dataBlockE, 16);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
    Serial.println();
       
    // Read data from the block (again, should now be what we have written)
    Serial.print(F("Reading data from block ")); Serial.print(blockAddrE+bb);
    Serial.println(F(" ..."));
    status = mfrc522.MIFARE_Read(blockAddrE+bb, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
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
    if (count == 16) {Serial.println(F("Success :-)")); success();       
  } else {Serial.println(F("Failure, no match :-(")); Serial.println(F("  perhaps the write didn't work properly..."));
      }
        Serial.println();}
    
    if (blockAddrE+4==62){mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
    else
    blockAddrE=blockAddrE+4;
    trailerBlockE=trailerBlockE+4;
    }
    
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();

  return;
}

//CP
else if (eeprom==1){
if (buffer[0]==255) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else if (buffer[0]==0) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else if (buffer[0]==flag) {mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;}
else {
 
     if ((point+1)==4){point2=1; address2=address+1;}
    else {point2 = point+1; address2=address;} 
    if (trailerBlock==(address2)){dataBlock[1]={address2=address2+1};}
    else {dataBlock[1]={address2};}
    dataBlock[0]={flag};
    //dataBlock[1]={address2};
    dataBlock[2]={point2};
  
    dataBlock2[0+r]={flag};
    dataBlock2[1+r]={tm.Hour};
    dataBlock2[2+r]={tm.Minute};
    dataBlock2[3+r]={tm.Second};  
    dataBlock2[4+r]={mtime};
}
}
//TEST
//else if (eeprom==5){
//flag=flag+1;
//Serial.println("Control point is now: "); Serial.print(flag);
//mfrc522.PICC_HaltA(); mfrc522.PCD_StopCrypto1(); return;
//}





//---------------------------------------------------------------------------------------
//dataBlock
// -----------------------------Write (FLAG) data to the Sector 1 Block 4
    //Serial.print(F("Writing data into block ")); Serial.print(4);
    //Serial.println(F(" ..."));
    dump_byte_array(dataBlock, 16); //Serial.println();
    status = mfrc522.MIFARE_Write(4, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
       // Serial.print(F("MIFARE_Write() failed: "));
       // Serial.println(mfrc522.GetStatusCodeName(status));}
}
    //Serial.println();
    // Read data from the block (again, should now be what we have written)
    //Serial.print(F("Reading data from block ")); Serial.print(4);
    //Serial.println(F(" ..."));
    status = mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        //Serial.print(F("MIFARE_Read() failed: "));
        //Serial.println(mfrc522.GetStatusCodeName(status));
      }
    //Serial.print(F("Data in block ")); Serial.print(4); Serial.println(F(":"));
    dump_byte_array(buffer, 16); //Serial.println();     
//Serial.println(F("Checking result..."));
    byte count1 = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with dataBlock (= what we've written)
        if (buffer[i] == dataBlock[i])
            count1++;}
// Serial.print(F("Number of bytes that match = ")); //Serial.println(count1);
    if (count1 == 16) {//Serial.println(F("Success :-)"));
  }
        else {//Serial.println(F("Failure, no match :-(")); Serial.println(F("  perhaps the write didn't work properly..."));}
}
        //Serial.println();
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// Authenticate to needed block using key B
   // Serial.println(F("Authenticating again using key B..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
     //   Serial.print(F("PCD_Authenticate() failed: "));
      //  Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
// ------------------------------------------------------------------------------------------------------------------------
//address, dataBlock2
// -----------------------------Write data to the block--------------------------------------------------------------------
   // Serial.print(F("Writing data into block ")); Serial.print(address);
   // Serial.println(F(" ..."));
    dump_byte_array(dataBlock2, 16);// Serial.println();
    status = mfrc522.MIFARE_Write(address, dataBlock2, 16);
    if (status != MFRC522::STATUS_OK) {
    //    Serial.print(F("MIFARE_Write() failed: "));
    //    Serial.println(mfrc522.GetStatusCodeName(status));
     mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
    return;
    }
   // Serial.println();
    // Read data from the block (again, should now be what we have written)
   // Serial.print(F("Reading data from block xxxxxx")); Serial.print(address);
   // Serial.println(F(" ..."));
    status = mfrc522.MIFARE_Read(address, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
    //    Serial.print(F("MIFARE_Read() failed: "));
     //   Serial.println(mfrc522.GetStatusCodeName(status));
    }
   // Serial.print(F("Data in block ")); Serial.print(address); Serial.println(F(":"));
    dump_byte_array(buffer, 16);// Serial.println();
       
//Serial.println(F("Checking result..."));
    byte count = 0;
    for (byte i = 0; i < 16; i++) {
        // Compare buffer (= what we've read) with dataBlock (= what we've written)
        if (buffer[i] == dataBlock2[i])
            count++;
    }
 //Serial.print(F("Number of bytes that match = ")); Serial.println(count);
    if (count == 16) {
      //Serial.println(F("Success :-)"));
      success();      
  }
        else {
          //Serial.println(F("Failure, no match :-(")); Serial.println(F("  perhaps the write didn't work properly..."));
      }
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
    
    
  
    
//---------------------------------------------------------------------------------------  
}
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//-----------------------------ADDITIONAL PROCEDURES-------------------------------------



void cp(){delay(100);digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); delay(40); digitalWrite(led, LOW); }

void success(){digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); delay(20); digitalWrite(led, LOW); }

void s(){delay(100);digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); delay(100); digitalWrite(led, LOW); digitalWrite(buzzPin, LOW); delay(100); digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); delay(100);  digitalWrite(led, LOW);}

void f(){delay(100); digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); delay(100); digitalWrite(led, LOW);digitalWrite(buzzPin, LOW); delay(100); digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); delay(100);digitalWrite(led, LOW);digitalWrite(buzzPin, LOW); delay(100);digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); delay(100);  digitalWrite(led, LOW);}

void e(){
delay(100); 
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100); 
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); 
delay(100);
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH);
delay(100); 
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100); 
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); 
delay(100);
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100); 
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH); 
delay(100);
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led, LOW);digitalWrite(buzzPin, LOW);
delay(100);
digitalWrite(led, HIGH);digitalWrite(buzzPin, HIGH);
delay(100);
digitalWrite(led, LOW);}
void error(){ digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  

}

//---------------------------------------------------------------------------------------
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
    //    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
   //     Serial.print(buffer[i], HEX);
    }
}
//--------------------------------------------------------------------------------------







