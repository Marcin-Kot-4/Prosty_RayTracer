#include "pointlight.h"

// Domyślny konstruktor (lokacją światła zajmuje się qbVector, który ustawia światło w pozycji 0 0 0)
RT::PointLight::PointLight() {
    m_color = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}}; // kolor biały
    m_intensity = 1.0;
}

// Destruktor
RT::PointLight::~PointLight() {

}

// Metoda do obliczania oświetlenia
bool RT::PointLight::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                         const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                         const std::shared_ptr<RT::ObjectBase> &currentObject,
                                         qbVector<double> &color, double &intensity) {
    // Utwórz wektor jednostkowy wskazujący od punktu przecięcia do źródła światła
    qbVector<double> lightDir = (m_location - intPoint).Normalized();
    double lightDist = (m_location - intPoint).norm();

    // Punkt startowy
    qbVector<double> startPoint = intPoint;

    // Utwórz promień z punktu przecięcia do źródła światła
    RT::Ray lightRay(startPoint, startPoint + lightDir);

    // Znajdź punkty przecięć dla wszystkich obiektów z wyjątkiem obecnego
    qbVector<double> poi{3};
    qbVector<double> poiNormal{3};
    qbVector<double> poiColor{3};
    bool validInt = false;
    // Pętla przez wszystkie obiekty
    for (auto sceneObject: objectList) {
        // Jeśli aktualnie sprawdzany obiekt to nie "obecny" obiekt
        if (sceneObject != currentObject) {
            // Sprawdź, czy obiekt zasłania światło obecnemu obiektowi
            validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor);
            if (validInt) {
                double dist = (poi - startPoint).norm();
                // Jeśli punkt przecięcia nie leży pomiędzy źródłem światła a obiektem nie rzucaj cienia
                if (dist > lightDist)
                    validInt = false;
            }
        }

        // Jeśli obiekt zasłania światło obecnemu obiektowi wyjdź z pętli
        if (validInt)
            break;
    }

    // Wykonaj tylko dla obiektu, który nie jest zasłaniany przez inne obiekt od źródła światła
    if (!validInt) {
        // Oblicz kąt pomiędzy lokalną normalną i źródłem światła
        // (obliczamy w ten sposób kąt padania światła na powierzchnię obiektu)
        // Lokalna normalna to wektor jednostkowy
        double angle = acos(qbVector<double>::dot(localNormal, lightDir));

        // Jeśli normalna nie wskazuje w stronę padającego światła wówczas nie mamy oświetlenia w tym punkcie
        if (angle > 1.5708) {
            // Brak oświetlenia
            color = m_color;
            intensity = 0.0;
            return false;
        } else {
            // Oświetlenie
            color = m_color;
            intensity = m_intensity * (1.0 - (angle / 1.5708));
            return true;
        }
    } else {
        // Cień, więc brak oświetlenia
        color = m_color;
        intensity = 0.0;
        return false;
    }
}