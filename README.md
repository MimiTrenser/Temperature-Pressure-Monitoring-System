1. Introduction
This document describes the complete Software Development Life Cycle (SDLC) for a multi-threaded embedded application that periodically polls parameters (Temperature, Pressure, Configuration Version) and processes them to send if it exceeds the Threshold values within sampling time.
The system contains:
•	A polling thread that periodically (In ms) reads sensor values
•	A processing thread that analyses polled values at calculated intervals (In ms)
•	A global data structure storing the latest polled values (Temperature, Pressure and Configuration Version)
•	Notification logic (If Threshold exceeds).
2. Requirement Analysis
The system reads sensor values periodically, Process the values against threshold, and generates notifications as the alert if value remain out of range for a defined sampling duration.
Sensors: Temperature Sensor, Pressure Sensor
Polling Interval: 50ms
Processing Interval: 200ms
Sampling Time (Violation persistent Time) : 400ms
•	Temperature (Range: -20°C to 100°C)
•	Pressure (Range: 0 to 10,000 Pascals)
•	Configuration Version (5 characters)

2.1 Functional Requirements
=> Sensor Polling: 
•	The system shall poll Temperature and Pressure sensors every 50ms.
•	The system shall store polled values to the global data structure.
=> Data Processing:
•	The system shall process all the available polled values every 200ms.
•	Compare each parameter against threshold values.
=>Threshold Violation Detection:
•	System shall detect polled value above or below the threshold value.
•	The system shall not send notification immediately on the first violation.
=>Sampling Time Logic:
•	If violation is persistent above or equal to sampling time, send notification (Sampling Time = 400ms)
•	If violation is not persistent up to sampling time clear violation flag.
3. System Design
High-Level Architecture
•	Thread 1: Polling Task
•	Thread 2: Processing Task
•	Shared Memory: Global Structure storing latest values
•	Mutex or lock for thread safety
•	Notification Module

	
	
	
	














Low-Level Architecture

Polling Configuration for each parameter:
Field	Value
Parameter ID	PARAM_TEMP(eg)
Value	char
Poll Interval	50 ms (polling thread)
	
Processing Configuration:
Field	Value
Parameter ID	PARAM_TEMP
Processing Interval	200 ms   
Validation Rule	Check if value < -10 OR value > 70
Notification Condition   	If data values above/below threshold for Sampling Time
Global Polled Data Structure:
Field	Purpose
Param	Identifies the sensor
Value	Holds actual data (char or string)
LowerThreshold	Minimum allowed value
UpperThreshold	Maximum allowed value

1. Initialization (Start)
Start -> sensor system init 
2. Main Polling and Processing Loop
-> get polled value(Temperature, pressure, Configuration Version)
-> Process Thread
3. Configuration Read and Global Setup
Decision 1: Read Temp/Pressure/ConfigVer(Polled Once) Success?
IF Successful -> read data -> read successfully -> read corresponding timestamp -> set to global structure (setpolled value)
IF Fails -> read data -> read fails -> try again
-> Wait/Delay: 50ms cyclic (This defines the polling frequency)
-> Global structure (sensor.h)
4. Threshold Processing and Violation Handling
Decision 2: Values within threshold range?
IF WITHIN -> if values are within threshold -> process normal execution
IF ABOVE/BELOW -> if values are above/below threshold
-> increment violation time by 200
Decision 3: Violation Time Exceeds Sampling Time (400ms)?
IF YES -> if violation time time exceeds (400ms) sampling time -> send notification
IF NO -> (else reset to zero) -> reset violation time to zero
-> 200ms cyclic process
-> Decision 4: In case of config value?
IF YES -> in case of config value -> polled only once
IF NO/Config Version Changes -> if config version changes -> updates new configuration

4.  Implementation
•	Main.c -> Create Threads
•	Polling.c -> Read sensor value
•	Process.c -> Compare with Threshold
•	Sensor.c -> Initialize Sensor with hardcoded values
•	Sensor.h -> Global structure
5.Testing
•	Developer Testing
•	Unit Testing
•	Integration Testing
6.Maintenence
•	Flash firmware to production devices

  

