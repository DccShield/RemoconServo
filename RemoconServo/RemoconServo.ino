#include <IRremote.h>
#include "ServoDrv.h"
#include "IrCmd_List.h"
#include "eepromDrv.h"

//各種設定、宣言

#define PIN_F0_F  3     // D3 PD3,PWM
#define PIN_F0_R  4     // D4 PD4
#define PIN_AUX1  5     // D5 PD5
#define PIN_AUX2  6     // D6 PD6
#define PIN_AUX3  7     // D7 PD7
#define PIN_AUX4  8     // D8 PB0
#define PIN_AUX5  9     // D9 PB1,DIGITAL TR,PWM
#define PIN_AUX6  10    // D10 PB2,DIGITAL TR,PWM
#define PIN_AUX7  11    // D11 PB3,DIGITAL TR,PWM

#define PIN_SERVO1 5    // D5 PD5
#define PIN_SERVO2 4    // D4 PD4
#define PIN_LED_DIV1 6  // D6 PD6 SERVO1用DIV(分岐) LED
#define PIN_LED_STR1 7  // D7 PD7 SERVO1用STR(直進) LED
#define PIN_LED_DIV2 8  // D8 PB0 SERVO2用DIV(分岐) LED
#define PIN_LED_STR2 9  // D9 PB1 SERVO2用STR(直進) LED


//Function State
uint8_t gState_F0 = 0;
uint8_t gState_F1 = 0;
uint8_t gState_F2 = 0;
uint8_t gState_F3 = 0;
uint8_t gState_F4 = 0;
uint8_t gState_F5 = 0;
uint8_t gState_F6 = 0;
uint8_t gState_F7 = 0;
uint8_t gState_F8 = 0;
uint8_t gState_F9 = 0;
uint8_t gState_F10 = 0;
uint8_t gState_F11 = 0; 
uint8_t gState_F12 = 0;
uint8_t gState_Function = 0;

int RECV_PIN = 11;
unsigned long gPreviousL5 = 0;  // PreviosTime 前時間

IRrecv irrecv(RECV_PIN);
decode_results results;
ServoDriver ServoCH0;
ServoDriver ServoCH1;

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Servo Remocon."));

  ServoInit();
 
  irrecv.enableIRIn();          // Start the receiver
  gPreviousL5 = millis();       // setup()終了時の時刻をPreviosTimeに格納
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    state(results.value);
    irrecv.resume(); // Receive the next value
  }
  if ( (millis() - gPreviousL5) >= 10) // 100:100msec  10:10msec  Function decoder は 10msecにしてみる。
  {
    ServoCH0.stateCheck();
    ServoCH1.stateCheck();
    gPreviousL5 = millis();
  }
}

void state(long val){
  switch(val){
      case IRCMD_NEC_FUNC1:
      case IRCMD_SONYTV_FUNC1:
      case IRCMD_SONYMD_FUNC1:
                              if(gState_F1 == OFF)
                                gState_F1 = ON;
                              else
                                gState_F1 = OFF;
                              break;
      case IRCMD_NEC_FUNC2:
      case IRCMD_SONYTV_FUNC2:
      case IRCMD_SONYMD_FUNC2:
                              if(gState_F2 == OFF)
                                gState_F2 = ON;
                              else
                                gState_F2 = OFF;
                              break;
      case IRCMD_NEC_FUNC3:
      case IRCMD_SONYTV_FUNC3:
      case IRCMD_SONYMD_FUNC3:
                              break;  
      case IRCMD_NEC_FUNC4:
      case IRCMD_SONYTV_FUNC4:
      case IRCMD_SONYMD_FUNC4:
                              break;  
      case IRCMD_NEC_FUNC5:
      case IRCMD_SONYTV_FUNC5:
      case IRCMD_SONYMD_FUNC5:
                              break;
      case IRCMD_NEC_FUNC6:
      case IRCMD_SONYTV_FUNC6:
      case IRCMD_SONYMD_FUNC6:
                              break;
      case IRCMD_NEC_FUNC7:
      case IRCMD_SONYTV_FUNC7:
      case IRCMD_SONYMD_FUNC7:
                              break;
      case IRCMD_NEC_FUNC8:
      case IRCMD_SONYTV_FUNC8:
      case IRCMD_SONYMD_FUNC8:
                              break;
      case IRCMD_NEC_FUNC9:
      case IRCMD_SONYTV_FUNC9:
      case IRCMD_SONYMD_FUNC9:
                              break;
      default:
                break;
  }
}

void ServoInit(void)
{
  //Init CVs
  ServoCH0.ch = 0;
  ServoCH0.port = PIN_SERVO1;
  ServoCH0.onDeg = 180;   // ON時の角度
  ServoCH0.offDeg = 0;    // OFF時の角度
  ServoCH0.initDeg = 0;   // 電源切る前の角度
  ServoCH0.onSpeed = 1;   // OFF->ONのスピード
  ServoCH0.offSpeed = 1;  // ON->OFFのスピード
  ServoCH0.sdir = readCV( 52 );     // gDirの最新値保存用 STR/DIV
  ServoCH0.cv = 52;
  ServoCH0.servoAddress = 1;// サーボモータをON/OFFするファンクッション番号
  ServoCH0.STR = PIN_LED_STR1;
  ServoCH0.DIV = PIN_LED_DIV1;
  ServoCH0.MinAngle = 670;
  ServoCH0.MaxAngle = 2600;
  ServoCH0.Set();
    
  ServoCH1.ch = 1;
  ServoCH1.port = PIN_SERVO2;
  ServoCH1.onDeg = 180;   // ON時の角度
  ServoCH1.offDeg = 0;    // OFF時の角度
  ServoCH1.initDeg = 0;   // 電源切る前の角度
  ServoCH1.onSpeed = 1;   // OFF->ONのスピード
  ServoCH1.offSpeed = 1;  // ON->OFFのスピード
  ServoCH1.sdir = readCV( 59 );     // gDirの最新値保存用 STR/DIV
  ServoCH1.cv = 59;
  ServoCH1.servoAddress = 2;// サーボモータをON/OFFするファンクッション番号  
  ServoCH1.STR = PIN_LED_STR2;
  ServoCH1.DIV = PIN_LED_DIV2;
  ServoCH1.MinAngle = 400;
  ServoCH1.MaxAngle = 2100;
  ServoCH1.Set();

}
