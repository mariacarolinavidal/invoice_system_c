/**
 * @file commands.c
 * @brief Comandos e parsing.
 */


#include "funcoes.h"

int letra_sozinha(char *input) {
    for (int i = 1; input[i] != '\0'; i++) {
        if (input[i] != ' ' && input[i] != '\n' && input[i] != '\r') {
            return 0; 
        }
    }
    return 1;
}

int identificar_nome(char *input, char *cliente_temp, int pos_lida){
	char *inicio_cliente=input+1+pos_lida;
	while (*inicio_cliente == ' ') inicio_cliente++;
	if (*inicio_cliente == '"') {
		inicio_cliente++;
		char *aspa_final = strrchr(inicio_cliente, '"');
		if (!aspa_final) {
			return 0;
		}
		char *check = aspa_final + 1;
        while (*check && isspace((unsigned char)*check)) check++;
        if (*check != '\0') return 0; 

        int len = aspa_final - inicio_cliente;
        strncpy(cliente_temp, inicio_cliente, len);
        cliente_temp[len] = '\0';
	}
	else {
		strcpy(cliente_temp, inicio_cliente);
		cliente_temp[strcspn(cliente_temp, "\n\r")] = '\0';
		int len = strlen(cliente_temp);
        while (len > 0 && isspace((unsigned char)cliente_temp[len - 1])) {
            cliente_temp[--len] = '\0';
		}
	}
	return 1;
}

/** * Adiciona ou atualiza um produto no inventario (Comando 'p').
 * Valida os argumentos de entrada e aplica as alteracoes 
 * se passarem nas restricoes.
 * @param sys   Ponteiro para a estrutura principal do sistema.
 * @param input String contendo a linha de comando lida.
 */
void comando_p(Sys *sys, char *input) {
	double preco;
	char ean[COMPRIMENTOEAN], iva, descricao[INSTRUCAOMAX];
	int quantidade, pos_lida=0;
	int lidos = sscanf(input+1, "%13s %c %lf %d %n", ean, &iva, 
		&preco, &quantidade, &pos_lida);

	if (lidos<4){
		return;
	}
	strcpy(descricao, input+1+pos_lida);
	descricao[strcspn(descricao, "\n\r")] = '\0';
	
	int idx = procurar_produto(sys, ean);

	if (validar_comando_p(sys, ean, iva, preco, quantidade, descricao, idx))
		atualiza_sistema(sys, ean, iva, preco,quantidade, 
			descricao, idx);
}


/** * Lista produtos no sistema com suporte a wildcards (Comando 'l').
 * Permite filtrar por padroes ou listar todos os produtos com stock.
 * @param sys   Ponteiro para a estrutura principal do sistema.
 * @param input String contendo a linha de comando lida.
 */
void comando_l (Sys *sys, char *input){
	if (letra_sozinha(input)){
		lista_produtos(sys);
		return;
	} 
	char *wildcard = strtok(input + 1, " \n");
	while (wildcard != NULL) {
		if (strcmp(wildcard, "*")==0)
			lista_produtos(sys);
		else {
			int encontrou_wildcard = 0;
			for (int i=0; i<sys->cont; i++) {
				if (sys->produto[i].stock > 0 &&
					wildcard_corresponde(wildcard, 
						sys->produto[i].ean)) {
					imprimir_linha_produto(sys, i);
					encontrou_wildcard=1;
				}
			}
		if (!encontrou_wildcard)
			printf("%s: %s\n", wildcard, SEMCORRESPONDENCIA); 
		}
		wildcard = strtok(NULL, " \n");
	}
}

/** * Gere a reserva de produtos no cesto de compras (Comando 'a').
 * Adiciona itens ao cesto, atualiza stock e remove itens 
 * se a quantidade for zero.
 * @param sys   Ponteiro para a estrutura principal do sistema.
 * @param input String contendo a linha de comando lida.
 */
void comando_a(Sys *sys, char *input) {
    char ean[COMPRIMENTOEAN];
    int quantidade = 1; 

    if (letra_sozinha(input)) {
		listar_cesto(sys);
        return;
    }

	int res = sscanf(input + 1, "%d %s", &quantidade, ean);
	if (res == 1) { 
		sscanf(input + 1, "%s", ean);
		quantidade = 1;
	}

	int ind_prod = procurar_produto(sys, ean);
	int pos = procurar_cesto(sys, ean);
	if (!verificacao_comando_a(sys, ean, quantidade, ind_prod, pos)) 
        return; 

	Produto *p = &sys->produto[ind_prod];
	if (pos == -1) {
		pos = ordenar(sys, ean, p);
	}
	p->stock -= quantidade;
	sys->cesto.item[pos].quantidade += quantidade;
	imprimir_produto(sys, p, sys->cesto.item[pos].quantidade);

	if (sys->cesto.item[pos].quantidade==0){
		for (int i = pos; i < sys->cesto.capacidade - 1; i++) {
            sys->cesto.item[i] = sys->cesto.item[i + 1];
        }
        sys->cesto.capacidade--;
	}
}
   
/** * Exibe resumos de faturacao global ou por produto (Comando 'r').
 * @param sys   Ponteiro para a estrutura principal do sistema.
 * @param input String contendo a linha de comando lida.
 */
void comando_r(Sys *sys, char *input){
	if (letra_sozinha(input)){
		resumo_faturacao(sys);
	}
	else {
		char *ean = strtok(input + 1, " \n");
		resumo_produto(sys,ean);
	}
}

/** * Finaliza a compra e gera a fatura correspondente (Comando 'f').
 * Processa o NIF, o nome do cliente e efetiva o registo da fatura.
 * @param sys   Ponteiro para a estrutura principal do sistema.
 * @param input String contendo a linha de comando lida.
 */
void comando_f(Sys *sys, char *input) {
	char nif[COMPRIMENTOEAN2+1];
	char cliente_temp[INSTRUCAOMAX];
	unsigned long nif_val = NIFOMITIDO;
	
	int res = argumentos_fatura(input, nif, &nif_val, cliente_temp);

	if (res==-1)
		return;

	if (!verificacao_nif(nif_val) && nif_val != NIFOMITIDO) {
		printf("%s: %s\n", nif, NIFINVALIDO);
		return;
	}

	if (res==0 || !verificacao_nome(cliente_temp)) {
		puts(CLIENTEINVALIDO);
		return;
	}

	if (!strcmp(cliente_temp, "error")) {
		cancelamento(sys);
		return;
	}

	finalizar_registo_fatura(sys, cliente_temp, nif_val);
}

/** * Lista faturas emitidas no sistema (Comando 'c').
 * @param sys   Ponteiro para a estrutura principal do sistema.
 * @param input String contendo a linha de comando lida.
 */
void comando_c(Sys *sys, char *input) {
	char cliente[INSTRUCAOMAX];

	if (letra_sozinha(input)) {
		lista_faturas(sys, NULL);
		return;
	}

	if (!identificar_nome(input, cliente, 0)) {
		if (sscanf(input + 1, "%s", cliente) != 1) 
			return;
	}

	if (!verificacao_nome(cliente)){
		puts(CLIENTEINVALIDO);
		return;
	}

	lista_faturas(sys,cliente);
}

/** * Apaga uma fatura ou atualiza o stock de um produto (Comando 'd').
 * @param sys   Ponteiro para a estrutura principal do sistema.
 * @param input String contendo a linha de comando lida.
 */
void comando_d(Sys *sys, char *input) {
    char numero[COMPRIMENTOEAN];
	int quantidade;
    int lidos = sscanf(input + 1, "%s %d", numero, &quantidade);

    if (lidos == 1) {
		apaga_fatura(sys, atoi(numero));

    } else if (lidos == 2) {
        if (!verificacao_ean(numero)) {
            puts(EANERRADO);
            return;
        }
        atualiza_produto(sys, numero, quantidade);
    }
}

int executar_comandos(Sys *sys) {
	char input[INSTRUCAOMAX];
	while (fgets(input, INSTRUCAOMAX, stdin)) 
		switch (input[0]) {
			case 'q': return 0;
			case 'p': comando_p(sys, input); break;
			case 'l': comando_l(sys, input); break;
			case 'a': comando_a(sys, input); break;
			case 'r': comando_r(sys, input); break;
			case 'f': comando_f(sys, input); break;
			case 'c': comando_c(sys, input); break;
			case 'd': comando_d(sys, input); break;
		}
	return 0;
}
