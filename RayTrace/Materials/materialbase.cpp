#include "materialbase.h"

// Konstruktor
RT::MaterialBase::MaterialBase() {
    m_maxReflectionRays = 3;
    m_reflectionRayCount = 0;
}

// Destruktor
RT::MaterialBase::~MaterialBase() {

}

// Funkcja obliczająca kolor tworzywa
qbVector<double> RT::MaterialBase::ComputeColor(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                                const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                                const std::shared_ptr<RT::ObjectBase> &currentObject,
                                                const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                                const RT::Ray &cameraRay) {
    // Wstępny kolor tworzywa
    qbVector<double> matColor{3};

    return matColor;
}

// Metoda do obliczania koloru rozproszenia (kolor powierzchni obiektu jest ciemniejszy, w miejscach, gdzie
// kąt padania światła jest większy)
qbVector<double>
RT::MaterialBase::ComputeDiffuseColor(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                      const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                      const std::shared_ptr<RT::ObjectBase> &currentObject,
                                      const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                      const qbVector<double> &baseColor) {
    // Oblicz kolor ze względu na rozproszenie oświetlenia
    qbVector<double> diffuseColor{3};
    double intensity;
    qbVector<double> color{3};
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;
    bool validIllum = false;
    bool illumFound = false;
    // Przeiteruj po liście oświetlenia
    for (auto currentLight: lightList) {
        validIllum = currentLight->ComputeIllumination(intPoint, localNormal, objectList, currentObject, color,
                                                       intensity);
        // Jeśli obiekt nie jest zasłaniany przez inny obiekt od tego źródła światła
        if (validIllum) {
            illumFound = true;
            red += color.GetElement(0) * intensity;
            green += color.GetElement(1) * intensity;
            blue += color.GetElement(2) * intensity;
        }
    }

    // Jeśli obiekt nie jest zasłaniany przez inne obiekty/obiekt od wszystkich źródeł światła.
    // Po wyjściu z pętli, ustaw kolor obiektu na podstawie tego ile źródeł światła i pod jakim kątem na niego świeci
    // oraz jakiego koloru jest ten obiekt
    if (illumFound) {
        diffuseColor.SetElement(0, red * baseColor.GetElement(0));
        diffuseColor.SetElement(1, green * baseColor.GetElement(1));
        diffuseColor.SetElement(2, blue * baseColor.GetElement(2));
    } else {
        // Warunki oświetleniowe otoczenia (rozjaśnianie cieni)
        for (int i=0; i<3; ++i)
            diffuseColor.SetElement(i, (m_ambientColor.GetElement(i) * m_ambientIntensity) * baseColor.GetElement(i));
    }

    // Zwróć kolor materiału
    return diffuseColor;

}

// Metoda obliczająca kolor po odbiciu
qbVector<double>
RT::MaterialBase::ComputeReflectionColor(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                         const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                         const std::shared_ptr<RT::ObjectBase> &currentObject,
                                         const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                         const RT::Ray &incidentRay) {
    qbVector<double> reflectionColor{3};

    // Oblicz wektor odbicia
    qbVector<double> d = incidentRay.m_lab;
    qbVector<double> reflectionVector = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);

    // Utwórz promień odbicia
    RT::Ray reflectionRay(intPoint, intPoint + reflectionVector);

    // Rzuć promień na scenę i znajdź najbliższy obiekt, z którym się przecina
    std::shared_ptr<RT::ObjectBase> closestObject;
    qbVector<double> closestIntPoint{3};
    qbVector<double> closestLocalNormal{3};
    qbVector<double> closestLocalColor{3};
    bool intersectionFound = CastRay(reflectionRay, objectList, currentObject, closestObject, closestIntPoint,
                                     closestLocalNormal, closestLocalColor);

    // Jeśli znaleziono punkt przecięcia i ilość odbić nie została przekroczona to oblicz oświetlenie dla najbliżej
    // leżącego obiektu
    qbVector<double> matColor{3};
    if ((intersectionFound) && (m_reflectionRayCount < m_maxReflectionRays)) {
        // Zinkrementuj reflectionRayCount.
        m_reflectionRayCount++;

        // Jeśli obiekt posiada przypisane tworzywo
        if (closestObject->m_hasMaterial) {
            // Wykorzystaj tworzywo do obliczenia koloru
            matColor = closestObject->m_pMaterial->ComputeColor(objectList, lightList, closestObject, closestIntPoint,
                                                                closestLocalNormal, reflectionRay);
        } else {
            matColor = RT::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint,
                                                             closestLocalNormal, closestObject->m_baseColor);
        }
    } else {
        // Pozostaw kolor taki jaki jest
    }

    reflectionColor = matColor;
    return reflectionColor;
}

// Metoda rzucająca promień na scene
bool
RT::MaterialBase::CastRay(const RT::Ray &castRay, const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                          const std::shared_ptr<RT::ObjectBase> &thisObject,
                          std::shared_ptr<RT::ObjectBase> &closestObject,
                          qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal,
                          qbVector<double> &closestLocalColor) {
    // Sprawdzenie przecięć promienia ze wszystkimi obiektami
    qbVector<double> intPoint{3};
    qbVector<double> localNormal{3};
    qbVector<double> localColor{3};

    double minDist = 1e6;
    bool intersectionFound = false;
    for (auto currentObject: objectList) {
        if (currentObject != thisObject) {
            bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

            // Jeśli istnieje punkt przecięcia promienia z obiektem
            if (validInt) {
                // Ustaw flagę informującą o znalezieniu punktu przecięcia
                intersectionFound = true;

                // Oblicz odległość od kamery do punktu przecięcia
                double dist = (intPoint - castRay.m_point1).norm();

                // Przechowaj referencje do obiektu, jeśli jest to obiekt leżący najbliżej
                if (dist < minDist) {
                    minDist = dist;
                    closestObject = currentObject;
                    closestIntPoint = intPoint;
                    closestLocalNormal = localNormal;
                    closestLocalColor = localColor;
                }
            }
        }
    }

    // Zwróć flagę informującą czy udało się odnaleźć punkt przecięcia promienia z obiektem
    return intersectionFound;
}

// Metoda do przypisywania tekstury
void RT::MaterialBase::AssignTexture(const std::shared_ptr<RT::Texture::TextureBase> &inputTexture) {
    m_textureList.push_back(inputTexture);
    m_hasTexture = true;
}