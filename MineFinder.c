#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void mine_start(int n, int** mine, char** mine_temp);
void init_mine(int n, int** mine, char** mine_temp);
void print_board(int n, char** mine_temp);
int  mine_check(int num, int** mine, char** mine_temp);
void open_cell(int n, int row, int col, int** mine, char** mine_temp);
int  count_adjacent_mines(int n, int row, int col, int** mine);
int game_end_check(int num, int** mine, char** mine_temp);

int main() {
    int num = 0, state = 0;

    printf("N * N 크기의 지뢰찾기를 할지 입력 해 주세요 : ");
    scanf("%d", &num);

    // 동적 배열 할당
    int** mine = malloc(num * sizeof(int*));
    char** mine_temp = malloc(num * sizeof(char*));
    for (int i = 0; i < num; i++) {
        mine[i] = malloc(num * sizeof(int));
        mine_temp[i] = malloc(num * sizeof(char));
    }

    // 지뢰 초기화
    init_mine(num, mine, mine_temp);

    printf("\n=== 지뢰찾기 보드 (%d×%d) ===\n", num, num);
    print_board(num, mine_temp);

    // 안내
    mine_start(num, mine, mine_temp);

    // 사용자 입력 루프
    // 사용자 입력 루프
    while(1) {
        state = mine_check(num, mine, mine_temp);  // 0: 지뢰, 1: 안전
        print_board(num, mine_temp);

        if (state == 0) {              // 지뢰 밟음 → 게임 오버
            printf("게임이 종료되었습니다.\n");
            break;
        }

        if (game_end_check(num, mine, mine_temp)) {   // 1이면 승리
            printf("축하합니다! 모든 안전한 칸을 열었습니다!\n");
            break;
        }

    } 


    // 메모리 해제
    for (int i = 0; i < num; i++) {
        free(mine[i]);
        free(mine_temp[i]);
    }
    free(mine);
    free(mine_temp);

    return 0;
}

void init_mine(int n, int** mine, char** mine_temp) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // 1/5 확률로 지뢰(1), 아니면 0
            mine[i][j] = (rand() % 5 == 1) ? 1 : 0;
            mine_temp[i][j] = '#';     // 가려진 칸
        }
    }
}

void print_board(int n, char** mine_temp) {
    // 열 번호 출력
    printf("   ");                 // 왼쪽 여백 (행 번호 자리)
    for (int j = 0; j < n; j++) {
        printf("%2d ", j);         // 폭 2칸으로 출력
    }
    printf("\n");

    // 보드 + 행 번호 출력
    for (int i = 0; i < n; i++) {
        printf("%2d ", i);         // 행 번호도 2칸
        for (int j = 0; j < n; j++) {
            printf(" %c ", mine_temp[i][j]);  // 가운데도 폭 맞추기
        }
        printf("\n");
    }
}



// 주변 8칸에 지뢰가 몇 개 있는지 세는 함수
int count_adjacent_mines(int n, int row, int col, int** mine) {
    int cnt = 0;
    int dr[8] = { -1,-1,-1, 0, 0, 1, 1, 1 };
    int dc[8] = { -1, 0, 1,-1, 1,-1, 0, 1 };

    for (int k = 0; k < 8; k++) {
        int nr = row + dr[k];
        int nc = col + dc[k];

        if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;

        if (mine[nr][nc] == 1) cnt++;
    }

    return cnt;
}

// 0인 칸 자동 펼치기 + 숫자 표시
void open_cell(int n, int row, int col, int** mine, char** mine_temp) {
    // 범위 체크
    if (row < 0 || row >= n || col < 0 || col >= n) return;

    // 이미 열린 칸이면 다시 처리하지 않음
    if (mine_temp[row][col] != '#') return;

    // 지뢰칸이면 여기서 끝 (mine_check에서만 처리하도록)
    if (mine[row][col] == 1) return;

    int cnt = count_adjacent_mines(n, row, col, mine);
    mine_temp[row][col] = cnt + '0';

    // 주변 지뢰가 하나라도 있으면 여기서 끝 (더 이상 확장 X)
    if (cnt > 0) return;

    // cnt == 0이면 주변 칸들도 계속 열어줌 (재귀)
    int dr[8] = { -1,-1,-1, 0, 0, 1, 1, 1 };
    int dc[8] = { -1, 0, 1,-1, 1,-1, 0, 1 };

    for (int k = 0; k < 8; k++) {
        int nr = row + dr[k];
        int nc = col + dc[k];

        if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
        if (mine[nr][nc] == 1) continue; // 지뢰는 열지 않음

        open_cell(n, nr, nc, mine, mine_temp);
    }
}

int mine_check(int num, int** mine, char** mine_temp) {
    int row, col;
    printf("\n확인할 좌표를 입력하세요 (행 열): ");
    scanf("%d %d", &row, &col);

    if (row < 0 || col < 0 || row >= num || col >= num) {
        printf("좌표가 범위를 벗어났습니다! 다시 입력하세요.\n");
        return 1;   // 게임 계속
    }

    if (mine[row][col] == 1) {
        printf("\n 지뢰입니다! 게임 종료 \n");
        mine_temp[row][col] = '*';
        return 0;   // 게임 종료
    }
    else {
        printf("안전한 칸입니다.\n");
        open_cell(num, row, col, mine, mine_temp);
        return 1;   // 계속 진행
    }
}

void mine_start(int n, int** mine, char** mine_temp) {
    int row = 0, col = 0;

    // 좌표 입력
    while (1) {
        printf("\n확인할 좌표를 입력하세요 (행 열): ");
        scanf("%d %d", &row, &col);

        if (row < 0 || col < 0 || row >= n || col >= n) {
            printf("좌표가 범위를 벗어났습니다! 다시 입력하세요.\n");
        }
        else {
            break; // 올바른 좌표 입력되면 탈출
        }
    }

    // 첫 클릭이 지뢰면 지뢰를 제거해서 첫 클릭은 항상 안전하게
    if (mine[row][col] == 1)
        mine[row][col] = 0;

    // 내가 선택한 칸 주변 지뢰 개수 먼저 계산
    int cnt = count_adjacent_mines(n, row, col, mine);

    // 선택한 칸 열기 (0이면 원래대로 주변까지 자동 확장됨)
    open_cell(n, row, col, mine, mine_temp);

    //선택한 칸만 열렸을 경우(= 주변에 지뢰가 있어서 cnt > 0) → 주변 8칸도 같이 열기
    if (cnt > 0) {
        int dr[8] = { -1,-1,-1, 0, 0, 1, 1, 1 };
        int dc[8] = { -1, 0, 1,-1, 1,-1, 0, 1 };

        for (int k = 0; k < 8; k++) {
            int nr = row + dr[k];
            int nc = col + dc[k];

            // 범위 체크
            if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
            // 지뢰는 열지 않음
            if (mine[nr][nc] == 1) continue;

            // 이웃 칸도 열어준다
            open_cell(n, nr, nc, mine, mine_temp);
        }
    }

    // 첫 클릭 이후 보드 출력
    print_board(n, mine_temp);
}

int game_end_check(int num, int** mine, char** mine_temp) {
    for (int i = 0; i < num; i++) {
        for (int k = 0; k < num; k++) {
            if (mine[i][k] == 0 && mine_temp[i][k] == '#') {
                return 0; 
            }
        }
    }

    return 1; 
}
