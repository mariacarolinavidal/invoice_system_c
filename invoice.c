/**
 * @file invoice.c
 * @brief Funções sobre faturas.
 */


#include "funcoes.h"
   
void resumo_faturacao(Sys *sys){
	int total_itens=0;
	double total_faturado=0; 
	for (int i = 0; i < sys->total_faturas; i++) {
        total_itens += sys->data[i].quantidade;
		total_faturado += sys->data[i].valor;
       	}
	total_faturado = (long)(total_faturado * 100.0 + 0.5) / 100.0;
	printf("%d %d %.2lf\n", 
			total_itens, 
			sys->faturas_emitidas, 
			total_faturado);
	
	for (int i = 0; i < CODIGOIVAMAX; i++) {
        if (sys->iva.definido[i]) {
            printf("%c %d%%\n", 'A' + i, sys->iva.perc[i]);
        }
    }
}

void cancelamento(Sys *sys){
	for (int i = 0; i < sys->cesto.capacidade; i++) {
		if (sys->cesto.item[i].produtoc != NULL) {
		sys->cesto.item[i].produtoc->stock += 
			sys->cesto.item[i].quantidade;
		}
	}
	sys->cesto.capacidade=0;
}

/**
 * Regista uma nova fatura no sistema, mantendo a ordenacao por nome.
 * Aloca memoria dinamicamente para o nome do cliente.
 * @param sys      ponteiro para a estrutura do sistema
 * @param nome     nome do cliente a registar
 * @param nif      numero de identificacao fiscal
 * @param artigos  quantidade total de itens na fatura
 * @param valor    valor total faturado (com IVA)
 * @return Retorna o indice onde a fatura foi guardada ou -1 em caso de erro.
 */
int registar_fatura(Sys *sys, char *nome, unsigned long nif,
	int artigos, double valor) {
	if (sys->total_faturas >= PRODUTOSFATURACAOMAX) {
		puts(MEMORIAINV);
		return -1;
	}

	int j = sys->total_faturas-1;
	while (j >= 0 && sys->data[j].cliente != NULL && 
		strcmp(sys->data[j].cliente, nome) > 0) {
		sys->data[j + 1] = sys->data[j];
		j--;
	}
	int idx = j+1;
	sys->data[idx].cliente = malloc(strlen(nome) + 1);
	if (sys->data[idx].cliente == NULL) {
        puts(MEMORIAINV);
        return -1;
    }
	strcpy(sys->data[idx].cliente, nome);
	sys->data[idx].numero = ++(sys->faturas_emitidas);
	sys->data[idx].valor = valor;
	sys->data[idx].nif = nif;
	sys->data[idx].quantidade = artigos;

	sys->total_faturas++;
	return idx;
}

/**
 * Processa cada item no cesto, atualizando as vendas e acumulando totais.
 * @param sys             informacao sistema
 * @param total_artigos   ponteiro para acumular o numero total de itens
 * @param total_faturado  ponteiro para acumular o valor total com IVA
 */
void processar_item(Sys *sys, int *total_artigos, double *total_faturado){
	for (int i = 0; i < sys->cesto.capacidade; i++) {
		CestoItem *item = &sys->cesto.item[i];
		if (item->produtoc == NULL) continue; 
		item->produtoc->vendas += item->quantidade;
		int perc_iva = obter_perc_iva(sys, item->produtoc->iva); 
		int qd=item->quantidade;
		*total_faturado+=
			calcular_total(item->produtoc->preco, perc_iva, qd);
		*total_artigos += item->quantidade;
	}
}

void finalizar_registo_fatura(Sys *sys, char *nome, unsigned long nif) {
	double total_faturado=0; 
	int total_artigos=0;
	processar_item(sys, &total_artigos, &total_faturado);
	total_faturado = (long)(total_faturado * 100.0 + 0.5) / 100.0;
	int idx = registar_fatura(sys, nome,nif,total_artigos, total_faturado);
	if (idx!=-1) {
		printf("%d %.2f %d\n", 
			total_artigos, 
			total_faturado, 
			sys->data[idx].numero);
		sys->cesto.capacidade = 0;
	}
}

int argumentos_fatura(char *input, char *nif, unsigned long *nif_val,
					char *cliente_temp) {
	char arg1[INSTRUCAOMAX], arg2[INSTRUCAOMAX];
    if (letra_sozinha(input)){
		*nif_val = NIFOMITIDO;
        strcpy(cliente_temp, CLIENTEDEFINIDO);
		return 1;
	}
    char *ptr = input + 1;
    while (*ptr == ' ' || *ptr == '\t') ptr++;
	int lidos = sscanf(ptr, "%s %s", arg1, arg2);
    if (lidos == 1 || *ptr == '"')  {
        *nif_val = NIFOMITIDO;
        return identificar_nome(input,cliente_temp, 
			(int)(ptr-(input+1)));
    } 
    for (int i = 0; arg1[i]; i++) {
    	if (!isdigit((unsigned char)arg1[i])) {
            printf("%s: %s\n", arg1, NIFINVALIDO);
            return -1;
        }
	}
	*nif_val = strtoul(arg1, NULL, 10);
	strcpy(nif, arg1);
	int pos_nif = (int)(strstr(ptr, arg1) - (input + 1)) + 
				(int)strlen(arg1);
	return identificar_nome(input, cliente_temp, pos_nif);
}

void lista_faturas(Sys *sys, char *nome_cliente){
	int encontrou=0;
	for (int i = 0; i < sys->total_faturas; i++) {
		if (nome_cliente == NULL || 
			(!(strcmp(sys->data[i].cliente, nome_cliente)))) {
			printf("%d %.2f %s\n", 
				sys->data[i].numero, 
				sys->data[i].valor, 
				sys->data[i].cliente);
			encontrou = 1;
		}
	}
	if (!encontrou && nome_cliente != NULL) {
		printf("%s: %s\n", nome_cliente, NENHUMCLIENTE);
	}
}

void apaga_fatura(Sys *sys, int numero) {
	int encontrado = -1;
	for (int i = 0; i < sys->total_faturas; i++) {
		if (sys->data[i].numero == numero) {
			encontrado = i;
			break;
		}
	}
	if (encontrado == -1) {
		printf("%d: %s\n", numero, FATURAINEX);
		return;
	}
	printf("%.2f %lu %s\n", 
			sys->data[encontrado].valor, 
			sys->data[encontrado].nif, 
			sys->data[encontrado].cliente);
	
	free(sys->data[encontrado].cliente);

	for (int i = encontrado; i < sys->total_faturas - 1; i++) {
		sys->data[i] = sys->data[i + 1];
	}
	sys->total_faturas--;
}
