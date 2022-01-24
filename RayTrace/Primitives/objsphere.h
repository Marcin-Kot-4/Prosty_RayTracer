#ifndef RAYTRACING_OBJSPHERE_H
#define RAYTRACING_OBJSPHERE_H


#include "objectbase.h"
#include "../gtfm.h"

namespace RT {
    class ObjSphere : public ObjectBase {
    public:
        // Domyślny konstruktor. Utworzy sferę w środku układu
        ObjSphere();

        // Nadpisanie destruktora
        virtual ~ObjSphere() override;

        // Nadpisanie metody odpowiedzialnej za sprawdzanie przecięcia się promienia z powierzchnią obiektu
        virtual bool TestIntersection(const RT::Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal,
                                      qbVector<double> &localColor) override;

    private:


    };
}


#endif //RAYTRACING_OBJSPHERE_H
