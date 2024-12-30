// Para compilar:
// g++ rainhas.cpp -o rainhas.exe -Wall

#include <iostream> 
#include <cstdlib>
#include <set>
#include <vector>
#include <stack>

// Funções para debug:
void imprime_vetor_de_naturais(unsigned int x, unsigned int* S);
void imprime_conjunto_de_naturais(std::set<unsigned int>& omega);
void imprime_vetor_de_conjuntos_de_naturais(unsigned int x, std::vector<std::set<unsigned int>>& E);
void imprime_matriz_quadrada_de_conjuntos_de_naturais(unsigned int x, std::vector<std::vector<std::set<unsigned int>>>& mem);

// x        : número de possibilidades de valores para as
//            coordenadas de uma casa de um (2, x)-tabuleiro;
// n_sol    : número de soluções encontradas;
// R        : conjunto de soluções;
// mem      : memória de remoções de possibilidades.
void salva_solucao(unsigned int x, unsigned int* n_sol, unsigned int*** R,
                    std::vector<std::vector<std::set<unsigned int>>>& mem)
{
    // Incrementa o número de soluções:
    (*n_sol)++;
    // Salva a solução:
    (*R) = (unsigned int**)realloc((*R), sizeof(unsigned int*)*(*n_sol));
    if(*R == NULL)
    {
        std::cerr << "Erro de alocação de memória." << std::endl;
        exit(1);
    }
    (*R)[(*n_sol)-1] = (unsigned int*)malloc(sizeof(unsigned int)*x); 
    for(unsigned int i = 0; i <= x-1; i++)
    {
        (*R)[(*n_sol)-1][i] = *(mem[i][i].begin());
    }
}

// i        : índice de rainha adicionada em solução parcial;
// j        : índice de profundidade;
// valor    : possibilidade restringida;
// E        : espaço de possibilidades;
// mem      : memória de remoções.
bool remove_possibilidade(unsigned int i, unsigned int j, unsigned int valor, std::vector<std::set<unsigned int>>& E,
                                            std::vector<std::vector<std::set<unsigned int>>>& mem)
{
    // Tenta remover do espaço de possibilidades da (i+j+1)-ésima posição,
    // uma das possibilidades não aceitas pela (i+1)-ésima rainha:
    size_t r = E[i+j].erase(valor);
    // Se fez remoção:
    if(r)
    {
        // Salva a remoção na memória:
        mem[i+j][i].insert(valor);
        // Se a remoção zerou as possibilidades:
        if(E[i+j].empty())
        {
            // Retorna sinal positivo:
            return true;
        }
    }
    // Retorna sinal negativo:
    return false;
}

// i    : índice de rainha adicionada em solução parcial;
// j    : índice de profundidade;
// E    : espaço de possibilidades;
// mem  : memória de remoções.
bool remove_possibilidades(unsigned int i, unsigned int j, std::vector<std::set<unsigned int>>& E,
                                std::vector<std::vector<std::set<unsigned int>>>& mem)
{
    // Tenta remover do espaço de possibilidades da (i+j+1)-ésima posição,
    // a possibilidade de magnitude j unidades a esquerda da possibilidade
    // usada pela (i+1)-ésima rainha:
    if(remove_possibilidade(i, j, *(mem[i][i].begin())-j, E, mem))
    {
        // Se removeu zerando as possibilidades, retorna sinal positivo:
        return true;
    }
    // Tenta remover do espaço de possibilidades da (i+j+1)-ésima posição,
    // a possibilidade usada pela (i+1)-ésima rainha:
    if(remove_possibilidade(i, j, *(mem[i][i].begin()), E, mem))
    {
        // Se removeu zerando as possibilidades, retorna sinal positivo:
        return true;
    }
    // Tenta remover do espaço de possibilidades da (i+j+1)-ésima posição,
    // a possibilidade de magnitude j unidades a direita da possibilidade
    // usada pela (i+1)-ésima rainha:
    if(remove_possibilidade(i, j, *(mem[i][i].begin())+j, E, mem))
    {
        // Se removeu zerando as possibilidades, retorna sinal positivo:
        return true;
    }
    // Retorna sinal negativo para zeramento de possibilidades:
    return false;
}

// i    : índice de rainha adicionada em solução parcial;
// j    : índice de profundidade;
// E    : espaço de possibilidades.
// mem  : memória de remoções.
void reinsere_possibilidades_de_uma_posicao(unsigned int i, unsigned int j, std::vector<std::set<unsigned int>>& E,
                                    std::vector<std::vector<std::set<unsigned int>>>& mem)
{
    // Enquanto houver possibilidades removidas da (i+j+1)-ésima posição na inserção da (i+1)-ésima rainha:
    while(!(mem[i+j][i].empty()))
    {
        // Insere possibilidade removida pela inserção da (i+1)-ésima rainha:
        E[i+j].insert(*(mem[i+j][i].begin()));
        // Remove a possibilidade da memória de removidos:
        mem[i+j][i].erase(mem[i+j][i].begin());
    }
}

// x    : número de possibilidades de valor de coordenada de dimensão de um espaço;
// i    : índice de rainha adicionada em solução parcial;
// E    : espaço de possibilidades.
// mem  : memória de remoções.
void reinsere_possibilidades(unsigned int x, unsigned int i, std::vector<std::set<unsigned int>>& E,
                                    std::vector<std::vector<std::set<unsigned int>>>& mem)
{
    // Para todas as profundidades a partir da posição corrente:
    for(unsigned int j = 1; j <= (x-1)-i; j++)
    {   
        // Reinsere possibilidades cabíveis a (i+j+1)-ésima rainha
        // referente as eliminadas pelo posicionamento da (i+1)-ésima rainha:
        reinsere_possibilidades_de_uma_posicao(i, j, E, mem);
    }
    // Remove o uso da possibilidade da memória: 
    mem[i][i].erase(mem[i][i].begin());
}

// Estrutura de estado de produção da gramática
// desenvolvida para heurística observada:
typedef struct Estado
{
    unsigned int i; // índice de profundidade.
    unsigned int r; // coordenada de rainha.
} estado;

// x        : número de possibilidades de valor de coordenada de dimensão de um espaço;
// n_sol    : número de soluções encontradas;
// R        : soluções encontradas.
void gera_solucoes(unsigned int x, unsigned int* n_sol, unsigned int*** R)
{
    // Conjunto de possibilidades de coordenadas de posicionamento das rainhas:
    std::set<unsigned int> omega;

    // Memória de remoções de possibilidades:
    std::vector<std::vector<std::set<unsigned int>>> mem(x);

    // Pilha para executar recursão:
    std::stack<estado> pilha;

    // Para todas as possibilidades (e referente a memória, posições):
    for(unsigned int p = 0; p <= x-1; p++)
    {
        // Salva a possibilidade pelo seu índice de ordem:
        omega.insert(p);
        
        // Cria vetor de p+1 conjuntos, dos quais, cada um
        // representa as possibilidades removidas do espaço
        // de possibilidades da (p+1)-ésima componente de
        // solução (coordenada de rainha), pela inserção
        // de uma rainha na posição de componente respectiva
        // ao seu índice de coluna no vetor de conjuntos:
        mem[p] = std::vector<std::set<unsigned int>>(p+1);
        // Note que, como as rainhas são inseridas sequencialmente,
        // basta uma matriz triangular para representar as remoções.

        // Empilha estado inicial:
        pilha.push({0, p});
    }

    // Cria espaço de possibilidades:
    std::vector<std::set<unsigned int>> E(x, omega);

    // Enquanto houver estados:
    while(!(pilha.empty()))
    {
        // Pega estado no topo da pilha:
        estado q = pilha.top();
        
        // Desempilha estado:
        pilha.pop();

        // Salva uso da possibilidade atual na memória:
        mem[q.i][q.i].insert(q.r);

        // Se o índice do estado atual é o de uma
        // possibilidade para rainha em fim de solução:
        if(q.i == x-1)
        {
            // Completou uma solução:
            salva_solucao(x, n_sol, R, mem);

            // Se há próximo estado:
            if(!(pilha.empty()))
            {
                // Pega o próximo estado (do qual,
                // a posição de inserção é anterior a atual):
                estado aux = pilha.top();

                // Para todas as posições entre as do estado
                // atual e do próximo estado:
                for(unsigned int i = aux.i; i <= q.i; i++)
                {
                    // Reinsere as possibilidades eliminadas pelo
                    // posicionamento da (i+1)-ésima rainha:
                    reinsere_possibilidades(x, i, E, mem);
                }
            }
        } else
        {
            // Bandeira para sinalizar se as remoções seguintes
            // zeram ou não as possibilidades para alguma posição:  
            bool zerou = false;

            // Para todas as profundidades seguintes:
            for(unsigned int j = 1; j <= (x-1)-q.i; j++)
            {
                // Faz remoções exigidas pela (i+1)-ésima rainha:
                if(remove_possibilidades(q.i, j, E, mem))
                {
                    // Se zerou as possibilidades da (i+j+1)-ésima posição,
                    // para todas as profundidades que tiveram remoções por
                    // causa da (i+1)-ésima rainha:
                    for(unsigned int k = 1; k <= j; k++)
                    {
                        // Reinsere as possibilidades removidas:
                        reinsere_possibilidades_de_uma_posicao(q.i, k, E, mem);
                    }
                    // Remove o uso da possibilidade da memória: 
                    mem[q.i][q.i].erase(mem[q.i][q.i].begin());

                    // Atualiza a bandeira:
                    zerou = true;
                    
                    // Sai do loop:
                    break;
                }
            }

            // Se zerou as possibilidades de alguma posição:
            if(zerou)
            {
                // Se há próximo estado:
                if(!(pilha.empty()))
                {
                    // Pega o próximo estado (do qual, a posição
                    // de inserção é anterior ou igual a atual):
                    estado aux = pilha.top();

                    // Para todas as posições entre as do próximo
                    // estado e do atual menos um (porque as remoções
                    // causadas pela (i+1)-ésima rainhas já foram
                    // restauradas no loop anterior):
                    for(unsigned int i = aux.i; int(i) <= int(q.i-1); i++)
                    {
                        // Reinsere as possibilidades eliminadas pelo
                        // posicionamento de uma (i+1)-ésima rainha:
                        reinsere_possibilidades(x, i, E, mem);
                    }
                }
            } else
            { // Senão:
                // Para todas as possibilidades da posição seguinte:
                for(auto e : E[q.i+1])
                {
                    // Empilha o uso da possibilidade:
                    pilha.push({q.i+1, e});
                }
            }
        }
    }
}

#include <cmath> // abs

// x: número de possibilidades de valores para as
//    coordenadas de uma casa de um (2, x)-tabuleiro;
// S: suposta solução.
bool eh_solucao(unsigned int x, unsigned int* S)
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
    // Número de possibilidades de valores para as coordenadas de uma casa de um (2, x)-tabuleiro:
    unsigned int x;
    std::cout << "Entre com um número de possibilidades por dimensão desejado: ";
    std::cin >> x;
    
    // Se número nula:
    if(!x)
    {
        std::cout << "Erro. O número de possibilidades deve ser um natural não nulo." << std::endl;
        return 0;
    }
    
    // Conjunto de soluções:
    unsigned int** R = (unsigned int**)malloc(sizeof(unsigned int*));
    // Número de soluções:
    unsigned int n_sol = 0;
    // Gera as soluções:
    gera_solucoes(x, &n_sol, &R);
    // Número de falsas soluções:
    unsigned int n_f_sol = 0;
    // Para todas as supostas soluções:
    for(unsigned int i = 0; i < n_sol; i++)
    {
        // Se não for de fato solução:
        if(!eh_solucao(x, R[i]))
        {
            n_f_sol++;
        }
    }
    std::cout << "Número de sequências geradas que não são solução do problema: " << n_f_sol << std::endl;
    std::cout << "Número de soluções encontradas para o problema (2, " << x << ")-Rainhas Padrão: " << n_sol-n_f_sol << std::endl;

    // Libera a memória alocada:
    for(unsigned int i = 0; i < n_sol; i++)
    {
        // Libera a (i+1)-ésima solução:
        free(R[i]);
    }
    free(R);
    return 0;
}

// x: número de elementos;
// S: vetor de naturais a ser imprimido.
void imprime_vetor_de_naturais(unsigned int x, unsigned int* S)
{
    // Abre vetor:
    std::cout << "[";

    // Para todos os elementos exceto o último:
    for(unsigned int i = 0; i < x-1; i++)
    {
        // Imprime elemento e separador:
        std::cout << S[i] << ", ";
    }
    // Imprime o último elemento:
    if(x) std::cout << S[x-1];
    
    // Fecha vetor:
    std::cout << "]" << std::endl;
}

// omega: conjunto de números naturais a ser imprimido.
void imprime_conjunto_de_naturais(std::set<unsigned int>& omega)
{
    // Abre conjunto:
    std::cout << "{";

    // Se o conjunto não está vazio:
    if(!omega.empty())
    {
        // Inicia iterador:
        auto it = omega.begin();
        // Para todos os elementos, exceto o último:
        for(auto proximo_it = std::next(it); proximo_it != omega.end(); it++, proximo_it++)
        {
            // Imprime com separador:
            std::cout << *it << ", ";
        }
        // Imprime o último sem separador:
        std::cout << *it;
    }

    // Fecha conjunto:
    std::cout << "}";
}

// x: número de conjuntos de naturais;
// E: vetor de conjuntos de naturais a ser imprimido.
void imprime_vetor_de_conjuntos_de_naturais(unsigned int x, std::vector<std::set<unsigned int>>& E)
{
    // Abre vetor:
    std::cout << "[";

    // Para todos os conjuntos:
    for(unsigned int i = 0; i < x-1; i++)
    {
        // Imprime conjunto:
        imprime_conjunto_de_naturais(E[i]);
        // Imprime separador:
        std::cout << ", ";
    }

    // Se tem pelo menos um conjunto:
    if(x)
    {
        // Imprime conjunto:
        imprime_conjunto_de_naturais(E[x-1]);
    }

    // Fecha vetor:
    std::cout << "]";
}

// x    : número de vetores de conjuntos de naturais;
// mem  : matriz de conjuntos de naturais a ser imprimida.
void imprime_matriz_quadrada_de_conjuntos_de_naturais(unsigned int x, std::vector<std::vector<std::set<unsigned int>>>& mem)
{
    // Abre matriz:
    std::cout << "[";

    // Para todos os vetores de conjuntos:
    for(unsigned int i = 0; i < x-1; i++)
    {
        // Imprime vetor de conjuntos:
        imprime_vetor_de_conjuntos_de_naturais(x, mem[i]);
        // Imprime separador:
        std::cout << "," << std::endl;
    }

    // Se tem pelo menos um vetor de conjuntos:
    if(x)
    {
        // Imprime vetor de conjuntos:
        imprime_vetor_de_conjuntos_de_naturais(x, mem[x-1]);
    }

    // Fecha matriz:
    std::cout << "]" << std::endl;

}