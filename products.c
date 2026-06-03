/**
 * @file products.c
 * @brief Funcoes de produtos.
 */
   
#include "funcoes.h"

void carregar_ivas(Sys *sys, char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (!f) return; // Se o ficheiro não abrir, mantemos o que está

    char letra;  
    int percentagem;
    // Lê uma letra e um inteiro até ao fim do ficheiro
    while (fscanf(f, " %c %d", &letra, &percentagem) == 2) {
        if (letra >= 'A' && letra <= 'Z') {
            int idx = letra - 'A';
            sys->iva.definido[idx] = 1;
            sys->iva.perc[idx] = percentagem;
        }
    }
    fclose(f);
}

void criar_ivas(Sys *sys) {
	for (int i=0; i<CODIGOIVAMAX; i++) {
		sys->iva.definido[i] = 0;
	}
	for (int i=0; i<='D'-'A'; i++ ){
		sys -> iva.definido[i] = 1;
	}

	sys -> iva.perc['A'-'A'] = 0;
	sys -> iva.perc['B'-'A'] = 6;
	sys -> iva.perc['C'-'A'] = 13;
	sys -> iva.perc['D'-'A'] = 23;
}

int wildcard_corresponde(char *padrao, char *texto){
    if (*padrao == '\0')
        return *texto == '\0';

    if (*padrao == '*')
        return wildcard_corresponde(padrao + 1, texto) ||
               (*texto && wildcard_corresponde(padrao, texto + 1));

    if (*padrao == '?')
        return *texto && wildcard_corresponde(padrao + 1, texto + 1);

    if (*padrao == *texto)
        return wildcard_corresponde(padrao + 1, texto + 1);

    return 0;
}


double calcular_total(double preco, int perc_iva, int quantidade) {
    if (quantidade <= 0) return 0.0;
    
    double preco_com_iva = preco * (1.0 + (perc_iva / 100.0));
    long total_centimo = (long)(preco_com_iva*quantidade*100.0+0.500001);
    
    return (double)total_centimo / 100.0;
}


int obter_perc_iva(Sys *sys, char codigo_iva){
	return sys->iva.perc[codigo_iva - 'A'];  
}


int procurar_produto(Sys *sys, char *ean){
	int abaixo = 0;
	int maior = sys->cont-1;

	while (abaixo<=maior) {
		int metade = abaixo + (maior - abaixo ) /2;
		int pos_real = sys->indice_ean[metade];
		int comp = strcmp(sys->produto[pos_real].ean, ean);
		
		if (comp==0)
			return pos_real;
		if (comp<0)
			abaixo = metade + 1;
		else
			maior = metade-1;
	}
	return -1;
}

int criar_produto(Sys *sys, char *ean, char iva, double preco,
	int quantidade, char *descricao){
	int pos_nova = sys->cont;

	strncpy(sys -> produto[pos_nova].ean, ean, COMPRIMENTOEAN-1);
	sys->produto[pos_nova].ean[COMPRIMENTOEAN-1] = '\0';
	sys -> produto[pos_nova].iva= iva;
	sys -> produto[pos_nova].preco = preco;
	sys -> produto[pos_nova].stock = quantidade;
	strncpy(sys->produto[pos_nova].descricao, descricao, NRDESCRICAOMAX-1);
	sys->produto[pos_nova].descricao[NRDESCRICAOMAX-1] = '\0';

	int j = sys -> cont -1;
	while (j >= 0 && strcmp(sys->produto[sys->indice_ean[j]].ean, ean) > 0){
        sys->indice_ean[j + 1] = sys->indice_ean[j];
        j--;
    }
	sys->indice_ean[j + 1] = pos_nova;
    sys->cont++;
    return pos_nova;
}

void lista_produtos(Sys *sys){
	int encontrou=0;
	for (int i=0; i<sys->cont; i++) {
		if (sys->produto[i].stock > 0) {
			int total = calcular_vendas_totais(sys, i);
			printf("%s %c %.2lf %d %d %s\n", 
				sys -> produto[i].ean, 
				sys -> produto[i].iva, 
				sys -> produto[i].preco,
				total, 
				sys -> produto[i].stock,
				sys -> produto[i].descricao);
			encontrou=1;
		}
	}
	if (!encontrou){
		char c = '*';
		printf("%c: %s\n", c, SEMCORRESPONDENCIA); 
	}
}


void imprimir_linha_produto(Sys *sys, int i) {
	int total_vendas = calcular_vendas_totais(sys, i);
    printf("%s %c %.2lf %d %d %s\n", 
		sys -> produto[i].ean, 
		sys -> produto[i].iva,
		sys -> produto[i].preco,
		total_vendas,
		sys -> produto[i].stock,
		sys -> produto[i].descricao);
}

void imprimir_produto(Sys *sys, Produto *p, int q) {
	int perc = obter_perc_iva(sys, p->iva);
	double total = calcular_total(p->preco, perc, q);
	printf("%c %.2f %d %.2f %s\n", 
		p->iva, p->preco, q, total, p->descricao);
}

void resumo_produto(Sys *sys, char *ean){
	if (!verificacao_ean(ean)) {
		puts(EANERRADO);
		return;
        }
	int ind = procurar_produto(sys,ean);
	if (ind == -1) {
		printf("%s: %s\n", ean, SEMCORRESPONDENCIA);
		return;
	}
	int total = calcular_vendas_totais(sys, ind);
	printf("%d %d %s\n",
		sys->produto[ind].stock,
		total,
		sys->produto[ind].descricao);
}

void atualiza_produto(Sys *sys, char *ean, int qtd){
	int idx = procurar_produto(sys, ean);
	if (idx == -1) {
		printf("%s: %s\n", ean, SEMCORRESPONDENCIA);
		return;
	}
	if (procurar_cesto(sys, ean) != -1) {
		puts(PRODUTOCESTO);
		return;
	}
	if (qtd<=0 || qtd > sys->produto[idx].stock) {
		puts(QUANTNEG);
		return;
	}

	sys->produto[idx].stock -= qtd;
	printf("%d %s\n", 
		sys->produto[idx].stock, 
		sys->produto[idx].descricao);
}
