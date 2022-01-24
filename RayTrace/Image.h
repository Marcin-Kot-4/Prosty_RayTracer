#ifndef RAYTRACING_IMAGE_H
#define RAYTRACING_IMAGE_H

#include <string>
#include <vector>
#include "../include/SDL.h"

class Image {
public:
    // Konstruktor
    Image();

    // Destruktor
    ~Image();

    // Funkcja do inicjalizacji obrazu
    void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);

    // Funkcja do ustawiania koloru piksela
    void SetPixel(const int x, const int y, const double red, const double green, const double blue);

    // Funkcja wyświetlająca obraz
    void Display();

    // Gettery do rozmiaru obrazu
    int GetXSize();
    int GetYSize();

private:
    // Funkcja przyjmująca reprezentację koloru przez trzy zmienna double i zwracająca reprezentację tego koloru
    // w zmiennej Uint32 w 8-bitowej reprezentacji przestrzeni barw
    Uint32 ConvertColor(const double red, const double green, const double blue);

    void InitTexture();
    // Oblicz maksymalne wartości kolorów
    void ComputeMaxValues();

private:
    // Vectory do przechowywania danych obrazu
    // Vektor zawiera vektory zawierające zmienne typu double
    std::vector<std::vector<double>> m_rChannel;
    std::vector<std::vector<double>> m_gChannel;
    std::vector<std::vector<double>> m_bChannel;

    // Wymiary obrazu
    int m_xSize, m_ySize;

    // Maksymalne wartości kolorów
    double m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;

    // SDL2
    SDL_Renderer *m_pRenderer; // wskaźnik na renderer
    SDL_Texture *m_pTexture;   // wskaźnik na teksturę
};


#endif //RAYTRACING_IMAGE_H
