#ifndef ANIMAL_H
#define ANIMAL_H

#include <QStringList>

class Animal
{
public:
    Animal(const int &id,const bool &done, const QString &type, const QString &size);

    int animalId() const;
    void setAnimalId(int animalid);

    QString animalType() const;
    void setAnimalType(const QString &animalType);

    QString animalSize() const;
    void setAnimalSize(const QString &animalSize);

    bool done() const;
    void setDone(bool done);




private:
    int m_animalId;
    bool m_done;
    QString m_animalType;
    QString m_animalSize;
};

#endif // ANIMAL_H
