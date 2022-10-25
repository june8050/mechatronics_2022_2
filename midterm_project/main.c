#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>

//define requried things
#define ENCODERA 17 //Hall Sensor A
#define ENCODERB 27  //Hall Sensor B
#define ENC2REDGEAR 216


#define MOTOR1 19 //GPIO19
#define MOTOR2 26 //GPIO26

#define PGAIN 1000
#define IGAIN 0.2
#define DGAIN 100
#define PULSEINPUT 18 //pulse

#define SAMPLING_PERIOD 1 //millisecond

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
		}
		else
		{
			encoderPosition--;
		}
	}
	else
	{
		if (encB == LOW)
		{
			encoderPosition--;
		}
		else
		{
			encoderPosition++;
		}
	}
	redGearPosition = (float)encoderPosition / ENC2REDGEAR;
	errorPosition = referencePosition - redGearPosition;
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
	redGearPosition = (float)encoderPosition / ENC2REDGEAR;
	errorPosition = referencePosition - redGearPosition;
}

//To make the control process more precise, 
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
	int pulse = 0;

	while (!pulse) {
		pulse = digitalRead(PULSEINPUT);
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

		//마지막 시행에서 5초 뒤 프로그램 종료하기 위함
		if (count == total - 1) {
			checkTimeBefore4last = millis();
		}

		//각 시행에 대해 새로운 펄스 들어올때 까지 제어 실행
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
		currentPosition = target[count];
		count++;
	}

	fclose(fp);

	return 0;
}


