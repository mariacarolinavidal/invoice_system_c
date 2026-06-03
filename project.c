/**
 * @file project.c
 * @brief Função principal.
 */

#include "funcoes.h"

/**   
 * Ponto de entrada do sistema de faturacao.
 * Inicializa as estruturas, processa comandos e garante a limpeza de memoria.
 * @param argc  numero de argumentos do programa
 * @param argv  vetor de argumentos (pode conter ficheiro de IVAs)
 * @return Retorna 0 em caso de sucesso, 1 em caso de erro critico de memoria.
 */
int main(int argc, char *argv[]) {
    /* Inicializacao do sistema e carregamento de configuracoes */
    Sys *sys = inicializar_sistema(argc, argv);
    
    if (!sys) {
        puts(MEMORIAINV);
        return 1;
    }

    /* Execucao do ciclo principal de comandos */
    executar_comandos(sys);

    /* Libertacao de toda a memoria dinamica antes de fechar */
    libertar_sistema(sys);

    return 0;
}
