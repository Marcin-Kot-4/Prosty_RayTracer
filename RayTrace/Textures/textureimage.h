
#ifndef RAYTRACING_TEXTUREIMAGE_H
#define RAYTRACING_TEXTUREIMAGE_H


#include "texturebase.h"
#include "../../include/SDL.h"

namespace RT
{
    namespace Texture
    {
        class Image : public TextureBase
        {
        public:
            // Konstruktor
            Image();

            // Destruktor
            virtual ~Image() override;

            // Funkcja zwracająca kolor
            virtual qbVector<double> GetColor(const qbVector<double> &uvCoords) override;

            // Funkcja wczytująca obraz
            bool LoadImage(std::string fileName);

        private:
            std::string m_fileName;
            SDL_Surface *m_imageSurface;
            SDL_PixelFormat *m_pixelFormat;
            bool m_imageLoaded = false;
            int m_xSize, m_ySize, m_pitch;
            uint8_t m_bytesPerPixel;
            uint32_t m_rMask, m_gMask, m_bMask, m_aMask;

        };
    }
}


#endif //RAYTRACING_TEXTUREIMAGE_H
