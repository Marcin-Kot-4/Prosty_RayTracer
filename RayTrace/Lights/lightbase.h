#ifndef RAYTRACING_LIGHTBASE_H
#define RAYTRACING_LIGHTBASE_H


#include <memory>
#include "../qbLinAlg/qbVector.h"
#include "../ray.h"
#include "../Primitives/objectbase.h"

namespace RT {
    class LightBase {
    public:
        // Konstruktor
        LightBase();

        // Destruktor
        virtual ~LightBase();

        // Metoda do obliczania oświetlenia
        virtual bool ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                         const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                         const std::shared_ptr<RT::ObjectBase> &currentObject,
                                         qbVector<double> &color, double &intensity);

    public:
        // Kolor światła
        qbVector<double> m_color{3};
        // Położenie światła
        qbVector<double> m_location{3};
        // Intensywność światła
        double m_intensity;
    };
}


#endif //RAYTRACING_LIGHTBASE_H
