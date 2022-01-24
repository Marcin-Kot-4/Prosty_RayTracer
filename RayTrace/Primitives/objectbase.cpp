#include "objectbase.h"
#include <math.h>

#define EPSILON 1e-21f; // = 0,000 000 000 000 000 000 001 - jedna tryliardowa

// Domyślny konstruktor
RT::ObjectBase::ObjectBase() {

}

// Destruktor.
RT::ObjectBase::~ObjectBase() {

}

// Sprawdzanie przecięcia się promienia z powierzchnią obiektu
bool RT::ObjectBase::TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal,
                                      qbVector<double> &localColor) {
    return false;
}

// Metoda ustawiająca macierz transformacji
void RT::ObjectBase::SetTransformMatrix(const RT::GTform &transformMatrix)
{
    m_transformMatrix = transformMatrix;
}

// Metoda przypisująca tworzywo do obiektu
bool RT::ObjectBase::AssignMaterial(const std::shared_ptr<RT::MaterialBase> &objectMaterial)
{
    m_pMaterial = objectMaterial;
    m_hasMaterial = true;
    return m_hasMaterial;
}

// Metoda sprawdzająca, czy dwie zmienne zmiennoprzecinkowe przechowują bardzo podobną wartość
bool RT::ObjectBase::CloseEnough(const double f1, const double f2) {
    return fabs(f1 - f2) < EPSILON; // fabs - floating point absolute value
}