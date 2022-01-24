#ifndef RAYTRACING_SIMPLEMATERIAL_H
#define RAYTRACING_SIMPLEMATERIAL_H


#include "materialbase.h"

namespace RT
{
    class SimpleMaterial : public MaterialBase
    {
    public:
        // Konstruktor
        SimpleMaterial();

        // Destruktor
        virtual ~SimpleMaterial() override;

        // Zwróć kolor obiektu
        virtual qbVector<double> ComputeColor(	const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                                  const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                                  const std::shared_ptr<RT::ObjectBase> &currentObject,
                                                  const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                  const RT::Ray &cameraRay) override;

        // Metoda do obliczania refleksów
        qbVector<double> ComputeSpecular(	const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                             const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                             const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                             const RT::Ray &cameraRay);

    public:
        // Kolor tworzywa
        qbVector<double> m_baseColor {std::vector<double> {1.0, 0.0, 1.0}};
        double m_reflectivity = 0.0;
        double m_shininess = 0.0;
    };
}


#endif //RAYTRACING_SIMPLEMATERIAL_H
