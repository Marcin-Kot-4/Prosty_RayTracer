#ifndef RAYTRACING_CYLINDER_H
#define RAYTRACING_CYLINDER_H


#include "objectbase.h"
#include "../gtfm.h"

namespace RT
{
    class Cylinder : public ObjectBase
    {
    public:
        // Domyślny konstruktor
        Cylinder();

        // Nadpisany destruktor
        virtual ~Cylinder() override;

        // Nadpisana metoda do sprawdzania przecięcia promienia z obiektem
        virtual bool TestIntersection(const RT::Ray &castRay, qbVector<double> &intPoint,
                                      qbVector<double> &localNormal, qbVector<double> &localColor) override;
    };
}



#endif //RAYTRACING_CYLINDER_H
