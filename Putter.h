/** \file
 * Example of subsystem task declaration.
 *
 * This class is derived from the standard Putter base class and includes
 * initialization for the devices used to control a given subsystem.
 *
 */
#ifndef PUTTER_H
#define PUTTER_H

/**
	A template class for creating new components
 */
#include <pthread.h>

//Robot
#include "WPILib.h"

#include "ComponentBase.h"			//For ComponentBase class

class Putter : public ComponentBase
{
public:
	Putter();
	~Putter();
	static void *StartTask(void *pThis)
	{
		((Putter *)pThis)->DoWork();
		return(NULL);
	}

private:
	CANTalon* putterMotor;
	float fSpeed = 0.50;

	void OnStateChange();
	void Run();
};

#endif			//PUTTER_H
