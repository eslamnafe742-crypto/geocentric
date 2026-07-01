/*
 * Use the wheel mode to generate very low speed rotations.
 * Since the servo position command mode doesn't allow wrapping, this uses the
 * wheel mode to move the servo a little bit each time step.
 */
#include <SCServo.h>
#include <WiFi.h>

#if 0
unsigned long update_usec = 1000000; // 1 Hz
float update_steps = 4096.0 / (20*60); // 1 rev every 20 minutes
#else
#define UPDATE_SEC 10
unsigned long update_usec = UPDATE_SEC * 1000000;
float update_steps = 4096.0 / (20*60/UPDATE_SEC); // 1 rev every 20 minutes
#endif
int speed = 10;

const char * ssid = "XXXX";
const char * password = "YYYY";
const char * ntp_server = "pool.ntp.org";


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
float target_steps = -1;


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

#ifdef CONFIG_USE_WIFI
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println(WiFi.localIP());

	configTime(0, 0, ntp_server);
#endif
	
}

void process_input()
{
	String input = Serial.readString();
	input.trim(); // Remove extra spaces or newlines

	if (('0' <= input[0] && input[0] <= '9') || input[0] == '-') {
		float value = input.toFloat();
		Serial.print("speed ");
		Serial.println(value);
		
		if (value == 0)
		{
			servo_stop();
			target_steps = -1;
		} else {
			servo_start();
			st.WriteSpe(servo_id, value);
			running = 2;
		}
	} else
	if (input[0] == '?') {
		Serial.print(servo_pos());
		Serial.print(" ");
		Serial.print(update_steps);
		Serial.print(" @ ");
		Serial.print(update_usec);
		Serial.println();
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
	if (running == 1)
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

	if (dt < update_usec || running == 2)
		return;

	// it is time to run our update loop
	last_micros = now;
	if (update_steps == 0)
		return;

#if 1
	// initialize the target from our current steps
	if (target_steps == -1)
		target_steps = current_steps;

	target_steps += update_steps;
	if (target_steps > 4096)
		target_steps -= 4096;
#else
	struct tm timeinfo;
	getLocalTime(&timeinfo);
	int seconds_past_twenty = (timeinfo.tm_min % 20) * 60 + timeinfo.tm_sec;
	target_steps = (seconds_past_twenty * 4096) / (60 * 20);

	Serial.print(timeinfo.tm_min);
	Serial.print(":");
	Serial.print(timeinfo.tm_sec);
	Serial.print("=");
	Serial.print(seconds_past_twenty);
	Serial.print(" => ");
	Serial.println(target_steps);
#endif
	
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
