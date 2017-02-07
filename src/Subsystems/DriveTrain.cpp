///////////////////////////////////////////////////////////////////////////////
//
// DriveTrain.cpp
//
//  Created on: Jan 7, 2017
//      Author: Silas A.
//
///////////////////////////////////////////////////////////////////////////////

#include "DriveTrain.h"
#include "../RobotMap.h"
#include "../Commands/Drive.h"
#include <iostream>

DriveTrain::DriveTrain() :
	frc::Subsystem("Drive")
{
}

DriveTrain::~DriveTrain()
{
	delete m_shifter;
	delete m_drive;
	delete m_lf;
	delete m_lb;
	delete m_rf;
	delete m_rb;
}

void DriveTrain::InitDefaultCommand()
{
	SetDefaultCommand(new Drive());
}

void DriveTrain::Initialize(frc::Preferences* prefs)
{
	m_shifter = new Solenoid(DRIVE_SHIFT_PORT);

	m_lb = new CANTalon(DRIVE_LEFT_PORT);
	m_lb->SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);

	m_lf = new CANTalon(DRIVE_LF_PORT);
	m_lf->SetControlMode(CANTalon::ControlMode::kFollower);
	m_lf->Set(m_lb->GetDeviceID());

	m_rb = new CANTalon(DRIVE_RIGHT_PORT);
	m_rb->SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);

	m_rf = new CANTalon(DRIVE_RF_PORT);
	m_rf->SetControlMode(CANTalon::ControlMode::kFollower);
	m_rf->Set(m_rb->GetDeviceID());

	SetTalonMode(CANTalon::ControlMode::kPercentVbus);
	set_pid_values();

	//m_rf->SetInverted(true);
	//m_rb->SetInverted(true);

	//m_rb->SetSensorDirection(true);

	m_drive = new RobotDrive(m_lb, m_rb);

	m_shiftState = m_shifter->Get();
}

void DriveTrain::DashboardOutput(bool verbose)
{
	frc::SmartDashboard::PutNumber("Count L", m_lb->GetEncVel());
	frc::SmartDashboard::PutNumber("Count R", m_rb->GetEncVel());

	frc::SmartDashboard::PutNumber("Pre L", m_lb->Get());
	frc::SmartDashboard::PutNumber("Pre R", m_rb->Get());

	if (verbose)
	{

	}
}

CANTalon::TalonControlMode DriveTrain::get_talon_mode()
{
	return (CANTalon::TalonControlMode)(m_lb->GetTalonControlMode() & m_rb->GetTalonControlMode());
}

void DriveTrain::set_pid_values()
{
	switch (CANTalon::TalonControlMode::kSpeedMode)
	{
	case CANTalon::TalonControlMode::kPositionMode:
		m_lb->SetPID(
				frc::SmartDashboard::GetNumber("drive_left_pos_p", DRIVE_LEFT_POS_P),
				frc::SmartDashboard::GetNumber("drive_left_pos_i", DRIVE_LEFT_POS_I),
				frc::SmartDashboard::GetNumber("drive_left_pos_d", DRIVE_LEFT_POS_D));
		m_rb->SetPID(
				frc::SmartDashboard::GetNumber("drive_right_pos_p", DRIVE_RIGHT_POS_P),
				frc::SmartDashboard::GetNumber("drive_right_pos_i", DRIVE_RIGHT_POS_I),
				frc::SmartDashboard::GetNumber("drive_right_pos_d", DRIVE_RIGHT_POS_D));
		break;
	default:
	case CANTalon::TalonControlMode::kSpeedMode:
		m_lb->SetPID(
				frc::SmartDashboard::GetNumber("drive_left_spd_p", DRIVE_LEFT_POS_P),
				frc::SmartDashboard::GetNumber("drive_left_spd_i", DRIVE_LEFT_POS_I),
				frc::SmartDashboard::GetNumber("drive_left_spd_d", DRIVE_LEFT_POS_D));
		m_rb->SetPID(
				frc::SmartDashboard::GetNumber("drive_right_spd_p", DRIVE_RIGHT_POS_P),
				frc::SmartDashboard::GetNumber("drive_right_spd_i", DRIVE_RIGHT_POS_I),
				frc::SmartDashboard::GetNumber("drive_right_spd_d", DRIVE_RIGHT_POS_D));
		break;
	}
}

void DriveTrain::SetTalonMode(CANTalon::ControlMode mode)
{
	//m_lb->SetTalonControlMode(mode);
	//m_rb->SetTalonControlMode(mode);

	m_lb->SetControlMode(mode);
	m_rb->SetControlMode(mode);

	set_pid_values();
}

void DriveTrain::ArcadeDrive(double move, double rotate)
{
	if (m_reverse)
	{
		move = -move;
		rotate = -rotate;
	}
	m_drive->ArcadeDrive(move * DRIVE_ENC_CPR, rotate * DRIVE_ENC_CPR);
}

void DriveTrain::TankDrive(double left, double right)
{
	if (m_reverse)
	{
		left = -left;
		right = -right;
	}
	m_drive->TankDrive(left, right);
	//std::cout << "L counts: " << m_lb->GetEncPosition() << ": R count: " << m_rb->GetEncPosition() << std::endl;
}

void DriveTrain::Reverse(bool reverse)
{
	m_reverse = reverse;
}

void DriveTrain::Shift()
{
	m_shifter->Set(!m_shiftState);
	m_shiftState = m_shifter->Get();
}

void DriveTrain::SetScale(double scale)
{
	m_scaleFactor = scale;
}

void DriveTrain::SetPosition(double pos)
{
	m_lb->Set(pos);
	m_rb->Set(-pos);
	std::cout << "L counts: " << m_lb->GetEncPosition() << " : R count: " << m_rb->GetEncPosition() << std::endl;
}
