#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define SIZE 8

#define PAREDE    0  
#define SUJEIRA   1  
#define CAMINHO   2  
#define ESTACAO   3  
#define ROBO      4  

// parede, sujeira, caminho limpo, estação, robo
char states[5][6] = {"|---|", "|***|", "|   |", "| E |", "| @ |"};

double distance(int y1, int x1, int y2, int x2) {
    return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

void print_table(int table[SIZE][SIZE]) {
    // fix: limpa ambos, win,
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            printf("%s", states[ table[y][x] ]);
        }
        printf("\n");
    }
    printf("\n");
}

void ask_point(char* prompt, int point[2]) {
    printf("%s\n", prompt);
    printf("Linha: ");
    scanf("%d", &point[0]);
    printf("Coluna: ");
    scanf("%d", &point[1]);
}

// fix: so termina qnd não tiver
int dirt_exists(int table[SIZE][SIZE]) {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (table[y][x] == SUJEIRA)
                return 1;
        }
    }
    return 0;
}

// pegar todas as sujeiras e ver qual esta mais proxima
void get_closest(int table[SIZE][SIZE], int player[2], int closest[2]) {
    double minDist = -1;
    closest[0] = -1;
    closest[1] = -1;
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (table[y][x] == SUJEIRA) {
                double d = distance(player[0], player[1], y, x);
                if (minDist < 0 || d < minDist) {
                    minDist = d;
                    closest[0] = y;
                    closest[1] = x;
                }
            }
        }
    }
}

void move_towards(int table[SIZE][SIZE], int player[2], int target[2]) {
    // cima, direita, baixo, esquerda
    int dy[4] = {-1, 0, 1, 0};
    int dx[4] = {0, 1, 0, -1};
    double bestDist = -1;
    int nextY = player[0], nextX = player[1];

    for (int i = 0; i < 4; i++) {
        int ny = player[0] + dy[i];
        int nx = player[1] + dx[i];

        if (ny < 0 || ny >= SIZE || nx < 0 || nx >= SIZE)
            continue;

            if (table[ny][nx] != SUJEIRA && table[ny][nx] != CAMINHO && table[ny][nx] != ESTACAO)
            continue;
        // usa distancia euclidiana para voltar 
        double d = distance(ny, nx, target[0], target[1]);
        if (bestDist < 0 || d < bestDist) {
            bestDist = d;
            nextY = ny;
            nextX = nx;
        }
    }

    if (table[player[0]][player[1]] != ESTACAO)
        table[player[0]][player[1]] = CAMINHO;

    player[0] = nextY;
    player[1] = nextX;
    table[player[0]][player[1]] = ROBO;
}

int main() {
    int table[SIZE][SIZE];
    int i, j;
    
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            table[i][j] = PAREDE;
        }
    }
    
    int station[2];
    print_table(table);
    ask_point("Informe a posição da estação (E):", station);
    if (station[0] < 0 || station[0] >= SIZE || station[1] < 0 || station[1] >= SIZE) {
        printf("Posição inválida!\n");
        exit(1);
    }
    table[station[0]][station[1]] = ESTACAO;
    
    int dirt[2];
    while (1) {
        print_table(table);
        ask_point("Informe a posição da sujeira (digite -1 -1 para finalizar):", dirt);
        if (dirt[0] == -1 && dirt[1] == -1)
            break;
        if (dirt[0] < 0 || dirt[0] >= SIZE || dirt[1] < 0 || dirt[1] >= SIZE) {
            printf("Posição inválida!\n");
            continue;
        }
        // fix: não pode botar no lugar da estação :)
        if (table[dirt[0]][dirt[1]] == PAREDE)
            table[dirt[0]][dirt[1]] = SUJEIRA;
        else
            printf("Célula já ocupada!\n");
    }
    
    int player[2] = {station[0], station[1]};
    table[player[0]][player[1]] = ROBO;
    
    print_table(table);
    sleep(1);
    
    while (dirt_exists(table)) {
        int target[2];
        get_closest(table, player, target);
        if (target[0] == -1)
            break;
        while (!(player[0] == target[0] && player[1] == target[1])) {
            move_towards(table, player, target);
            print_table(table);
            sleep(1);
        }
    }
    
    while (!(player[0] == station[0] && player[1] == station[1])) {
        move_towards(table, player, station);
        print_table(table);
        sleep(1);
    }
    
    print_table(table);
    printf("Jogo finalizado!\n");
    return 0;
}
