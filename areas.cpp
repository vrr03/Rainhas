// Para compilar:
// g++ areas.cpp -o areas.exe -Wall

#include <cmath> // abs
#include <cstdlib>
#include <iomanip> // fixed e setprecision
#include <iostream>
#include <set>
#include <stack>
#include <vector>

void gera_solucoes(unsigned int x, unsigned int* n_sol, unsigned int*** R);

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
    std::cout << "]";
}

// x:           número de vértices;
// poligono:    vértices de um polígono (convexo ou não) sem entrelaçamentos.
float area(unsigned int x, std::vector<std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>>>& poligono)
{
    // Inicia soma com zero:
    float soma = 0;
    
    // Para todo par de vértices sequentes em loop:
    for(unsigned int i = 0; i < x; i++)
    {
        // Calcula índice do próximo vértice:
        unsigned int prox = (i+1)%x;
        // Acumula a diferença do produto cruzado:
        soma += float(poligono[i].second.first*poligono[prox].second.second)-float(poligono[i].second.second*poligono[prox].second.first);
    }

    // Retorna a área pela fórmula de Shoelace:
    return (std::fabs(soma)/2.0);
}

#include <algorithm>
#include <utility>

// V:           vértice a ser inserido;
// poligono:    vértices do polígono atual.
void insere_vertice_ordenado_em_sentido_antihorario(std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>>& V,
                                        std::vector<std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>>>& poligono)
{
    // Encontra a posição do par a ser inserido por comparação de ordem crescente de ângulo e decrescente de raio: 
    auto it = std::lower_bound(poligono.begin(), poligono.end(), V,
        [](const std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>>& a,
           const std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>>& b)
           {
               if(a.first.second != b.first.second)
               {
                   return a.first.second < b.first.second; 
               }
               return a.first.first > b.first.first;
           });

    // Insere o novo par na posição encontrada:
    poligono.insert(it, V);
}

// O: coordenadas da origem de um sistema cartesiano;  
// P: coordenadas cartesianas de um ponto no sistema.
std::pair<float, float> transforma_em_coordenadas_polares(std::pair<float, float> O, std::pair<float, float> P)
{
    // Calcula a distância entre a origem e o ponto:
    // (a norma do vetor com segmento representante OP)
    float r = std::sqrt(std::pow(P.first-O.first, 2)+std::pow(P.second-O.second, 2));

    // Componente de ângulo:
    float angulo;

    if(P.first > O.first)
    { // Se o ponto está no 1º ou 4º quadrante ou sobre o eixo x:
        if(P.second > O.second)
        { // Se o ponto está no 1° quadrante:
            // x = r*cos(t) -> t = arccos(x/r):
            // (arccos -> [0, pi]).
            angulo = std::acos((P.first-O.first)/r);
            // y = r*sen(t) -> t = arcsen(y/r):
            // (arcsen -> [-pi/2, pi/2]).
            // angulo = std::asin((P.second-O.second)/r);
            // t = arctan(y/x):
            // (arctan -> [-pi/2, pi/2]).
            // angulo = std::atan((P.second-O.second)/(P.first-O.first));
        } else if(P.second < O.second)
        { // Senão, se o ponto está no 4° quadrante:
            angulo = 2*M_PI-std::acos((P.first-O.first)/r);
            // angulo = 2*M_PI+std::asin((P.second-O.second)/r);
            // angulo = 2*M_PI+std::atan((P.second-O.second)/(P.first-O.first));
        } else
        { // Senão, o ponto está sobre o eixo x (entre o 1º e o 4º quadrante):
            // (O ponto está a direita da origem).
            angulo = 0;
        }
    } else if(P.first < O.first)
    { // Senão, se o ponto está no 2° ou 3° quadrante ou sobre o eixo x:
        if(P.second > O.second)
        { // Se o ponto está no 2° quadrante:
            angulo = std::acos((P.first-O.first)/r);
            // angulo = M_PI-std::asin((P.second-O.second)/r);
            // angulo = M_PI+std::atan((P.second-O.second)/(P.first-O.first));
        } else if(P.second < O.second)
        { // Senão, se o ponto está no 3° quadrante:
            angulo = 2*M_PI-std::acos((P.first-O.first)/r);
            // angulo = M_PI-std::asin((P.second-O.second)/r);
            // angulo = M_PI+std::atan((P.second-O.second)/(P.first-O.first));
        } else
        { // Senão, o ponto está sobre o eixo x (entre o 2º e o 3º quadrante):
            // (O ponto está a esquerda da origem).
            angulo = M_PI;
        }
    } else
    { // Senão, se o ponto está sobre o eixo y:
        if(P.second > O.second)
        { // Se o ponto está entre o 1° e o 2° quadrante:
            // (O ponto forma ângulo de 90° com a origem).
            angulo = M_PI/2.;
        } else if(P.second < O.second)
        { // Senão, se o ponto está entre o 3° e o 4° quadrante:
            // (O ponto forma ângulo de 270° com a origem).
            angulo = 3*M_PI/2.;
        } else
        { // Senão, o ponto está sobre o eixo x e sobre o eixo y:
            // (O ponto coincide com a origem).
            angulo = 0;
        }
    }

    // Retorna o par de coordenadas polares:
    return {r, angulo};
}

// x: número de vértices;
// S: solução de um problema (2, x)-Rainhas.
std::vector<std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>>> ordena_vertices(unsigned int x, unsigned int* S)
{
    // Cria vetor de pares de ângulo com pares de coordenadas para representar
    // o polígono convexo formado por uma solução do problema (2, x)-Rainhas:
    std::vector<std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>>> poligono;

    // Coordenadas do centro do tabuleiro:
    float C = float(x-1)/2.0;

    // Para todas as rainhas:
    for(unsigned int k = 0; k < x; k++)
    {
        // Coordenadas polares da (k+1)-ésima rainha: 
        auto P = transforma_em_coordenadas_polares({C, C}, {float(k), float(S[k])});

        // Salva par de coordenadas da rainha nos dois sistemas de coordenadas: 
        std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>> R = {P, {k, S[k]}};
        
        // Insere o par em ordem crescente de ângulo e decrescente de raio (se mesmo ângulo):
        insere_vertice_ordenado_em_sentido_antihorario(R, poligono);
    }

    // Retorna os vértices ordenados:
    return poligono;
}

void imprime_vetor_de_pares(const std::vector<std::pair<std::pair<float, float>, std::pair<unsigned int, unsigned int>>>& vetor)
{
    for(const auto& par : vetor)
    {
        std::cout << "(" << par.second.first << ", " << par.second.second << ") <---> ";
        std::cout << "(" << par.first.first << ", " << par.first.second*(180.0/M_PI) << "°)\n";
    }
}

int main(int argc, char const *argv[])
{
    // Número de possibilidades de valores para as coordenadas de uma casa de um (2, x)-tabuleiro:
    unsigned int x;
    std::cout << "Entre com um número de possibilidades por dimensão desejado: ";
    std::cin >> x;
    
    // Se número nulo:
    if(!x)
    {
        std::cerr << "Erro. O número de possibilidades deve ser um natural não nulo." << std::endl;
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
    // Áreas distintas das configurações de polígonos a partir das soluções:
    std::set<float> areas_distintas;
    // Para todas as supostas soluções:
    for(unsigned int i = 0; i < n_sol; i++)
    {
        // Se não for de fato solução:
        if(!eh_solucao(x, R[i]))
        {
            n_f_sol++;
        } else
        { // Senão:
            // Imprime a solução:
            imprime_vetor_de_naturais(x, R[i]);
            std::cout << " --> ";
            std::cout << std::endl;
            
            // Configura o polígono relativo a solução:
            auto poligono = ordena_vertices(x, R[i]);
            imprime_vetor_de_pares(poligono);
            // Calcula a área:
            float A = area(x, poligono);
            std::cout << "Área = " << std::fixed << std::setprecision(2) << A;
            std::cout << std::endl;
            // Insere a área no conjunto se ainda não foi inserida:
            areas_distintas.insert(A);
        }
    }
    std::cout << "Número de sequências geradas que não são solução do problema: " << n_f_sol << std::endl;
    std::cout << "Número de soluções encontradas para o problema (2, " << x << ")-Rainhas Padrão: " << n_sol-n_f_sol << std::endl;
    std::cout << "Número de áreas distintas: " << areas_distintas.size() << std::endl;

    // Libera a memória alocada:
    for(unsigned int i = 0; i < n_sol; i++)
    {
        // Libera a (i+1)-ésima solução:
        free(R[i]);
    }
    free(R);

    return 0;
}

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