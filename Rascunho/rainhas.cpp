#include <cmath> // abs
#include <cstdlib> // malloc, realloc, free, exit, EXIT_FAILURE
#include <iostream> // cout, endl

#include "./rainhas.hpp"

//  Entrada: número de casas por dimensão.
Rainhas::Rainhas(unsigned int x)
{
    // Salva o número de casas por dimensão:
    this->x = x;
    // Inicia o número de soluções salvas:
    this->n_sol = 0;
}

Rainhas::~Rainhas()
{
    // Se concluiu uma busca:
    if(this->n_sol)
    {
        // Para todas as soluções encontradas:
        for(unsigned long k = 0; k < this->n_sol; k++)
        {
            // Libera a (k+1)-ésima solução
            free(this->R[k]);
        }
        // Libera a matriz de soluções:
        free(this->R);
        // Atualiza o número de soluções salvas:
        this->n_sol = 0;
    } // Senão, não faz algo.
}