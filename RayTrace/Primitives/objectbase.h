#ifndef RAYTRACING_OBJECTBASE_H
#define RAYTRACING_OBJECTBASE_H


#include <memory>
#include "../qbLinAlg/qbVector.h"
#include "../ray.h"
#include "../gtfm.h"

namespace RT {
    // Deklaracja klasy MaterialBase
    class MaterialBase;

    class ObjectBase {
    public:
        // Konstruktor
        ObjectBase();

        // Destruktor
        virtual ~ObjectBase();

        // Sprawdzanie przecięcia się promienia z powierzchnią obiektu
        virtual bool TestIntersection(const Ray &castRay, qbVector<double> &intPoint, qbVector<double> &localNormal,
                                      qbVector<double> &localColor);

        // Metoda ustawiająca macierz transformacji
        void SetTransformMatrix(const RT::GTform &transformMatrix);

        // Metoda sprawdzająca, czy dwie zmienne zmiennoprzecinkowe przechowują bardzo podobną wartość
        bool CloseEnough(const double f1, const double f2);

        // Metoda do przypisywania tworzywa
        bool AssignMaterial(const std::shared_ptr<RT::MaterialBase> &objectMaterial);

        // Publiczne zmienne
    public:
        // Bazowy kolor obiektu
        qbVector<double> m_baseColor{3};

        // Macierz geometrycznych przekształceń
        RT::GTform m_transformMatrix;

        // Referencja do tworzywa przypisanego do tego obiektu
        std::shared_ptr<RT::MaterialBase> m_pMaterial;

        // Flaga informująca o tym czy ten obiekt posiada przypisane tworzywo
        bool m_hasMaterial = false;

        // Przechowaj współrzędne (u,v) z wykrytego przecięcia
        qbVector<double> m_uvCoords {2};
    };
}


#endif //RAYTRACING_OBJECTBASE_H
