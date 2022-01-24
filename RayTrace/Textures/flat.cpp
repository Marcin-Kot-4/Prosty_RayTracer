#include "flat.h"

// Konstruktor
RT::Texture::Flat::Flat() {
    m_color = qbVector < double > {std::vector < double > {1.0, 0.0, 0.0, 1.0}}; // domyślny kolor tekstury to czerwony
}

// Destruktor
RT::Texture::Flat::~Flat() {

}

// Metoda zwracająca kolor
qbVector<double> RT::Texture::Flat::GetColor(const qbVector<double> &uvCoords) {
    return m_color;
}

// Metoda ustawiająca kolor
void RT::Texture::Flat::SetColor(const qbVector<double> &inputColor) {
    m_color = inputColor;
}