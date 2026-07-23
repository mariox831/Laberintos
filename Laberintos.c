#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FILAS 21
#define COLUMNAS 21

/*Estructura para manejar coordenadas en la matriz*/
typedef struct {
    int x;
    int y;
} Coordenada;

/* ESTRUCTURAS DE DATOS PARA LOS ALGORITMOS */

/*Generación (DFS)*/
Coordenada pila[FILAS * COLUMNAS];
int tope = -1;

void push(Coordenada c) {
    pila[++tope] = c;
}

Coordenada pop() {
    return pila[tope--];
}

/* Resolución Automática (BFS) */
Coordenada cola[FILAS * COLUMNAS];
int frente = 0;
int final = 0;

void enqueue(Coordenada c) {
    cola[final++] = c;
}

Coordenada dequeue() {
    return cola[frente++];
}

int cola_vacia() {
    return frente == final;
}

/* FUNCIONES DEL LABERINTO */

/*Inicializa todo el laberinto lleno de paredes (1) */
void inicializar_laberinto(int laberinto[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            laberinto[i][j] = 1;
        }
    }
}

/* Imprime el laberinto en pantalla */
void mostrar_laberinto(int laberinto[FILAS][COLUMNAS], Coordenada jugador, Coordenada inicio, Coordenada fin) {
    // Secuencia ANSI para limpiar pantalla y mover el cursor al inicio
    printf("\033[H\033[J"); 

    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == jugador.x && j == jugador.y) {
                printf("P "); // Representación del Jugador
            } else if (i == inicio.x && j == inicio.y) {
                printf("E "); // Entrada
            } else if (i == fin.x && j == fin.y) {
                printf("S "); // Salida
            } else if (laberinto[i][j] == 1) {
                printf("# "); // Pared
            } else if (laberinto[i][j] == 0) {
                printf(". "); // Camino libre
            } else if (laberinto[i][j] == 2) {
                printf("* "); // Ruta óptima del BFS
            }
        }
        printf("\n");
    }
}

/* Generación del laberinto usando DFS */
void generar_laberinto(int laberinto[FILAS][COLUMNAS], Coordenada inicio) {
    laberinto[inicio.x][inicio.y] = 0;
    push(inicio);

    /* Direcciones de movimiento (Arriba, Abajo, Izquierda, Derecha) de 2 en 2 casillas */
    int dx[] = {-2, 2, 0, 0};
    int dy[] = {0, 0, -2, 2};

    while (tope >= 0) {
        Coordenada actual = pila[tope];
        Coordenada vecinos_validos[4];
        int direcciones_validas[4];
        int contador_vecinos = 0;

        /* Buscar vecinos no visitados a distancia 2 */
        for (int i = 0; i < 4; i++) {
            int nx = actual.x + dx[i];
            int ny = actual.y + dy[i];

            if (nx > 0 && nx < FILAS - 1 && ny > 0 && ny < COLUMNAS - 1) {
                if (laberinto[nx][ny] == 1) {
                    vecinos_validos[contador_vecinos] = (Coordenada){nx, ny};
                    direcciones_validas[contador_vecinos] = i;
                    contador_vecinos++;
                }
            }
        }

        if (contador_vecinos > 0) {
            /* Elegir un vecino aleatorio */
            int r = rand() % contador_vecinos;
            Coordenada proximo = vecinos_validos[r];
            int dir = direcciones_validas[r];

            /* reemplazar 1 por 0 */
            laberinto[actual.x + dx[dir]/2][actual.y + dy[dir]/2] = 0;
            laberinto[proximo.x][proximo.y] = 0;

            push(proximo);
        } else {
            /* Backtracking si no hay salida */
            pop();
        }
    }
}

/* FASE MANUAL*/
void jugar_manual(int laberinto[FILAS][COLUMNAS], Coordenada inicio, Coordenada fin) {
    Coordenada jugador = inicio;
    char movimiento;

    while (jugador.x != fin.x || jugador.y != fin.y) {
        mostrar_laberinto(laberinto, jugador, inicio, fin);
        printf("\n--- MODO MANUAL ---");
        printf("\nUsa W (Arriba), S (Abajo), A (Izquierda), D (Derecha) + Enter para moverte.");
        printf("\nTu movimiento: ");
        scanf(" %c", &movimiento);

        int n_x = jugador.x;
        int n_y = jugador.y;

        if (movimiento == 'w' || movimiento == 'W') n_x--;
        if (movimiento == 's' || movimiento == 'S') n_x++;
        if (movimiento == 'a' || movimiento == 'A') n_y--;
        if (movimiento == 'd' || movimiento == 'D') n_y++;

        /* Verificar si el movimiento es un camino libre (0) o la salida */
        if (n_x >= 0 && n_x < FILAS && n_y >= 0 && n_y < COLUMNAS) {
            if (laberinto[n_x][n_y] == 0 || (n_x == fin.x && n_y == fin.y)) {
                jugador.x = n_x;
                jugador.y = n_y;
            }
        }
    }
    mostrar_laberinto(laberinto, jugador, inicio, fin);
    printf("\n¡Felicidades! Lograste resolver el laberinto manualmente.\n");
    printf("Presiona Enter para ver la resolucion automatica con BFS...\n");
    getchar(); getchar();
}

/* FASE INTELIGENTE: Resolución automática usando BFS*/
void resolver_bfs(int laberinto[FILAS][COLUMNAS], Coordenada inicio, Coordenada fin) {
    /* Matriz para rastrear las casillas visitadas */
    int visitado[FILAS][COLUMNAS] = {0};
    /* Matriz de padres para poder reconstruir el camino al final */
    Coordenada padre[FILAS][COLUMNAS];

    frente = 0; final = 0; /* Reiniciar la cola */
    
    enqueue(inicio);
    visitado[inicio.x][inicio.y] = 1;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    int encontrado = 0;

    while (!cola_vacia()) {
        Coordenada actual = dequeue();

        if (actual.x == fin.x && actual.y == fin.y) {
            encontrado = 1;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = actual.x + dx[i];
            int ny = actual.y + dy[i];

            if (nx >= 0 && nx < FILAS && ny >= 0 && ny < COLUMNAS) {
                if (laberinto[nx][ny] == 0 && !visitado[nx][ny]) {
                    visitado[nx][ny] = 1;
                    padre[nx][ny] = actual;
                    enqueue((Coordenada){nx, ny});
                }
            }
        }
    }

    /* Si se encontró la salida, marcamos el camino de regreso con un '2' */
    if (encontrado) {
        Coordenada paso = padre[fin.x][fin.y];
        while (paso.x != inicio.x || paso.y != inicio.y) {
            laberinto[paso.x][paso.y] = 2; // El número 2 se imprimirá como '*'
            paso = padre[paso.x][paso.y];
        }
        
        /* Mostrar resultado final */
        mostrar_laberinto(laberinto, (Coordenada){-1, -1}, inicio, fin);
        printf("\n--- MODO AUTOMATICO (BFS) ---");
        printf("\nEl algoritmo BFS ha encontrado la ruta mas corta (marcada con *).\n");
    } else {
        printf("\nNo se encontro una ruta de solucion.\n");
    }
}

int main() {
    srand(time(NULL)); /* Semilla aleatoria */

    int laberinto[FILAS][COLUMNAS];
    
    Coordenada inicio = {1, 1};
    Coordenada fin = {FILAS - 2, COLUMNAS - 2};

    printf("Generando laberinto perfecto con DFS...\n");
    inicializar_laberinto(laberinto);
    generar_laberinto(laberinto, inicio);
    
    /* Asegurar que la entrada y la salida estén conectadas y libres */
    laberinto[inicio.x][inicio.y] = 0;
    laberinto[fin.x][fin.y] = 0;

    /* Ejecución de las fases descritas en tu proyecto */
    jugar_manual(laberinto, inicio, fin);
    resolver_bfs(laberinto, inicio, fin);

    return 0;
}