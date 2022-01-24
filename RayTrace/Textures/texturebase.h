#ifndef RAYTRACING_TEXTUREBASE_H
#define RAYTRACING_TEXTUREBASE_H


#include <memory>
#include "../qbLinAlg/qbMatrix.h"
#include "../qbLinAlg/qbVector.h"
#include "../ray.h"

namespace RT
{
    namespace Texture
    {
        class TextureBase
        {
        public:
            // Konstruktor domyślny
            TextureBase();
            // Destruktor
            virtual ~TextureBase();

            // Metoda zwracająca kolor w podanym punkcie systemu współrzędnych (u,v)
            // Zwracany kolor to 4-wymiarowy wektor (RGBA)
            virtual qbVector<double> GetColor(const qbVector<double> &uvCoords);

            // Metoda do ustawiania przekształcenia tekstury
            void SetTransform(const qbVector<double> &translation, const double &rotation, const qbVector<double> &scale);

            // Metoda mieszająca kolory RGBA i zwracająca 3-wymiarowy kolor (RGB)
            static qbVector<double> BlendColors(const std::vector<qbVector<double>> &inputColorList);

            // Metoda zastosowująca lokalne przekształcenie dla otrzymanego wektora wejściowego
            qbVector<double> ApplyTransform(const qbVector<double> &inputVector);

        private:

        private:
            // Inicjalizacja macierzy transformacji jako macierzy identyczności
            qbMatrix2<double> m_transformMatrix {3, 3, std::vector<double>{1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}};

        };
    }
}


#endif //RAYTRACING_TEXTUREBASE_H
