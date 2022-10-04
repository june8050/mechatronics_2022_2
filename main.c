#include <stdio.h> //Standard I/O 헤더
#include <wiringPi.h> //wiringPi 헤더
#include <stdlib.h> //난수 생성을 위한 헤더
#include <time.h> //매번 다른 난수를 생성하기 위함과 0.5초, 1초 시간간격을 측정하기위함

//OUTPUT pin numbers
#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 20

//INPUT pin numbers
#define BTN_RED 21
#define BTN_GREEN 22
#define BTN_BLUE 23

//반복되는 게임의 과정을 함수로 구현
int game(int* led_index, int* score, int* missCount);

void main() {
	//매번 다른 난수를 생성하기 위함
	srand((unsigned int)time(NULL));

	//Setup & Initialize GPIO
	wiringPiSetupGpio();
	pinMode(LED_RED,OUTPUT);
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	pinMode(BTN_RED, INPUT);
	pinMode(BTN_GREEN, INPUT);
	pinMode(BTN_BLUE, INPUT);

	int score = 0;
	int miss_count = 0;

	//편의상 red를 1번, green을 2번, blue를 3번 led로 부르겠다
	//연속으로 동일한 LED가 점등되지 않도록 하기위해
	//이전에 어떤 LED에 점등됐었는지 저장
	//0은 점등되지 않은 LED, 1은 점등된 LED
	int led_index[3] = { 0, };


	//첫 시행만 1~3까지의 난수를 생성하므로
	//반복문 밖에서 먼저 처리해줌
	led_index[rand() % 3] = 1;
	//game 함수에 생성된 led_index 전달
	int first_round = game(led_index, &score, &miss_count);

	if (first_round) {
		printf("Score: %d", score);
	}
	else {
		printf("MissCount: %d", miss_count);
	}
	//게임이 종료될 때 까지 반복
	while (1) {
		//두번째 시행부터는 1~2까지의 난수를 생성
		int random = rand() % 2; //0또는 1

		//이전 led_index에서 1을 0으로 바꿔주고 난수 값에 따라 이전에 점등되지 않은 두개의 LED중 하나의 값을 1로 변경
		if (led_index[0] == 1) {
			led_index[0] = 0;
			led_index[random + 1] = 1;
		}
		else if (led_index[1] == 1) {
			led_index[1] = 0;
			if (random == 0) {
				led_index[0] = 1;
			}
			else if (random == 1) {
				led_index[2] = 1;
			}
		}
		else if (led_index[2] == 1) {
			led_index[2] = 0;
			led_index[random] = 1;
		}

		//game 함수에 생성된 led_index 전달
		int result = game(led_index, &score, &miss_count);

		if (result==1) {
			printf("GOOD   ");
			if (score == 20) {
				printf("GAME WIN");
				break;
			}
			else {
				printf("Score: %d", score);
			}
		}
		else if (result == 0) {
			printf("MISS   ");
			if (miss_count == 5) {
				printf("GAME OVER");
				break;
			}
			else {
				printf("MissCount: %d", miss_count);
			}
		}
	}
}

int game(int* led_index, int* score, int* miss_count) {
	int random_led;
	int target_switch;
	int pressed_switch=0;

	if (led_index[0] == 1) {
		random_led = LED_RED;
		target_switch = BTN_RED;
		printf("LED1 blink     ");
	}
	else if (led_index[1] == 1) {
		random_led = LED_GREEN;
		target_switch = BTN_GREEN;
		printf("LED2 blink     ");
	}
	else if (led_index[2] == 1) {
		random_led = LED_BLUE;
		target_switch = BTN_BLUE;
		printf("LED3 blink     ");
	}


	//LED 점등
	digitalWrite(random_led, HIGH);

	clock_t half_second_passed = clock() + 0.5 * CLOCKS_PER_SEC;
	clock_t one_second_passed = clock() + CLOCKS_PER_SEC;
	//0.5초 안에 해당하는 버튼을 누르면 성공으로 판단
	while (clock()<half_second_passed) {
		int BTN_RED_ON = digitalRead(BTN_RED);
		int BTN_GREEN_ON = digitalRead(BTN_GREEN);
		int BTN_BLUE_ON = digitalRead(BTN_BLUE);

		//어떤 스위치가 한번 눌리면 while문 break
		if (BTN_RED_ON || BTN_GREEN_ON || BTN_BLUE_ON) {
			if (BTN_RED_ON) {
				pressed_switch = BTN_RED;
				printf("BUTTON1 pushed  ");
			}
			else if (BTN_GREEN_ON) {
				pressed_switch = BTN_GREEN;
				printf("BUTTON2 pushed  ");
			}
			else if (BTN_BLUE_ON) {
				pressed_switch = BTN_BLUE;
				printf("BUTTON3 pushed  ");
			}
			break;
		}
	}

	//점등간격 1초가 지날때까지 대기
	while (clock() < one_second_passed) {
	}

	//성공했으면 1, 실패했으면 0을 반환
	if (pressed_switch == 0) { //버튼을 누르지 않거나 0.5초 이후에 버튼이 눌린경우
		*miss_count++;
		printf("No BUTTON pushed ");
		return 0;
	}
	else if (pressed_switch == target_switch) { //정답 버튼 누른경우
		*score++;
		return 1;
	}
	else { //오답 버튼 누른경우
		*miss_count;
		return 0;
	}
}