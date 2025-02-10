#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define TAMANHO_QUADRADO 50

// Função para gerar uma cor aleatória
SDL_Color corAleatoria() {
    SDL_Color cor;
    cor.r = rand() % 256;
    cor.g = rand() % 256;
    cor.b = rand() % 256;
    cor.a = 255;
    return cor;
}

int main(int argc, char* argv[]) {
    // Inicializando SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Criando a janela
    SDL_Window* janela = SDL_CreateWindow("Teste SDL2: Animação",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          LARGURA_TELA, ALTURA_TELA, SDL_WINDOW_SHOWN);
    if (!janela) {
        printf("Erro ao criar a janela: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Criando o renderizador
    SDL_Renderer* renderizador = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);
    if (!renderizador) {
        printf("Erro ao criar o renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(janela);
        SDL_Quit();
        return 1;
    }

    // Inicializando a semente do gerador de números aleatórios
    srand(time(NULL));

    // Definindo variáveis para o quadrado
    int x = LARGURA_TELA / 2 - TAMANHO_QUADRADO / 2;
    int y = ALTURA_TELA / 2 - TAMANHO_QUADRADO / 2;
    int velX = 5, velY = 5; // Velocidade do quadrado
    SDL_Color corQuadrado = corAleatoria();

    // Loop principal
    int rodando = 1;
    SDL_Event evento;
    while (rodando) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                rodando = 0;
            }
        }

        // Movimentação do quadrado
        x += velX;
        y += velY;

        // Verificar colisões com as bordas da tela
        if (x <= 0 || x + TAMANHO_QUADRADO >= LARGURA_TELA) {
            velX = -velX; // Inverte a direção horizontal
            corQuadrado = corAleatoria(); // Troca a cor
        }
        if (y <= 0 || y + TAMANHO_QUADRADO >= ALTURA_TELA) {
            velY = -velY; // Inverte a direção vertical
            corQuadrado = corAleatoria(); // Troca a cor
        }

        // Limpa a tela
        SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255); // Preto
        SDL_RenderClear(renderizador);

        // Define a cor do quadrado
        SDL_SetRenderDrawColor(renderizador, corQuadrado.r, corQuadrado.g, corQuadrado.b, corQuadrado.a);

        // Desenha o quadrado
        SDL_Rect quadrado = {x, y, TAMANHO_QUADRADO, TAMANHO_QUADRADO};
        SDL_RenderFillRect(renderizador, &quadrado);

        // Atualiza a tela
        SDL_RenderPresent(renderizador);

        // Controla a taxa de atualização (60 FPS)
        SDL_Delay(1000 / 60);
    }

    // Limpa e finaliza SDL
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(janela);
    SDL_Quit();

    return 0;
}
