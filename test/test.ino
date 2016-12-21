#include<avr/interrupt.h>
#include"valve.h"



boolean VA_timeout = false, VB_timeout = false, VC_timeout = false, VD_timeout = false, send_timeout = false, return_timeout = false;
boolean trip_status = false;
boolean run_status=false;
volatile uint16_t isr_count = 0;
uint16_t current_sec= 0, counter=0,counter_max;

valve_status VA_stat=CLOSE,VB_stat=CLOSE,VC_stat=CLOSE,VD_stat=CLOSE;
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
  pinMode(TRIPPIN,OUTPUT);
  pinMode(VACL, INPUT_PULLUP);
  pinMode(VAOP, INPUT_PULLUP);
  pinMode(VBCL, INPUT_PULLUP);
  pinMode(VBOP, INPUT_PULLUP);
  pinMode(VCCL, INPUT_PULLUP);
  pinMode(VCOP, INPUT_PULLUP);
  pinMode(VDCL, INPUT_PULLUP);
  pinMode(VDOP, INPUT_PULLUP);
  noInterrupts();           // disable all interrupts
  //EIMSK=
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
    if ((VA_stat == CLOSE) && (VB_stat == CLOSE) && (pos == SEND)&&(send_timeout!=true))
    {
      VAOPEN;
      VBOPEN;
      current_sec = isr_count;
       do
      {
        if (VAOPENED == LOW)
        {
          VA_stat = OPEN;
          VASTALL;
        }
        if (VBOPENED == LOW)
        {
          VB_stat = OPEN;
          VBSTALL;
        }
        if ((current_sec + TIMEOUT) < isr_count)
        {
          send_timeout = true; trip_status = true;TripEnable;
        }
      }while (((VA_stat!=OPEN) || (VB_stat!=OPEN))&&(send_timeout!=true));
      if ((VA_stat == OPEN) && (VB_stat == OPEN))
      {

        RESETTIMER1
        current_sec = isr_count;
        while ((current_sec + t1) > isr_count); //Wait for T1 Period
        VACLOSE;
        current_sec = isr_count;
        do
        {
          if (VACLOSED == LOW)
          {
            VA_stat = CLOSE;
            VASTALL;
            break;
          }
          if ((current_sec + TIMEOUT) < isr_count)
          {
            VA_timeout = true;
            trip_status = true;
            TripEnable;
          }
        } while ((VA_stat != CLOSE)&&(VA_timeout!= true));
        VBCLOSE;
        current_sec = isr_count;
        do
        {
          if (VBCLOSED == LOW)
          {
            VB_stat = CLOSE;
            VBSTALL;
            break;
          }
          if ((current_sec + TIMEOUT) < isr_count)
          {
            VB_timeout = true;
            trip_status = true;
            TripEnable;
          }
          Serial.print("V1=");
          Serial.println(VA_stat);
        } while ((VB_stat != CLOSE) && (VB_timeout != true));
        if ((VA_stat == CLOSE) && (VB_stat == CLOSE))
          pos = RETURN;
      }
    } //POS=SEND

    if ((pos == RETURN) && (trip_status != true))
    {
      VCOPEN;
      VDOPEN;
      current_sec = isr_count;
      do
      {
        if (VCOPENED == LOW)
        {
          VC_stat = OPEN;
          VCSTALL;
        }
        if (VDOPENED == LOW)
        {
          VD_stat = OPEN;
          VDSTALL;
        }
        if ((current_sec + TIMEOUT) < isr_count)
        {
          return_timeout = true; trip_status = true;TripEnable;
        }
        Serial.print("V1=");
        Serial.println(VA_stat);

        Serial.print("V2=");
        Serial.println(VB_stat);

      } while (((VC_stat!=OPEN) || (VD_stat!=OPEN))&&(send_timeout!=true));
      if ((VC_stat == OPEN) && (VD_stat == OPEN))
      {
        RESETTIMER1
        current_sec = isr_count;
        while ((current_sec + t2) > isr_count);
        VCCLOSE;
        current_sec = isr_count;
        do
        {
          if (VCCLOSED == LOW)
          {
            VC_stat = CLOSE;
            VCSTALL;
            break;
          }
          if ((current_sec + TIMEOUT) < isr_count)
          {
            VC_timeout = true;
            trip_status = true;
            TripEnable;
          }
          Serial.print("VC=");
          Serial.println(VC_stat);
        } while ((VC_stat != CLOSE) && (VC_timeout != true));
        VDCLOSE;
        current_sec = isr_count;
        do
        {
          if (VDCLOSED == LOW)
          {
            VD_stat = CLOSE;
            VDSTALL;
            break;
          }
          if ((current_sec + TIMEOUT) < isr_count)
          {
            VD_timeout = true;
            trip_status = true;
            TripEnable;
          }
          Serial.print("VD=");
          Serial.println(VD_stat);
        } while ((VD_stat != CLOSE) && (VD_timeout != true));

      }
      if ((VC_stat == CLOSE) && (VD_stat == CLOSE))
      {
        RESETTIMER1
        current_sec = isr_count;
        while ((current_sec + t3) > isr_count);
        counter += 1;
        if(counter==counter_max){run_status=false;}
        pos = SEND;
      }
    } //POS=RETURN
  }//MODE=AUTO
}//loop
ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
  ++isr_count;
}
ISR(INT0_vect) //Wake up from Sleep
{
  
}
ISR(INT1_vect) //Enter Power Down Mode
{
  
}

ISR(INT2_vect) // Trip Status triggered by Software
{
//Send OFF Status to BMS
digitalWrite(TRIPPIN,HIGH);
while(1);  
}
ISR(INT4_vect) //Flow Switch Interrupt,Any Logic Change Alters runmode
{
  run_status=!run_status;
}



