#include "cone.h"
#include <cmath>

// Konstruktor domyślny
RT::Cone::Cone() {

}

// Destruktor
RT::Cone::~Cone() {

}

// Metoda sprawdzającą przecięcia promieni ze stożkiem
bool RT::Cone::TestIntersection(const RT::Ray &castRay, qbVector<double> &intPoint,
                                qbVector<double> &localNormal, qbVector<double> &localColor) {
    // Przekształcenie współrzędnych globalnych castRay na współrzędne lokalne obiektu
    // Kopiowanie promienia i zastosowanie wstecznej transformacji
    // Wsteczna transformacja zwraca nam lokalne współrzędne, ponieważ po transformacji obiekt jest opisany przez
    // globalne współrzędne
    RT::Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM);

    // Skopiuj wektor m_lab z bckRay znormalizuj go
    qbVector<double> v = bckRay.m_lab;
    v.Normalize();

    // Pobierz początkowy punkt linii
    qbVector<double> p = bckRay.m_point1;

    // Oblicz wartości a, b i c
    double a = std::pow(v.GetElement(0), 2.0) + std::pow(v.GetElement(1), 2.0) - std::pow(v.GetElement(2), 2.0);
    double b = 2 * (p.GetElement(0) * v.GetElement(0) + p.GetElement(1) * v.GetElement(1) -
                    p.GetElement(2) * v.GetElement(2));
    double c = std::pow(p.GetElement(0), 2.0) + std::pow(p.GetElement(1), 2.0) - std::pow(p.GetElement(2), 2.0);

    // Oblicz b^2 - 4ac.
    double numSQRT = sqrtf(std::pow(b, 2.0) - 4 * a * c);

    std::array<qbVector<double>, 3> poi;
    std::array<double, 3> t;
    bool t1Valid, t2Valid, t3Valid;
    if (numSQRT > 0.0) {
        // Oblicz wartości t
        t.at(0) = (-b + numSQRT) / (2 * a);
        t.at(1) = (-b - numSQRT) / (2 * a);

        // Oblicz punkty przecięcia
        poi.at(0) = bckRay.m_point1 + (v * t[0]);
        poi.at(1) = bckRay.m_point1 + (v * t[1]);

        if ((t.at(0) > 0.0) && (poi.at(0).GetElement(2) > 0.0) && (poi.at(0).GetElement(2) < 1.0)) {
            t1Valid = true;
        } else {
            t1Valid = false;
            t.at(0) = 100e6;
        }

        if ((t.at(1) > 0.0) && (poi.at(1).GetElement(2) > 0.0) && (poi.at(1).GetElement(2) < 1.0)) {
            t2Valid = true;
        } else {
            t2Valid = false;
            t.at(1) = 100e6;
        }
    } else {
        t1Valid = false;
        t2Valid = false;
        t.at(0) = 100e6;
        t.at(1) = 100e6;
    }

    // Sprawdź, czy promień jest równoległy do podstawy stożka
    if (CloseEnough(v.GetElement(2), 0.0)) {
        t3Valid = false;
        t.at(2) = 100e6;
    } else {
        // Oblicz wartości t
        t.at(2) = (bckRay.m_point1.GetElement(2) - 1.0) / -v.GetElement(2);

        // Oblicz punkty przecięcia
        poi.at(2) = bckRay.m_point1 + t.at(2) * v;

        // Sprawdź, czy jesteśmy wewnątrz stożka
        if ((t.at(2) > 0.0) &&
            (sqrtf(std::pow(poi.at(2).GetElement(0), 2.0) + std::pow(poi.at(2).GetElement(1), 2.0)) < 1.0)) {
            t3Valid = true;
        } else {
            t3Valid = false;
            t.at(2) = 100e6;
        }
    }

    // Jeśli nie znaleziono punktu przecięcia
    if ((!t1Valid) && (!t2Valid) && (!t3Valid))
        return false;

    // Znajdz najmniejszą poprawną wartość t
    int minIndex = 0;
    double minValue = 10e6;
    for (int i = 0; i < 3; ++i) {
        if (t.at(i) < minValue) {
            minValue = t.at(i);
            minIndex = i;
        }
    }

    // Jeśli minIndex jest równy 0 lub 1, wówczas mamy przecięcie ze stożkiem
    qbVector<double> validPOI = poi.at(minIndex);
    if (minIndex < 2) {
        // Przekonwertuj współrzędne punktu przecięcia do współrzędnych globalnych
        intPoint = m_transformMatrix.Apply(validPOI, RT::FWDTFORM);

        // Oblicz lokalną normalną
        qbVector<double> orgNormal{3};
        qbVector<double> newNormal{3};
        qbVector<double> localOrigin{std::vector<double>{0.0, 0.0, 0.0}};
        qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, RT::FWDTFORM);
        double tX = validPOI.GetElement(0);
        double tY = validPOI.GetElement(1);
        double tZ = -sqrtf(pow(tX, 2.0) + pow(tY, 2.0));
        orgNormal.SetElement(0, tX);
        orgNormal.SetElement(1, tY);
        orgNormal.SetElement(2, tZ);
        newNormal = m_transformMatrix.Apply(orgNormal, RT::FWDTFORM) - globalOrigin;
        newNormal.Normalize();
        localNormal = newNormal;

        // Ustaw bazowy kolor
        localColor = m_baseColor;

        // Oblicz i zapisz współrzędne przecięcia (u,v) do późniejszego użycia
        double x = validPOI.GetElement(0);
        double y = validPOI.GetElement(1);
        double z = validPOI.GetElement(2);
        double u = atan2(y,x) / M_PI;
        double v = (z * 2.0) + 1.0;
        m_uvCoords.SetElement(0, u);
        m_uvCoords.SetElement(1, v);

        return true;
    } else {
        // W przeciwnym razie sprawdź podstawę stożka
        if (!CloseEnough(v.GetElement(2), 0.0)) {
            // Sprawdź, czy jesteśmy wewnątrz dysku
            if (sqrtf(std::pow(validPOI.GetElement(0), 2.0) + std::pow(validPOI.GetElement(1), 2.0)) < 1.0) {
                // Przekonwertuj współrzędne punktu przecięcia do współrzędnych globalnych
                intPoint = m_transformMatrix.Apply(validPOI, RT::FWDTFORM);

                // Oblicz lokalną normalną
                qbVector<double> localOrigin{std::vector<double>{0.0, 0.0, 0.0}};
                qbVector<double> normalVector{std::vector<double>{0.0, 0.0, 1.0}};
                qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, RT::FWDTFORM);
                localNormal = m_transformMatrix.Apply(normalVector, RT::FWDTFORM) - globalOrigin;
                localNormal.Normalize();

                // Ustaw bazowy kolor
                localColor = m_baseColor;

                // Oblicz i zapisz współrzędne przecięcia (u,v) do późniejszego użycia
                double x = validPOI.GetElement(0);
                double y = validPOI.GetElement(1);
                double z = validPOI.GetElement(2);
                m_uvCoords.SetElement(0, x);
                m_uvCoords.SetElement(1, y);

                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    }
    return false;
}