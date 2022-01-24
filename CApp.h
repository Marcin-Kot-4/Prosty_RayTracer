#ifndef RAYTRACING_CAPP_H
#define RAYTRACING_CAPP_H

#include "include/SDL.h"
#include "./RayTrace/Image.h"
#include "./RayTrace/Scene.h"
#include "./RayTrace/camera.h"
#include "./RayTrace/Textures/flat.h"
#include "./RayTrace/Textures/checker.h"

class CApp
{
public:
    CApp();

    int OnExecute();
    bool OnInit();
    void OnEvent(SDL_Event *event);
    void OnLoop();
    void OnRender();
    void OnExit();

private:
    // Wydrukuj wektor do terminala
    void PrintVector(const qbVector<double> &inputVector);

private:
    // Instancja klasy Image do przechowywania obrazu
    Image m_image;

    // Instancja klasy Scene
    RT::Scene m_scene;

    // SDL2
    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
};

#endif //RAYTRACING_CAPP_H
