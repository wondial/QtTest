#ifndef ANIMAL_H
#define ANIMAL_H

#include <QStringList>

class Animal
{
public:
    Animal(const bool &done, const QString &type, const QString &size);

    QString animalType() const;
    void setAnimalType(const QString &animalType);

    QString animalSize() const;
    void setAnimalSize(const QString &animalSize);

    bool done() const;
    void setDone(bool done);

private:
    bool m_done;
    QString m_animalType;
    QString m_animalSize;
};

#endif // ANIMAL_H
