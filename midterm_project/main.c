#include <stdio.h> 
#include <wiringPi.h> 
#include <softPwm.h>

//define requried things
#define ENCODERA 17 //Hall Sensor A
#define ENCODERB 27  //Hall Sensor B
#define ENC2REDGEAR 216 // 1 rotation of Motor = 216 Edge


#define MOTOR1 19 //use GPIO19 pin
#define MOTOR2 26 //use GPIO26 pin

//hyper-parameters we set for PID control
#define PGAIN 1000 
#define IGAIN 0.2
#define DGAIN 100
#define PULSEINPUT 18 //To get pulse 

#define SAMPLING_PERIOD 1 //millisecond

// initialize
int encA;
int encB;
int encoderPosition = 0;
float redGearPosition = 0;

// measure the position; reference means how many rotation the motor has at previous target position to reach the target position and error means error between current positon and target position. we use the error when we control the motor
float referencePosition = 0;
float errorPosition = 0;

// speed of interpreting codes > speed of controling the motor. we need enough time to control the motor
unsigned int checkTime;
unsigned int checkTimeBefore;

void funcEncoderA() //encoder position
{
	encA = digitalRead(ENCODERA); // if edge detected, function A and B are excuted. We use two hall sensor to recognize the direction of motor's rotation
	encB = digitalRead(ENCODERB);
	if (encA == HIGH)
	{
		if (encB == LOW)
		{
			encoderPosition++; // ClockWise direction 
		}
		else
		{
			encoderPosition--; // Counter ClockWise direction
		}
	}
	else
	{
		if (encB == LOW)
		{
			encoderPosition--; // Counter ClockWise direction
		}
		else
		{
			encoderPosition++; // ClockWise direction
		}
	}
	redGearPosition = (float)encoderPosition / ENC2REDGEAR; // encoderposition is represented in edge dimension, so we need to convert the edge dimension to rotation of motor dimension
	errorPosition = referencePosition - redGearPosition; // Calculate error
}
void funcEncoderB() //encoder position 
{
	encA = digitalRead(ENCODERA);
	encB = digitalRead(ENCODERB);
	if (encB == HIGH)
	{
		if (encA == LOW)
		{
			encoderPosition--;
		}
		else
		{
			encoderPosition++;
		}
	}
	else
	{
		if (encA == LOW)
		{
			encoderPosition++;
		}
		else
		{
			encoderPosition--;
		}
	}
	redGearPosition = (float)encoderPosition / ENC2REDGEAR; // encoderposition is represented in edge dimension, so we need to convert the edge dimension to rotation of motor dimension
	errorPosition = referencePosition - redGearPosition;
}

//To make the control process more precise, initialize the motor to locate the edge!
void MoterReady()
{

	while (1) {
		softPwmWrite(MOTOR1, 50);
		softPwmWrite(MOTOR2, 0);
		if (encA == HIGH || encB == LOW || encA == HIGH || encB == LOW) { //At begining, There were nothing in enc A and B. if edge detected, then functionA is excuted and encA,B have value. we use these!
			softPwmWrite(MOTOR1, 0);
			break;
		}
	}
	errorPosition = 0;
	encoderPosition = 0;
	redGearPosition = 0;
	printf("Motor ready!\n");
}


int main() {
	wiringPiSetupGpio();
	pinMode(ENCODERA, INPUT);		// Set ENCODERA as input
	pinMode(ENCODERB, INPUT);		// Set ENCODERB as input
	pinMode(PULSEINPUT, INPUT);     // Set PulseInput as input

	softPwmCreate(MOTOR1, 0, 100);		// Create soft Pwm
	softPwmCreate(MOTOR2, 0, 100); 		// Create soft Pwm

	wiringPiISR(ENCODERA, INT_EDGE_BOTH, funcEncoderA); // if Edge detected, functionA automaticly excuted
	wiringPiISR(ENCODERB, INT_EDGE_BOTH, funcEncoderB); // if Edge detected, functionB automaticly excuted

	MoterReady(); //Motor in edge for precise control

	printf("Write total number of trials : ");
	int total; // total number of trials 
	scanf("%d", &total);
	while (total > 10) { // number of trial must be less than or equal to 10
		printf("Error : Please write total number of trials below 10 : ");
		scanf("%d", &total);
	}
	printf("Total number of trials : %d\n", total);

	int target[total]; // array for target position.

	for (int i = 0; i < total; ++i) {
		printf("Write %d th target location : ", i + 1);
		scanf("%d", &target[i]); // indexing ith element in array and allocating the ith target position to it
	}

	int count = 0; // to end the loop when all trials done
	float currentPosition = 0; //help variable for reference position
	int pulse = 0; // the pulse injecting time is longer than we thought

	while (!pulse) {
		pulse = digitalRead(PULSEINPUT); // Wait for starting
	}

	while (count < total) 
	{
		unsigned int checkTimeBefore = millis();
		unsigned int checkTimeBefore4last = 0;

		printf("%d trial ----------------------------------------\n", count + 1);
		referencePosition = target[count] - currentPosition;
		redGearPosition = 0; // regress redGearPosition to referencePosition
		encoderPosition=0;
		errorPosition = referencePosition;

		float errorSum = 0;
		float previousError = errorPosition;

		pulse = 0;

		int pulseOn = 1;

		//for Program to end after 5secs in last trial
		if (count == total - 1) {
			checkTimeBefore4last = millis();
		}

		// control the motor until next pulse is injected
		while (1)
		{
			pulse = digitalRead(PULSEINPUT);

			if ( !pulseOn && pulse ) {
				break;
			}

			if (pulse) pulseOn = 1;
			else if (!pulse) pulseOn = 0;

			unsigned int checkTime = millis();

			if (checkTime - checkTimeBefore >= SAMPLING_PERIOD) {


				float deltaTime = (float)SAMPLING_PERIOD / 1000;

				errorSum += deltaTime * errorPosition;

				float differentialError = (errorPosition - previousError) / deltaTime;

				float controlledPWM = PGAIN * errorPosition + IGAIN * errorSum + DGAIN * differentialError;

				if (errorPosition > 0)
				{
					softPwmWrite(MOTOR1, controlledPWM);
					softPwmWrite(MOTOR2, 0);
				}
				else
				{
					softPwmWrite(MOTOR2, -controlledPWM);
					softPwmWrite(MOTOR1, 0);
				}

				if (count == total - 1) {
					unsigned int checkTime4last = millis();
					if (checkTime4last - checkTimeBefore4last >= 5000) {
						break;
					}
				}

				previousError = errorPosition;
				checkTimeBefore = checkTime;
			}
		}
		currentPosition = target[count]; // 
		count++; //for while loop escape condition. End if all trials done
	}


	return 0;
}


