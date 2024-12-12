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

void gera(unsigned int x, float* e, unsigned int k, unsigned int* n_sol, float*** R, float** S, unsigned int** memoria)
{
    // Se posicionou todas as rainhas completando todos os níveis de profundidade:
    if(k == x)
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
        // Índice de ordem baricentrica da componente da solução parcial, uma posição à esquerda da componente alvo:
        unsigned int t = (*S)[k-1]-e[0];
        // Variável auxiliar para níveis de profundidade:
        int i;
        // Para todas as possibilidades menores que a t-ésima possibilidade do espaço de possibilidades ordenado:
        for(int j = 0; j <= int(t-2); j++)
        {
            // Se valor não utilizado:
            if(!(*memoria)[j])
            {
                // Insere elemento posterior válido em magnitude:
                (*S)[k] = e[j];
                // Marca o índice como utilizado na memória:
                (*memoria)[j] = 1;
                // Para todos os níveis de comparação relacionados aos alvos já inseridos:
                for(i = 1; i <= k; i++)
                {
                    // Se não passa em algum dos níveis:
                    if(((*S)[k-i]-(*S)[k] == -i) || ((*S)[k-i]-(*S)[k] == i))
                    {
                        // Então não configura uma solução parcial.
                        break;
                    }
                }
                // Se passou por todos os níveis possíveis até então, é solução parcial:
                if(i == k+1)
                {
                    // Gera soluções recursivamente:
                    gera(x, e, k+1, n_sol, R, S, memoria);
                }
                // Desmarca o índice da memória:
                (*memoria)[j] = 0;
            }
        }
        // Para todas as possibilidades maiores que a t-ésima possibilidade do espaço de possibilidades ordenado: 
        for(int j = t+2; j <= int(x-1); j++)
        {
            // Se valor não utilizado:
            if(!(*memoria)[j])
            {
                // Insere elemento posterior válido em magnitude:
                (*S)[k] = e[j];
                // Marca o índice como utilizado na memória:
                (*memoria)[j] = 1;
                // Para todos os níveis de comparação relacionados aos alvos já inseridos:
                for(i = 1; i <= k; i++)
                {
                    // Se não passa em algum dos níveis:
                    if(((*S)[k-i]-(*S)[k] == -i) || ((*S)[k-i]-(*S)[k] == i))
                    {
                        // Então não configura uma solução parcial.
                        break;
                    }
                }
                // Se passou por todos os níveis possíveis até então, é solução parcial:
                if(i == k+1)
                {
                    // Gera soluções recursivamente:
                    gera(x, e, k+1, n_sol, R, S, memoria);
                }
                // Desmarca o índice da memória:
                (*memoria)[j] = 0;
            }
        }
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
    // Gera as soluções para todas as possibilidades iniciais:
    for(int i = 0; i <= int(x-1); i++)
    {
        // Inicia a solução:
        S[0] = e[i];
        // Marca o índice como utilizado na memória:
        memoria[i] = 1;
        gera(x, e, 1, n_sol, R, &S, &memoria);
        memoria[i] = 0;
    }
    // Libera a memória alocada:
    free(S);
    free(memoria);
}

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
    // std::cout << "Soluções:" << std::endl;
    // Para todas as soluções:
    // for(unsigned int i = 0; i < n_sol; i++)
    // {
    //     // Se não for de fato solução:
    //     if(!eh_solucao(x, R[i]))
    //     {
    //         // Imprime mensagem de erro:
    //         std::cout << "Erro: há solução inválida." << std::endl;
    //         // Zera o número de soluções:
    //         n_sol = 0;
    //         // Sai do loop:
    //         break;
    //     } else
    //     {
    //         // Imprime a (i+1)-ésima solução:
    //         // imprime_vetor_de_float(x, R[i]);
    //     }
    // }
    std::cout << "Número de soluções encontradas do problema (2, " << x << ")-Rainhas Padrão: " << n_sol << std::endl;

    // Libera a memória alocada:
    for(unsigned int i = 0; i < n_sol; i++)
    {
        // Libera a (i+1)-ésima solução:
        free(R[i]);
    }
    free(R);
    return 0;
}