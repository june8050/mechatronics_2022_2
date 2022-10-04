#include <stdio.h>

void main() {

	int input;
	int *ptr=&input;
	int** ptrptr = &ptr;

	printf("정수를 입력하세요: ");
	scanf_s("%d",&input);

	printf("\n");
	printf("정수의 값 : %d\n", input);
	printf("정수의 주소값 : %p\n", &input);
	printf("정수를 가리키는 포인터의 값 : %p\n",ptr);
	printf("정수를 가리키는 포인터의 주소 : %p\n", ptrptr);
	printf("정수를 가리키는 포인터가 가리키는 변수의 값 : %d\n", *ptr);

	printf("\n");
	printf("정수에 1을 더해봅니다.\n");
	input++;
	printf("정수의 값 : %d\n", input);
	printf("정수의 주소값 : %p\n", &input);
	printf("정수를 가리키는 포인터의 값 : %p\n", ptr);
	printf("정수를 가리키는 포인터의 주소 : %p\n", ptrptr);
	printf("정수를 가리키는 포인터가 가리키는 변수의 값 : %d\n", *ptr);

	printf("\n");
	printf("포인터가 가리키는 주소에 1을 더해봅니다.\n");
	ptr++;
	printf("정수의 값 : %d\n", input);
	printf("정수의 주소값 : %p\n", &input);
	printf("정수를 가리키는 포인터의 값 : %p\n", ptr);
	printf("정수를 가리키는 포인터의 주소 : %p\n", ptrptr);
	printf("정수를 가리키는 포인터가 가리키는 변수의 값 : %d\n", *ptr);
}