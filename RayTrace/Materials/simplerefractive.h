#ifndef RAYTRACING_SIMPLEREFRACTIVE_H
#define RAYTRACING_SIMPLEREFRACTIVE_H


#include <vector>
#include <memory>
#include "materialbase.h"

namespace RT {
    class SimpleRefractive : public MaterialBase {
    public:
        // Konstruktor
        SimpleRefractive();

        // Destruktor
        virtual ~SimpleRefractive() override;

        // Metoda zwracająca kolor
        virtual qbVector<double> ComputeColor(const std::vector <std::shared_ptr<RT::ObjectBase>> &objectList,
                                              const std::vector <std::shared_ptr<RT::LightBase>> &lightList,
                                              const std::shared_ptr <RT::ObjectBase> &currentObject,
                                              const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                              const RT::Ray &cameraRay) override;

        // Metoda obliczająca połysk
        qbVector<double> ComputeSpecular(const std::vector <std::shared_ptr<RT::ObjectBase>> &objectList,
                                         const std::vector <std::shared_ptr<RT::LightBase>> &lightList,
                                         const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                         const RT::Ray &cameraRay);

        // Metoda obliczająca przezroczystość
        qbVector<double> ComputeTranslucency(const std::vector <std::shared_ptr<RT::ObjectBase>> &objectList,
                                             const std::vector <std::shared_ptr<RT::LightBase>> &lightList,
                                             const std::shared_ptr <RT::ObjectBase> &currentObject,
                                             const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                             const RT::Ray &incidentRay);

    public:
        qbVector<double> m_baseColor{std::vector < double > {1.0, 0.0, 1.0}};
        double m_reflectivity = 0.0;
        double m_shininess = 0.0;
        double m_translucency = 0.0;
        // index of refraction (współczynnik załamania światła)
        double m_ior = 1.0;

    };
}


#endif //RAYTRACING_SIMPLEREFRACTIVE_H
