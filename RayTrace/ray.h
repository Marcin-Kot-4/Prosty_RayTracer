#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "./qbLinAlg/qbVector.h"

namespace RT {
    class Ray {

    public:
        Ray();
        // Konstruktor. Tablice zawierające dwa wektory point1 i point2
        // qbVector klasa z biblioteki liniowej algebry: qbLinAlg
        Ray(const qbVector<double> &point1, const qbVector<double> &point2);

        qbVector<double> GetPoint1() const;

        qbVector<double> GetPoint2() const;

    public:
        // Punkt początkowy
        qbVector<double> m_point1{3};
        // Punkt końcowy
        qbVector<double> m_point2{3};
        // Wektor od punktu 1 do punktu 2
        qbVector<double> m_lab{3};

    };
}


#endif //RAYTRACING_RAY_H
