#include<avr/interrupt.h>
#include"valve.h"



boolean VA_timeout = false, VB_timeout = false, VC_timeout = false, VD_timeout = false, send_timeout = false, return_timeout = false;
boolean trip_status = false;
volatile uint16_t isr_count = 0;
uint16_t current_sec = 0, counter = 0;

valve_status VA_status=CLOSE,VB_status=CLOSE,VC_status=CLOSE,VD_status=CLOSE;
state pos = SEND;
opmode mode = AUTO;

void setup()
{
  pinMode(VAP, OUTPUT);
  pinMode(VAN, OUTPUT);
  pinMode(VBP, OUTPUT);
  pinMode(VBN, OUTPUT);
  pinMode(VCP, OUTPUT);
  pinMode(VCN, OUTPUT);
  pinMode(VDP, OUTPUT);
  pinMode(VDN, OUTPUT);
  pinMode(VACL, INPUT_PULLUP);
  pinMode(VAOP, INPUT_PULLUP);
  pinMode(VBCL, INPUT_PULLUP);
  pinMode(VBOP, INPUT_PULLUP);
  pinMode(VCCL, INPUT_PULLUP);
  pinMode(VCOP, INPUT_PULLUP);
  pinMode(VDCL, INPUT_PULLUP);
  pinMode(VDOP, INPUT_PULLUP);
  noInterrupts();           // disable all interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15625;            // 2 Seconds
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= 0x05;
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  interrupts();
  Serial.begin(9600);
}

void loop()
{
  if (mode == AUTO)
  {
    RESETTIMER1
    if ((VA_status == CLOSE) && (VB_status == CLOSE) && (pos == SEND)&&(send_timeout!=true))
    {
      VAOPEN;
      VBOPEN;
      current_sec = isr_count;
      do
      {
        if (VAOPENED == LOW)
        {
          VA_status = OPEN;
          VASTALL;
        }
        if (VBOPENED == LOW)
        {
          VB_status = OPEN;
          VBSTALL;
        }
        if ((current_sec + TIMEOUT) < isr_count)
        {
          send_timeout = true; trip_status = true;
        }
        Serial.print("V1=");
        Serial.println(VA_status);

        Serial.print("V2=");
        Serial.println(VB_status);

      } while ((VA_status != CLOSE) && (VA_status != CLOSE) && (send_timeout != true));
      if ((VA_status == OPEN) && (VB_status == OPEN))
      {

        RESETTIMER1
        current_sec = isr_count;
        while ((current_sec + t1) > isr_count);
        VACLOSE;
        current_sec = isr_count;
        do
        {
          if (VACLOSED == LOW)
          {
            VA_status = CLOSE;
            VASTALL;
            break;
          }
          if ((current_sec + TIMEOUT) < isr_count)
          {
            VA_timeout = true;
            trip_status = true;
          }
          Serial.print("V1=");
          Serial.println(VA_status);
        } while ((VA_status != CLOSE) && (VA_timeout != true));
        VBCLOSE;
        current_sec = isr_count;
        do
        {
          if (VBCLOSED == LOW)
          {
            VB_status = CLOSE;
            VBSTALL;
            break;
          }
          if ((current_sec + TIMEOUT) < isr_count)
          {
            VB_timeout = true;
            trip_status = true;
          }
          Serial.print("V1=");
          Serial.println(VA_status);
        } while ((VB_status != CLOSE) && (VB_timeout != true));
        if ((VA_status == CLOSE) && (VB_status == CLOSE))
          pos = RETURN;
      }
    }//POS=SEND

    if ((pos == RETURN) && (trip_status != true))
    {
      VCOPEN;
      VDOPEN;
      current_sec = isr_count;
      do
      {
        if (VCOPENED == LOW)
        {
          VC_status = OPEN;
          VCSTALL;
        }
        if (VDOPENED == LOW)
        {
          VD_status = OPEN;
          VDSTALL;
        }
        if ((current_sec + TIMEOUT) < isr_count)
        {
          return_timeout = true; trip_status = true;
        }
        Serial.print("V1=");
        Serial.println(VA_status);

        Serial.print("V2=");
        Serial.println(VB_status);

      } while ((VC_status != CLOSE) && (VD_status != CLOSE) && (return_timeout != true));
      if ((VC_status == OPEN) && (VD_status == OPEN))
      {
        TCCR1B &= ~0x05;
        TCNT1 = 0x0000;
        isr_count = 0;
        TCCR1B |= 0x05;
        current_sec = isr_count;
        while ((current_sec + t2) > isr_count);
        VCCLOSE;
        current_sec = isr_count;
        do
        {
          if (VCCLOSED == LOW)
          {
            VC_status = CLOSE;
            VCSTALL;
            break;
          }
          if ((current_sec + TIMEOUT) < isr_count)
          {
            VC_timeout = true;
            trip_status = true;
          }
          Serial.print("VC=");
          Serial.println(VC_status);
        } while ((VC_status != CLOSE) && (VC_timeout != true));
        VDCLOSE;
        current_sec = isr_count;
        do
        {
          if (VDCLOSED == LOW)
          {
            VD_status = CLOSE;
            VDSTALL;
            break;
          }
          if ((current_sec + TIMEOUT) < isr_count)
          {
            VD_timeout = true;
            trip_status = true;
          }
          Serial.print("VD=");
          Serial.println(VD_status);
        } while ((VD_status != CLOSE) && (VD_timeout != true));

      }
      if ((VC_status == CLOSE) && (VD_status == CLOSE))
      {
        TCCR1B &= ~0x05;
        TCNT1 = 0x0000;
        isr_count = 0;
        TCCR1B |= 0x05;
        current_sec = isr_count;
        while ((current_sec + t3) > isr_count);
        counter += 1;
        pos = SEND;
      }
    }
  }//MODE=AUTO
}//loop
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  ++isr_count;
}

