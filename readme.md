Otomo
======

Otomo is the silly name I came up with for a robot that would mow my lawn automatically. However, I don't have the resources to make a proper lawn mower, it became a learning space for autonomous rover experimentation. Mostly also because I haven't made a proper one, and I feel like it's something every mechatronic engineer should do _at some point_.

The focus of this system (for now) is to use the STMicroelectronics STM32F4 Discovery board (because I had one lying around) and a DFRobot Baron chassis.  I don't recommend the Baron.  If you are going to make a robot, do it from scratch.  I just didn't have the tools to make a proper chassis.

Since this is a from-the-ground-up process for learning how to do things properly: from programming practices to hardware and beyond, expect lots of problems and issues.

**// todo**
- [x] figure out if all 8 motor outputs are PWM (YES).
- [x] GPIO input based on encoder (poll/vs interrupt)
	- [x] Encoders I have don't work very well. (DF Robot fails me again.)
- [ ] speed controller
	- PID loop
	- Kalman filter for speed/positional control (accel/gyro)
	- Find some working encoders
- [ ] external sensors
	- Accelerometer/Gyro first (use the onboard gyro from the Discovery, and an accelerometer I've had lying around for years.)
	- Distance: Ultrasonic, LIDAR.
- [x] Remove dependency on ST Eclipse tools
    - Move to platformIO
    - Keep CubeMX code generator as it's handy

**// external design decisions**
* abstraction
	* Specific comms
		* Peripheral control
	* After all of above
		* Robot control
* Need for RTOS
	* once we start adding external peripherals like gps then we'll need the RTOS
	* RTOS needed once we start needing to filter enough information that a cooperative system isn't going to work
	* or if timing specific communication is necessary.
* HAL vs roll-own - Use HAL until I need to make my own hardware
    * Use CubeMX for now, maybe refactor to libopencm3 at some point in the distant future
	~~* HAL's initTypeDef structures and enums are very useful~~
	~~* Need HAL for SysTick and other happy peripherals~~
	~~* Would have to roll my own IRQ handlers...yikes~~
