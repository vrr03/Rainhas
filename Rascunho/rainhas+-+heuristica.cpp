#include <iostream> 
#include <cstdlib>

// x: número de possibilidades de valores para uma componente de coordenada de casa de um (d, x)-tabuleiro;
// c: componente de coordenada do centro do (d, x)-tabuleiro.
float* intervalo(unsigned int x, float c)
{
    // Aloca as possibilidades:
    float* e = (float*)malloc(sizeof(float)*x);
    // Inicia limite esquerdo:
    e[0] = c-((float(x)-1.)/2.);
    // Para todos as demais possibilidades:
    for(unsigned int i = 1; i < x; i++)
    {
        // Incrementa 1 do anterior:
        e[i] = e[i-1] + 1.;
    }
    // Retorna o espaço de possibilidades:
    return e;
}

void imprime_vetor_de_float(unsigned int x, float* e)
{
    std::cout << "[";
    for(unsigned int i = 0; i < x-1; i++)
    {
        std::cout << e[i] << ", ";
    }
    if(x) std::cout << e[x-1];
    
    std::cout << "]" << std::endl;
}

void troca(float *a, float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

// Função recursiva para gerar permutações de um conjunto de x elementos distintos.
void permutacoes_simples_recursivo(unsigned int x, float *e, unsigned int inicio, unsigned int fim, unsigned int* contador)
{
    // Quando chegar no fim:
    if(inicio == fim)
    {
        // Imprime a permutação:
        imprime_vetor_de_float(x, e);
        (*contador)++;
    } else
    {
        for(unsigned int i = inicio; i <= fim; i++)
        {
            // Troca os valores:
            troca((e + inicio), (e + i));
            // Chama a função recursivamente:
            permutacoes_simples_recursivo(x, e, inicio + 1, fim, contador);
            // Desfaz a troca para voltar ao estado anterior:
            troca((e + inicio), (e + i));
        }
    }
}

// Função iterativa (via algoritmo de heap) para gerar permutações de um conjunto de x elementos distintos.
void permutacoes_simples_iterativo(unsigned int x, float* e, unsigned int* contador)
{
    // Vetor de controle de troca (contador para cada componente).
    unsigned int* c = (unsigned int*)malloc(sizeof(unsigned int)*x);
    // Inicia com zero:
    for(unsigned int i = 0; i < x; i++)
    {
        c[i] = 0;
    }

    // Imprime a permutação inicial:
    imprime_vetor_de_float(x, e);
    // Incrementa o contador total:
    (*contador)++;

    unsigned int i = 0;
    while(i < x)
    {
        // Se o índice de controle c[i] é menor que o índice i, troca:
        if(c[i] < i)
        {
            // Se o índice i for par:
            if(i % 2 == 0)
            {
                // Troca o primeiro e o i-ésimo elemento:
                troca(&e[0], &e[i]);
            } else
            {
                // Troca o c[i]-ésimo e o i-ésimo elemento:
                troca(&e[c[i]], &e[i]);
            }

            // Imprime a permutação gerada:
            imprime_vetor_de_float(x, e);
            // Incrementa o contador de controle:
            c[i]++;
            // Incrementa o contador total:
            (*contador)++;
            // Reinicia a busca:
            i = 0;
        } else
        {
            // Reseta o contador para o próximo índice:
            c[i] = 0;
            // Avança para o próximo índice:
            i++;
        }
    }

    // Libera a memória alocada:
    free(c);
}

// Verifica se não há rainhas (ordenadas em relação a uma dimensão)
// sequentes crescente ou decrescentemente em relação à dimensão de
// distribuição baricentrica.
bool suposta_1a_condicao_necessaria(unsigned int x, float *e)
{
    // Para todas as componentes (exceto a última):
    for(unsigned int i = 0; i < x-1; i++)
    {
        // Se a componente correspondente da rainha
        // seguinte (em relação a uma dimensão)
        // for seguinte anterior ou posterior:
        if((e[i+1] - e[i] == -1) || (e[i+1] - e[i] == 1))
        {
            // Então não configura uma solução.
            return false;
        }
    }
    return true;
}
// Observação: a condição da função é necessária mas não é suficiente.
// (Logo, a função não pode ser descrita como verificadora de solução,
// porém, reduz e muito a quantidade de possíveis soluções).

#include <cmath> // abs
bool eh_solucao(unsigned int x, float* S)
{
    // Para todas as componentes de linha das rainhas (exceto da última):
    for(unsigned int j = 0; j < x-1; j++)
    {
        // Para todas as componentes de linha das rainhas (do loop anterior em diante):
        for(unsigned int i = j+1; i < x; i++)
        {
            // Se estão em mesma linha ou coluna ou diagonal:
            if(S[j] == S[i] || std::abs(int(j - i)) == std::abs(int(S[j] - S[i])))
            {
                // Não é solução.
                return false;
            }
        }
    }
    // É solução.
    return true;
}


// Soluções de um problema (2, x)-Rainhas Padrão. 
// c1: contador de distribuições que passam pela suposta 1ª condição necessária;
// c2: contador de distribuições que passam pela suposta 1ª condição necessária e são soluções;
// c3: contador de distribuições que passam pela suposta 1ª condição necessária e não são soluções.
void solucoes(unsigned int x, float *e, unsigned int inicio, unsigned int fim, unsigned int* c1, unsigned int* c2, unsigned int* c3)
{
    // Quando chegar no fim:
    if(inicio == fim)
    {
        // Se passa pela suposta condição necessária:
        if(suposta_1a_condicao_necessaria(x, e))
        {
            // Incrementa contador de distribuições que passam pela suposta condição necessária:
            (*c1)++;
            // E é solução:
            if(eh_solucao(x, e))
            {
                // Imprime a permutação:
                imprime_vetor_de_float(x, e);
                // Incrementa contador de distribuições que passam pela suposta condição necessária e são soluções:
                (*c2)++;
            } else
            { // Senão:
                // Incrementa contador de distribuições que passam pela suposta condição necessária e não são soluções.
                (*c3)++;
            }
        }
    } else
    {
        for(unsigned int i = inicio; i <= fim; i++)
        {
            // Troca os valores:
            troca((e + inicio), (e + i));
            // Chama a função recursivamente:
            solucoes(x, e, inicio + 1, fim, c1, c2, c3);
            // Desfaz a troca para voltar ao estado anterior:
            troca((e + inicio), (e + i));
        }
    }
}

// c1: contador de distribuições que passam pela suposta 1ª condição necessária;
// c2: contador de distribuições que passam pela suposta 1ª condição necessária e são soluções;
// c3: contador de distribuições que passam pela suposta 1ª condição necessária e não são soluções.
void distribuicoes_que_passam_pela_suposta_1a_condicao_necessaria_mas_nao_sao_solucoes(unsigned int x, float *e, unsigned int inicio, unsigned int fim, unsigned int* c1, unsigned int* c2, unsigned int* c3)
{
    // Quando chegar no fim:
    if(inicio == fim)
    {
        // Se passa pela suposta 1ª condição necessária:
        if(suposta_1a_condicao_necessaria(x, e))
        {
            // Incrementa contador de distribuições que passam pela suposta 1ª condição necessária:
            (*c1)++;
            // E é solução:
            if(eh_solucao(x, e))
            {
                // Incrementa contador de distribuições que passam pela suposta 1ª condição necessária e são soluções:
                (*c2)++;
            } else
            { // Senão:
                // Incrementa contador de distribuições que passam pela suposta 1ª condição necessária e não são soluções.
                (*c3)++;
                // Imprime a permutação:
                imprime_vetor_de_float(x, e);
            }
        }
    } else
    {
        for(unsigned int i = inicio; i <= fim; i++)
        {
            // Troca os valores:
            troca((e + inicio), (e + i));
            // Chama a função recursivamente:
            distribuicoes_que_passam_pela_suposta_1a_condicao_necessaria_mas_nao_sao_solucoes(x, e, inicio + 1, fim, c1, c2, c3);
            // Desfaz a troca para voltar ao estado anterior:
            troca((e + inicio), (e + i));
        }
    }
}

// Verifica se não há rainhas (ordenadas em relação a uma dimensão)
// sequentes crescente ou decrescentemente em magnitude k em relação
// à dimensão de ordem e à de distribuição baricentrica.
bool suposta_k_esima_condicao_necessaria(unsigned int x, float* e, unsigned int k)
{
    // Para todas as componentes (exceto a última):
    for(unsigned int i = 0; i < x-k; i++)
    {
        // Se a componente correspondente da rainha seguinte em maginitude k (em relação a uma dimensão)
        // for seguinte anterior ou posterior em magnitude k da distribuição baricentrica:
        if((e[i+k] - e[i] == int(-k)) || (e[i+k] - e[i] == k))
        {
            // Então não configura uma solução.
            return false;
        }
    }
    return true;
}

void distribuicoes_que_passam_pela_suposta_k_esima_condicao_necessaria_mas_nao_sao_solucoes(unsigned int x, float *e, unsigned int inicio, unsigned int fim, unsigned int k, unsigned int** c1, unsigned int* c2, unsigned int* c3)
{
    // Quando chegar no fim:
    if(inicio == fim)
    {
        // Índice de suposta condição necessária: 
        unsigned int i = 0;
        for(i = 1; i <= k; i++)
        // for(i = k; i >= 1; i--)
        {
            // Se passa pela i-ésima condição necessária:
            if(suposta_k_esima_condicao_necessaria(x, e, i))
            {
                // Incrementa contador de distribuições que passam da suposta 1a à suposta i-ésima condição necessária:
                (*c1)[i-1]++;
            } else
            { // Senão:
                // Supostamente não é solução.
                break;
            }
        }
        // Se passou por todas as supostas condições necessárias:
        if(i == k+1)
        // if(i == 0)
        {
            if(eh_solucao(x, e))
            {
                // Incrementa contador de distribuições que passam da suposta 1a à suposta k-ésima condição necessária e são soluções:
                (*c2)++;
            } else
            { // Senão:
                // Incrementa contador de distribuições que passam da suposta 1a à suposta k-ésima condição necessária e não são soluções:
                (*c3)++;
                // Imprime a permutação:
                imprime_vetor_de_float(x, e);
            }
        }
    } else
    {
        for(unsigned int i = inicio; i <= fim; i++)
        {
            // Troca os valores:
            troca((e + inicio), (e + i));
            // Chama a função recursivamente:
            distribuicoes_que_passam_pela_suposta_k_esima_condicao_necessaria_mas_nao_sao_solucoes(x, e, inicio + 1, fim, k, c1, c2, c3);
            // Desfaz a troca para voltar ao estado anterior:
            troca((e + inicio), (e + i));
        }
    }
}

int main()
{
    // Número de possibilidades de valores para as componentes de coordenada de casa de um (d, x)-tabuleiro;
    unsigned int x;
    std::cout << "Entre com o número de possibilidades por dimensão: ";
    std::cin >> x;
    // Centro de um (2, x)-tabuleiro:
    float C[2];
    // Espaços de possibilidades dos valores de componentes de coordenada respectivos as dimensões quais o (2, x)-tabuleiro pertence:
    float** E = (float**)malloc(sizeof(float*)*2);
    // Para todas as dimensões:
    for(unsigned int i = 0; i < 2; i++)
    {
        std::cout << "Entre com a " << i+1 << "a componente do centro do (2, x)-tabuleiro: ";
        std::cin >> C[i];
        // Gera espaço de possibilidade respectivo a dimensão:
        E[i] = intervalo(x, C[i]);
        // Imprime espaço:
        imprime_vetor_de_float(x, E[i]);
    }
    
    // std::cout << "Distribuições baricentricas do primeiro espaço de possibilidades com " << x << " elementos distintos de " << x << " elementos distintos:";
    // std::cout << std::endl;
    // unsigned int total = 0;
    // permutacoes_simples_iterativo(x, E[0], &total);
    // permutacoes_simples_recursivo(x, E[0], 0, x-1, &total);
    // std::cout << "Total = " << x << "! = "<< total << std::endl;
    
    // Número de soluções do respectivo problema (2, x)-Rainhas Padrão:
    // Contador de distribuições que passam pela suposta condição necessária:
    // unsigned int c1 = 0;
    // Contador de distribuições que passam pela suposta condição necessária e são soluções:
    // unsigned int c2 = 0;
    // Contador de distribuições que passam pela suposta condição necessária e não são soluções.
    // unsigned int c3 = 0;
    // std::cout << "Soluções do problema (2, " << x << ")-Rainhas Padrão considerando o primeiro espaço para ordem de representação das rainhas, e o segundo para ordem de distribuição baricentrica:" << std::endl;
    // solucoes(x, E[1], 0, x-1, &c1, &c2, &c3);
    // std::cout << "Distribuições que passam pela suposta 1ª condição necessária mas não são soluções:" << std::endl;
    // distribuicoes_que_passam_pela_suposta_1a_condicao_necessaria_mas_nao_sao_solucoes(x, E[1], 0, x-1, &c1, &c2, &c3);
    // std::cout << "Número de distribuições que passam pela suposta 1ª condição necessária: " << c1 << std::endl;
    // std::cout << "Número de distribuições que passam pela suposta 1ª condição necessária e são soluções: " << c2 << std::endl;
    // std::cout << "Número de distribuições que passam pela suposta 1ª condição necessária e não são soluções: " << c3 << std::endl;

    // Número de soluções do respectivo problema (2, x)-Rainhas Padrão:
    // Número de condições necessárias (profundidade):
    unsigned int k = x-1;
    // Contador de distribuições que passam da suposta 1a às supostas i-ésimas condições necessárias (1 <= i <= k):
    unsigned int* c1 = (unsigned int*)malloc(sizeof(unsigned int)*k);
    for(unsigned int i = 0; i < k; i++)
    {
        c1[i] = 0;
    }
    // Contador de distribuições que passam da suposta 1a à suposta k-ésima condição necessária e são soluções:
    unsigned int c2 = 0;
    // Contador de distribuições que passam da suposta 1a à suposta k-ésima condição necessária e não são soluções:
    unsigned int c3 = 0;
    std::cout << "Distribuições que passam pela suposta k-ésima condição necessária mas não são soluções:" << std::endl;
    distribuicoes_que_passam_pela_suposta_k_esima_condicao_necessaria_mas_nao_sao_solucoes(x, E[1], 0, x-1, k, &c1, &c2, &c3);
    for(unsigned int i = 0; i < k; i++)
    {
        std::cout << "Número de distribuições que passam da suposta 1ª à suposta " << i+1 << "-ésima condição necessária: " << c1[i] << std::endl;
    }
    std::cout << "Número de distribuições que passam da suposta 1ª à suposta " << k << "-ésima condição necessária e são soluções: " << c2 << std::endl;
    std::cout << "Número de distribuições que passam da suposta 1ª à suposta " << k << "-ésima condição necessária e não são soluções: " << c3 << std::endl;

    // Libera a memória alocada:
    for(unsigned int i = 0; i < 2; i++)
    {
        free(E[i]);
    }
    free(E);
    free(c1);
    return 0;
}