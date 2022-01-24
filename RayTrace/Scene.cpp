#include "Scene.h"
#include "./Materials/simplematerial.h"
#include "./Textures/checker.h"
#include "Textures/textureimage.h"
#include "Materials/simplerefractive.h"

// Konstruktor
RT::Scene::Scene() {
    // Konfiguracja kamery
    m_camera.SetPosition(qbVector<double>{std::vector<double>{0.0, -10.0, -1.0}});
    m_camera.SetLookAt(qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}});
    m_camera.SetUp(qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}});
    m_camera.SetHorzSize(0.25);
    m_camera.SetAspect(16.0 / 9.0);
    m_camera.UpdateCameraGeometry();

    // Ustawienie oświetlenia otoczenia
    RT::MaterialBase::m_ambientColor = std::vector<double>{1.0, 1.0, 1.0}; // kolor biały
    RT::MaterialBase::m_ambientIntensity = 0.2; // jasność

    // Utwórz tekstury
    auto floorTexture = std::make_shared<RT::Texture::Checker>(RT::Texture::Checker());
    auto sphereTexture = std::make_shared<RT::Texture::Checker>(RT::Texture::Checker());
    auto cylinderTexture = std::make_shared<RT::Texture::Checker>(RT::Texture::Checker());
    auto coneTexture = std::make_shared<RT::Texture::Checker>(RT::Texture::Checker());
    auto imageTexture = std::make_shared<RT::Texture::Image>(RT::Texture::Image());

    // Skonfiguruj tekstury
    floorTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
                               0.0,
                               qbVector<double>{std::vector<double>{16.0, 16.0}});
    sphereTexture->SetColor(qbVector<double>{std::vector<double>{0.2, 0.2, 0.8}},
                            qbVector<double>{std::vector<double>{0.8, 0.8, 0.2}});
    sphereTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
                                0.0,
                                qbVector<double>{std::vector<double>{16.0, 16.0}});
    cylinderTexture->SetColor(qbVector<double>{std::vector<double>{1.0, 0.5, 0.0}},
                              qbVector<double>{std::vector<double>{0.8, 0.8, 0.2}});
    cylinderTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
                                  0.0,
                                  qbVector<double>{std::vector<double>{4.0 * M_PI, 4.0}});
    coneTexture->SetColor(qbVector<double>{std::vector<double>{0.2, 0.2, 0.8}},
                          qbVector<double>{std::vector<double>{1.0, 0.5, 0.0}});
    coneTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
                              0.0,
                              qbVector<double>{std::vector<double>{8.0 * (M_PI / 2.0), 8.0}});
    imageTexture->LoadImage("testImage.bmp");
    imageTexture->SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0}},
                               0.0,
                               qbVector<double>{std::vector<double>{1.0, 1.0}});


    // Utwórz tworzywa
    auto silverMetal = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
    auto goldMetal = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
    auto blueDiffuse = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
    auto yellowDiffuse = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
    auto orangeDiffuse = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
    auto floorMaterial = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
    auto wallMaterial = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
    auto imageMaterial = std::make_shared<RT::SimpleMaterial>(RT::SimpleMaterial());
    auto glassMaterial = std::make_shared<RT::SimpleRefractive> (RT::SimpleRefractive());

    // Skonfiguruj tworzywa
    silverMetal->m_baseColor = qbVector<double>{std::vector<double>{0.5, 0.5, 0.8}};
    silverMetal->m_reflectivity = 0.5;
    silverMetal->m_shininess = 20.0;

    goldMetal->m_baseColor = qbVector<double>{std::vector<double>{0.8, 0.8, 0.3}};
    goldMetal->m_reflectivity = 0.25;
    goldMetal->m_shininess = 20.0;

    blueDiffuse->m_baseColor = qbVector<double>{std::vector<double>{0.2, 0.2, 0.8}};
    blueDiffuse->m_reflectivity = 0.05;
    blueDiffuse->m_shininess = 5.0;
    blueDiffuse->AssignTexture(coneTexture);

    yellowDiffuse->m_baseColor = qbVector<double>{std::vector<double>{0.8, 0.8, 0.2}};
    yellowDiffuse->m_reflectivity = 0.05;
    yellowDiffuse->m_shininess = 20.0;
    yellowDiffuse->AssignTexture(sphereTexture);

    orangeDiffuse->m_baseColor = qbVector<double>{std::vector<double>{1.0, 0.5, 0.0}};
    orangeDiffuse->m_reflectivity = 0.05;
    orangeDiffuse->m_shininess = 5.0;
    orangeDiffuse->AssignTexture(cylinderTexture);

    wallMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 0.125, 0.125}};
    wallMaterial->m_reflectivity = 0.75;
    wallMaterial->m_shininess = 0.0;

    floorMaterial->m_baseColor = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
    floorMaterial->m_reflectivity = 0.2; // odbicia
    floorMaterial->m_shininess = 10.0; // połysk (widoczne źródła światła na kuli)
    // połysk jest zależny od kąta odbicia światła i kąta promienia rzuconego z kamery
    floorMaterial->AssignTexture(floorTexture); // przypisanie tekstury do tworzywa

    imageMaterial -> m_baseColor = qbVector<double>{std::vector<double>{1.0, 0.125, 0.125}};
    imageMaterial -> m_reflectivity = 0.0;
    imageMaterial -> m_shininess = 0.0;
    imageMaterial -> AssignTexture(imageTexture);

    glassMaterial -> m_baseColor = qbVector<double>{std::vector<double>{1.0, 1.0, 1.0}};
    glassMaterial -> m_reflectivity = 0.99;
    glassMaterial -> m_shininess = 40.0;
    glassMaterial -> m_translucency = 0.99;
    glassMaterial -> m_ior = 1.009; // współczynnik załamania światła (1.333 woda)

    // Utwórz obiekty
    m_objectList.push_back(std::make_shared<RT::ObjSphere>(RT::ObjSphere())); // sfera
    m_objectList.push_back(std::make_shared<RT::Cylinder>(RT::Cylinder())); // cylinder
    m_objectList.push_back(std::make_shared<RT::Cone>(RT::Cone())); // stożek

    // Utwórz testową płaszczyznę
    m_objectList.push_back(std::make_shared<RT::ObjPlane>(RT::ObjPlane()));
    m_objectList.at(3)->m_baseColor = qbVector<double>{std::vector<double>{0.5, 0.5, 0.5}};

    // Utwórz transformacje dla płaszczyzny
    RT::GTform planeMatrix;
    planeMatrix.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0, 0.75}},
                             qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{4.0, 4.0, 1.0}});

    // Zastosuj transformacje płaszczyzny
    m_objectList.at(3)->SetTransformMatrix(planeMatrix);

    // Modyfikacja sfer
    RT::GTform testMatrix1, testMatrix2, testMatrix3;
    testMatrix1.SetTransform(qbVector<double>{std::vector<double>{-2.0, 0.0, 0.0}}, // transformacja
                             qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},    // rotacja
                             qbVector<double>{std::vector<double>{0.5, 0.5, 0.75}});    // skalowanie

    testMatrix2.SetTransform(qbVector<double>{std::vector<double>{0.0, 0.0, 0.2}},
                             qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.5, 1.0, 0.5}});

    testMatrix3.SetTransform(qbVector<double>{std::vector<double>{2.0, 0.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.0, -M_PI / 8.0, 0.0}},
                             qbVector<double>{std::vector<double>{0.75, 1.5, 0.75}});

    m_objectList.at(0)->SetTransformMatrix(testMatrix1);
    m_objectList.at(1)->SetTransformMatrix(testMatrix2);
    m_objectList.at(2)->SetTransformMatrix(testMatrix3);

    // Przypisz tworzywa do obiektów
    m_objectList.at(0)->AssignMaterial(glassMaterial);
    m_objectList.at(1)->AssignMaterial(silverMetal);
    m_objectList.at(2)->AssignMaterial(blueDiffuse);
    m_objectList.at(3)->AssignMaterial(floorMaterial);

    // Utwórz testowe źródło światła
    // Tworzy źródło światła i dodaje je do listy
    m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
    // Modyfikuje położenie źródła światła
    // x(prawo, lewo) y(dalej, bliżej od kamery) z(dół, góra)
    m_lightList.at(0)->m_location = qbVector<double>{std::vector<double>{5.0, -10.0, -5.0}};
    // Modyfikuje kolor źródła światła
    m_lightList.at(0)->m_color = qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}};

    m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
    m_lightList.at(1)->m_location = qbVector<double>{std::vector<double>{-5.0, -10.0, -5.0}};
    m_lightList.at(1)->m_color = qbVector<double>{std::vector<double>{1.0, 0.0, 0.0}};

    m_lightList.push_back(std::make_shared<RT::PointLight>(RT::PointLight()));
    m_lightList.at(2)->m_location = qbVector<double>{std::vector<double>{0.0, -10.0, -5.0}};
    m_lightList.at(2)->m_color = qbVector<double>{std::vector<double>{0.0, 1.0, 0.0}};
}

// Funkcja renderująca
bool RT::Scene::Render(Image &outputImage) {
    // Pobiera wymiary obrazu
    int xSize = outputImage.GetXSize();
    int ySize = outputImage.GetYSize();

    // Pętla przez wszystkie piksele obrazu
    RT::Ray cameraRay; // do przechowywania promienia dla każdego piksela w pętli
    qbVector<double> intPoint(3); // intersection point (punkt przecięcia)
    qbVector<double> localNormal(3);
    qbVector<double> localColor(3);
    double xFact = 1.0 / (static_cast<double>(xSize) / 2.0); // wartość zawarta w xFact reprezentuje w przedziale <0; 2>
    // różnicę wartości położenia pomiędzy kolejnymi pikselami na osi x
    double yFact = 1.0 / (static_cast<double>(ySize) / 2.0); // wartość zawarta w yFact reprezentuje w przedziale <0; 2>
    // różnicę wartości położenia pomiędzy kolejnymi pikselami na osi y
    double minDist = 1e6;
    double maxDist = 0.0;
    for (int y = 0; y < ySize; ++y) {
        // Wyświetl indykator postępu
        std::cout << "Przetwarzanie linii " << y << " z " << ySize << "." << std::endl;
        std::cout.flush();
        for (int x = 0; x < xSize; ++x) {
            // Normalizacja współrzędnych x i y.
            double normX = (static_cast<double>(x) * xFact) - 1.0; // Otrzymujemy wartość z przedziału <-1; 1>
            double normY = (static_cast<double>(y) * yFact) - 1.0; // Otrzymujemy wartość z przedziału <-1; 1>

            // Wygeneruj promień dla tego piksela
            m_camera.GenerateRay(normX, normY, cameraRay);

            // Sprawdź przecięcia promienia ze wszystkimi obiektami na scenie
            std::shared_ptr<RT::ObjectBase> closestObject;
            qbVector<double> closestIntPoint{3};
            qbVector<double> closestLocalNormal{3};
            qbVector<double> closestLocalColor{3};
            bool intersectionFound = CastRay(cameraRay, closestObject, closestIntPoint, closestLocalNormal,
                                             closestLocalColor);

            // Oblicz oświetlenie dla najbliżej położonego obiektu, jeśli znaleziono przecięcie
            if (intersectionFound) {
                // Sprawdź, czy obiekt posiada tworzywo
                if (closestObject->m_hasMaterial) {
                    // Wyzeruj licznik odbić (dla każdego piksela)
                    RT::MaterialBase::m_reflectionRayCount = 0;
                    // Wykorzystaj tworzywo do obliczania koloru
                    qbVector<double> color = closestObject->m_pMaterial->ComputeColor(m_objectList, m_lightList,
                                                                                      closestObject, closestIntPoint,
                                                                                      closestLocalNormal, cameraRay);
                    // Ustaw kolor piksela
                    outputImage.SetPixel(x, y, color.GetElement(0), color.GetElement(1), color.GetElement(2));
                } else {
                    // Użyj zwykłej metody obliczania koloru
                    qbVector<double> matColor = RT::MaterialBase::ComputeDiffuseColor(m_objectList, m_lightList,
                                                                                      closestObject, closestIntPoint,
                                                                                      closestLocalNormal,
                                                                                      closestObject->m_baseColor);
                    // Ustaw kolor piksela
                    outputImage.SetPixel(x, y, matColor.GetElement(0), matColor.GetElement(1), matColor.GetElement(2));
                }
            }
        }
    }

    std::cout << std::endl;
    return true;
}

// Metoda rzucająca promień na scene
bool RT::Scene::CastRay(RT::Ray &castRay, std::shared_ptr<RT::ObjectBase> &closestObject,
                        qbVector<double> &closestIntPoint, qbVector<double> &closestLocalNormal,
                        qbVector<double> &closestLocalColor) {
    qbVector<double> intPoint{3};
    qbVector<double> localNormal{3};
    qbVector<double> localColor{3};
    double minDist = 1e6;
    bool intersectionFound = false;
    for (auto currentObject: m_objectList) {
        bool validInt = currentObject->TestIntersection(castRay, intPoint, localNormal, localColor);

        // Jeśli występuje przecięcie promienia z obiektem
        if (validInt) {
            // Ustaw flagę oznaczającą znalezienie przecięcia
            intersectionFound = true;

            // Oblicz dystans pomiędzy kamerą i punktem przecięcia z powierzchnią obiektu
            double dist = (intPoint - castRay.m_point1).norm();

            // Jeśli obiekt jest bliżej niż wszystkie poprzednio sprawdzane, przechowaj referencje do niego
            if (dist < minDist) {
                minDist = dist;
                closestObject = currentObject;
                closestIntPoint = intPoint;
                closestLocalNormal = localNormal;
                closestLocalColor = localColor;
            }
        }
    }

    // Zwróć flagę oznaczającą czy odnaleziono przecięcie
    return intersectionFound;
}