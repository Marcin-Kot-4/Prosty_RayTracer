#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H


#include "./qbLinAlg/qbVector.h"
#include "ray.h"

namespace RT {
    class Camera {

    public:
        // Konstruktor domyślny
        Camera();

        // Funkcje do ustawiania parametrów kamery
        void SetPosition(const qbVector<double> &newPosition);
        void SetLookAt(const qbVector<double> &newLookAt);
        void SetUp(const qbVector<double> &upVector);

        // Funkcja ustawiająca odległość od kamery do punktu, na który patrzymy
        void SetLength(double newLength);

        // Horyzontalny/poziomy rozmiar kamery
        void SetHorzSize(double newSize);

        // Funkcja ustawiająca proporcje kamery. Proporcje 16:9 = 1.78
        void SetAspect(double newAspectRatio);

        // Gettery
        qbVector<double> GetPosition();
        qbVector<double> GetLookAt();
        qbVector<double> GetUp();

        // Wektory U oraz V to wektory ekranu
        qbVector<double> GetU();
        qbVector<double> GetV();

        // Wektor środka ekranu
        qbVector<double> GetScreenCentre();

        double GetLength();
        double GetHorzSize();
        double GetAspect();

        // Funkcja inicjująca instancję klasy Ray. Reprezentuje promień wychodzący z kamery i przechodzący przez
        // pozycję na ekranie określoną przez parametry proScreenX i proScreenY
        // 0 to środek ekranu, -1 i 1 do lewa lub prawa, lub góra, lub dół ekranu.
        // X reprezentuje część wektora U.
        // Y reprezentuje część wektora V.
        // Są one translacją pozycji w przestrzeni trójwymiarowej, kiedy pomnożymy je przez te wektory.
        bool GenerateRay(float proScreenX, float proScreenY, RT::Ray &cameraRay);

        // Funkcja aktualizująca geometrię kamery (w przypadku zmiany jej położenia lub punktu, na który patrzy)
        void UpdateCameraGeometry();

    private:
        qbVector<double> m_cameraPosition{3};
        qbVector<double> m_cameraLookAt{3};
        qbVector<double> m_cameraUp{3};
        double m_cameraLength;
        double m_cameraHorzSize;
        double m_cameraAspectRatio;

        // Główna oś kamery
        qbVector<double> m_alignmentVector{3};
        qbVector<double> m_projectionScreenU{3};
        qbVector<double> m_projectionScreenV{3};
        qbVector<double> m_projectionScreenCentre{3};
    };
}


#endif //RAYTRACING_CAMERA_H
