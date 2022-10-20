#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

//define requried things
#define LOOPTIME 5 //sampling time
#define ENCODERA 17 //Hall Sensor A
#define ENCODERB 27  //Hall Sensor B
#define ENC2REDGEAR 216


#define MOTOR1 19 //GPIO19
#define MOTOR2 26 //GPIO26

#define PGAIN 100
#define IGAIN 0
#define DGAIN 0

#define PULSEINPUT 18 //pulse

// initialize
int encA;
int encB;
int encoderPosition = 0;
float redGearPosition = 0;


float referencePosition = 0;
float errorPosition = 0;

unsigned int checkTime;
unsigned int checkTimeBefore;


void funcEncoderA() //encoder position
{
	encA = digitalRead(ENCODERA);
	encB = digitalRead(ENCODERB);
	if (encA == HIGH)
	{
		if (encB == LOW)
		{
			encoderPosition++;
			printf("CW");
		}
		else
		{
			encoderPosition--;
			printf("CCW");
		}
	}
	else
	{
		if (encB == LOW)
		{
			encoderPosition--;
			printf("CCW");
		}
		else
		{
			encoderPosition++;
			printf("CW");
		}
	}
	redGearPosition = (float)encoderPosition / ENC2REDGEAR;
	errorPosition = referencePosition - redGearPosition;
	printf("encoderPosition : %d / redGearPosition : %f / errorPosition : %f\n", encoderPosition, redGearPosition, errorPosition);
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
			printf("CCW");
		}
		else
		{
			encoderPosition++;
			printf("CW");
		}
	}
	else
	{
		if (encA == LOW)
		{
			encoderPosition++;
			printf("CW");
		}
		else
		{
			encoderPosition--;
			printf("CCW");
		}
	}
	redGearPosition = (float)encoderPosition / ENC2REDGEAR;
	errorPosition = referencePosition - redGearPosition;
	printf("encoderPosition : %d / redGearPosition : %f / errorPosition : %f\n", encoderPosition, redGearPosition, errorPosition);
}


void MoterReady()
{

	while (1) {
		softPwmWrite(MOTOR1, 50);
		softPwmWrite(MOTOR2, 0);
		if (encA == HIGH || encB == LOW || encA == HIGH || encB == LOW) {
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

	wiringPiISR(ENCODERA, INT_EDGE_BOTH, funcEncoderA);
	wiringPiISR(ENCODERB, INT_EDGE_BOTH, funcEncoderB);

	MoterReady();


	printf("Write total number of trials : ");
	int total;
	scanf("%d", &total);
	while (total > 10) {
		printf("Error : Please write total number of trials below 10 : ");
		scanf("%d", &total);
	}
	printf("Total number of trials : %d\n", total);
	int target[total];
	for (int i = 0; i < total; ++i) {
		printf("Write %d th target location : ", i + 1);
		scanf("%d", &target[i]);
	}
	int count = 0;
	float currentPosition = 0;

	while (count < total)
	{
		referencePosition = target[count] - currentPosition;
		redGearPosition = currentPosition;
		errorPosition = referencePosition - redGearPosition;
		printf("%f\n", errorPosition);

		float errorSum = 0;
		float previousError = errorPosition;

		unsigned int checkTimeBefore = millis();

		while (errorPosition > 0.1 || errorPosition < -0.1)
		{
			//printf("%f\n",errorPosition);

			unsigned int checkTime = millis();
			float deltaTime = (float) (checkTime - checkTimeBefore)/1000;

			errorSum += deltaTime * errorPosition;

			float derivativeError = (errorPosition - previousError) / deltaTime;

			float controlledPWM = PGAIN * errorPosition + IGAIN * errorSum + DGAIN * derivativeError;

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

			previousError = errorPosition;
			checkTimeBefore = checkTime;

		}
		currentPosition = target[count];
		count++;
	}


	return 0;
}
