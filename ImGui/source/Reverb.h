/******************************************************************************/
/*!
\file   Reverb.h
\author Zack Krolikowksi
\par    email: z.krolikowski\@digipen.edu
\par    DigiPen login: z.krolikowski
\par    Course: MAT321
\par    Assignment #2
\date   4/2/2021
*/
/******************************************************************************/
#include <vector>
#include <queue>

template<typename T>
class AllPass
{
public:
    /**
     * @brief Construct a new All Pass object
     * 
     * @param a All pass coeffint
     */
    AllPass(T a) : a(a), x1(0), y1(0) {}

    /**
     * @brief runs all pass on input value
     * 
     * @param x  n'th input
     * @return T n'th output
     */
    T operator()(T x)
    {
        T y = a*(x - y1) + x1;
        x1 = x;
        y1 = y;
        return y;
    }

    /**
     * @brief change allpass coeffient
     * 
     * @param newA new coeffient
     */
    void update(T newA)
    {
        a = newA;
    }

private:
    T a;
    T x1;
    T y1;
};

template<typename T>
class LowPassComb
{
public: 
    LowPassComb(T L, T G, T R) : L(L), 
                            R(R),
                            G(G)
    {
        for(int i = 0; i < L; ++i)
            inDelay.push(0);
        
        for(int i = 0; i <= L; ++i)
            outDelay.push(0);
    }

    void update(T newL, T newG, T newR)
    {
        while(inDelay.size() > newL)
            inDelay.pop();
        
        while(inDelay.size() < newL)
            inDelay.push(0);

        while(outDelay.size() >= newL)
            outDelay.pop();

        while(outDelay.size() <= newL)
            outDelay.push(0);

        G = newG;
        R = newR;
    }

    T operator()(T x)
    {
        T x_l_1 = inDelay.front();
        inDelay.pop();   
        T x_l = inDelay.front();
        
        T y_1 = outDelay.back();
        T y_l = outDelay.front();
        outDelay.pop();

        T y = x_l         - 
              (x_l_1 * G) +
              (y_1 * G)   -
              (y_l * R)   ;

        inDelay.push(x);
        outDelay.push(y);
        
        return y;
    }

private:
    T L, R, G;
    std::queue<T> inDelay;
    std::queue<T> outDelay;
};

template<typename T>
class Reverb
{
public:
    Reverb(int miliSec1, T G1, T R1, // Comb 1
           int miliSec2, T G2, T R2, // Comb 2
           int miliSec3, T G3, T R3, // Comb 3
           int miliSec4, T G4, T R4, // Comb 4
           int miliSec5, T G5, T R5, // Comb 5
           int miliSec6, T G6, T R6, // Comb 6
           T a, T k )
    : allPass(a), k(k) 
    {
        CombFilters.emplace_back((miliSec1 / 1000.0f) * RATE, G1, R1);
        CombFilters.emplace_back((miliSec2 / 1000.0f) * RATE, G2, R2);
        CombFilters.emplace_back((miliSec3 / 1000.0f) * RATE, G3, R3);
        CombFilters.emplace_back((miliSec4 / 1000.0f) * RATE, G4, R4);
        CombFilters.emplace_back((miliSec5 / 1000.0f) * RATE, G5, R5);
        CombFilters.emplace_back((miliSec6 / 1000.0f) * RATE, G6, R6);
    }  

    void update(int miliSec1, T G1, T R1, // Comb 1
                int miliSec2, T G2, T R2, // Comb 2
                int miliSec3, T G3, T R3, // Comb 3
                int miliSec4, T G4, T R4, // Comb 4
                int miliSec5, T G5, T R5, // Comb 5
                int miliSec6, T G6, T R6, // Comb 6
                T newA, T newK )
    {
        CombFilters[0].update((miliSec1 / 1000.0f) * RATE, G1, R1);
        CombFilters[1].update((miliSec2 / 1000.0f) * RATE, G2, R2);
        CombFilters[2].update((miliSec3 / 1000.0f) * RATE, G3, R3);
        CombFilters[3].update((miliSec4 / 1000.0f) * RATE, G4, R4);
        CombFilters[4].update((miliSec5 / 1000.0f) * RATE, G5, R5);
        CombFilters[5].update((miliSec6 / 1000.0f) * RATE, G6, R6);

        allPass.update(newA);
        k = newK;
    }

    T operator()(T x)
    {
        T sum = 0;
        for(auto& comb : CombFilters)
        {
            sum += comb(x);
        }

        T y = allPass((sum * (1/6.0f)) * (1 - k));
        return (y + (x * k));
    }

private:
    AllPass<T> allPass;
    T k;
    std::vector<LowPassComb<T>> CombFilters;
};
