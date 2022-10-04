#include <stdio.h> //Standard I/O ���
#include <wiringPi.h> //wiringPi ���
#include <stdlib.h> //���� ������ ���� ���
#include <time.h> //�Ź� �ٸ� ������ �����ϱ� ���԰� 0.5��, 1�� �ð������� �����ϱ�����

//OUTPUT pin numbers
#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 20

//INPUT pin numbers
#define BTN_RED 21
#define BTN_GREEN 22
#define BTN_BLUE 23

//�ݺ��Ǵ� ������ ������ �Լ��� ����
int game(int* led_index, int* score, int* missCount);

void main() {
	//�Ź� �ٸ� ������ �����ϱ� ����
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

	//���ǻ� red�� 1��, green�� 2��, blue�� 3�� led�� �θ��ڴ�
	//�������� ������ LED�� ������� �ʵ��� �ϱ�����
	//������ � LED�� ����ƾ����� ����
	//0�� ������� ���� LED, 1�� ����� LED
	int led_index[3] = { 0, };


	//ù ���ุ 1~3������ ������ �����ϹǷ�
	//�ݺ��� �ۿ��� ���� ó������
	led_index[rand() % 3] = 1;
	//game �Լ��� ������ led_index ����
	int first_round = game(led_index, &score, &miss_count);

	if (first_round) {
		printf("Score: %d", score);
	}
	else {
		printf("MissCount: %d", miss_count);
	}
	//������ ����� �� ���� �ݺ�
	while (1) {
		//�ι�° ������ʹ� 1~2������ ������ ����
		int random = rand() % 2; //0�Ǵ� 1

		//���� led_index���� 1�� 0���� �ٲ��ְ� ���� ���� ���� ������ ������� ���� �ΰ��� LED�� �ϳ��� ���� 1�� ����
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

		//game �Լ��� ������ led_index ����
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


	//LED ����
	digitalWrite(random_led, HIGH);

	clock_t half_second_passed = clock() + 0.5 * CLOCKS_PER_SEC;
	clock_t one_second_passed = clock() + CLOCKS_PER_SEC;
	//0.5�� �ȿ� �ش��ϴ� ��ư�� ������ �������� �Ǵ�
	while (clock()<half_second_passed) {
		int BTN_RED_ON = digitalRead(BTN_RED);
		int BTN_GREEN_ON = digitalRead(BTN_GREEN);
		int BTN_BLUE_ON = digitalRead(BTN_BLUE);

		//� ����ġ�� �ѹ� ������ while�� break
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

	//����� 1�ʰ� ���������� ���
	while (clock() < one_second_passed) {
	}

	//���������� 1, ���������� 0�� ��ȯ
	if (pressed_switch == 0) { //��ư�� ������ �ʰų� 0.5�� ���Ŀ� ��ư�� �������
		*miss_count++;
		printf("No BUTTON pushed ");
		return 0;
	}
	else if (pressed_switch == target_switch) { //���� ��ư �������
		*score++;
		return 1;
	}
	else { //���� ��ư �������
		*miss_count;
		return 0;
	}
}