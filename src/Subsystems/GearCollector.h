#ifndef GEAR_COLLECTOR_H
#define GEAR_COLLECTOR_H

#include <Commands/Subsystem.h>

class GearCollector : public Subsystem
{
private:
	CANTalon* m_intake;

public:
	GearCollector();
	void InitDefaultCommand();
};

#endif  // GEAR_COLLECTOR_H
