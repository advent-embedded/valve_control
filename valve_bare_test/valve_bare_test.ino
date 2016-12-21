#include<avr/interrupt.h>
#include"valve.h"

boolean VA_timeout=false,VB_timeout=false,VC_timeout=false,VD_timeout=false,send_timeout=false;
boolean trip_stat=false;
boolean reset_flag=false;
boolean flag_VA=true,flag_VB=true,flag_VC=true,flag_VD=true;
volatile uint16_t isr_count=0;
uint16_t current_sec=0;

valve_status VA_stat=OPEN,VB_stat=CLOSE,VC_stat=CLOSE,VD_stat=OPEN;
uint8_t pos_flag=0;
void setup()
{
  Serial.begin(9600);
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
}
void loop()
{
  ////////////////////////////VALVE A//////////////////////////
  if(pos_flag==0)
  {
    flag_VD=true;
    if((VA_stat==CLOSE)&&(flag_VA==true))
    {
    VAOPEN;
    current_sec=isr_count;
    do
    {
      if(VAOPENED==LOW)
      {VA_stat=OPEN;VASTALL;flag_VA=false;break;}
      if((current_sec+TIMEOUT)<isr_count)
      {VA_timeout=true;trip_stat=true;}
      Serial.println(isr_count);
      Serial.print("VA_Status=");
      Serial.println(VA_stat);
      }while((VA_stat!=OPEN)&&(VA_timeout!=true));
      Serial.print("VA_Status=");
      Serial.println(VA_stat);
      VASTALL;
    }
    if((VA_stat==OPEN)&&(flag_VA==true))
    {
    VACLOSE;
    current_sec=isr_count;
    do
    {
      if(VACLOSED==LOW)
      {VA_stat=CLOSE;VASTALL;flag_VA=false;break;}
      if((current_sec+TIMEOUT)<isr_count)
      {VA_timeout=true;trip_stat=true;}
      Serial.println(isr_count);
      Serial.print("VA_Status=");
      Serial.println(VA_stat);
      }while((VA_stat!=CLOSE)&&(VA_timeout!=true));
      Serial.print("VA_Status=");
      Serial.println(VA_stat);
      VASTALL;
    }
    pos_flag+=1;
  }
  /////////////////////////VALVE B////////////////////
    if((pos_flag==1)&&(flag_VB==true))
  {
    flag_VA=true;
    if(VB_stat==CLOSE)
    {
    VBOPEN;
    current_sec=isr_count;
    do
    {
      if(VBOPENED==LOW)
      {VB_stat=OPEN;VBSTALL;flag_VB=false;break;}
      if((current_sec+TIMEOUT)<isr_count)
      {VB_timeout=true;trip_stat=true;}
      Serial.println(isr_count);
      Serial.print("VB_Status=");
      Serial.println(VB_stat);
      }while((VB_stat!=OPEN)&&(VB_timeout!=true));
      Serial.print("VB_Status=");
      Serial.println(VB_stat);
      VBSTALL;
    }
    if((VB_stat==OPEN)&&(flag_VB==true))
    {
    VBCLOSE;
    current_sec=isr_count;
    do
    {
      if(VBCLOSED==LOW)
      {VB_stat=CLOSE;VBSTALL;flag_VB=false;break;}
      if((current_sec+TIMEOUT)<isr_count)
      {VB_timeout=true;trip_stat=true;}
      Serial.println(isr_count);
      Serial.print("VB_Status=");
      Serial.println(VB_stat);
      }while((VB_stat!=CLOSE)&&(VB_timeout!=true));
      Serial.print("VB_Status=");
      Serial.println(VB_stat);
      VBSTALL;
    }
    pos_flag+=1;
  }
//  /////////////////////////////////VALVE C//////////////////////////////////
    if(pos_flag==2)
  {
    flag_VB=true;
    if((VC_stat==CLOSE)&&(flag_VC==true))
    {
    VCOPEN;
    current_sec=isr_count;
    do
    {
      if(VCOPENED==LOW)
      {VC_stat=OPEN;VCSTALL;flag_VC=false;break;}
      if((current_sec+TIMEOUT)<isr_count)
      {VC_timeout=true;trip_stat=true;}
      Serial.println(isr_count);
      Serial.print("VC_Status=");
      Serial.println(VC_stat);
      }while((VC_stat!=OPEN)&&(VC_timeout!=true));
      Serial.print("VC_Status=");
      Serial.println(VC_stat);
      VCSTALL;
    }
    if((VC_stat==OPEN)&&(flag_VC==true))
    {
    VCCLOSE;
    current_sec=isr_count;
    do
    {
      if(VCCLOSED==LOW)
      {VC_stat=CLOSE;VCSTALL;flag_VC=false;break;}
      if((current_sec+TIMEOUT)<isr_count)
      {VC_timeout=true;trip_stat=true;}
      Serial.println(isr_count);
      Serial.print("VC_Status=");
      Serial.println(VC_stat);
      }while((VC_stat!=CLOSE)&&(VC_timeout!=true));
      Serial.print("VC_Status=");
      Serial.println(VC_stat);
      VCSTALL;
    }
    pos_flag+=1;
  }
  //  /////////////////////////////////VALVE D//////////////////////////////////
    if(pos_flag==3)
  {
    flag_VC=true;
    if((VD_stat==CLOSE)&&(flag_VD==true))
    {
    VDOPEN;
    current_sec=isr_count;
    do
    {
      if(VDOPENED==LOW)
      {VD_stat=OPEN;VDSTALL;flag_VD=false;break;}
      if((current_sec+TIMEOUT)<isr_count)
      {VD_timeout=true;trip_stat=true;}
      Serial.println(isr_count);
      Serial.print("VD_Status=");
      Serial.println(VD_stat);
      }while((VD_stat!=OPEN)&&(VD_timeout!=true));
      Serial.print("VD_Status=");
      Serial.println(VD_stat);
      VDSTALL;
    }
    if((VD_stat==OPEN)&&(flag_VD==true))
    {
    VDCLOSE;
    current_sec=isr_count;
    do
    {
      if(VDCLOSED==LOW)
      {VD_stat=CLOSE;VDSTALL;flag_VD=false;break;}
      if((current_sec+TIMEOUT)<isr_count)
      {VD_timeout=true;trip_stat=true;}
      Serial.println(isr_count);
      Serial.print("VD_Status=");
      Serial.println(VD_stat);
      }while((VD_stat!=CLOSE)&&(VD_timeout!=true));
      Serial.print("VD_Status=");
      Serial.println(VD_stat);
      VDSTALL;
    }
    pos_flag=0;
 
  }
}

ISR(TIMER1_COMPA_vect)          // timer compare interrupt service routine
{
isr_count++;
}
  
