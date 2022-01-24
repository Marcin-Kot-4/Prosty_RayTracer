#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include <memory>
#include <vector>
#include "../include/SDL.h"
#include "Image.h"
#include "camera.h"
#include "Primitives/objsphere.h"
#include "Primitives/objplane.h"
#include "Primitives/cylinder.h"
#include "Primitives/cone.h"
#include "Lights/pointlight.h"

namespace RT {
    class Scene {
    public:
        // Domyślny konstruktor
        Scene();

        // Funkcja renderująca
        bool Render(Image &outputImage);

        // Metoda do rzucania promieni na scene
        bool CastRay(RT::Ray &castRay, std::shared_ptr<RT::ObjectBase> &closestObject,
                     qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal,
                     qbVector<double> &closestLocalColor);

    private:
        // Kamera
        RT::Camera m_camera;

        // Lista obiektów na scenie
        std::vector<std::shared_ptr<RT::ObjectBase>> m_objectList;

        // Lista źródeł światła na scenie
        std::vector<std::shared_ptr<RT::LightBase>> m_lightList;
    };
}


#endif //RAYTRACING_SCENE_H
