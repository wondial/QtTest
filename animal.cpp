#include "animal.h"

Animal::Animal(const int &id, const bool &done, const QString &type, const QString &size)
    : m_animalId(id), m_done(done), m_animalType(type), m_animalSize(size)
{}

QString Animal::animalType() const
{
    return m_animalType;
}

QString Animal::animalSize() const
{
    return m_animalSize;
}

void Animal::setAnimalType(const QString &animalType)
{
    m_animalType = animalType;
}

void Animal::setAnimalSize(const QString &animalSize)
{
    m_animalSize = animalSize;
}

bool Animal::done() const
{
    return m_done;
}

void Animal::setDone(bool done)
{
    m_done = done;
}

int Animal::animalId() const
{
    return m_animalId;
}

void Animal::setAnimalId(int animalId)
{
    m_animalId = animalId;
}
