#include "objplane.h"
#include <cmath>

// Domyślny konstruktor
RT::ObjPlane::ObjPlane() {

}

// Destruktor
RT::ObjPlane::~ObjPlane() {

}

// Metoda do sprawdzania przecięć
bool RT::ObjPlane::TestIntersection(const RT::Ray &castRay, qbVector<double> &intPoint,
                                    qbVector<double> &localNormal, qbVector<double> &localColor) {
    // Przekształcenie współrzędnych globalnych castRay na współrzędne lokalne obiektu
    // Kopiowanie promienia i zastosowanie wstecznej transformacji
    // Wsteczna transformacja zwraca nam lokalne współrzędne, ponieważ po transformacji obiekt jest opisany przez
    // globalne współrzędne
    RT::Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM); // z globalnych na lokalne

    // Skopiuj i normalizuj wektor
    qbVector<double> k = bckRay.m_lab;
    k.Normalize();

    // Sprawdź czy występuje przecięcie promienia z obiektem i czy promień nie jest równoległy z płaszczyzną obiektu
    if (!CloseEnough(k.GetElement(2), 0.0)) {
        // Jeśli występuje przecięcie
        double t = bckRay.m_point1.GetElement(2) / -k.GetElement(2);

        // Jeśli t jest ujemne, wówczas punkt przecięcia musi znajdować się za kamerą więc możemy go zignorować
        if (t > 0.0) {
            // Oblicz wartości dla u i v
            double u = bckRay.m_point1.GetElement(0) + (k.GetElement(0) * t);
            double v = bckRay.m_point1.GetElement(1) + (k.GetElement(1) * t);

            // Jeśli wielkość u i v jest, mniejsza niż 1 to znaczy, że jesteśmy na płaszczyźnie
            if ((abs(u) < 1.0) && (abs(v) < 1.0)) {
                // Oblicz punkt przecięcia
                qbVector<double> poi = bckRay.m_point1 + t * k;

                // Przekształć punkt przecięcia na globalne współrzędne
                intPoint = m_transformMatrix.Apply(poi, RT::FWDTFORM);

                // Oblicz normalną lokalną
                qbVector<double> localOrigin{std::vector<double>{0.0, 0.0, 0.0}};
                qbVector<double> normalVector{std::vector<double>{0.0, 0.0, -1.0}};
                qbVector<double> globalOrigin = m_transformMatrix.Apply(localOrigin, RT::FWDTFORM);
                localNormal = m_transformMatrix.Apply(normalVector, RT::FWDTFORM) - globalOrigin;
                localNormal.Normalize();

                // Ustaw kolor piksela na kolor bazowy
                localColor = m_baseColor;

                // Zapisz współrzędne przecięcia (u,v) do późniejszego użycia
                m_uvCoords.SetElement(0, u);
                m_uvCoords.SetElement(1, v);

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