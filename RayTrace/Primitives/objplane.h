#ifndef RAYTRACING_OBJPLANE_H
#define RAYTRACING_OBJPLANE_H


#include "objectbase.h"
#include "../gtfm.h"

namespace RT {
    class ObjPlane : public ObjectBase {
    public:
        // Domyślny konstruktor
        ObjPlane();

        // Destruktor
        virtual ~ObjPlane() override;

        // Nadpisana metoda do sprawdzania przecięć
        virtual bool TestIntersection(const RT::Ray &castRay, qbVector<double> &intPoint,
                                      qbVector<double> &localNormal, qbVector<double> &localColor) override;

    private:


    };
}


#endif //RAYTRACING_OBJPLANE_H
