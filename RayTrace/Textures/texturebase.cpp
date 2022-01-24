#include "texturebase.h"
#include <cmath>

// Konstruktor
RT::Texture::TextureBase::TextureBase() {

}

// Destruktor
RT::Texture::TextureBase::~TextureBase() {

}

// Metoda zwracająca kolor w podanym punkcie systemu współrzędnych (u,v)
qbVector<double> RT::Texture::TextureBase::GetColor(const qbVector<double> &uvCoords) {
    // Stwórz wektor czterowymiarowy
    qbVector<double> outputColor{4};

    // Zwróć wektor reprezentujący kolor
    return outputColor;
}

// Metoda do ustawiania macierzy przekształceń tekstury
void RT::Texture::TextureBase::SetTransform(const qbVector<double> &translation, const double &rotation,
                                            const qbVector<double> &scale) {
    // Stwórz macierz przekształceń
    qbMatrix2<double> rotationMatrix = {3, 3, std::vector<double>{
            cos(rotation), -sin(rotation), 0.0,
            sin(rotation), cos(rotation), 0.0,
            0.0, 0.0, 1.0}};

    qbMatrix2<double> scaleMatrix = {3, 3, std::vector<double>{
            scale.GetElement(0), 0.0, 0.0,
            0.0, scale.GetElement(1), 0.0,
            0.0, 0.0, 1.0}};

    qbMatrix2<double> translationMatrix = {3, 3, std::vector<double>{
            1.0, 0.0, translation.GetElement(0),
            0.0, 1.0, translation.GetElement(1),
            0.0, 0.0, 1.0}};

    // Połącz matryce tworząc finalną matrycę
    m_transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

// Metoda mieszająca kolory
qbVector<double> RT::Texture::TextureBase::BlendColors(const std::vector<qbVector<double>> &inputColorList) {
    // Utwórz wektor trójwymiarowy
    qbVector<double> outputColor{3};

    // Zwróć wektor reprezentujący kolor
    return outputColor;
}

// Metoda zastosowująca przekształcenie
qbVector<double> RT::Texture::TextureBase::ApplyTransform(const qbVector<double> &inputVector) {
    // Skopiuj wektor wejściowy i zmodyfikuj, aby otrzymać trzy elementy
    qbVector<double> newInput{3};
    newInput.SetElement(0, inputVector.GetElement(0));
    newInput.SetElement(1, inputVector.GetElement(1));

    // Zastosuj transformacje
    qbVector<double> result = m_transformMatrix * newInput;

    // Utwórz wynik
    qbVector<double> output{2};
    output.SetElement(0, result.GetElement(0));
    output.SetElement(1, result.GetElement(1));

    return output;
}