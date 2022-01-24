#include "simplematerial.h"

// Konstruktor
RT::SimpleMaterial::SimpleMaterial() {

}

// Destruktor
RT::SimpleMaterial::~SimpleMaterial() {

}

// Funkcja zwracająca kolor
qbVector<double> RT::SimpleMaterial::ComputeColor(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                                  const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                                  const std::shared_ptr<RT::ObjectBase> &currentObject,
                                                  const qbVector<double> &intPoint,
                                                  const qbVector<double> &localNormal,
                                                  const RT::Ray &cameraRay) {
    // Początkowe kolory tworzywa
    qbVector<double> matColor{3};
    qbVector<double> refColor{3};
    qbVector<double> difColor{3};
    qbVector<double> spcColor{3};

    // Oblicz kolor rozproszenia
    if (!m_hasTexture) // zwykły kolor w przypadku kiedy obiekt nie posiada tekstury
        difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);
    else // tekstura zamiast koloru
        difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_textureList.at(0)->GetColor(currentObject->m_uvCoords));

    // Oblicz komponent refleksów
    if (m_reflectivity > 0.0)
        refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

    // Połącz komponent refleksów i kolor rozproszenia
    matColor = (refColor * m_reflectivity) + (difColor * (1 - m_reflectivity));

    // Oblicz komponent połysku
    if (m_shininess > 0.0)
        spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);

    // Dodaj komponent połysku do końcowego koloru
    matColor = matColor + spcColor;

    return matColor;
}

// Metoda do obliczania refleksów świetlnych (widocznych źródeł światła na obiekcie)
// połysk jest zależny od kąta odbicia światła i kąta promienia rzuconego z kamery
qbVector<double> RT::SimpleMaterial::ComputeSpecular(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                                     const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                                     const qbVector<double> &intPoint,
                                                     const qbVector<double> &localNormal,
                                                     const RT::Ray &cameraRay) {
    qbVector<double> spcColor{3};
    double red = 0.0;
    double green = 0.0;
    double blue = 0.0;

    // Pętla przez wszystkie źródła światła
    for (auto currentLight: lightList) {
        // Sprawdź przecięcia ze wszystkimi obiektami na scenie
        double intensity = 0.0;

        // Utwórz wektor wskazujący od punktu przecięcia do źródła światła
        qbVector<double> lightDir = (currentLight->m_location - intPoint).Normalized();

        // Oblicz punkt startowy
        qbVector<double> startPoint = intPoint + (lightDir * 0.001);

        // Utwórz promień od punktu przecięcia do źródła światła
        RT::Ray lightRay(startPoint, startPoint + lightDir);

        // Przeprowadź pętlę przez wszystkie obiekty w scenie, aby sprawdzić, czy światło z tego źródła nie jest blokowane
        qbVector<double> poi{3};
        qbVector<double> poiNormal{3};
        qbVector<double> poiColor{3};
        bool validInt = false;
        for (auto sceneObject: objectList) {
            validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
            if (validInt)
                break;
        }

        // Jeśli nie znaleziono żadnych przecięć, kontynuuj obliczanie składowej zwierciadlanej
        if (!validInt) {
            // Oblicz wektor odbicia
            qbVector<double> d = lightRay.m_lab;
            qbVector<double> r = d - (2 * qbVector<double>::dot(d, localNormal) * localNormal);
            r.Normalize();

            // Oblicz iloczyn skalarny
            qbVector<double> v = cameraRay.m_lab;
            v.Normalize();
            double dotProduct = qbVector<double>::dot(r, v);

            // Jeśli iloczyn skalarny jest większy od 0
            if (dotProduct > 0.0) {
                intensity = m_reflectivity * std::pow(dotProduct, m_shininess);
            }
        }

        red += currentLight->m_color.GetElement(0) * intensity;
        green += currentLight->m_color.GetElement(1) * intensity;
        blue += currentLight->m_color.GetElement(2) * intensity;
    }

    spcColor.SetElement(0, red);
    spcColor.SetElement(1, green);
    spcColor.SetElement(2, blue);
    return spcColor;
}