#include "camera.h"
#include "ray.h"
#include "math.h"

// Konstruktor domyślny
RT::Camera::Camera() {
    m_cameraPosition = qbVector<double>{std::vector<double>{0.0, -10.0, 0.0}};
    m_cameraLookAt = qbVector<double>{std::vector<double>{0.0, 0.0, 0.0}};
    m_cameraUp = qbVector<double>{std::vector<double>{0.0, 0.0, 1.0}};
    m_cameraLength = 1.0;
    m_cameraHorzSize = 1.0;
    m_cameraAspectRatio = 1.0;
}

void RT::Camera::SetPosition(const qbVector<double> &newPosition) {
    m_cameraPosition = newPosition;
}

void RT::Camera::SetLookAt(const qbVector<double> &newLookAt) {
    m_cameraLookAt = newLookAt;
}

void RT::Camera::SetUp(const qbVector<double> &upVector) {
    m_cameraUp = upVector;
}

void RT::Camera::SetLength(double newLength) {
    m_cameraLength = newLength;
}

void RT::Camera::SetHorzSize(double newHorzSize) {
    m_cameraHorzSize = newHorzSize;
}

void RT::Camera::SetAspect(double newAspect) {
    m_cameraAspectRatio = newAspect;
}

qbVector<double> RT::Camera::GetPosition() {
    return m_cameraPosition;
}

qbVector<double> RT::Camera::GetLookAt() {
    return m_cameraLookAt;
}

// Zwraca wektor wychodzący pionowo z punktu, w którym znajduje się kamera
qbVector<double> RT::Camera::GetUp() {
    return m_cameraUp;
}

// Zwraca odległość od kamery do punktu, na który patrzymy
double RT::Camera::GetLength() {
    return m_cameraLength;
}

// Zwraca horyzontalny/poziomy rozmiar kamery
double RT::Camera::GetHorzSize() {
    return m_cameraHorzSize;
}

// Zwraca proporcje kamery.
double RT::Camera::GetAspect() {
    return m_cameraAspectRatio;
}

// Zwraca wektor U
qbVector<double> RT::Camera::GetU() {
    return m_projectionScreenU;
}

// Zwraca wektor V
qbVector<double> RT::Camera::GetV() {
    return m_projectionScreenV;
}

// Wektor środka ekranu
qbVector<double> RT::Camera::GetScreenCentre() {
    return m_projectionScreenCentre;
}

// Funkcja aktualizująca geometrię kamery (w przypadku zmiany jej położenia lub punktu, na który patrzy)
void RT::Camera::UpdateCameraGeometry() {
    // Oblicz wektor z pozycji, w której znajduje się kamera do pozycji, na którą patrzy
    m_alignmentVector = m_cameraLookAt - m_cameraPosition;
    // Normalizacja w celu uzyskania jedynie kierunku (bez odległości)
    m_alignmentVector.Normalize();

    // Oblicz wektory U i V.
    // Produkt krzyżowy wektorów m_alignmentVector i m_camerUp.
    m_projectionScreenU = qbVector<double>::cross(m_alignmentVector, m_cameraUp);
    m_projectionScreenU.Normalize();
    // Produkt krzyżowy wektorów m_projectionScreenU i m_alignmentVector.
    m_projectionScreenV = qbVector<double>::cross(m_projectionScreenU, m_alignmentVector);
    m_projectionScreenV.Normalize();

    // Oblicz pozycję środka ekranu
    // Zaczynając od położenia, w którym znajduje się kamera, przesuwamy się po głównej osi o odległość od kamery
    // do ekranu
    m_projectionScreenCentre = m_cameraPosition + (m_cameraLength * m_alignmentVector);

    // Zmodyfikuj wektory U i V tak, aby pasowały do rozmiaru i proporcji ekranu
    m_projectionScreenU = m_projectionScreenU * m_cameraHorzSize;                         // szerokość ekranu
    m_projectionScreenV = m_projectionScreenV * (m_cameraHorzSize / m_cameraAspectRatio); // wysokość ekranu
}

// Zwraca promień wychodzący ze środka kamery i przechodzący przez ekran w konkretnym punkcie (przekazanym przez parametry)
bool RT::Camera::GenerateRay(float proScreenX, float proScreenY, RT::Ray &cameraRay) {
    // Przekształć położenie punktu na ekranie na położenie punktu w globalnych współrzędnych
    // Wektor U pomnożony przez zmienną zmiennoprzecinkową z przedziału <-1; 1> daje pozycję od środka ekranu na osi poziomej
    // Wektor V pomnożony przez zmienną zmiennoprzecinkową z przedziału <-1; 1> daje pozycję od środka ekranu na osi pionowej
    // Dodanie nowo powstałych wektorów daje konkretne położenie punktu w globalnych współrzędnych
    qbVector<double> screenWorldPart1 = m_projectionScreenCentre + (m_projectionScreenU * proScreenX);
    qbVector<double> screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * proScreenY);

    // Wykorzystaj wektor kamery i punktu na ekranie, przez który ma przechodzić promień do zmodyfikowania parametrów
    // instancji promienia
    cameraRay.m_point1 = m_cameraPosition;
    cameraRay.m_point2 = screenWorldCoordinate;
    cameraRay.m_lab = screenWorldCoordinate - m_cameraPosition;

    return true;
}