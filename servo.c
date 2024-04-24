#include <LPC21xx.H>
#include "led.h"
#include "timer_interrupts.h"

#define DetectorOut_bm 1<<10


enum DetectorState {ACTIVE, INACTIVE};
enum ServoState {CALLIB, IDLE, IN_PROGRESS,OFFSET};

struct Servo
{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

struct Servo sServo;

void DetectorInit()
{
	IO0DIR = IO0DIR & (~(DetectorOut_bm));
}

enum DetectorState eReadDetector()
{
	if((IO0PIN & DetectorOut_bm) == 0)
	{
		return ACTIVE;
	}
	else
	{
		return INACTIVE;
	}
}

void Automat()
{
	switch(sServo.eState)
	{
		case CALLIB:
			if(eReadDetector() == ACTIVE)
			{
				sServo.uiCurrentPosition = 0;
				sServo.uiDesiredPosition = 0;
				sServo.eState = OFFSET;
			
			}
			else 
			{
				LedStepLeft();
				
			}
			break;
		case OFFSET:
			if(sServo.uiCurrentPosition == 12)
			{
			  sServo.uiCurrentPosition = 0; 
				
				sServo.eState = IN_PROGRESS;
			
			}
	
			else
			{
				sServo.uiCurrentPosition++;
				LedStepRight();
			}
			break;
			
		case IDLE:
			if(sServo.uiCurrentPosition != sServo.uiDesiredPosition)
			{
				
				sServo.eState = IN_PROGRESS;
			}
			break;
		case IN_PROGRESS:
			if(sServo.uiCurrentPosition < sServo.uiDesiredPosition)
			{
				LedStepRight();
				sServo.uiCurrentPosition++;
			}
			else if(sServo.uiCurrentPosition > sServo.uiDesiredPosition)
			{
				LedStepLeft();
				sServo.uiCurrentPosition--;
			}
			else if(sServo.uiCurrentPosition == sServo.uiDesiredPosition)
			{
				sServo.eState = IDLE;
			}
			break;
	}
}

void ServoInit(unsigned int uiServoFrequency)
{
	LedInit();
	DetectorInit();
	sServo.eState = CALLIB;
	Timer0Interrupts_Init(1000000/uiServoFrequency, &Automat);
		//while(sServo.eState != IDLE) 			
	{
		
	}
}

void ServoCallib(void)
{
	sServo.eState = CALLIB;
}

void ServoGoTo(unsigned int uiPosition)
{
			sServo.uiDesiredPosition = uiPosition;
	sServo.eState = IN_PROGRESS;			
	//while(sServo.eState != IDLE)
	{
		
	}
}


