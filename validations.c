/**
 * @file validations.c
 * @brief Verificacoes e validacoes.
 */

#include "funcoes.h"
   
// funcao que verifica a existência do código iva 
int verificacao_iva(Sys *sys, char letra) {
	if (letra < 'A' || letra > 'Z') 
		return 0;
	int indice = letra -'A';
	if (sys->iva.definido[indice]==1)
		return 1;
	return 0;
}

// funcao que verifica as condições do ean
int verificacao_ean(char *ean) {
	int soma=0;
	int comp = strlen (ean);
	if (comp!=COMPRIMENTOEAN-1 && comp!=COMPRIMENTOEAN2-1)
		return 0;
	for (int i=0; i<comp-1; i++) {
		int digito = ean[i] - '0';
		if (i%2==0) 
			soma = soma + digito;
		else
			soma=soma + digito*3;
		}
	int dig_esp = (10-(soma %10)) % 10;
	int ult_dig = ean[comp-1] - '0';
	if (dig_esp == ult_dig)
		return 1;
	return 0;
}

// funcao que verifica as condições do preço
int verificacao_preco(double preco) {
		if (preco <= 0)
			return 0;
		return 1;
	}

// funcao que verifica as condições da quantidade
int verificacao_quantidade(int quantidade){
		if (quantidade<0)
			return 0;
		return 1;
	}

// funcao que verifica as restrições para o nome do cliente
int verificacao_nome(char *cliente) {
	char acentuadas[][3] = {
		"Á", "À", "Â", "Ã", "É", "È", "Ê", "Í", "Ó", "Ò",
		"Ô", "Õ", "Ú", "Ç", "á", "à", "â", "ã", "é", "è",
		"ê", "í", "ó", "ò", "ô", "õ", "ú", "ç"};
    int com = strlen(cliente);
    if (com == 0) return 0;
	int i_inicio = 0; int i_fim = com;
    if (cliente[0] == '"') {
        if (com < 2 || cliente[com-1] != '"') return 0;
        i_inicio = 1;
        i_fim = com - 1;
	}
    int letra_valida = 0;
    unsigned char c = (unsigned char)cliente[i_inicio];
    if (isalpha(c)) letra_valida = 1;
	else {
        for (int i = 0; i < 2*COMPRIMENTOEAN; i++) {
            if (strncmp(&cliente[i_inicio], acentuadas[i], 2) == 0) {
                letra_valida = 1;
                break;
            }
        }
    }
    if (!letra_valida) return 0;
    for (int j = i_inicio; j < i_fim; j++) {
        if (cliente[j] == '"') return 0;
    }
    return 1;
}

// funcao que verifica as restrições para o nif
int verificacao_nif(unsigned long nif){
	if ((nif<100000000 || nif > 999999999))
		return 0;
	return 1;
}

// funcao que verifica as condições da descrição
int verificacao_descricao(char *descricao){
	char acentos[][3] = {"Á", "À", "Â", "Ã", "É", "È", "Ê", "Í", "Ó",
		"Ò", "Ô", "Õ", "Ú", "Ç"};
	if (descricao == NULL || descricao[0] == '\0')
        return 0;
	unsigned char c = (unsigned char) descricao[0];
	if ((strlen(descricao) >= NRDESCRICAOMAX))
		return 0;
	if (isupper(c))
		return 1;
	for (int i = 0; i < COMPRIMENTOEAN; i++) {
        if (strncmp(descricao, acentos[i], 2) == 0) 
            return 1;
    }
	return 0;
}

// funcao que verifica numero de produtos registáveis
int verificacao_produto(Sys *sys, char *ean){
	int idx = procurar_produto(sys, ean);
	if (idx == -1 && sys->cont >= PRODUTOSFATURACAOMAX)
		return 0;
	return 1;
}

// funcao de verificacao de restrições dos inputs comando p
int verificacao_comando_p(Sys *sys, char *ean, char iva, 
	double preco, int qnt, char *desc) {

	if (!verificacao_ean(ean)) {
		puts(EANERRADO);
		return 0;
	}
	if (!verificacao_iva(sys, iva)){
		puts(IVASEMDESIGNACAO);
		return 0;
	} 
	if (!verificacao_preco(preco)) {
		puts(PRECONEG);
		return 0;
	}
	if (!verificacao_quantidade(qnt)) {
		puts(QUANTNEG);
		return 0;
	}
	if (!verificacao_descricao(desc)){
		puts(DESCRICAOMAX);
		return 0;
	}
	return 1;
}


//funcao de validação das restrições do comando p
int validar_comando_p(Sys *sys, char *ean, char iva, 
	double preco, int qnt, char *desc, int idx) {

	if (!verificacao_comando_p(sys, ean, iva, preco, qnt, desc))
		return 0;

	if (idx != -1) {
	if (sys->produto[idx].preco!=preco) {
		if (procurar_cesto(sys, ean)!=-1) {
			puts(PRODUTOCESTO);
			return 0;
		}
	}
	}
	else {
		if (!verificacao_produto(sys, ean)) {
			puts(NRPRODUTOSEXC);
			return 0;
		}
	}
	return 1;
}

//função que verifica as restrições do comando a.
int verificacao_comando_a(Sys *sys, char *ean, int qnt, 
	int ind_prod, int pos){
	if (!verificacao_ean(ean)) {
		puts(EANERRADO);
		return 0;
	}
	
	if (qnt < 0) {
		if (pos==-1 || sys->cesto.item[pos].quantidade < -qnt) {
			puts(QUANTNEG);
			return 0;
		}
	}

	if (ind_prod == -1) {
		printf("%s: %s\n", ean, SEMCORRESPONDENCIA);
		return 0;
	}

	if (qnt > 0) {
		if (sys->produto[ind_prod].stock < qnt) {
			puts(INDISPONIVEL);
			return 0;
		}
	}
	return 1;
}
