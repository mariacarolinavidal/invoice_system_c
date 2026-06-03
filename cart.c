/**
 * @file cart.c
 * @brief Funções sobre o cesto.
 */

#include "funcoes.h"
   

int procurar_cesto(Sys *sys, char *ean){
	int i;
	for (i=0; i< sys -> cesto.capacidade; i++) {
		if (!strcmp(sys->cesto.item[i].produtoc->ean, ean) && 
		sys->cesto.item[i].quantidade>0)
			return i;
	}
	return -1;
}

int calcular_vendas_totais(Sys *sys, int indice_produto){
	int pos_cesto = procurar_cesto(sys, sys->produto[indice_produto].ean);
	int no_cesto=0;
	if (pos_cesto != -1) 
		no_cesto = sys->cesto.item[pos_cesto].quantidade;
	return sys->produto[indice_produto].vendas + no_cesto;
}

void listar_cesto(Sys *sys) {
	for (int i = 0; i < sys->cesto.capacidade; i++) {
		if (sys-> cesto.item[i].quantidade>0){            
			Produto *p = sys->cesto.item[i].produtoc;
			int q = sys->cesto.item[i].quantidade;
			imprimir_produto(sys, p , q);
		}
	}
}

int ordenar(Sys *sys, char *ean, Produto *p) {
	int j = sys-> cesto.capacidade -1;
	while (j >= 0 && strcmp(sys->cesto.item[j].produtoc->ean, ean) > 0) {
		sys->cesto.item[j + 1] = sys->cesto.item[j];
		j--;
	}
	int pos = j+1;
	sys->cesto.item[pos].produtoc = p;
	sys->cesto.item[pos].quantidade = 0;
	sys->cesto.capacidade++;
	return pos;
}

