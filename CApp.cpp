#include "CApp.h"
#include "./RayTrace/qbLinAlg/qbVector.h"

// Konstruktor
CApp::CApp() {
    isRunning = true;
    pWindow = NULL;
    pRenderer = NULL;
}

// Inicjalizacja
bool CApp::OnInit() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

    int xSize = 1280;
    int ySize = 720;

    pWindow = SDL_CreateWindow("RayTracer - Patrycja Kalita, Marcin Kot", SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED, xSize, ySize, SDL_WINDOW_SHOWN);


    if (pWindow != NULL) {
        pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

        // Inicjalizacja instancji Image
        m_image.Initialize(xSize, ySize, pRenderer);

        // Ustaw tło na białe
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
        SDL_RenderClear(pRenderer);

        // Wyrenderuj scene
        m_scene.Render(m_image);


/**
 * Test renderowania tekstury (należy zakomentować renderowanie sceny powyżej)^
 */
//        // Ustaw teksture
//        RT::Texture::Checker testTexture;
//        testTexture.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
//                                 M_PI / 4.0, // rotacja tekstury
//                                 qbVector<double>{std::vector<double>{4.0, 4.0}});
//
//        // Renderuj teksturę
//        for (int y = 0; y < ySize; ++y) {
//            for (int x = 0; x < xSize; ++x) {
//                // Oblicz u,v
//                double u = (static_cast<double>(x) / (static_cast<double>(xSize) / 2.0)) - 1.0; // 2.0 - rozmiar kafelka (odwrócony)
//                double v = (static_cast<double>(y) / (static_cast<double>(ySize) / 2.0)) - 1.0;
//                qbVector<double> uvCoords{std::vector<double>{u, v}};
//
//                // Pobierz kolor z tekstury
//                qbVector<double> pixelColor = testTexture.GetColor(uvCoords);
//
//                // Aktualizuj ten piksel obrazu
//                m_image.SetPixel(x, y, pixelColor.GetElement(0), pixelColor.GetElement(1), pixelColor.GetElement(2));
//            }
//        }

        // Wyświetl obraz
        m_image.Display();

        // Prezentuj
        SDL_RenderPresent(pRenderer);

    } else {
        return false;
    }

    return true;
}

int CApp::OnExecute() {
    SDL_Event event;

    if (OnInit() == false) {
        return -1;
    }

    // Main loop
    while (isRunning) {
        while (SDL_PollEvent(&event) != 0) {
            OnEvent(&event);
        }

        OnLoop();
        OnRender();
    }

    return -1;
};

void CApp::OnEvent(SDL_Event *event) {
    if (event->type == SDL_QUIT) { // jeśli nowym zdarzeniem jest SDL_QUIT to zamknij aplikacje
        isRunning = false;
    }
}

void CApp::OnLoop() {

}

void CApp::OnRender() {
//    // Ustaw kolor tła na fioletowy
//    SDL_SetRenderDrawColor(pRenderer, 90, 60, 90, 255);
//    SDL_RenderClear(pRenderer);
//
//    // Renderuj scene
//    m_scene.Render(m_image);
//
//    // Wyświetl obraz
//    m_image.Display();
//
//    // Wyświetl
//    SDL_RenderPresent(pRenderer);
}

void CApp::OnExit() {
    // SDL2
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}

// Funkcje prywatne
void CApp::PrintVector(const qbVector<double> &inputVector) {
    int nRows = inputVector.GetNumDims();
    for (int row = 0; row < nRows; ++row) {
        std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(row) << std::endl;
    }
}