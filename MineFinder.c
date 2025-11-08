#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include<stdlib.h>

void mine_print(int n, int** mine);
void mine_find(int n, int temp, int** mine);

int main() {
	int num = 0, temp_i = 0, temp_j = 0;
	printf("N * N 크기의 지뢰찾기를 할지 입력 해 주세요 : ");
	scanf("%d", &num);

	int** mine = malloc(num * sizeof(int*));
	for (int i = 0; i < num; i++) {
		mine[i] = malloc(num * sizeof(int));
	}

	printf("\n=== 지뢰찾기 보드 (%d×%d) ===\n", num, num);
	
	mine_print(num, mine);

	while (1) {
		printf("N * N 위치를 확인 하실 건가요?");
		scanf("%d %d", &temp_i, &temp_j);
	}

	for (int i = 0; i < num; i++) {
		free(mine[i]);
	}
	free(mine);

	return 0;
}

void mine_print(int n, int** mine) {
	srand((unsigned)time(NULL));
	for (int i = 0; i < n; i++) {     
		for (int j = 0; j < n; j++) { 
			mine[i][j] = rand() % 2;
			printf("ㅁ ");
		}
		printf("\n");
	}
	printf("\n");
}

void mine_find(int n, int temp, int** mine) {
	if (mine[n][temp] == 1) {
		printf("지뢰입니다.\n 게임종료\n");
		return 0;
	}
}