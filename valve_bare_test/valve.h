#ifndef VALVE_H
#define VALVE_H

#define t1 10
#define t2 120
#define t3 600
//Valve A MACROS
#define VAN 2
#define VAP 3
#define VAOP 4
#define VACL 5
#define VAOPEN  digitalWrite(VAP,LOW);digitalWrite(VAN,HIGH)
#define VACLOSE digitalWrite(VAP,HIGH);digitalWrite(VAN,LOW)
#define VASTALL digitalWrite(VAP,LOW);digitalWrite(VAN,LOW)
#define VAOPENED digitalRead(VAOP)
#define VACLOSED digitalRead(VACL)
//Valve B MACROS
#define VBN 6
#define VBP 7
#define VBOP 8
#define VBCL 9
#define VBSTALL digitalWrite(VBP,LOW);digitalWrite(VBN,LOW)
#define VBOPEN digitalWrite(VBP,LOW);digitalWrite(VBN,HIGH)
#define VBCLOSE digitalWrite(VBP,HIGH);digitalWrite(VBN,LOW)
#define VBOPENED digitalRead(VBOP)
#define VBCLOSED digitalRead(VBCL)
//Valve C MACROS
#define VCN 10
#define VCP 11
#define VCOP 12
#define VCCL 13
#define VCSTALL digitalWrite(VCP,LOW);digitalWrite(VCN,LOW)
#define VCOPEN digitalWrite(VCP,LOW);digitalWrite(VCN,HIGH)
#define VCCLOSE digitalWrite(VCP,HIGH);digitalWrite(VCN,LOW)
#define VCOPENED digitalRead(VCOP)
#define VCCLOSED digitalRead(VCCL)
//Valve D MACROS
#define VDN 14
#define VDP 15
#define VDOP 16
#define VDCL 17
#define VDSTALL digitalWrite(VDP,LOW);digitalWrite(VDN,LOW)
#define VDOPEN digitalWrite(VDP,LOW);digitalWrite(VDN,HIGH)
#define VDCLOSE digitalWrite(VDP,HIGH);digitalWrite(VDN,LOW)
#define VDOPENED digitalRead(VDOP)
#define VDCLOSED digitalRead(VDCL)
#define TIMEOUT 10
#define RESETTIMER1 {TCCR1B&=~0x05;TCNT1=0x0000;isr_count=0;TCCR1B|=0x05;}

  

typedef enum {OPEN = 1, CLOSE = 0} valve_status;
typedef enum {SEND = 1, RETURN = 0} state;
typedef enum {AUTO = 1, MANUAL = 0} opmode;
#endif
