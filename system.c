/**
 * @file system.c
 * @brief Funções do sistema de faturação.
 */


#include "funcoes.h" 
  
// função que atualiza o sistema
void atualiza_sistema(Sys *sys, char *ean, char iva, 
	double preco, int qnt, char *desc, int idx) { 
	if (idx != -1) {
		sys -> produto[idx].iva= iva;
		sys -> produto[idx].preco = preco;
		sys -> produto[idx].stock += qnt;
		strcpy(sys -> produto[idx].descricao,desc);
		printf("%d\n", sys->produto[idx].stock);
	}
	else {
		int novo = criar_produto(sys, ean, iva, preco, 
			qnt, desc);
		printf("%d\n", sys->produto[novo].stock);
	}
}

Sys* inicializar_sistema(int argc, char *argv[] ) {
	Sys *sys = calloc(1, sizeof(Sys));
	if (!sys) return NULL;
	sys->cesto.item = calloc(PRODUTOSFATURACAOMAX, sizeof(CestoItem));
	if (!sys->cesto.item) {
        free(sys);
        return NULL;
    }
    if (argc > 1) carregar_ivas(sys, argv[1]);
    else criar_ivas(sys);

    return sys;
}


void libertar_sistema(Sys *sys){
	if (!sys) return;

	for (int i = 0; i < sys->total_faturas; i++) {
		if (sys->data[i].cliente != NULL) {
            free(sys->data[i].cliente);
            sys->data[i].cliente = NULL; 
        }
    }

    if(sys->cesto.item)
		free(sys->cesto.item);
	free(sys);
	return;
}
