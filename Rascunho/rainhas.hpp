#ifndef RAINHAS_HPP
#define RAINHAS_HPP

class Rainhas
{
    public:
        // Construtor:
        Rainhas(unsigned int x);
        // Destrutor:
        ~Rainhas();
    private:
        // Número de casas por dimensão:
        unsigned int x;
        // Número de soluções encontrado:
        unsigned long n_sol;
        // Soluções:
        float** R;
};

#endif