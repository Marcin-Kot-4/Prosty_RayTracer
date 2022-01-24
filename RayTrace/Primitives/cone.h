#ifndef RAYTRACING_CONE_H
#define RAYTRACING_CONE_H


#include "objectbase.h"
#include "../gtfm.h"

namespace RT
{
    class Cone : public ObjectBase
    {
    public:
        // Konstruktor domyślny
        Cone();

        // Nadpisany destruktor
        virtual ~Cone() override;

        // Nadpisz metodę sprawdzającą przecięcia promieni ze stożkiem
        virtual bool TestIntersection(const RT::Ray &castRay, qbVector<double> &intPoint,
                                      qbVector<double> &localNormal, qbVector<double> &localColor) override;
    };
}


#endif //RAYTRACING_CONE_H
