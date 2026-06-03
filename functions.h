/**
 * @file funcoes.h
 * @brief Protótipos das funções.
 * @author Maria Carolina Vidal ist1118018
 */

#include "project.h"

//VALIDAÇÕES
  
/** Verifica se o Iva existe.
 * @param sys	informação sistema
 * @param letra	corresponde a iva
 * @return 1 se for valido, 0 caso contrario.
 */
int verificacao_iva(Sys *sys, char letra);

/** Verifica se o codigo EAN é numerico e se o digito de controlo esta 
correto.
 * @param ean	string ean
 * @return Retorna 1 se for valido, 0 caso contrario.
 */
int verificacao_ean(char *ean);


/** Verifica se o preço é um valor positivo.
 * @param preco	double preco
 * @return 1 se for valido, 0 caso contrario.
 */
int verificacao_preco(double preco);

/** Verifica se a quantidade é um número não negativo.
 * @param quantidade	inteiro quantidade
 * @return Retorna 1 se for valido, 0 caso contrario.
 */
int verificacao_quantidade(int quantidade);


/** Verifica se a string passa nas restrições do nome. 
 * @param cliente	string nome cliente
 * @return Retorna 1 se for valido, 0 caso contrario.
 */
int verificacao_nome(char *cliente);

/** Verifica se o inteiro tem 9 digitos. 
 * @param nif	unsigned long nif
 * @return Retorna 1 se for valido, 0 caso contrario.
 */
int verificacao_nif(unsigned long nif);

/** Verifica se a string passa nas restrições da descrição. 
 * @param descricao	string descricao
 * @return Retorna 1 se for valido, 0 caso contrario.
 */
int verificacao_descricao(char *descricao);

/** Verifica se o número de produtos registáveis não é excedido. 
 * @param ean	string ean
 * @param sys	informação sistema
 * @return Retorna 1 se for valido, 0 caso contrario.
 */
int verificacao_produto(Sys *sys, char *ean);

/** Valida as restricoes do comando p.
 * @param sys       informacao sistema
 * @param ean       string ean
 * @param iva       codigo da taxa de iva
 * @param preco     preco unitario
 * @param qnt       quantidade em stock
 * @param desc      descricao do produto
 * @param idx       indice do produto (se ja existir)
 * @return Retorna 1 se for valido, 0 caso contrario.
 */
int validar_comando_p(Sys *sys, char *ean, char iva, 
                    double preco, int qnt, char *desc, int idx);

/** Verifica as restricoes do comando a (adicionar ao cesto).
 * @param sys       informacao sistema
 * @param ean       string ean
 * @param qnt       quantidade a adicionar
 * @param ind_prod  indice do produto no inventario
 * @param pos       posicao no input
 * @return Retorna 1 se for valido, 0 caso contrario.
 */
int verificacao_comando_a(Sys *sys, char *ean, int qnt, 
                        int ind_prod, int pos);

//PRODUTOS E IVA

/** Calcula o valor total de um item com base no preco e taxa de IVA.
 * @param preco      preco unitario sem iva
 * @param perc_iva   percentagem da taxa (ex: 23)
 * @param quantidade unidades compradas
 * @return Retorna o valor total calculado.
 */
double calcular_total(double preco, int perc_iva, int quantidade);

/** Obtem a percentagem numérica associada a um codigo de IVA (A-Z).
 * @param sys        informacao sistema
 * @param codigo_iva letra identificadora da taxa
 * @return Retorna a percentagem (inteiro).
 */
int obter_perc_iva(Sys *sys, char codigo_iva);

/** Imprime os detalhes de um produto formatados para o comando a.
 * @param sys   informacao sistema
 * @param p     ponteiro para o produto
 * @param q     quantidade adicionada
 */
void imprimir_produto(Sys *sys, Produto *p, int q);

/** Cria ou atualiza um produto no inventario do sistema.
 * @param sys        informacao sistema
 * @param ean        string ean
 * @param iva        codigo da taxa iva
 * @param preco      preco unitario
 * @param quantidade quantidade em stock
 * @param descricao  descricao do produto
 * @return Retorna 0 se o input for invalido, 1 em caso de sucesso.
 */
int criar_produto(Sys *sys, char *ean, char iva, double preco,
                int quantidade, char *descricao);

/** Calcula as vendas considerando o stock vendido e o que esta no cesto.
 * @param sys            informacao sistema
 * @param indice_produto posicao do produto no array
 * @return Retorna a soma da quantidade vendida e reservada no cesto.
 */
int calcular_vendas_totais(Sys *sys, int indice_produto);

/** Procura um produto no inventario atraves do seu codigo EAN.
 * @param sys informacao sistema
 * @param ean string ean a procurar
 * @return Retorna o indice no array ou -1 se nao for encontrado.
 */
int procurar_produto(Sys *sys, char *ean);

/** Verifica se um texto corresponde a um padrao.
 * @param padrao string com wildcards
 * @param texto  string ean para comparar
 * @return Retorna 1 se houver correspondencia, 0 caso contrario.
 */
int wildcard_corresponde(char *padrao, char *texto);
 
/** Carrega as taxas de iva a partir de um ficheiro.
 * @param sys           informacao sistema
 * @param nome_ficheiro caminho para o ficheiro de configuracao
 */
void carregar_ivas(Sys *sys, char *nome_ficheiro);

/** Inicializa as taxas de iva com valores por omissao.
 * @param sys informacao sistema
 */
void criar_ivas(Sys *sys);


//CESTO

/** Determina a posicao de ordenacao de um produto no cesto.
 * @param sys informacao sistema
 * @param ean string ean
 * @param p   ponteiro para o produto
 * @return Retorna o indice da posicao no cesto.
 */
int ordenar(Sys *sys, char *ean, Produto *p);

/** Procura um produto especifico dentro do cesto de compras.
 * @param sys informacao sistema
 * @param ean string ean a procurar
 * @return Retorna o indice no cesto ou -1 se nao encontrado.
 */
int procurar_cesto(Sys *sys, char *ean);

/** Cancela a compra atual, devolvendo os itens do cesto ao stock.
 * @param sys informacao sistema
 */
void cancelamento(Sys *sys);


//PARSING

/** Verifica se a linha de comando contem apenas a letra do comando.
 * @param input string lida do terminal
 * @return Retorna 0 se houver argumentos, 1 se estiver sozinha.
 */
int letra_sozinha(char *input);

/** Extrai e valida o nome do cliente do input (trata aspas e espacos).
 * @param input        string original
 * @param cliente_temp buffer para guardar o nome extraido
 * @param pos_lida     posicao atual no buffer de input
 * @return Retorna 1 se o nome for valido, 0 caso contrario.
 */
int identificar_nome(char *input, char *cliente_temp, int pos_lida);

/** Processa e separa os argumentos especificos de uma fatura (NIF e Nome).
 * @param input        string de input
 * @param nif          buffer string para o nif
 * @param nif_val      ponteiro para guardar o valor numerico do nif
 * @param cliente_temp buffer para o nome do cliente
 * @return Retorna 1 se for valido, -1 em caso de erro.
 */
int argumentos_fatura(char *input, char *nif, unsigned long *nif_val,
                    char *cliente_temp);

// OUTPUT

/** Imprime o resumo de stock e vendas de um produto (comando r).
 * @param sys informacao sistema
 * @param ean string ean
 */
void resumo_produto(Sys *sys, char *ean);

/** Lista todos os itens presentes no cesto de compras atual.
 * @param sys informacao sistema
 */
void listar_cesto(Sys *sys);

/** Imprime uma linha formatada com os dados de um produto (comando l).
 * @param sys informacao sistema
 * @param i   indice do produto no inventario
 */
void imprimir_linha_produto(Sys *sys, int i);

/** Lista todos os produtos registados no sistema.
 * @param sys informacao sistema
 */
void lista_produtos(Sys *sys);

/** Lista as faturas emitidas, podendo filtrar por um cliente especifico.
 * @param sys          informacao sistema
 * @param nome_cliente nome do cliente ou NULL para listar todas
 */
void lista_faturas(Sys *sys, char *nome_cliente);

/** Imprime o resumo global de faturacao do sistema.
 * @param sys informacao sistema
 */
void resumo_faturacao(Sys *sys);


//SISTEMA

/** Atualiza os dados de um produto ja existente no inventario.
 * @param sys   informacao sistema
 * @param ean   string ean
 * @param iva   novo codigo iva
 * @param preco novo preco
 * @param qnt   quantidade a somar ao stock
 * @param desc  nova descricao
 * @param idx   indice do produto
 */
void atualiza_sistema(Sys *sys, char *ean, char iva, 
                    double preco, int qnt, char *desc, int idx);

/** Ciclo principal que le e despacha os comandos do utilizador.
 * @param sys informacao sistema
 * @return Retorna 0 ao terminar a execucao.
 */
int executar_comandos(Sys *sys);

/** Atualiza o stock de um produto apos uma operacao.
 * @param sys informacao sistema
 * @param ean string ean
 * @param qtd quantidade a alterar
 */
void atualiza_produto(Sys *sys, char *ean, int qtd);

/** Aloca memoria e inicializa as estruturas do sistema.
 * @param argc numero de argumentos do programa
 * @param argv array de argumentos (pode conter ficheiro de IVAs)
 * @return Retorna ponteiro para o sistema criado ou NULL se falhar.
 */
Sys* inicializar_sistema(int argc, char *argv[]);

/** Liberta toda a memoria dinamica associada ao sistema.
 * @param sys informacao sistema
 */
void libertar_sistema(Sys *sys);


//FATURAS

/** Processa o cesto e regista a fatura final no sistema.
 * @param sys  informacao sistema
 * @param nome nome do cliente
 * @param nif  nif do cliente
 */
void finalizar_registo_fatura(Sys *sys, char *nome, unsigned long nif);

/** Remove uma fatura do historico atraves do seu numero sequencial.
 * @param sys    informacao sistema
 * @param numero identificador da fatura
 */
void apaga_fatura(Sys *sys, int numero);

