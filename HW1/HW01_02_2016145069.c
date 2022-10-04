#include <stdio.h>

int plus(int a, int b) { return a + b; }

int minus(int a, int b) { return a - b; }

int multiply(int a, int b) { return a * b; }

int divide(int a, int b) { return a / b; }

/*void main() {
	int sum = 0;

	while (sum <= 100) {
		int a, b;
		int result = 0;
		int arithmetic;
		printf("Write First Num and Second Num: ");
		scanf_s("%d%d", &a, &b);
		printf("\n");
		printf("Operator List - [1] Plus / [2] Minus / [3] Multiply / [4] Divide\n");
		printf("Write Operator Num : ");
		scanf_s("%d", &arithmetic);

		switch (arithmetic) {
		case 1:
			result = plus(a, b);
			break;
		case 2:
			result = minus(a, b);
			break;
		case 3:
			result = multiply(a, b);
			break;
		case 4:
			result = divide(a, b);
			break;
		default:
			printf("Error : Wrong Input!");
		}
		
		sum += result;
		printf("\n");
		printf("Answer : %d / Sum of Answer : %d\n", result, sum);
		printf("\n");
	}
	printf("Sum is larger than 100 - Break!");
}
*/