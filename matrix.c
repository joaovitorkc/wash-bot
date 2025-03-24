#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>

#define SIZE 8
#define BUFFER_SIZE 100

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

bool read_int(const char* prompt, int *number) {
    char buffer[BUFFER_SIZE];
    char *endptr;
    printf("%s", prompt);
    if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        return false;
    buffer[strcspn(buffer, "\n")] = '\0';
    *number = (int)strtol(buffer, &endptr, 10);
    if (*endptr != '\0' || buffer[0] == '\0') {
        return false;
    }
    return true;
}

void ask_point(char* prompt, int point[2]) {
    bool valid = false;
    while (!valid) {
        printf("%s\n", prompt);
        if (!read_int("Linha: ", &point[0])) {
            printf("Entrada inválida para a linha! Digite um número.\n");
            continue;
        }
        if (!read_int("Coluna: ", &point[1])) {
            printf("Entrada inválida para a coluna! Digite um número.\n");
            continue;
        }
        valid = true;
    }
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

// fix: verifica as sujeira que podem ser limpadas
bool cell_allowed(int cell) {
    return (cell == SUJEIRA || cell == CAMINHO || cell == ESTACAO || cell == ROBO);
}

// fix: usa FBS (Breadth-First Search) para isso
bool is_reachable(int table[SIZE][SIZE], int start[2], int target[2]) {
    bool visited[SIZE][SIZE] = {false};
    
    typedef struct { int y, x; } Coord;
    Coord queue[SIZE * SIZE];
    int front = 0, back = 0;
    
    queue[back++] = (Coord){start[0], start[1]};
    visited[start[0]][start[1]] = true;
    
    int dy[4] = {-1, 0, 1, 0};
    int dx[4] = {0, 1, 0, -1};
    
    while (front < back) {
        Coord cur = queue[front++];
        if (cur.y == target[0] && cur.x == target[1])
            return true;
        
        for (int i = 0; i < 4; i++) {
            int ny = cur.y + dy[i], nx = cur.x + dx[i];
            if (ny < 0 || ny >= SIZE || nx < 0 || nx >= SIZE)
                continue;
            if (!visited[ny][nx] && cell_allowed(table[ny][nx])) {
                visited[ny][nx] = true;
                queue[back++] = (Coord){ny, nx};
            }
        }
    }
    return false;
}

// pegar todas as sujeiras e ver qual esta mais proxima e que seja alcançável
void get_closest(int table[SIZE][SIZE], int player[2], int closest[2]) {
    double minDist = -1;
    closest[0] = -1;
    closest[1] = -1;
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (table[y][x] == SUJEIRA) {
                int target[2] = {y, x};
                // fix: so considera se houver caminho
                if (!is_reachable(table, player, target))
                    continue;
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

        if (!cell_allowed(table[ny][nx]))
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
