#include "simplerefractive.h"

// Konstruktor
RT::SimpleRefractive::SimpleRefractive() {

}

// Destruktor
RT::SimpleRefractive::~SimpleRefractive() {

}

// Metoda zwracająca kolor
qbVector<double> RT::SimpleRefractive::ComputeColor(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                                    const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                                    const std::shared_ptr<RT::ObjectBase> &currentObject,
                                                    const qbVector<double> &intPoint,
                                                    const qbVector<double> &localNormal,
                                                    const RT::Ray &cameraRay) {
    // Zdefiniuj początkowe kolory tworzywa
    qbVector<double> matColor{3};
    qbVector<double> refColor{3};
    qbVector<double> difColor{3};
    qbVector<double> spcColor{3};
    qbVector<double> trnColor{3};

    // Oblicz komponent rozproszenia
    if (!m_hasTexture)
        difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal, m_baseColor);
    else
        difColor = ComputeDiffuseColor(objectList, lightList, currentObject, intPoint, localNormal,
                                       m_textureList.at(0)->GetColor(currentObject->m_uvCoords));

    // Oblicz komponent odbicia
    if (m_reflectivity > 0.0)
        refColor = ComputeReflectionColor(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

    // Połącz komponenty odbicia i rozproszenia
    matColor = (refColor * m_reflectivity) + (difColor * (1.0 - m_reflectivity));

    // Oblicz przezroczystość
    if (m_translucency > 0.0)
        trnColor = ComputeTranslucency(objectList, lightList, currentObject, intPoint, localNormal, cameraRay);

    // Połącz z aktualnym kolorem
    matColor = (trnColor * m_translucency) + (matColor * (1.0 - m_translucency));

    // Oblicz połysk
    if (m_shininess > 0.0)
        spcColor = ComputeSpecular(objectList, lightList, intPoint, localNormal, cameraRay);

    // Dodaj połysk do aktualnego koloru
    matColor = matColor + spcColor;

    return matColor;
}

// Metoda obliczająca kolor dla przezroczystości
qbVector<double>
RT::SimpleRefractive::ComputeTranslucency(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                          const std::vector<std::shared_ptr<RT::LightBase>> &lightList,
                                          const std::shared_ptr<RT::ObjectBase> &currentObject,
                                          const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                          const RT::Ray &incidentRay) {
    qbVector<double> trnColor{3};

    // Oblicz wektor po załamaniu
    qbVector<double> p = incidentRay.m_lab;
    p.Normalize();
    qbVector<double> tempNormal = localNormal;
    double r = 1.0 / m_ior;
    double c = -qbVector<double>::dot(tempNormal, p);
    if (c < 0.0) {
        tempNormal = tempNormal * -1.0;
        c = -qbVector<double>::dot(tempNormal, p);
    }

    // Oblicz wektor załamania
    qbVector<double> refractedVector = r * p + (r * c - sqrtf(1.0 - pow(r, 2.0) * (1.0 - pow(c, 2.0)))) * tempNormal;

    // Utwórz załamany promień
    RT::Ray refractedRay(intPoint + (refractedVector * 0.01), intPoint + refractedVector);

    // Sprawdź drugi punkt przecięcia przy wychodzeniu promienia z obiektu
    std::shared_ptr<RT::ObjectBase> closestObject;
    qbVector<double> closestIntPoint{3};
    qbVector<double> closestLocalNormal{3};
    qbVector<double> closestLocalColor{3};
    qbVector<double> newIntPoint{3};
    qbVector<double> newLocalNormal{3};
    qbVector<double> newLocalColor{3};
    bool test = currentObject->TestIntersection(refractedRay, newIntPoint, newLocalNormal, newLocalColor);
    bool intersectionFound = false;
    RT::Ray finalRay;
    if (test) {
        // Oblicz wektor po załamaniu
        qbVector<double> p2 = refractedRay.m_lab;
        p2.Normalize();
        qbVector<double> tempNormal2 = newLocalNormal;
        double r2 = m_ior;
        double c2 = -qbVector<double>::dot(tempNormal2, p2);
        if (c2 < 0.0) {
            tempNormal2 = tempNormal2 * -1.0;
            c2 = -qbVector<double>::dot(tempNormal2, p2);
        }

        // Oblicz wektor załamania
        qbVector<double> refractedVector2 =
                r2 * p2 + (r2 * c2 - sqrtf(1.0 - pow(r2, 2.0) * (1.0 - pow(c2, 2.0)))) * tempNormal2;

        // Utwórz załamany promień
        RT::Ray refractedRay2(newIntPoint + (refractedVector2 * 0.01), newIntPoint + refractedVector2);

        // Rzutuj ten promień na scenę
        intersectionFound = CastRay(refractedRay2, objectList, currentObject, closestObject, closestIntPoint,
                                    closestLocalNormal, closestLocalColor);
        finalRay = refractedRay2;
    } else {
        // Nie znaleziono drugiego punktu przecięcia (najprawdopodobniej jest to przezroczysta płaszczyzna)
        intersectionFound = CastRay(refractedRay, objectList, currentObject, closestObject, closestIntPoint,
                                    closestLocalNormal, closestLocalColor);
        finalRay = refractedRay;
    }

    // Oblicz kolor dla najbliższego obiektu
    qbVector<double> matColor{3};
    if (intersectionFound) {
        // Sprawdź, czy obiekt posiada przypisane tworzywo
        if (closestObject->m_hasMaterial) {
            matColor = closestObject->m_pMaterial->ComputeColor(objectList, lightList, closestObject, closestIntPoint,
                                                                closestLocalNormal, finalRay);
        } else {
            matColor = RT::MaterialBase::ComputeDiffuseColor(objectList, lightList, closestObject, closestIntPoint,
                                                             closestLocalNormal, closestObject->m_baseColor);
        }
    } else {
        // Pozostaw kolor bez zmian
    }

    trnColor = matColor;
    return trnColor;
}

// Metoda do obliczania refleksów świetlnych (widocznych źródeł światła na obiekcie)
// połysk jest zależny od kąta odbicia światła i kąta promienia rzuconego z kamery
qbVector<double> RT::SimpleRefractive::ComputeSpecular(const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
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