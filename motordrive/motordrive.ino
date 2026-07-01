/*
 * Use the wheel mode to generate very low speed rotations.
 */
#include <SCServo.h>

// Define the correct serial port for your target board
#if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(CONFIG_IDF_TARGET_ESP32C6) || defined(CONFIG_IDF_TARGET_ESP32S3)
#define COMSerial Serial0
#else
#define COMSerial Serial1
#endif

// Define RX/TX pins for the servo bus (for reference)
// Note: On ESP32, pins are usually specified in COMSerial.begin().
// For example: COMSerial.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
// If your board uses the default pins for Serial1, no extra specification is needed.
#define S_RXD D7
#define S_TXD D6

SMS_STS st; // Servo control object

const int servo_id = 1;
int running = 0;
int speed = 500;

void servo_stop()
{
	running = 0;
	st.writeByte(servo_id, SMS_STS_MODE, 0);
}
void servo_start()
{
	running = 1;
	st.writeByte(servo_id, SMS_STS_MODE, 1);
	st.WriteSpe(servo_id, speed);
}

unsigned servo_pos()
{
	return st.ReadPos(servo_id);
}

int servo_delta(short target, short current)
{
	// distances of more than 180 degrees wrap around
	// to the other direction
	int delta = target - current;
	if (delta < -2048)
		delta += 4096;
	if (delta > +2048)
		delta -= 4096;

	return delta;
}

unsigned long last_micros = 0;
unsigned long update_usec = 1000000; // 1 Hz
//float update_steps = 4096.0 / 60; // 1 RPM
float update_steps = 4096.0 / (20*60); // 1 rev every 20 minutes
float target_steps = 0;


void setup()
{
	Serial.begin(115200);
	delay(2000); 
	Serial.println("--- Servo Control Program Start ---");

	// Start the serial port for controlling the servos
	COMSerial.begin(1000000, SERIAL_8N1);
	st.pSerial = &COMSerial; // Associate the control object with the serial port
  
	Serial.println("Checking servo connection status...");
	if (st.Ping(1) == -1)
		Serial.println("No servo found?");

	servo_stop();
	target_steps = servo_pos();
	Serial.println(target_steps);
	last_micros = micros();
}

void process_input()
{
	String input = Serial.readString();
	input.trim(); // Remove extra spaces or newlines

	if (('0' <= input[0] && input[0] <= '9') || input[0] == '-') {
		int value = input.toInt();
		Serial.print("speed ");
		Serial.println(value);
		update_steps = value;
	} else {
		Serial.println("unable to parse speed");
	}
}


void loop()
{
	const unsigned current_steps = servo_pos();
	const unsigned long now = micros();
	const unsigned long dt = now - last_micros;

	int delta = servo_delta(target_steps, current_steps);
	if (running)
	{
		// don't process any input while we are running
		// stop once we are close to our target
		if (delta > 0)
			return;

		servo_stop();
		Serial.print((int)target_steps);
		Serial.print(" - ");
		Serial.print(current_steps);
		Serial.print(" = ");
		Serial.print(delta);
		Serial.print(" (");
		Serial.print(dt);
		Serial.println(")");
	}

	if (Serial.available())
		process_input();

	if (dt < update_usec)
		return;

	// it is time to run our update loop
	last_micros = now;
	target_steps += update_steps;
	if (target_steps > 4096)
		target_steps -= 4096;
	delta = servo_delta(target_steps, current_steps);

	if (delta < 0)
	{
		// we overshot? don't do anything this cycle
		Serial.print("OVERSHOT ");
		Serial.print(target_steps);
		Serial.print(" < ");
		Serial.println(current_steps);
		return;
	}

	// start the servo going towards our new target
/*
	Serial.print(delta);
	Serial.print("= ");
	Serial.print(target_steps);
	Serial.print(" - ");
	Serial.println(current_steps);
*/
	servo_start();
}
