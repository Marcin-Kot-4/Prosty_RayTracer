#include "textureimage.h"

// Konstruktor
RT::Texture::Image::Image()
{

}

// Destruktor
RT::Texture::Image::~Image()
{
    if (m_imageLoaded)
    {
        // Upewnij się, że zwolniliśmy pamięć wykorzystaną na przechowywanie powierzchni obrazu
        SDL_FreeSurface(m_imageSurface);
        m_imageLoaded = false;
    }
}

qbVector<double> RT::Texture::Image::GetColor(const qbVector<double> &uvCoords)
{
    qbVector<double> outputColor {4};

    if (!m_imageLoaded)
    {
        // Jeśli nie wczytano żadnego obrazu ustaw kolor na fioletowy niezależnie od pozycji na osiach (u,v)(u,v)
        outputColor = qbVector<double>{std::vector<double>{1.0, 0.0, 1.0, 1.0}};
    }
    else
    {
        // Zastosuj lokalne przekształcenie dla współrzędnych (u,v)
        qbVector<double> inputLoc = uvCoords;
        qbVector<double> newLoc = ApplyTransform(inputLoc);
        double u = newLoc.GetElement(0);
        double v = newLoc.GetElement(1);

        // Przekształć (u,v) na wymiary obrazu (x,y)
        // Interpolacja najbliższego sąsiada
        int x = static_cast<int>(round(((u + 1.0) / 2.0) * static_cast<double>(m_xSize)));
        int y = m_ySize - (static_cast<int>(round(((v + 1.0) / 2.0) * static_cast<double>(m_ySize))));

        /* Arytmetyka modulo w celu uwzględnienia możliwego kafelkowania
            m_xSize = 10;
            x = 5 =>
                ((5 % 10) + 10) % 10 = 5

            x = 10 =>
                ((10 % 10) + 10) % 10 = 0

            x = 11 =>
                ((11 % 10) + 10) % 10 = 1

            x = -1 =>
                ((-1 % 10) + 10) % 10 = 9

            x = -5 =>
                ((-5 % 10) + 10) % 10 = 5

            x = -10 =>
                ((-10 % 10) + 10) % 10 = 0

            x = -11 =>
                ((-11 % 10) + 10) % 10 = 9  */

        x = ((x % m_xSize) + m_xSize) % m_xSize;
        y = ((y % m_ySize) + m_ySize) % m_ySize;

        // Sprawdzenie, czy jesteśmy wewnątrz obrazu
        if ((x >= 0) && (x < m_xSize) && (y >= 0) && (y < m_ySize))
        {
            // Przekonwertuj (x,y) na liniowy indeks
            int pixelIndex = x + (y * (m_pitch / m_bytesPerPixel));

            // Wskaźnik na piksel
            uint32_t *pixels = (uint32_t *)m_imageSurface->pixels;

            // Pobierz wartość aktualnie przetwarzanego piksela
            uint32_t currentPixel = pixels[pixelIndex];

            // Przekonwertuj na RGB
            uint8_t r, g, b, a;
            SDL_GetRGBA(currentPixel, m_imageSurface->format, &r, &g, &b, &a);

            // Ustaw odpowiednio wektor outputColor
            outputColor.SetElement(0, static_cast<double>(r) / 255.0);
            outputColor.SetElement(1, static_cast<double>(g) / 255.0);
            outputColor.SetElement(2, static_cast<double>(b) / 255.0);
            outputColor.SetElement(3, static_cast<double>(a) / 255.0);
        }
    }

    return outputColor;
}

// Metoda wczytująca obraz, jako parametr przyjmuje nazwę pliku
bool RT::Texture::Image::LoadImage(std::string fileName)
{
    if (m_imageLoaded)
        SDL_FreeSurface(m_imageSurface);

    m_fileName = fileName;
    m_imageSurface = SDL_LoadBMP(fileName.c_str());

    if (!m_imageSurface)
    {
        std::cout << "Błąd podczas wczytywania obrazu. " << SDL_GetError() << "." << std::endl;
        m_imageLoaded = false;
        return false;
    }

    // Zapisz użyteczne informacje o obrazie
    m_xSize = m_imageSurface->w;
    m_ySize = m_imageSurface->h;
    m_pitch = m_imageSurface->pitch;
    m_pixelFormat = m_imageSurface->format;
    m_bytesPerPixel = m_pixelFormat->BytesPerPixel;
    m_rMask = m_pixelFormat->Rmask;
    m_gMask = m_pixelFormat->Gmask;
    m_bMask = m_pixelFormat->Bmask;
    m_aMask = m_pixelFormat->Amask;

    std::cout << "Wczytano " << m_imageSurface->w << " na " << m_imageSurface->h << "." << std::endl;
    std::cout << "Bajty na piksel = " << +m_bytesPerPixel << std::endl;
    std::cout << "Rozstaw pikseli = " << m_pitch << std::endl;

    m_imageLoaded = true;
    return true;
}