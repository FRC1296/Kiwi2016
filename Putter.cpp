/** \file
 * Example of subsystem task behavior.
 *
 * This class is derived from the standard Putter base class and includes
 * initialization for the devices used to control a given subsystem.
 *
 * The task receives messages from the main robot class and implements behaviors
 * for a given subsystem.
 */

#include <Putter.h>
#include <ComponentBase.h>
#include <RobotParams.h>
#include "WPILib.h"

//Robot

Putter::Putter()
: ComponentBase(PUTTER_TASKNAME, PUTTER_QUEUE, PUTTER_PRIORITY)
{
	putterMotor = new CANTalon(CAN_PUTTER_MOTOR);
	wpi_assert(putterMotor);

	putterMotor->SetControlMode(CANSpeedController::kPercentVbus);
	putterMotor->SetVoltageRampRate(240.0);
	wpi_assert(putterMotor->IsAlive());

	pTask = new Task(PUTTER_TASKNAME, (FUNCPTR) &Putter::StartTask, this);
	wpi_assert(pTask);
};

Putter::~Putter()
{
	delete(pTask);
	delete(putterMotor);
};

void Putter::OnStateChange()
{
	// stop the putter whenever the state changes

	putterMotor->Set(0.0);
	printf("got here %d\n", 4);
};

void Putter::Run()
{
	switch(localMessage.command)			//Reads the message command
	{
		case COMMAND_PUTTER_SLOW_MOVE:
			// slow move to position the putter
			putterMotor->Set(localMessage.params.putterMove.fDistanceOrSpeed);
			break;

		case COMMAND_PUTTER_PUTT:
			putterMotor->Set(fSpeed / 2.0);
			Wait(1.0 * (.25/fSpeed));
			putterMotor->Set(0.0);
			Wait(0.2);
			putterMotor->Set(-fSpeed);
			Wait(1.0 * (.25/fSpeed));
			putterMotor->Set(0.0);
			Wait(0.2);
			putterMotor->Set(fSpeed / 2.0);
			Wait(1.25 * (.25/fSpeed));
			ClearMessages();
			break;

		case COMMAND_PUTTER_INC_SPEED:
			if(fSpeed <= 0.99)
			{
				fSpeed += 0.01;
			}
			Wait(0.1);
			ClearMessages();
			break;

		case COMMAND_PUTTER_DEC_SPEED:
			if(fSpeed >= 0.1)
			{
				fSpeed -= 0.01;
			}
			Wait(0.1);
			ClearMessages();
			break;

		default:
			break;
	}

	//Put out information
	if ((iLoop % 25) == 0)
	{
		SmartDashboard::PutNumber("Putter Speed", TRUNC_HUND(fSpeed));
	}
};
