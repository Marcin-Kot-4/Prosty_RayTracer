#ifndef RAYTRACING_POINTLIGHT_H
#define RAYTRACING_POINTLIGHT_H


#include "lightbase.h"

namespace RT {
    class PointLight : public LightBase {
    public:
        // Domyślny konstruktor
        PointLight();

        // Nadpisany destruktor
        virtual ~PointLight() override;

        // Metoda do obliczania oświetlenia
        virtual bool ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                         const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                         const std::shared_ptr<RT::ObjectBase> &currentObject,
                                         qbVector<double> &color, double &intensity) override;
    };
}

#endif //RAYTRACING_POINTLIGHT_H
