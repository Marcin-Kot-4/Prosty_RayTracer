#ifndef RAYTRACING_GTFM_H
#define RAYTRACING_GTFM_H

#include "./qbLinAlg/qbVector.h"
#include "./qbLinAlg/qbMatrix.h"
#include "ray.h"

namespace RT {
    // Definicja flag kierunków transformacji
    constexpr bool FWDTFORM = true; // forward transform
    constexpr bool BCKTFORM = false; // backward transform

    class GTform {
    public:
        // Konstruktor
        GTform();

        // Destruktor
        ~GTform();

        // Konstruktor wykorzystujący dwie macierze
        GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck);

        // Metoda do ustawiania komponentów translacji, rotacji i skalowania
        void SetTransform(const qbVector<double> &translation,
                          const qbVector<double> &rotation,
                          const qbVector<double> &scale);

        // Metody zwracające matryce transformacji
        qbMatrix2<double> GetForward();
        qbMatrix2<double> GetBackward();

        // Metody do transformacji
        RT::Ray Apply(const RT::Ray &inputRay, bool dirFlag);
        qbVector<double> Apply(const qbVector<double> &inputVector, bool dirFlag);

        // Przeciążenie operatorów
        friend GTform operator*(const RT::GTform &lhs, const RT::GTform &rhs); // left hand side, right hand side
        GTform operator=(const GTform &rhs);

        // Metoda wypisująca macierze
        void PrintMatrix(bool dirFlag);

        // Metoda umożliwiająca wypisywanie wektorów
        static void PrintVector(const qbVector<double> &vector);

    private:
        void Print(const qbMatrix2<double> &matrix);

    private:
        // macierze jednorodne 4 x 4 pozwalają na przeprowadzenie operacji translacji, rotacji i skalowania za jednym zamachem
        qbMatrix2<double> m_fwdtfm{4, 4};
        qbMatrix2<double> m_bcktfm{4, 4};
    };
}


#endif //RAYTRACING_GTFM_H
