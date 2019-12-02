**// todo**
-[x] figure out if all 8 motor outputs are PWM (YES).
-[x] GPIO input based on encoder (poll/vs interrupt)
	a. Encoders I have don't work very well.
-[] speed controller
	a. PID loop
-[] external sensors
	a. Accelerometer/Gyro first

**// external design decisions**
* abstraction
	* GPIO
		* direct motor control/PID
	* ADC
		* Sensor values
	* Specific comms
		* Peripheral control
	* After all of above
		* Robot control
* tons of interrupts vs poll for stuff.
	* depends on NVIC priorities
* Need for RTOS
	* once we start adding external peripherals like gps then we'll need the RTOS
	* RTOS needed once we start needing to filter enough information that a cooperative system isn't going to work
	* or if timing specific communication is necessary.
* HAL vs roll-own - Use HAL until I need to make my own hardware
	* HAL's initTypeDef structures and enums are very useful
	* Need HAL for SysTick and other happy peripherals
	* Would have to roll my own IRQ handlers...yikes
	* Maybe use HAL init, but not HAL read/write/communicate.
		* Once init, send information directly to interrupts.
