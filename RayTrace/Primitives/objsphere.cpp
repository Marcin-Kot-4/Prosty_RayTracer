#include "objsphere.h"
#include <cmath>

// Domyślny konstruktor
RT::ObjSphere::ObjSphere() {

}

// Destruktor
RT::ObjSphere::~ObjSphere() {

}

// Metoda odpowiedzialna za sprawdzanie przecięcia się promienia z powierzchnią obiektu
bool RT::ObjSphere::TestIntersection(const RT::Ray &castRay, qbVector<double> &intPoint,
                                     qbVector<double> &localNormal, qbVector<double> &localColor) {
    // Przekształcenie współrzędnych globalnych castRay na współrzędne lokalne obiektu
    // Kopiowanie promienia i zastosowanie wstecznej transformacji
    // Wsteczna transformacja zwraca nam lokalne współrzędne, ponieważ po transformacji obiekt jest opisany przez
    // globalne współrzędne
    RT::Ray bckRay = m_transformMatrix.Apply(castRay, RT::BCKTFORM); // z globalnych na lokalne

    // Oblicz wartości a, b i c
    qbVector<double> vhat = bckRay.m_lab; // m_lab - kierunek promienia
    vhat.Normalize(); // Normalizacja wektora, czyli przeskalowanie go tak, aby jego długość była równa 1.
    // Czyli podzielenie go przez jego długość. Dzięki temu powstaje wektor jednostkowy.

    // A jest równe kwadratowi wielkości kierunku promienia.
    // Jako że będzie to wektor jednostkowy wartość "a" zawsze będzie równa 1
    // a = 1.0; nie musimy obliczać a

    // Oblicz b (m_point1 punkt startowy promienia)
    double b = 2.0 * qbVector<double>::dot(bckRay.m_point1, vhat);

    // Oblicz c
    double c = qbVector<double>::dot(bckRay.m_point1, bckRay.m_point1) - 1.0;

    // Sprawdzanie przecięcia
    double intTest = (b * b) - 4.0 * c;

    qbVector<double> poi; // point of intersection
    if (intTest > 0.0) { // Jeśli występuje przecięcie promienia ze sferą
        double numSQRT = sqrtf(intTest); // pierwiastek z delty
        double t1 = (-b + numSQRT) / 2.0; // x1
        double t2 = (-b - numSQRT) / 2.0; // x2

        // Jeśli t1 lub t2 to wartości ujemne, wówczas przynajmniej część obiektu jest za kamerą i możemy to zignorować
        if ((t1 < 0.0) && (t2 < 0.0)) {
            return false;
        } else {
            // Określ, który punkt jest bliżej kamery (przednia część sfery czy tylna)
            // [w przypadku gdy promień przecina się ze sferą w dwóch punktach]
            if (t1 < t2) {
                if (t1 > 0.0) {
                    poi = bckRay.m_point1 + (vhat * t1);
                } else {
                    if (t2 > 0.0) {
                        poi = bckRay.m_point1 + (vhat * t2);
                    } else {
                        return false;
                    }
                }
            } else {
                if (t2 > 0.0) {
                    poi = bckRay.m_point1 + (vhat * t2);
                } else {
                    if (t1 > 0.0) {
                        poi = bckRay.m_point1 + (vhat * t1);
                    } else {
                        return false;
                    }
                }
            }

            // teraz poi zawiera punkt przecięcia opisany przez lokalne współrzędne obiektu
            // Przekształcenie punktu przecięcia na współrzędne globalne
            intPoint = m_transformMatrix.Apply(poi, RT::FWDTFORM); // z lokalnych na globalne

            // Oblicz wektor normalny
            // Środek obiektu
            qbVector<double> objOrigin = qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}};
            // Zastosuj transformacje dla środka obiektu
            qbVector<double> newObjOrigin = m_transformMatrix.Apply(objOrigin, RT::FWDTFORM);
            // Teraz normalna lokalna to punkt przecięcia minus nowy środek obiektu
            localNormal = intPoint - newObjOrigin;
            localNormal.Normalize();

            // Ustaw kolor bazowy jako lokalny kolor
            localColor = m_baseColor;

            // Oblicz i zapisz współrzędne przecięcia (u,v) do późniejszego użycia
            double x = poi.GetElement(0);
            double y = poi.GetElement(1);
            double z = poi.GetElement(2);
            double u = atan2(sqrtf(pow(x, 2.0) + pow(y, 2.0)), z);
            double v = atan2(y, x);

            u /= M_PI;
            v /= M_PI;

            m_uvCoords.SetElement(0, u);
            m_uvCoords.SetElement(1, v);
        }

        return true;
    } else { // Jeśli nie występuje przecięcie promienia ze sferą
        return false;
    }

}