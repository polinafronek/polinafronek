#include <stdio.h>

void draw_field(int ball_x, int ball_y, int racket_1_y, int racket_2_y, int score_1, int score_2) {
    printf("SCORE: %d | %d\n", score_1, score_2);
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            if (x == ball_x && y == ball_y) {
                printf("@");

            } else if (((x == 1 && y == racket_1_y) || (x == 1 && y == racket_1_y - 1) ||
                        (x == 1 && y == racket_1_y + 1)) ||
                       ((x == 78 && y == racket_2_y) || (x == 78 && y == racket_2_y - 1) ||
                        (x == 78 && y == racket_2_y + 1))) {
                printf("#");
            } else if ((y == 0) || (y == 24)) {
                printf("#");
            } else if ((x == 0) || (x == 79) || (x == 39)) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}
int change_ball_dir_x(int ball_x, int ball_y, int racket_1_y, int racket_2_y) {
    int flag = 1;
    if (ball_x == 2 || ball_x == 77) {
        if (ball_y == racket_1_y || ball_y == racket_1_y - 1 || ball_y == racket_1_y + 1) {
            flag = -1;
        }
        if (ball_y == racket_2_y || ball_y == racket_2_y - 1 || ball_y == racket_2_y + 1) {
            flag = -1;
        }
    }
    return flag;
}

void game_init() {
    int score_1 = 0;
    int score_2 = 0;
    int exit;
    int ball_y, ball_x, racket_2_y, racket_1_y, ball_dir_y = 1, ball_dir_x = 1, end_score = 21;
    int reset_cord = 0;
    while (score_1 < end_score && score_2 < end_score && exit != 1) {
        if (reset_cord == 0) {
            ball_y = 12;
            ball_x = 40;
            racket_1_y = 12;
            racket_2_y = 12;
            ball_dir_y = -ball_dir_y;
            reset_cord = 1;
        }
        printf("\033[0d\033[2J");
        draw_field(ball_x, ball_y, racket_1_y, racket_2_y, score_1, score_2);
        ball_dir_x *= change_ball_dir_x(ball_x, ball_y, racket_1_y, racket_2_y);
        if (ball_y == 1 || ball_y == 23) ball_dir_y = -ball_dir_y;
        if (ball_x == 1) {
            score_2++;
            reset_cord = 0;
        }
        if (ball_x == 78) {
            reset_cord = 0;
            score_1++;
        }
        ball_x += ball_dir_x;
        ball_y += ball_dir_y;

        char key;
        while ((key = getchar()) != '\n') {
            if (key == 'a' && racket_1_y - 2 > 0) racket_1_y -= 1;
            if (key == 'z' && racket_1_y + 2 < 24) racket_1_y += 1;
            if (key == 'k' && racket_2_y - 2 > 0) racket_2_y -= 1;
            if (key == 'm' && racket_2_y + 2 < 24) racket_2_y += 1;
            if (key == 'q') {
                printf("\033[0d\033[2J");
                printf("GAME OVER\nFinal score : %d | %d\n", score_1, score_2);
                exit = 1;
            }
        }
    }
    if (score_1 == end_score) {
        printf("CONGRATULATIONS PLAYER 1 WINS!!!!!\nFinal score : %d | %d\n", score_1, score_2);
    }
    if (score_2 == end_score) {
        printf("CONGRATULATIONS PLAYER 2 WINS!!!!!\nFinal score : %d | %d\n", score_1, score_2);
    }
    exit = 1;
}

int main() {
    game_init();

    return 0;
}
