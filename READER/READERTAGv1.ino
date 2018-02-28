/*
Flag address: sector 1 block 4

Format: XX YY ZZ  where XX (buffer[0]) - Flag, YY (buffer[1]) - block address to write, ZZ (buffer[2]) - point(1 or 2 or 3 parts of block line)

CP flag - 207(CF)     = 1
Start flag - 204(CC)  = 2
Finish flag - 255(FF) = 3
Erase flag - 238(EE)  = 4
Test flag - 221(DD)   = 5

*/
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
int buzzPin = 2;
int led = 3;

unsigned long mtime;
int a=0;
//---------------------------------------------------------------------------------------
void setup()
{ 

Serial.begin(115200);
pinMode(led, OUTPUT);



while (!Serial);
SPI.begin();        // Init SPI bus
mfrc522.PCD_Init(); // Init MFRC522 card
// Prepare the key (used both as key A and as key B) using FFFFFFFFFFFFh which is the default at chip delivery from the factory
for (byte i = 0; i < 6; i++) {key.keyByte[i] = 0xFF;}
dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
}
//---------------------------------------------------------------------------------------
void loop(){
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
//Serial.print(F("Card UID:"));
//  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
 //   Serial.println();
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return;
    }
     // Authenticate using key B
//    Serial.println(F("Authenticating again using key B...nnn"));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 7, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;        
    }
    // Read data from the sector 1 block 4 
 //   Serial.print(F("Reading data from block ")); Serial.print(4);
 //   Serial.println(F(" ..."));
 //Serial.println();
    status = mfrc522.MIFARE_Read(4, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
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
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
  
 // Read data from the sector 1 block 4 
//    Serial.print(F("Reading data from block ")); Serial.print(address2);
//    Serial.println(F(" ..."));
    status = mfrc522.MIFARE_Read(address2, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }
//    Serial.print(F("Data in block ")); Serial.print(address2); Serial.println(F(":"));
 //   dump_byte_array(buffer, 16); Serial.println();
       
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
        a =buffer[1+(bbb*5)];
        b =buffer[2+(bbb*5)];
        c =buffer[3+(bbb*5)];
        d =buffer[4+(bbb*5)];
        
     
        unsigned long x= (a*360000)+(b*6000)+(c*100)+d;
        xx=String(x);
       // Serial.println();
       // Serial.println(x);
       // Serial.println();
        if      (xx.length()==1){Serial.print(buffer[0+(bbb*5)]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}    
        else if (xx.length()==2){Serial.print(buffer[0+(bbb*5)]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==3){Serial.print(buffer[0+(bbb*5)]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==4){Serial.print(buffer[0+(bbb*5)]);Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==5){Serial.print(buffer[0+(bbb*5)]);Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==6){Serial.print(buffer[0+(bbb*5)]);Serial.print("0"); Serial.print(x);}
        else                    {Serial.print(buffer[0+(bbb*5)]); Serial.print(x);}
      // Serial.println();
     }  
 }  
     else{
      // if(buffer[4]>99){buffer[4]=buffer[4]-100;}
       //if(buffer[9]>99){buffer[9]=buffer[9]-100;}
       //if(buffer[14]>99){buffer[14]=buffer[14]-100;}
        a =buffer[1];
        b =buffer[2];
        c =buffer[3];
        d =buffer[4];
        unsigned long x= (a*360000)+(b*6000)+(c*100)+d;
        xx=String(x);
        if      (xx.length()==1){Serial.print(buffer[0]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}    
        else if (xx.length()==2){Serial.print(buffer[0]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==3){Serial.print(buffer[0]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==4){Serial.print(buffer[0]);Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==5){Serial.print(buffer[0]);Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==6){Serial.print(buffer[0]);Serial.print("0"); Serial.print(x);}
        else                    {Serial.print(buffer[0]); Serial.print(x);}
      // Serial.println();
        a =buffer[6];
        b =buffer[7];
        c =buffer[8];
        d =buffer[9];
        x= (a*360000)+(b*6000)+(c*100)+d;
        xx=String(x);
        if      (xx.length()==1){Serial.print(buffer[5]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}    
        else if (xx.length()==2){Serial.print(buffer[5]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==3){Serial.print(buffer[5]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==4){Serial.print(buffer[5]);Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==5){Serial.print(buffer[5]);Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==6){Serial.print(buffer[5]);Serial.print("0"); Serial.print(x);}
        else                    {Serial.print(buffer[5]); Serial.print(x);}
    // Serial.println();
        a =buffer[11];
        b =buffer[12];
        c =buffer[13];
        d =buffer[14];
        x= (a*360000)+(b*6000)+(c*100)+d;
        xx=String(x);
        if      (xx.length()==1){Serial.print(buffer[10]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}    
        else if (xx.length()==2){Serial.print(buffer[10]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==3){Serial.print(buffer[10]);Serial.print("0");Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==4){Serial.print(buffer[10]);Serial.print("0");Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==5){Serial.print(buffer[10]);Serial.print("0");Serial.print("0"); Serial.print(x);}
        else if (xx.length()==6){Serial.print(buffer[10]);Serial.print("0"); Serial.print(x);}
        else                    {Serial.print(buffer[10]); Serial.print(x);}
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
//-----------------------------ADDITIONAL PROCEDURES-------------------------------------


void cp(){digitalWrite(led, HIGH); delay(300); digitalWrite(led, LOW); }

void s(){digitalWrite(led, HIGH); delay(300); digitalWrite(led, LOW); delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW);}

void f(){digitalWrite(led, HIGH); delay(300); digitalWrite(led, LOW); delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW); 
delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW);}

void e(){digitalWrite(led, HIGH); delay(300); digitalWrite(led, LOW); delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW); 
delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW);delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW);}
void t(){digitalWrite(led, HIGH); delay(300); digitalWrite(led, LOW); delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW); 
delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW);delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW); 
delay(299); digitalWrite(led, HIGH); delay(300);  digitalWrite(led, LOW);}

void error(){ digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(300); digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  delay(299);
              digitalWrite(led, HIGH); digitalWrite(buzzPin, LOW); delay(301);  digitalWrite(led, LOW); digitalWrite(buzzPin, HIGH);  

}
void success(){ digitalWrite(led, HIGH); delay(850); digitalWrite(led, LOW); }
//---------------------------------------------------------------------------------------
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        Serial.print(buffer[i], HEX);
    }
}
//--------------------------------------------------------------------------------------

