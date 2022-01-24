#ifndef RAYTRACING_MATERIALBASE_H
#define RAYTRACING_MATERIALBASE_H


#include <memory>
#include "../Textures/texturebase.h"
#include "../Primitives/objectbase.h"
#include "../Lights/lightbase.h"
#include "../qbLinAlg/qbVector.h"
#include "../ray.h"

namespace RT {
    class MaterialBase {
    public:
        // Konstruktor
        MaterialBase();

        // Destruktor
        virtual ~MaterialBase();

        // Metoda zwracająca kolor tworzywa
        virtual qbVector<double> ComputeColor(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                              const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                              const std::shared_ptr<RT::ObjectBase> &currentObject,
                                              const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                              const RT::Ray &cameraRay);

        // Metoda do obliczania koloru rozproszenia (kolor powierzchni obiektu jest ciemniejszy, w miejscach, gdzie
        // kąt padania światła jest większy)
        static qbVector<double> ComputeDiffuseColor(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                                    const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                                    const std::shared_ptr<RT::ObjectBase> &currentObject,
                                                    const qbVector<double> &intPoint,
                                                    const qbVector<double> &localNormal,
                                                    const qbVector<double> &baseColor);

        // Metoda do obliczania koloru odbicia
        qbVector<double> ComputeReflectionColor(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                                const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                                const std::shared_ptr<RT::ObjectBase> &currentObject,
                                                const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                const RT::Ray &incidentRay);

        // Metoda rzucająca promień na scene
        bool CastRay(const RT::Ray &castRay, const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                     const std::shared_ptr<RT::ObjectBase> &thisObject,
                     std::shared_ptr<RT::ObjectBase> &closestObject,
                     qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal,
                     qbVector<double> &closestLocalColor);

        // Metoda do przypisywania tekstury
        void AssignTexture(const std::shared_ptr<RT::Texture::TextureBase> &inputTexture);

    public:
        // Licznik refleksów
        inline static int m_maxReflectionRays;
        inline static int m_reflectionRayCount;

        // Oświetlenie otoczenia
        inline static qbVector<double> m_ambientColor {std::vector<double> {1.0, 1.0, 1.0}};
        inline static double m_ambientIntensity = 0.2;

        // Lista tekstur przypisanych do tego tworzywa
        std::vector<std::shared_ptr<RT::Texture::TextureBase>> m_textureList;

        // Flaga informująca czy tworzywo posiada przypisaną teksturę
        bool m_hasTexture = false;

    private:

    };
}


#endif //RAYTRACING_MATERIALBASE_H
