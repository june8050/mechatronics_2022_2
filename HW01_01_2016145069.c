#include <stdio.h>

void main() {

	int input;
	int *ptr=&input;
	int** ptrptr = &ptr;

	printf("������ �Է��ϼ���: ");
	scanf_s("%d",&input);

	printf("\n");
	printf("������ �� : %d\n", input);
	printf("������ �ּҰ� : %p\n", &input);
	printf("������ ����Ű�� �������� �� : %p\n",ptr);
	printf("������ ����Ű�� �������� �ּ� : %p\n", ptrptr);
	printf("������ ����Ű�� �����Ͱ� ����Ű�� ������ �� : %d\n", *ptr);

	printf("\n");
	printf("������ 1�� ���غ��ϴ�.\n");
	input++;
	printf("������ �� : %d\n", input);
	printf("������ �ּҰ� : %p\n", &input);
	printf("������ ����Ű�� �������� �� : %p\n", ptr);
	printf("������ ����Ű�� �������� �ּ� : %p\n", ptrptr);
	printf("������ ����Ű�� �����Ͱ� ����Ű�� ������ �� : %d\n", *ptr);

	printf("\n");
	printf("�����Ͱ� ����Ű�� �ּҿ� 1�� ���غ��ϴ�.\n");
	ptr++;
	printf("������ �� : %d\n", input);
	printf("������ �ּҰ� : %p\n", &input);
	printf("������ ����Ű�� �������� �� : %p\n", ptr);
	printf("������ ����Ű�� �������� �ּ� : %p\n", ptrptr);
	printf("������ ����Ű�� �����Ͱ� ����Ű�� ������ �� : %d\n", *ptr);
}