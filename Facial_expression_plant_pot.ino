#include <TimerOne.h> // TimeOne라이브러리 사용
//-------타이머 인터럽트를 위한 타이머 카운트------------
volatile unsigned long time_count = 0;
volatile unsigned long doteC_count = 0;

#include <LedControl.h> // LedControl 헥사파일을 선언
LedControl Dote = LedControl(7, 6, 5, 1);    // 모듈핀 위치 설정 ( DIN , CLK , CS , 몇번째 모듈)

int Soil = A0;

byte digits[3][8] =                       // 0~9까지의 숫자를 byte 타입 2차원 배열로 구성
{                                                   // 10개의 숫자(digits) , 8 byte의 신호
  {                                                 // 숫자 0 
    B01100110,                             // 총 64개의 LED에 원하는 모양의 신호를 준다
    B10011001,
    B10000001,
    B10000001,
    B01000010,
    B00100100,
    B00011000,
    B00000000
  },

  {
    B00000000,
    B01000010,
    B10100101,
    B00000000,
    B00000000,
    B01111110,
    B01000010,
    B00111100
  },

  {
    B00000000,
    B10100101,
    B01000010,
    B00000000,
    B00000000,
    B00111100,
    B01000010,
    B00000000
  }
};

void setup() 
{
  Dote.shutdown(0, false);                    // 절전모드로 (첫번째 모듈, 절전모드를 하지 않는다) 설정 
  Dote.setIntensity(0, 7);                       // 밝기를 조절 (첫번째 모듈, 1~15까지 원하는 밝기) 설정
  Dote.clearDisplay(0);                          // LED를 초기화 설정 (첫번째 모듈)

  Timer1.initialize(250000); //1000000μs = 1s, 1000us = 0.001s, 1000us = 1ms
  Timer1.attachInterrupt(timerIsr);

  Serial.begin(9600);
}

void loop() 
{
  int soil = analogRead(Soil);
  if(doteC_count == 2){
    Dote_face(soil);
    doteC_count = 0;
  }
}

void timerIsr(){
  time_count += 1;
  doteC_count += 1;
}

void Dote_face(int soil){
  int i;
  
  if(soil >= 800){
    for (i = 0; i <8; i++)                            // j는 미리 제작한 비트맵의 y축 8비트 신호를 하나씩 보낸다
    {
      Dote.setRow(0, i, digits[2][i]);           // setRow는 x축 LED를 한번에 제어하며,
    }
  } else {
    for (i = 0; i <8; i++)                            // j는 미리 제작한 비트맵의 y축 8비트 신호를 하나씩 보낸다
    {
      Dote.setRow(0, i, digits[1][i]);           // setRow는 x축 LED를 한번에 제어
    }
  }

  Serial.println(soil);
}
