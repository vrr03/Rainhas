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

void gera(unsigned int x, float* e, unsigned int bandeira, unsigned int k, unsigned int* n_sol, float*** R, float** S, unsigned int** memoria)
{
    // Se completou todos os níveis de profundidade:
    if(bandeira == x && k == x)
    {
        // Incrementa o número de soluções:
        (*n_sol)++;
        // Salva a solução:
        (*R) = (float**)realloc((*R), sizeof(float*)*(*n_sol));
        (*R)[(*n_sol)-1] = (float*)malloc(sizeof(float)*x); 
        for(unsigned int i = 0; i <= x-1; i++)
        {
            (*R)[(*n_sol)-1][i] = (*S)[i];
        }
    } else
    {
        // Índice de ordem baricentrica da componente da solução parcial, k posições à esquerda da componente alvo:
        unsigned int t = (*S)[bandeira-k]-e[0];

        // Para todas as possibilidades menores que a (t-k)-ésima possibilidade do espaço de possibilidades ordenado:
        for(int j = 0; j <= int((t-k-1)-1); j++)
        {
            // Se valor não utilizado:
            if(!(*memoria)[j])
            {
                // Insere elemento posterior válido em magnitude:
                (*S)[k] = e[j];
                // Marca o índice como utilizado na memória:
                (*memoria)[j] = 1;
                // Gera soluções recursivamente:
                gera(x, e, bandeira+1, k, n_sol, R, S, memoria);
                // Desmarca o índice da memória:
                (*memoria)[j] = 0;
            }
        }
        // Para todas as possibilidades entre a (t-k)-ésima e a t-ésima possibilidade do espaço de possibilidades ordenado:
        for(int j = int(t-k-1)+1; j <= int((t-1)-1); j++)
        {
            // Se valor não utilizado:
            if(!(*memoria)[j])
            {
                // Insere elemento posterior válido em magnitude:
                (*S)[k] = e[j];
                // Marca o índice como utilizado na memória:
                (*memoria)[j] = 1;
                // Gera soluções recursivamente:
                gera(x, e, bandeira+1, k, n_sol, R, S, memoria);
                // Desmarca o índice da memória:
                (*memoria)[j] = 0;
            }
        }
        // Para todas as possibilidades entre a t-ésima e a (t+k)-ésima possibilidade do espaço de possibilidades ordenado:
        for(int j = int(t-1)+1; j <= int((t+k-1)-1); j++)
        {
            // Se valor não utilizado:
            if(!(*memoria)[j])
            {
                // Insere elemento posterior válido em magnitude:
                (*S)[k] = e[j];
                // Marca o índice como utilizado na memória:
                (*memoria)[j] = 1;
                // Gera soluções recursivamente:
                gera(x, e, bandeira+1, k, n_sol, R, S, memoria);
                // Desmarca o índice da memória:
                (*memoria)[j] = 0;
            }
        }
        // Para todas as possibilidades entre a (t+k)-ésima e a x-ésima (com ela inclusa) possibilidade do espaço de possibilidades ordenado:
        for(int j = int(t+k-1)+1; j <= int(x-1); j++)
        {
            // Se valor não utilizado:
            if(!(*memoria)[j])
            {
                // Insere elemento posterior válido em magnitude:
                (*S)[k] = e[j];
                // Marca o índice como utilizado na memória:
                (*memoria)[j] = 1;
                // Gera soluções recursivamente:
                gera(x, e, bandeira+1, k, n_sol, R, S, memoria);
                // Desmarca o índice da memória:
                (*memoria)[j] = 0;
            }
        }
        // Avança para o próximo nível:
        gera(x, e, bandeira, k+1, n_sol, R, S, memoria);
    }
}

void gerador(unsigned int x, float* e, unsigned int* n_sol, float*** R)
{
    // Aloca espaço para uma solução:
    float* S = (float*)malloc(sizeof(float)*x);
    // Aloca memorizador de índices referentes às componentes do espaço de possibilidade que serão utilizadas:   
    unsigned int* memoria = (unsigned int*)malloc(sizeof(unsigned int)*x);
    // Inicia a memória:
    for(int i = 0; i <= int(x-1); i++)
    {
        // O elemento referente ao índice i ainda não foi utlizado:
        memoria[i] = 0;
    }
    // Gera as soluções:
    for(int i = 0; i <= int(x-1); i++)
    {
        // Inicia a solução:
        S[0] = e[i];
        // Marca o índice como utilizado na memória:
        memoria[i] = 1;
        gera(x, e, 1, 1, n_sol, R, &S, &memoria);
        memoria[i] = 0;
    }
    // Libera a memória alocada:
    free(S);
    free(memoria);
}

int main()
{
    // Número de possibilidades de valores para as componentes de coordenada de casa de um (d, x)-tabuleiro;
    unsigned int x;
    std::cout << "Entre com um número de possibilidades por dimensão desejado: ";
    std::cin >> x;
    if(x == 0)
    {
        std::cout << "Erro: o número de possibilidades deve ser um natural não nulo." << std::endl;
        return 0;
    }

    // Segunda componente de coordenada do centro de (2, x)-tabuleiro:
    float centro2;
    std::cout << "Entre com uma segunda componente de coordenada de centro de (2, x)-tabuleiro desejada: ";
    std::cin >> centro2;
    // Gera espaço de possibilidades (em ordem ascendente) respectivo a segunda dimensão.
    float* e = intervalo(x, centro2);
    // Imprime o espaço:
    std::cout << "Espaço de possibilidades:" << std::endl;
    imprime_vetor_de_float(x, e);
    
    // Conjunto de soluções:
    float** R = (float**)malloc(sizeof(float*));
    // Número de soluções:
    unsigned int n_sol = 0;
    // Gera as soluções:
    gerador(x, e, &n_sol, &R);
    std::cout << "Soluções:" << std::endl;
    // Para todas as soluções:
    for(unsigned int i = 0; i < n_sol; i++)
    {
        // Imprime a (i+1)-ésima solução:
        imprime_vetor_de_float(x, R[i]);
    }
    std::cout << "O número de soluções do problema (2, " << x << ")-Rainhas Padrão é: " << n_sol << std::endl;

    // Libera a memória alocada:
    for(unsigned int i = 0; i < n_sol; i++)
    {
        // Libera a (i+1)-ésima solução:
        free(R[i]);
    }
    free(R);
    return 0;
}