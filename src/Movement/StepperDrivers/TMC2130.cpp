/*
 * TMC2130.cpp
 *
 *  Created on: 17.09.2019
 *      Author: Alex
 */

#include "RepRapFirmware.h"

#if SUPPORT_TMC2130

#include "TMC2130.h"
#include "RepRap.h"
#include "Movement/Move.h"
#include "Movement/StepTimer.h"

# if SAME70
#  include "sam/drivers/xdmac/xdmac.h"
#  include "DmacManager.h"
# else
#  include "sam/drivers/pdc/pdc.h"
# endif

#include "sam/drivers/usart/usart.h"

const float MaximumMotorCurrent = 2000.0;
const uint32_t DefaultMicrosteppingShift = 4;				// x16 microstepping
const bool DefaultInterpolation = true;						// interpolation enabled
const int DefaultStallDetectThreshold = 1;
const bool DefaultStallDetectFiltered = false;
const unsigned int DefaultMinimumStepsPerSecond = 200;		// for stall detection: 1 rev per second assuming 1.8deg/step, as per the TMC2130 datasheet

static size_t numTmc2130Drivers;

static bool driversPowered = false;

// The SPI clock speed is a compromise:
// - too high and polling the driver chips take too much of the CPU time
// - too low and we won't detect stalls quickly enough
// With a 4MHz SPI clock:
// - polling the drivers makes calculations take 13.5% longer, so it is taking about 12% of the CPU time
// - we poll all 10 drivers in about 80us
// With a 2MHz SPI clock:
// - polling the drivers makes calculations take 8.3% longer, so it is taking about 7.7% of the CPU time
// - we poll all 10 drivers in about 170us
const uint32_t DriversSpiClockFrequency = 2000000;			// 2MHz SPI clock

const int DefaultStallGuardThreshold = 1;					// Range is -64..63. Zero seems to be too sensitive. Higher values reduce sensitivity of stall detection.

// TMC2130 register addresses
const uint32_t TMC_REGNUM_MASK    = 0x7F << 32;
const uint32_t TMC_REG_GCONF      = 0x00 << 32;
const uint32_t TMC_REG_GSTAT      = 0x01 << 32;
const uint32_t TMC_REG_IOIN       = 0x04 << 32;
const uint32_t TMC_REG_IHOLD_IRUN = 0x10 << 32;
const uint32_t TMC_REG_TPOWERDOWN = 0x11 << 32;
const uint32_t TMC_REG_TSTEP      = 0x12 << 32;
const uint32_t TMC_REG_TPWMTHRS   = 0x13 << 32;
const uint32_t TMC_REG_TCOOLTHR   = 0x14 << 32;
const uint32_t TMC_REG_THIGH      = 0x15 << 32;
const uint32_t TMC_REG_CHOPCONF   = 0x6C << 32;
const uint32_t TMC_REG_COOLCONF   = 0x6D << 32;
const uint32_t TMC_REG_DRV_STATUS = 0x6F << 32;
const uint32_t TMC_REG_PWMCONF    = 0x70 << 32;
const uint32_t TMC_REG_PWM_SCALE  = 0x71 << 32;
const uint32_t TMC_REG_LOST_STEPS = 0x73 << 32;
const uint32_t TMC_DATA_MASK      = 0xFFFFFFFF;

//--------------------------- Public interface ---------------------------------

namespace SmartDrivers
{
	// Initialise the driver interface and the drivers, leaving each drive disabled.
	// It is assumed that the drivers are not powered, so driversPowered(true) must be called after calling this before the motors can be moved.
	void Init(const Pin driverSelectPins[NumDirectDrivers], size_t numTmcDrivers)
	{
		numTmc2130Drivers = min<size_t>(numTmcDrivers, MaxSmartDrivers);

		// Make sure the ENN pins are high
		pinMode(GlobalTmc2130EnablePin, OUTPUT_HIGH);

		// The pins are already set up for SPI in the pins table
		ConfigurePin(TMC2130MosiPin);
		ConfigurePin(TMC2130MisoPin);
		ConfigurePin(TMC2130SclkPin);

		// Enable the clock to the USART or SPI
		pmc_enable_periph_clk(ID_TMC2130_SPI);
    }
};	// end namespace

#endif

// End

