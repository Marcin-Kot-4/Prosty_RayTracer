#ifndef RAYTRACING_CHECKER_H
#define RAYTRACING_CHECKER_H


#include "texturebase.h"

namespace RT {
    namespace Texture {
        class Checker : public TextureBase {
        public:
            // Konstruktor
            Checker();

            // Destruktor
            virtual ~Checker() override;

            // Metoda zwracająca kolor
            virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;

            // Metoda ustawiająca kolor
            void SetColor(const qbVector<double> &inputColor1, const qbVector<double> &inputColor2);

        private:
            qbVector<double> m_color1{4};
            qbVector<double> m_color2{4};

        };
    }
}


#endif //RAYTRACING_CHECKER_H
