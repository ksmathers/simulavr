/*
 ****************************************************************************
 *
 * simulavr - A simulator for the Atmel AVR family of microcontrollers.
 * Copyright (C) 2001, 2002, 2003   Klaus Rudolph		
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 ****************************************************************************
 */
#include "hwtimer01irq.h"
#include "irqsystem.h"

#include <iostream>
using namespace std;

/* Timer/Counter Interrupt MaSK register */
#define    TOIE1        7
#define    OCIE1A       6
#define    OCIE1B       5
#define    TICIE1       3
#define    TOIE0        1

/* Timer/Counter Interrupt Flag register */
#define    TOV1         7
#define    OCF1A        6
#define    OCF1B        5
#define    ICF1         3
#define    TOV0         1
HWTimer01Irq::HWTimer01Irq(AvrDevice *core, HWIrqSystem *is, unsigned int v1, unsigned int v2,unsigned int v3,unsigned int v4,unsigned int v5    ):
Hardware(core), irqSystem(is), vectorCapt(v1), vectorCompa(v2), vectorCompb(v3), vectorOvf1(v4), vectorOvf0(v5) {
    tifr=0;
    timsk=0;
}

void HWTimer01Irq::CheckForNewSetIrq(unsigned char tiac) {
    if (tiac&(1<<TICIE1)) { irqSystem->SetIrqFlag(this, vectorCapt); } 
    if (tiac&(1<<OCIE1A)) { irqSystem->SetIrqFlag(this, vectorCompa); }
    if (tiac&(1<<OCIE1B)) { irqSystem->SetIrqFlag(this, vectorCompb); }
    if (tiac&(1<<TOIE1))  { irqSystem->SetIrqFlag(this, vectorOvf1); } 
    if (tiac&(1<<TOIE0))  { irqSystem->SetIrqFlag(this, vectorOvf0); } 
}

void HWTimer01Irq::CheckForNewClearIrq(unsigned char tiac) {
    if (tiac&(1<<TICIE1)) { irqSystem->ClearIrqFlag(vectorCapt); }
    if (tiac&(1<<OCIE1A)) { irqSystem->ClearIrqFlag(vectorCompa); }
    if (tiac&(1<<OCIE1B)) { irqSystem->ClearIrqFlag(vectorCompb); }
    if (tiac&(1<<TOIE1))  { irqSystem->ClearIrqFlag(vectorOvf1); }
    if (tiac&(1<<TOIE0))  { irqSystem->ClearIrqFlag(vectorOvf0); }
}



void HWTimer01Irq::ClearIrqFlag(unsigned int vector) {
    if (vector == vectorCapt) {tifr&=0xff-(1<<TICIE1);irqSystem->ClearIrqFlag(vectorCapt);}
    if (vector == vectorCompa ) { tifr&=0xff-(1<<OCIE1A);irqSystem->ClearIrqFlag(vectorCompa);}
    if (vector == vectorCompb ) {tifr&=0xff-(1<<OCIE1B);irqSystem->ClearIrqFlag(vectorCompb);}
    if (vector == vectorOvf1 ) {tifr&=0xff-(1<<TOIE1);irqSystem->ClearIrqFlag(vectorOvf1);}
    if (vector == vectorOvf0 ) {tifr&=0xff-(1<<TOIE0);irqSystem->ClearIrqFlag(vectorOvf0);}
}


void HWTimer01Irq::AddFlagToTifr(unsigned char val){
    tifr|=val; 

    switch(val&timsk) {
        case 0: break; //nothing to set, (flag & mask) -> 0
        case (1<<TICIE1): { irqSystem->SetIrqFlag(this, vectorCapt); } break; 
        case (1<<OCIE1A): { irqSystem->SetIrqFlag(this, vectorCompa); } break;    
        case (1<<OCIE1B): { irqSystem->SetIrqFlag(this, vectorCompb);}  break;
        case (1<<TOIE1):  { irqSystem->SetIrqFlag(this, vectorOvf1); } break; 
        case (1<<TOIE0):  { irqSystem->SetIrqFlag(this, vectorOvf0); } break; 
        default: cerr << "HWTimer01Irq::AddFlagToTifr: Wrong Value For AddFlag 0x"<<hex << (unsigned int) val << endl;

    }
}


unsigned char HWTimer01Irq::GetTimsk() { return timsk;}
unsigned char HWTimer01Irq::GetTifr() {return tifr;}

void HWTimer01Irq::SetTimsk(unsigned char val) { 
    unsigned char tiacOld= timsk&tifr;
    timsk=val;
    unsigned char tiacNew= timsk&tifr;

    unsigned char changed=tiacNew^tiacOld;
    unsigned char setnew= changed&tiacNew;
    unsigned char clearnew= changed& (~tiacNew);

    CheckForNewSetIrq(setnew);
    CheckForNewClearIrq(clearnew);
}

void HWTimer01Irq::SetTifr(unsigned char val) { 
    unsigned char tiacOld= timsk&tifr;
    tifr&=~val;
    unsigned char tiacNew= timsk&tifr;

    unsigned char changed=tiacNew^tiacOld;
    unsigned char clearnew= changed& (~tiacNew);

    CheckForNewClearIrq(clearnew);
}

