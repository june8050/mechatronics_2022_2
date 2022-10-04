#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void add_to_hand(int* owner, int card, int count) {
	if (count >= 10) {
		count %= 10;
	}
	owner[count] = card;
}

int summation(int* player) {
	int sum = 0;
	for (int i = 0; i < 10; i++) {
		sum += player[i];
	}
	return sum;
}

void main() {
	srand((unsigned int)time(NULL));
	int num_of_card_pack;
	int count = 0;

	int A[10] = { 0, };
	int B[10] = { 0, };
	int C[10] = { 0, };
	int D[10] = { 0, };
	int E[10] = { 0, };

	printf("Write numer of card pack : ");
	scanf_s("%d", &num_of_card_pack);

	for (int i = 0; i < num_of_card_pack; i++) {
		
		int card_split[5] = { 0, };
		card_split[0] = rand() % 5 + 1;
		for (int i = 1; i < 5; i++) {
			while (card_split[i] == 0) {
				int temp = rand() % 5 + 1;
				for (int j = 0; j != i; j++) {
					if (card_split[j] == temp) break;
					if (j == i - 1) card_split[i] = temp;
				}

			}
		}

		add_to_hand(A, card_split[0] + count * 5, count);
		add_to_hand(B, card_split[1] + count * 5, count);
		add_to_hand(C, card_split[2] + count * 5, count);
		add_to_hand(D, card_split[3] + count * 5, count);
		add_to_hand(E, card_split[4] + count * 5, count);
		count++;
	}

	printf("\n");

	int sum[5] = { summation(A),summation(B),summation(C),summation(D),summation(E) };

	printf("Summation of A is %d\n", sum[0]);
	printf("Summation of B is %d\n", sum[1]);
	printf("Summation of C is %d\n", sum[2]);
	printf("Summation of D is %d\n", sum[3]);
	printf("Summation of E is %d\n", sum[4]);
	printf("\n");

	int max = sum[0], index = 0;
	for (int i = 1; i < 5; i++) {
		if (sum[i] > max){
			max = sum[i];
			index = i;
		}
	}
	for (int i = 0; i < 5; i++) {
		if ((i != index) && (max == sum[i])) {
			index = 10;
		}
	}

	if (index == 10) {
		printf("Draw!");
	}
	else {
		printf("Winner is %c", index + 65);
	}

}