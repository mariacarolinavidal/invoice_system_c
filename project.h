/**
 * @file project.h
 * @brief Definição de constantes e tipos de dados.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
 
/* Valores por defeito */
#define NIFOMITIDO 999999999       /* NIF atribuido por omissão */
#define NRDESCRICAOMAX 51          /* Tamanho maximo da descricao produto */
#define INSTRUCAOMAX 65536         /* Tamanho maximo do input */
#define CODIGOIVAMAX 26            /* Numero de taxas de IVA possiveis  */
#define PRODUTOSFATURACAOMAX 10000 /* Limite de produtos no sistema */
#define COMPRIMENTOEAN 14          /* Tamanho da string EAN-13 com \0 */
#define COMPRIMENTOEAN2 9          /* Tamanho da string EAN-8 com \0 */

/* Mensagens de erro do sistema */
 
#define CLIENTEDEFINIDO "Cliente final"
#define EANERRADO "invalid ean"
#define IVASEMDESIGNACAO "invalid iva"
#define PRECONEG "invalid price"
#define QUANTNEG "invalid quantity"
#define DESCRICAOMAX "invalid description"
#define PRODUTOCESTO "product in use"
#define NRPRODUTOSEXC "invalid product"
#define SEMCORRESPONDENCIA "no such product"
#define INDISPONIVEL "no stock"
#define NIFINVALIDO "no such nif"
#define CLIENTEINVALIDO "invalid name"
#define NENHUMCLIENTE "no such client"
#define FATURAINEX "no such invoice"
#define MEMORIAINV "no memory"

/* Representa um produto individual no inventario */
typedef struct {
    char descricao[NRDESCRICAOMAX]; /* Descricao do produto */
    char ean[COMPRIMENTOEAN];       /* Codigo EAN */
    double preco;                   /* Preco unitario sem IVA */
    char iva;                       /* Letra da taxa de IVA  */
    int vendas;                     /* Unidades vendidas */
    int stock;                      /* Unidades disponiveis */
} Produto;

/* Representa uma fatura emitida pelo sistema */
typedef struct {
    int quantidade;                 /* Numero total de itens na fatura */
    double valor;                   /* Valor total final com IVA */
    char *cliente;                  /* Nome do cliente */
    unsigned long nif;              /* Numero de Identificacao Fiscal */
    int numero;                     /* Numero da fatura */
} Fatura;

/* Item colocado no cesto de compras */
typedef struct {
    Produto *produtoc;              /* Ponteiro para o produto original */
    int quantidade;                 /* Quantidade a comprar */
} CestoItem;

/* Estrutura do cesto de compras atual */
typedef struct {
    CestoItem *item;                /* Array de itens no cesto */
    int capacidade;                 /* Numero de itens no cesto */
} Cesto;

/* Taxas de IVA definidas */
typedef struct {
    int definido[CODIGOIVAMAX];    /* 1 se a taxa existe, 0 caso contrario */
    int perc[CODIGOIVAMAX];        /* Valor percentual da taxa */
} Iva;
 
/* Estrutura principal que engloba todo o sistema */
typedef struct {
    Produto produto[PRODUTOSFATURACAOMAX];    /* Inventario de produtos */
    int indice_ean[PRODUTOSFATURACAOMAX];     /* Indice para procura */
    Fatura data[PRODUTOSFATURACAOMAX];        /* Historico de faturas */
    int total_faturas;                   /* Numero de faturas em memoria */
    int faturas_emitidas;                /* Contador total de faturas */
    Cesto cesto;                         /* Cesto de compras atual */
    int cont;                       /* Contador de produtos no inventario */
    Iva iva;                        /* Definição de IVA */
} Sys;
