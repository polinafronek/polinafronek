#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25
#define FPS 60
#define MAX_HISTORY_SIZE 10000

void draw_field(char **field);
int init_field(char ***field);
void read_field(char ***filed, const char *input);
void game_play(char **field, char **next_field, unsigned long *history, int speed, int epochs);
void free_field(char **field);
void copy_field(char **field, char **next_field);
int counting_living(char **field, int y, int x);
int found_in_history(int *history, unsigned long hash, int num_hashes);
// int is_find_in_history(unsigned long *history, unsigned long hash, int
// num_hashes);
unsigned long get_hash_field(char **field);
void my_input(char ***field);
void delay(int speed);
void game_over();

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        printf("В параметрах необходимо передать имя файла или \"myInput\"");
    } else {
        const char *input = argv[1];
        printf("Скорость в процентах и количество эпох:\n");

        int speed, epochs;
        if (scanf("%d %d", &speed, &epochs) != 2 || speed < 1 || epochs <= 0) {
            printf("Ошибка ввода! Пожалуйста, введите два целых числа.\n");
        } else {
            char **field, **next_field;
            if (!(init_field(&field) && init_field(&next_field))) {
                printf("Что-то не так с памятью :(");

            } else {
                unsigned long history[MAX_HISTORY_SIZE];
                read_field(&field, input);
                game_play(field, next_field, history, speed, epochs);

                free_field(next_field);
                free_field(field);
            }
        }
    }
    return 0;
}

void draw_field(char **field) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            printf("%c", field[i][j]);
        }
        printf("\n");
    }
}

void free_field(char **field) {
    for (int i = 0; i < HEIGHT; ++i) {
        free(field[i]);
    }
    free(field);
}

int init_field(char ***field) {
    int flag = 1;
    *field = (char **)malloc(HEIGHT * sizeof(char *));
    if (*field == NULL) {
        flag = 0;
    } else {
        for (int i = 0; i < HEIGHT; ++i) {
            (*field)[i] = (char *)malloc(WIDTH * sizeof(char));
            if ((*field)[i] == NULL) {
                for (int j = 0; j < i; ++j) free((*field)[j]);
                free(*field);
                flag = 0;
            }
        }
    }
    return flag;
}

void read_field(char ***field, const char *input) {
    if (freopen(input, "r", stdin) != NULL) {
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                scanf(" %c", &(*field)[i][j]);
            }
        }
        fclose(stdin);
    } else if (freopen("/dev/tty", "r", stdin) != NULL) {
        my_input(field);
    }
}

void my_input(char ***field) {
    int count, x, y;
    printf("Сколько точек хочешь разместить?\n");
    if (scanf("%d", &count) == 1 && count >= 0 && count <= HEIGHT * WIDTH) {
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                (*field)[i][j] = '.';
            }
        }
        for (int i = 0; i < count; i++) {
            printf("Введи x и y точки:\n");
            if (scanf("%d%d", &x, &y) == 2) {
                (*field)[y - 1][x - 1] = 'x';
            }
        }
    } else {
        my_input(field);
    }
}

unsigned long get_hash_field(char **field) {
    unsigned long hash = 0;
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            hash += field[y][x] * (y * WIDTH + x);
        }
    }
    return hash;
}

void copy_field(char **field, char **next_field) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            field[i][j] = next_field[i][j];
        }
    }
}

int counting_living(char **field, int y, int x) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            int new_y = (y + HEIGHT + i) % HEIGHT;
            int new_x = (x + WIDTH + j) % WIDTH;
            if (field[new_y][new_x] == 'x') count++;
        }
    }
    return count;
}

// int is_find_in_history(unsigned long *history, unsigned long hash, int
// num_hashes) {
//     for (int i = 0; i < num_hashes; i++) {
//         if (history[i] == hash) {
//             return 1;
//         }
//     }
//     return 0;
// }

void delay(int speed) {
    for (int i = 0; i < 100000000 * speed / 100; ++i) {
    }
}

void game_play(char **field, char **next_field, unsigned long *history, int speed, int epochs) {
    int num_hashes = 0, next_index = 0;
    int epoch = 0;
    while (epoch < epochs) {
        system("clear");
        draw_field(field);
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                int count_living = counting_living(field, i, j);
                if (count_living < 2 || count_living > 3) {
                    next_field[i][j] = '.';
                } else if (count_living == 3) {
                    next_field[i][j] = 'x';
                } else if (field[i][j] == 'x') {
                    next_field[i][j] = 'x';
                } else {
                    next_field[i][j] = '.';
                }
            }
        }
        copy_field(field, next_field);
        unsigned long hash = get_hash_field(field);
        // if (is_find_in_history(history, hash, num_hashes)) break;
        history[next_index] = hash;
        next_index = (next_index + 1) % MAX_HISTORY_SIZE;
        if (num_hashes < MAX_HISTORY_SIZE) {
            num_hashes++;
        }
        epoch++;
        delay(speed);
    }
    system("clear");
    game_over();
}

void game_over() {
    printf("*********************************************************\n");
    printf("*                                                       *\n");
    printf("*                                                       *\n");
    printf("*                                                       *\n");
    printf("*                     GAME OVER!                        *\n");
    printf("*                     GAME OVER!                        *\n");
    printf("*                     GAME OVER!                        *\n");
    printf("*                                                       *\n");
    printf("*                                                       *\n");
    printf("*                                                       *\n");
    printf("*********************************************************\n");
}
