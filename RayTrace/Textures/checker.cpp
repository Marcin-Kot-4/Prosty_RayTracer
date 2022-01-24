#include "checker.h"

// Konstruktor
RT::Texture::Checker::Checker() {
    m_color1 = qbVector < double > {std::vector < double > {1.0, 1.0, 1.0, 1.0}}; // biały
    m_color2 = qbVector < double > {std::vector < double > {0.2, 0.2, 0.2, 1.0}}; // czarny
}

// Destruktor
RT::Texture::Checker::~Checker() {

}

// Metoda zwracająca kolor
qbVector<double> RT::Texture::Checker::GetColor(const qbVector<double> &uvCoords) {
    // Zastosuj lokalne przekształcenie dla współrzędnych (u,v)
    qbVector<double> inputLoc = uvCoords;
    qbVector<double> newLoc = ApplyTransform(inputLoc);
    double newU = newLoc.GetElement(0);
    double newV = newLoc.GetElement(1);

    qbVector<double> localColor{4};
    int check = static_cast<int>(floor(newU)) + static_cast<int>(floor(newV));

    if ((check % 2) == 0) {
        localColor = m_color1;
    } else {
        localColor = m_color2;
    }

    return localColor;
}

// Metoda ustawiająca kolor
void RT::Texture::Checker::SetColor(const qbVector<double> &inputColor1, const qbVector<double> &inputColor2) {
    m_color1 = inputColor1;
    m_color2 = inputColor2;
}