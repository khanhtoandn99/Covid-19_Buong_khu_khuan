#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

#define QUAT_HUT  8 
#define VI_SIEU_AM  9
#define QUAT_DL 9
#define DEN_XANH  2
#define DEN_DO  3
#define DEN_VANG  4 
#define LOA 7

// IO_Input
#define PIR_SENSOR  10 

unsigned long Sensor_Detect_Time = 0;
#define SENSOR_DETECT_TIME_MAX  1000  // millisecond

unsigned long Sanitize_Time = 0; // thoi gian dien ra qua trinh khu khuan 
#define SANITIZE_TIME_MAX  25000  // millisecond
#define SANITIZE_TIME_PRE_END_TIME 20000 // millisecond, qua trinh sat khuan dc 10s thi bat quat hut 

unsigned long End_Time = 0;
#define END_TIME_MAX  5000  // millisecond

SoftwareSerial mySoftwareSerial(11, 12); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void IO_Init() ;
void IdleMode();
void EndMode();
void printDetail(uint8_t type, int value);

void setup() {
  Serial.begin(115200) ;
  mySoftwareSerial.begin(9600);
  IO_Init();
  myDFPlayer.begin(mySoftwareSerial);
//  while(!myDFPlayer.begin(mySoftwareSerial))  //Use softwareSerial to communicate with mp3.
  Serial.println(F("DFPlayer Mini started !"));
  
}

void loop() {
  // put your main code here, to run repeatedly:
  IdleMode();     // che do cho 
  SanitizeMode();     // che do khu khuan
  EndMode();      // ket thuc qua trinh khu khuan, bat dau lai chu trinh 
}


// Subroutines 

void IO_Init(){
  pinMode(QUAT_HUT,OUTPUT);
  pinMode(VI_SIEU_AM,OUTPUT);
  pinMode(QUAT_DL,OUTPUT);
  pinMode(DEN_XANH,OUTPUT);
  pinMode(DEN_DO,OUTPUT);
  pinMode(DEN_VANG,OUTPUT);
  pinMode(LOA,OUTPUT);

  pinMode(PIR_SENSOR,INPUT_PULLUP);

  digitalWrite(QUAT_HUT,HIGH);
  digitalWrite(VI_SIEU_AM,HIGH);
  digitalWrite(QUAT_DL,HIGH);
  digitalWrite(DEN_XANH,HIGH);
  digitalWrite(DEN_DO,HIGH);
  digitalWrite(DEN_VANG,HIGH);
  digitalWrite(LOA,HIGH);
}

void IdleMode(){
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(2);  //Play the first mp3
  Serial.println("Idle Mode :"); 
  Serial.println("Quat hut : OFF \nVi sieu am : OFF\nQuat doi luu : OFF\nDen xanh : ON\nDen do : OFF\nDen vang : OFF\nLoa : OFF "); 
  digitalWrite(QUAT_HUT,HIGH);
  digitalWrite(VI_SIEU_AM,HIGH);
  digitalWrite(QUAT_DL,HIGH);
  digitalWrite(DEN_XANH,LOW);
  digitalWrite(DEN_DO,HIGH);
  digitalWrite(DEN_VANG,HIGH);
  digitalWrite(LOA,HIGH);
  // bat dau che do cho khi co nguoi vao phong
  bool Next_Mode = false ;
  while(1){
    if(digitalRead(PIR_SENSOR) == 0){
      Serial.println("Sensor detected !") ;
      Sensor_Detect_Time = millis() ; 
      while(digitalRead(PIR_SENSOR) == 0){
        if((unsigned long) (millis() - Sensor_Detect_Time) > SENSOR_DETECT_TIME_MAX){
          Next_Mode = true ;
          break ;
        }
      }
      Sensor_Detect_Time = 0 ;
    }

    if(Next_Mode == true){
      Next_Mode = false ;
      Serial.println("Chuyen sang che do khu khuan !") ;
      break ;
    }
  }
  Sensor_Detect_Time = 0;
}

/*
 * Che do khu khuan 
 */
void SanitizeMode(){
  Serial.println("\n\n\n"); 
  Serial.println("Sanitize Mode :"); 
  Serial.println("Quat hut : OFF \nVi sieu am : ON\nQuat doi luu : ON\nDen xanh : OFF\nDen do : ON\nDen vang : OFF\nLoa : OFF "); 
  digitalWrite(QUAT_HUT,HIGH);
  digitalWrite(VI_SIEU_AM,LOW);
  digitalWrite(QUAT_DL,LOW);
  digitalWrite(DEN_XANH,HIGH);
  digitalWrite(DEN_DO,LOW);
  digitalWrite(DEN_VANG,HIGH);
  digitalWrite(LOA,LOW);
  delay(2000);
  digitalWrite(LOA,HIGH);
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3

/*
 * co 2 cac tao delay :
 * 1. Dung Timer
 */
  Sanitize_Time = millis() ; 
  while((unsigned long) (millis() - Sanitize_Time) < SANITIZE_TIME_MAX ){
    if((unsigned long) (millis() - Sanitize_Time) >= SANITIZE_TIME_PRE_END_TIME){
      digitalWrite(QUAT_HUT,LOW);
//      digitalWrite(QUAT_DL,HIGH);
//      digitalWrite(VI_SIEU_AM,HIGH);
      Serial.println("Quat hut : ON") ;
    }
  }

/*
 * 2. Dung delay
 */
//    delay(SANITIZE_TIME_MAX - ( SANITIZE_TIME_MAX - SANITIZE_TIME_PRE_END_TIME));
//    digitalWrite(QUAT_HUT,HIGH);
//    Serial.println("Quat hut : ON") ;
//    delay(SANITIZE_TIME_MAX - SANITIZE_TIME_PRE_END_TIME);
    
    Sanitize_Time = 0;
    digitalWrite(LOA,LOW);
    delay(1500) ;
    digitalWrite(LOA,HIGH);
}


void EndMode(){
  myDFPlayer.begin(mySoftwareSerial);
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(3);  //Play the first mp3
  Serial.println("\n\n\n"); 
  Serial.println("End Mode :"); 
  Serial.println("Quat hut : ON \nVi sieu am : OFF\nQuat doi luu : ON\nDen xanh : OFF\nDen do : OFF\nDen vang : ON\nLoa : OFF "); 
  digitalWrite(QUAT_HUT,LOW);
  digitalWrite(VI_SIEU_AM,HIGH);
  digitalWrite(QUAT_DL,HIGH);
  digitalWrite(DEN_XANH,HIGH);
  digitalWrite(DEN_DO,HIGH);
  digitalWrite(DEN_VANG,LOW);

/*
* co 2 cac tao delay :
* 1. Dung Timer
*/  
  delay(2000) ;
  digitalWrite(LOA,LOW);
  End_Time = millis() ; 
  while((unsigned long) (millis() - End_Time) < END_TIME_MAX ){
    // do nothing 
  }

/*
 * 2. Dung delay
 */
//  delay(END_TIME_MAX) ;
  
  digitalWrite(LOA,LOW);
  Serial.println("Done !"); 
  digitalWrite(QUAT_HUT,HIGH);
  digitalWrite(VI_SIEU_AM,HIGH);
  digitalWrite(QUAT_DL,HIGH);
  digitalWrite(DEN_XANH,LOW);
  digitalWrite(DEN_DO,HIGH);
  digitalWrite(DEN_VANG,HIGH);
  digitalWrite(LOA,HIGH);
  End_Time = 0 ;
}


void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
