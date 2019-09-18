/*
 * TMC2130.h
 *
 *  Created on: 17.09.2019
 *      Author: Alex
 */

#ifndef SRC_MOVEMENT_STEPPERDRIVERS_TMC2130_H_
#define SRC_MOVEMENT_STEPPERDRIVERS_TMC2130_H_

#if SUPPORT_TMC2130

#include "RepRapFirmware.h"
#include "DriverMode.h"
#include "Pins.h"
#include "MessageType.h"

namespace SmartDrivers
{
    void Init(const Pin[NumDirectDrivers], size_t numTmcDrivers)
	void Spin(bool powered);
	void TurnDriversOff();

	void SetAxisNumber(size_t driver, uint32_t axisNumber);
	void SetCurrent(size_t driver, float current);
	void EnableDrive(size_t driver, bool en);
	uint32_t GetLiveStatus(size_t driver);
	uint32_t GetAccumulatedStatus(size_t drive, uint32_t bitsToKeep);
	bool SetMicrostepping(size_t drive, unsigned int microsteps, bool interpolation);
	unsigned int GetMicrostepping(size_t drive, bool& interpolation);
	bool SetDriverMode(size_t driver, unsigned int mode);
	DriverMode GetDriverMode(size_t driver);
	void SetStallThreshold(size_t driver, int sgThreshold);
	void SetStallFilter(size_t driver, bool sgFilter);
	void SetStallMinimumStepsPerSecond(size_t driver, unsigned int stepsPerSecond);
	void AppendStallConfig(size_t driver, const StringRef& reply);
	void AppendDriverStatus(size_t driver, const StringRef& reply);
	float GetStandstillCurrentPercent(size_t driver);
	void SetStandstillCurrentPercent(size_t driver, float percent);
	bool SetRegister(size_t driver, SmartDriverRegister reg, uint32_t regVal);
	uint32_t GetRegister(size_t driver, SmartDriverRegister reg);
};

#endif

#endif /* SRC_MOVEMENT_STEPPERDRIVERS_TMC2130_H_ */
