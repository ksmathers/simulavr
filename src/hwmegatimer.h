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
#ifndef HWMEGATIMER
#define HWMEGATIMER

#include "hardware.h"
#include "pinatport.h"
#include "rwmem.h"

class HWIrqSystem;
class HWMegaTimer0123Irq;
class HWPrescaler;


class HWMegaTimer0: public Hardware {
	protected:
		unsigned char tccr;
		unsigned char tcnt;
		unsigned char ocr;

        AvrDevice *core;
		HWPrescaler *prescaler;
		HWMegaTimer0123Irq *timer01irq;
		PinAtPort pin_oc;


		bool t0_old; 	//last state of external t0 pin
		bool last_oc;
		bool cntDir;

		static const unsigned char top0=0x0000;
		static const unsigned char topFF=0xff;

		const unsigned char *pointerToTop;
		const unsigned char *tovCompare;

		unsigned char timerMode;

		enum {
			STOP=0,
			CK,
			CK8,
			CK32,
			CK64,
			CK128,
			CK256,
			CK1024,
		} T_prescaler;


		void TimerCompareAfterCount();
		void CheckForMode();
		bool OcrWork(unsigned char &ocr, bool &lastOcr, PinAtPort &pinOc, unsigned char ocrMode);
		void OcrResetPin(unsigned char &ocr, bool &lastOcr, PinAtPort &pinOc, unsigned char ocrMode);

	public:
		HWMegaTimer0(AvrDevice *core, HWPrescaler *p, HWMegaTimer0123Irq *s, PinAtPort oc);

		void Reset() {
			SetTccr(0);
			SetTcnt(0);
			t0_old=0;

		}
		virtual unsigned int CpuCycle();
		void SetTccr(unsigned char val);
		void SetTcnt(unsigned char val) { tcnt=val; }
		void SetOcr(unsigned char val) {ocr=val; } //double buffering is missing TODO
		unsigned char GetTccr() { return tccr; }
		unsigned char GetTcnt() { return tcnt; }
		unsigned char GetOcr() { return ocr; }
};

class HWMegaTimer2: public Hardware {
	protected:
		unsigned char tccr;
		unsigned char tcnt;
		unsigned char ocr;

        AvrDevice *core;
		HWPrescaler *prescaler;
		HWMegaTimer0123Irq *timer01irq;
		PinAtPort pin_t0;
		PinAtPort pin_oc;

		bool t0_old; 	//last state of external t0 pin
		bool last_oc;
		bool cntDir;

		static const unsigned char top0=0x0000;
		static const unsigned char topFF=0xff;

		const unsigned char *pointerToTop;
		const unsigned char *tovCompare;

		unsigned char timerMode;

		enum {
			STOP=0,
			CK,
			CK8,
			CK64,
			CK256,
			CK1024,
			T0_FALLING,
			T0_RISING
		} T_prescaler;


		void TimerCompareAfterCount();
		void CheckForMode();
		bool OcrWork(unsigned char &ocr, bool &lastOcr, PinAtPort &pinOc, unsigned char ocrMode);
		void OcrResetPin(unsigned char &ocr, bool &lastOcr, PinAtPort &pinOc, unsigned char ocrMode);

	public:
		HWMegaTimer2(AvrDevice *core, HWPrescaler *p, HWMegaTimer0123Irq *s, PinAtPort pi, PinAtPort oc);

		void Reset() {
			SetTccr(0);
			SetTcnt(0);
			t0_old=0;

		}
		virtual unsigned int CpuCycle();
		void SetTccr(unsigned char val);
		void SetTcnt(unsigned char val) { tcnt=val; }
		void SetOcr(unsigned char val) {ocr=val; } //double buffering is missing TODO
		unsigned char GetTccr() { return tccr; }
		unsigned char GetTcnt() { return tcnt; }
		unsigned char GetOcr() { return ocr; }
};


class HWMegaTimer1 : public Hardware {
	protected:

        AvrDevice *core;
		HWPrescaler *prescaler;
		//HWIrqSystem *irqSystem;
		HWMegaTimer0123Irq *timer01irq;

		unsigned char tccr1a;
		unsigned char tccr1b;
		unsigned char tccr1c;

		unsigned short tcnt1;
		unsigned char tcnt1htemp;

		unsigned short ocr1a;
		unsigned char ocr1ahtemp;

		unsigned short ocr1b;
		unsigned char ocr1bhtemp;

		unsigned short ocr1c;
		unsigned char ocr1chtemp;

		unsigned short icr1;
		unsigned char icr1htemp;

		//affected external pins
		//bool t1;
		bool t1_old;

		//bool oc1a;
		//bool oc1b;
		//
		bool isTimer1;
		PinAtPort pin_t1;
		PinAtPort pin_oc1a;
		PinAtPort pin_oc1b;
		PinAtPort pin_oc1c;

		bool icp;	//input capture
		bool icp_old;

		bool last_ocr1a;
		bool last_ocr1b;
		bool last_ocr1c;

		unsigned char inputCaptureNoiseCnt; //count for 4 cycles if set in ICNC1

		bool cntDir;
		int topValue; // needed for pwm Mode. 

		enum {
			STOP=0,
			CK,
			CK8,
			CK64,
			CK256,
			CK1024,
			T0_FALLING,
			T0_RISING
		} T_prescaler;

		void TimerCompareAfterCount();
		bool OcrWork(unsigned short &ocr, bool &lastOcr, PinAtPort &pinOc, unsigned char ocrMode);
		void OcrResetPin(unsigned short &ocr, bool &lastOcr, PinAtPort &pinOc, unsigned char ocrMode);

	public:
		HWMegaTimer1(AvrDevice *core, HWPrescaler *p, HWMegaTimer0123Irq *s, bool isTimer1, PinAtPort t1, PinAtPort oca, PinAtPort ocb, PinAtPort occ);
		void Reset();
		virtual unsigned int CpuCycle();
		unsigned char GetTccr1a() { return tccr1a;}
		unsigned char GetTccr1b() { return tccr1b;}
		unsigned char GetTccr1c() { return tccr1c;}

		unsigned char GetTcnt1h() { return tcnt1htemp;}
		unsigned char GetTcnt1l() { return tcnt1&0xff; tcnt1htemp=tcnt1>>8;}
		unsigned char GetOcr1ah() { return ocr1a>>8;}
		unsigned char GetOcr1al() { return ocr1a&0xff;}
		unsigned char GetOcr1bh() { return ocr1b>>8;}
		unsigned char GetOcr1bl() { return ocr1b&0xff;}
		unsigned char GetOcr1ch() { return ocr1c>>8;}
		unsigned char GetOcr1cl() { return ocr1c&0xff;}
		unsigned char GetIcr1h() { return icr1htemp;}
		unsigned char GetIcr1l() { return icr1&0xff; icr1htemp=icr1>>8;}

		static const unsigned short top0=0x0000;
		static const unsigned short topFFFF=0xffff;
		static const unsigned short topFF=0xff;
		static const unsigned short top1FF=0x1ff;
		static const unsigned short top3FF=0x3ff;

		const unsigned short *pointerToTop;
		const unsigned short *tovCompare;

		unsigned char timerMode;



		void SetTccr1a(unsigned char val) { tccr1a=val;	CheckForMode(); }
		void SetTccr1b(unsigned char val) ;
		void SetTccr1c(unsigned char val) { tccr1c=val; CheckForMode(); }
		void SetTcnt1h(unsigned char val) { tcnt1htemp=val;}
		void SetTcnt1l(unsigned char val) { tcnt1=val+(tcnt1htemp<<8);}
		void SetOcr1ah(unsigned char val) { ocr1ahtemp=val;}
		void SetOcr1al(unsigned char val) { ocr1a=val+(ocr1ahtemp<<8);}
		void SetOcr1bh(unsigned char val) { ocr1bhtemp=val;}
		void SetOcr1bl(unsigned char val) { ocr1b=val+(ocr1bhtemp<<8);}
		void SetOcr1ch(unsigned char val) { ocr1chtemp=val;}
		void SetOcr1cl(unsigned char val) { ocr1c=val+(ocr1bhtemp<<8);}

		void CheckForMode();
};


class RWTimskM: public RWMemoryMembers {
    protected:
        HWMegaTimer0123Irq *hwTimer01Irq;

    public:
        RWTimskM(HWMegaTimer0123Irq *s) {hwTimer01Irq=s;  }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWTifrM: public RWMemoryMembers {
    protected:
        HWMegaTimer0123Irq *hwTimer01Irq;
    public:
        RWTifrM(HWMegaTimer0123Irq *s) {hwTimer01Irq=s;}
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWEtimskM: public RWMemoryMembers {
    protected:
        HWMegaTimer0123Irq *hwTimer01Irq;

    public:
        RWEtimskM(HWMegaTimer0123Irq *s) {hwTimer01Irq=s;  }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWEtifrM: public RWMemoryMembers {
    protected:
        HWMegaTimer0123Irq *hwTimer01Irq;
    public:
        RWEtifrM(HWMegaTimer0123Irq *s) {hwTimer01Irq=s;}
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWTccr0: public RWMemoryMembers {
    protected:
        HWMegaTimer0 *timer0;
    public:
        RWTccr0(HWMegaTimer0 *t0) { timer0=t0; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWTcnt0: public RWMemoryMembers {
    protected:
        HWMegaTimer0 *timer0;
    public:
        RWTcnt0(HWMegaTimer0 *t0) { timer0=t0; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWOcr0: public RWMemoryMembers {
    protected:
        HWMegaTimer0 *timer0;
    public:
        RWOcr0(HWMegaTimer0 *t0) { timer0=t0; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
//Timer 2 (only for Mega)
class HWMegaTimer2;

class RWTccr2: public RWMemoryMembers {
    protected:
        HWMegaTimer2 *timer0;
    public:
        RWTccr2(HWMegaTimer2 *t0) { timer0=t0; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWTcnt2: public RWMemoryMembers {
    protected:
        HWMegaTimer2 *timer0;
    public:
        RWTcnt2(HWMegaTimer2 *t0) { timer0=t0; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWOcr2: public RWMemoryMembers {
    protected:
        HWMegaTimer2 *timer0;
    public:
        RWOcr2(HWMegaTimer2 *t0) { timer0=t0; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};


class RWTccraM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWTccraM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};

class RWTccrbM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWTccrbM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWTccrcM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWTccrcM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWTcnthM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWTcnthM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWTcntlM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWTcntlM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWOcrahM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWOcrahM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWOcralM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWOcralM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWOcrbhM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWOcrbhM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWOcrblM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWOcrblM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWOcrchM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWOcrchM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWOcrclM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWOcrclM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWIcrhM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWIcrhM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};
class RWIcrlM: public RWMemoryMembers {
    protected:
        HWMegaTimer1 *timer1;
    public:
        RWIcrlM(HWMegaTimer1 *t1) { timer1= t1; }
        virtual unsigned char operator=(unsigned char);
        virtual operator unsigned char() const;
};



#endif
