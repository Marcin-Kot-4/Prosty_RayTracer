#include "gtfm.h"

// Konstruktor
RT::GTform::GTform() {
    // Ustaw transformacje forward i backward na macierze jednostkowe
    m_fwdtfm.SetToIdentity();
    m_bcktfm.SetToIdentity();
}

// Destruktor
RT::GTform::~GTform() {

}

// Konstruktor wykorzystujący dwie macierze
RT::GTform::GTform(const qbMatrix2<double> &fwd, const qbMatrix2<double> &bck) {
    // Sprawdź czy macierze są o wymiarach 4 x 4
    if ((fwd.GetNumRows() != 4) || (fwd.GetNumCols() != 4) ||
        (bck.GetNumRows() != 4) || (bck.GetNumCols() != 4)) {
        throw std::invalid_argument("Nie mozna utworzyc GTform, matryce nie sa o wymiarach 4 x 4.");
    }

    m_fwdtfm = fwd;
    m_bcktfm = bck;
}

// Metoda do ustawiania transformacji
void RT::GTform::SetTransform(const qbVector<double> &translation,
                              const qbVector<double> &rotation,
                              const qbVector<double> &scale) {
    // Zdefiniuj macierze dla każdego komponentu transformacji
    qbMatrix2<double> translationMatrix{4, 4};
    qbMatrix2<double> rotationMatrixX{4, 4};
    qbMatrix2<double> rotationMatrixY{4, 4};
    qbMatrix2<double> rotationMatrixZ{4, 4};
    qbMatrix2<double> scaleMatrix{4, 4};

    // Ustaw na macierze identyczności
    translationMatrix.SetToIdentity();
    rotationMatrixX.SetToIdentity();
    rotationMatrixY.SetToIdentity();
    rotationMatrixZ.SetToIdentity();
    scaleMatrix.SetToIdentity();

    // Wypełnij macierze odpowiednimi wartościami
    // Macierz translacji
    translationMatrix.SetElement(0, 3, translation.GetElement(0)); // pierwszy wiersz, czwarta kolumna
    translationMatrix.SetElement(1, 3, translation.GetElement(1)); // drugi wiersz, czwarta kolumna
    translationMatrix.SetElement(2, 3, translation.GetElement(2)); // trzeci wiersz, czwarta kolumna

    // Macierze rotacji
    rotationMatrixZ.SetElement(0, 0, cos(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(0, 1, -sin(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(1, 0, sin(rotation.GetElement(2)));
    rotationMatrixZ.SetElement(1, 1, cos(rotation.GetElement(2)));

    rotationMatrixY.SetElement(0, 0, cos(rotation.GetElement(1)));
    rotationMatrixY.SetElement(0, 2, sin(rotation.GetElement(1)));
    rotationMatrixY.SetElement(2, 0, -sin(rotation.GetElement(1)));
    rotationMatrixY.SetElement(2, 2, cos(rotation.GetElement(1)));

    rotationMatrixX.SetElement(1, 1, cos(rotation.GetElement(0)));
    rotationMatrixX.SetElement(1, 2, -sin(rotation.GetElement(0)));
    rotationMatrixX.SetElement(2, 1, sin(rotation.GetElement(0)));
    rotationMatrixX.SetElement(2, 2, cos(rotation.GetElement(0)));

    // Macierz skalowania
    scaleMatrix.SetElement(0, 0, scale.GetElement(0));
    scaleMatrix.SetElement(1, 1, scale.GetElement(1));
    scaleMatrix.SetElement(2, 2, scale.GetElement(2));

    // Połącz matryce
    m_fwdtfm = translationMatrix *
               rotationMatrixX *
               rotationMatrixY *
               rotationMatrixZ *
               scaleMatrix;

    // Zapisz transformacje wsteczną
    m_bcktfm = m_fwdtfm;
    m_bcktfm.Inverse();

}

// Metody zwracające matryce transformacji
qbMatrix2<double> RT::GTform::GetForward() {
    return m_fwdtfm;
}
qbMatrix2<double> RT::GTform::GetBackward() {
    return m_bcktfm;
}

// Metoda zastosowująca transformacje
RT::Ray RT::GTform::Apply(const RT::Ray &inputRay, bool dirFlag) {
    // Utwórz obiekt wyjściowy
    RT::Ray outputRay;

    if (dirFlag) {
        // Zastosuj transformacje
        outputRay.m_point1 = this->Apply(inputRay.m_point1, RT::FWDTFORM);
        outputRay.m_point2 = this->Apply(inputRay.m_point2, RT::FWDTFORM);
        outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
    } else {
        // Zastosuj wsteczną transformację
        outputRay.m_point1 = this->Apply(inputRay.m_point1, RT::BCKTFORM);
        outputRay.m_point2 = this->Apply(inputRay.m_point2, RT::BCKTFORM);
        outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
    }

    return outputRay;
}

// Metoda zastosowująca transformację dla wektora
qbVector<double> RT::GTform::Apply(const qbVector<double> &inputVector, bool dirFlag) {
    // Przekonwertuj wejściowy wektor na wektor czterowymiarowy
    std::vector<double> tempData{inputVector.GetElement(0),
                                 inputVector.GetElement(1),
                                 inputVector.GetElement(2),
                                 1.0};
    qbVector<double> tempVector{tempData};

    // Utwórz wektor wynikowy
    qbVector<double> resultVector;

    if (dirFlag) {
        // Zastosuj transformacje
        resultVector = m_fwdtfm * tempVector;
    } else {
        // Zastosuj wsteczną transformację
        resultVector = m_bcktfm * tempVector;
    }

    // Przekształć wynik na trójwymiarowy wektor
    qbVector<double> outputVector{std::vector<double>{resultVector.GetElement(0),
                                                      resultVector.GetElement(1),
                                                      resultVector.GetElement(2)}};

    return outputVector;
}

// Przeciążanie operatorów
namespace RT {
    RT::GTform operator*(const RT::GTform &lhs, const RT::GTform &rhs) {
        // Utwórz iloczyn dwóch transformacji
        qbMatrix2<double> fwdResult = lhs.m_fwdtfm * rhs.m_fwdtfm;

        // Oblicz wsteczną transformację jako odwrotność transformacji
        qbMatrix2<double> bckResult = fwdResult;
        bckResult.Inverse();

        // Utwórz obiekt klasy gtfm przechowujący wynik
        RT::GTform finalResult(fwdResult, bckResult);

        return finalResult;
    }
}

// Przeciążenie operatora przypisania
RT::GTform RT::GTform::operator=(const RT::GTform &rhs) {
    // Jeśli nie przypisujemy tej samej wartości tych samych obiektów
    if (this != &rhs) {
        m_fwdtfm = rhs.m_fwdtfm;
        m_bcktfm = rhs.m_bcktfm;
    }

    return *this;
}

// Metoda wypisująca macierz transformacji
void RT::GTform::PrintMatrix(bool dirFlag) {
    if (dirFlag) {
        Print(m_fwdtfm);
    } else {
        Print(m_bcktfm);
    }
}

void RT::GTform::Print(const qbMatrix2<double> &matrix) {
    int nRows = matrix.GetNumRows();
    int nCols = matrix.GetNumCols();
    for (int row = 0; row < nRows; ++row) {
        for (int col = 0; col < nCols; ++col) {
            std::cout << std::fixed << std::setprecision(3) << matrix.GetElement(row, col) << " ";
        }
        std::cout << std::endl;
    }
}

// Metoda wypisująca wektor
void RT::GTform::PrintVector(const qbVector<double> &inputVector) {
    int nRows = inputVector.GetNumDims();
    for (int row = 0; row < nRows; ++row) {
        std::cout << std::fixed << std::setprecision(3) << inputVector.GetElement(row) << std::endl;
    }
}