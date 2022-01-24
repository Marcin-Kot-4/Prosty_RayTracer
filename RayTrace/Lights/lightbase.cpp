#include "lightbase.h"
#include "../Primitives/objectbase.h"

// Konstruktor
RT::LightBase::LightBase() {

}

// Destruktor
RT::LightBase::~LightBase() {

}

// Metoda do obliczania oświetlenia
bool RT::LightBase::ComputeIllumination(const qbVector<double> &intPoint, const qbVector<double> &localNormal,
                                        const std::vector<std::shared_ptr<RT::ObjectBase>> &objectList,
                                        const std::shared_ptr<RT::ObjectBase> &currentObject,
                                        qbVector<double> &color, double &intensity) {
    return false;
}