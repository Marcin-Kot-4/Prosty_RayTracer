#ifndef RAYTRACING_FLAT_H
#define RAYTRACING_FLAT_H


#include "texturebase.h"

namespace RT {
    namespace Texture {
        class Flat : public TextureBase {
        public:
            // Konstruktor
            Flat();

            // Destruktor
            virtual ~Flat() override;

            // Metoda zwracająca kolor
            virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;

            // Metoda ustawiająca kolor
            void SetColor(const qbVector<double> &inputColor);

        private:
            qbVector<double> m_color{4};

        };
    }
}


#endif //RAYTRACING_FLAT_H
