#include "Image.h"

// Domyślny konstruktor
Image::Image() {
    m_xSize = 0;
    m_ySize = 0;
    m_pTexture = NULL;
}

// Destruktor
Image::~Image() {
    if (m_pTexture != NULL)
        SDL_DestroyTexture(m_pTexture);
}

// Funkcja do inicjalizacji
void Image::Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer) {
    // Zmiana rozmiaru tablic
    // resize metoda z biblioteki stl_vector.h
    m_rChannel.resize(xSize, std::vector<double>(ySize, 0.0));
    m_gChannel.resize(xSize, std::vector<double>(ySize, 0.0));
    m_bChannel.resize(xSize, std::vector<double>(ySize, 0.0));

    // Wymiary
    m_xSize = xSize;
    m_ySize = ySize;

    // Wskaźnik na renderer
    m_pRenderer = pRenderer;

    // Inicjalizacja tekstury
    InitTexture();
}

// Funkcja do ustawiania kolorów pikseli w podanej przez parametry pozycji
void Image::SetPixel(const int x, const int y, const double red, const double green, const double blue) {
    m_rChannel.at(x).at(y) = red;
    m_gChannel.at(x).at(y) = green;
    m_bChannel.at(x).at(y) = blue;
}

// Gettery do rozmiaru obrazu
int Image::GetXSize() {
    return m_xSize;
}

int Image::GetYSize() {
    return m_ySize;
}

// Funkcja wyświetlająca
void Image::Display() {
    // Znajdź maksymalne wartości kolorów
    ComputeMaxValues();

    // Alokacja pamięci na bufor pikseli. Jednowymiarowa tablica typu Uint32.
    Uint32 *tempPixels = new Uint32[m_xSize * m_ySize];

    // Czyszczenie bufora pikseli.
    memset(tempPixels, 0, m_xSize * m_ySize * sizeof(Uint32));

    // Pętla przez vectory zawierające wartości reprezentujące kolory pikseli
    // Konwersja kolorów ze zmienny typu double na zmienną typu Uint32 przy użyciu metody ConvertColor
    for (int x = 0; x < m_xSize; ++x) {
        for (int y = 0; y < m_ySize; ++y) {
            tempPixels[(y * m_xSize) + x] = ConvertColor(m_rChannel.at(x).at(y),
                                                         m_gChannel.at(x).at(y),
                                                         m_bChannel.at(x).at(y));
        }
    }

    // Aktualizacja tekstury buforem pikseli (m_pTexture - wskaźnik na teksturę)
    SDL_UpdateTexture(m_pTexture, NULL, tempPixels, m_xSize * sizeof(Uint32));

    // Usuwanie bufora pikseli
    delete[] tempPixels;

    // Kopiowanie tekstury do renderera
    SDL_Rect srcRect, bounds; // obiekty SDL_Rect
    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = m_xSize; // width
    srcRect.h = m_ySize; // height
    bounds = srcRect;
    // "Copy a portion of the texture to the current rendering" - SDL_render.h
    SDL_RenderCopy(m_pRenderer, m_pTexture, &srcRect, &bounds);
}

// Funkcja do inicjalizacji tekstury
void Image::InitTexture() {
    // Inicjalizacja tekstury
    Uint32 rmask, gmask, bmask, amask;

    // Jeśli używamy kodowania big endian (z dokumentacji SDL2)
    // koduj maski w następujący sposób
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else // w przeciwnym razie w sposób podany poniżej
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    // Usuń wszystkie utworzone wcześniej tekstury
    if (m_pTexture != NULL)
        SDL_DestroyTexture(m_pTexture);

    // Utwórz teksturę, która będzie przechowywać obraz
    SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, m_xSize, m_ySize, 32, rmask, gmask, bmask, amask);
    m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

// Funkcja konwertująca kolory na Uint32 wykorzystująca maximum dynamic range (maksymalne wartości kolorów i odpowiednie ich skalowanie)
Uint32 Image::ConvertColor(const double red, const double green, const double blue) {
    // Konwersja kolorów na unsigned int
    unsigned char r = static_cast<unsigned char>((red / m_overallMax) * 255.0);
    unsigned char g = static_cast<unsigned char>((green / m_overallMax) * 255.0);
    unsigned char b = static_cast<unsigned char>((blue / m_overallMax) * 255.0);

    // Jeśli używamy kodowania big endian (z dokumentacji SDL2)
    // koduj kolory w następujący sposób
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 pixelColor = (b << 24) + (g << 16) + (r << 8) + 255; // przesunięcia bitowe o podaną liczbę pozycji <<
#else // w przeciwnym razie w sposób podany poniżej
    Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif

    return pixelColor;
}

// Metoda znajdująca maksymalne wartości komponentów kolorów spośród wszystkich pikseli.
// Maksymalne wartości poszczególnych komponentów kolorów nie są sobie równe.
void Image::ComputeMaxValues() {
    m_maxRed = 0.0;
    m_maxGreen = 0.0;
    m_maxBlue = 0.0;
    m_overallMax = 0.0;
    for (int x = 0; x < m_xSize; ++x) {
        for (int y = 0; y < m_ySize; ++y) {
            double redValue = m_rChannel.at(x).at(y);
            double greenValue = m_gChannel.at(x).at(y);
            double blueValue = m_bChannel.at(x).at(y);

            if (redValue > m_maxRed)
                m_maxRed = redValue;

            if (greenValue > m_maxGreen)
                m_maxGreen = greenValue;

            if (blueValue > m_maxBlue)
                m_maxBlue = blueValue;

            if (m_maxRed > m_overallMax)
                m_overallMax = m_maxRed;

            if (m_maxGreen > m_overallMax)
                m_overallMax = m_maxGreen;

            if (m_maxBlue > m_overallMax)
                m_overallMax = m_maxBlue;
        }
    }
}