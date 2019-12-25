#include "animalmodel.h"
#include <QDebug>
#include <QString>

AnimalModel::AnimalModel(QObject *parent) : QAbstractListModel(parent)
{
    m_dbName = "MyTest.db";
    m_table = "animal_info";

    QMap<QString, QString> map;

    map.insert("done", "bool");
    map.insert("type", "QString");
    map.insert("size", "QString");
    m_db.initDb(m_dbName, m_table, map);
}

void AnimalModel::m_pushdata(const bool &done, const QString &animalType, const QString &animalSize)
{
    Animal a(done, animalType, animalSize);
    m_animals.append(a);
}

void AnimalModel::m_insert(int index,
                           const bool &done,
                           const QString &animalType,
                           const QString &animalSize)
{
    Animal a(done, animalType, animalSize);
    beginInsertRows(QModelIndex(), index, index);
    m_animals.insert(m_animals.begin() + index, a);
    endInsertRows();

    m_map.insert("done", done);
    m_map.insert("type", animalType);
    m_map.insert("size", animalSize);
    m_db.insertItem(m_table, m_map);
    m_map.clear();
}

void AnimalModel::m_removeOne(int index)
{
    //    qDebug() << index; //QString::number(index);
    Animal animal = m_animals.takeAt(index);
    qDebug() << animal.animalType();
    beginRemoveRows(QModelIndex(), index, index);
    m_animals.erase(m_animals.begin() + index);
    endRemoveRows();
    //int i = int(m_animals.begin() + index);

}

void AnimalModel::m_removeCompleted()
{
    for (int i = 0; i < m_animals.size();) {
        if (m_animals.at(i).done()) {
            beginRemoveRows(QModelIndex(), i, i);

            m_animals.removeAt(i);

            endRemoveRows();
        } else {
            ++i;
        }
    }
}

void AnimalModel::m_clear()
{
    //清除rows 界面将不显示
    beginRemoveRows(QModelIndex(), 0, m_animals.size());
    //清空动态数组
    m_animals.clear();
    endRemoveRows();
}

void AnimalModel::m_addAnimal()
{
    Animal animal(false, "", "");
    beginInsertRows(QModelIndex(), m_animals.size(), m_animals.size());
    //    qDebug() << "m_animals.size:" << m_animals.size ();
    m_animals.append(animal);
    //    qDebug() << "m_animals.size:" << m_animals.size ();
    endInsertRows();
}

int AnimalModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;
    return m_animals.count();
}

QVariant AnimalModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_animals.count())
        return QVariant();

    const Animal &animal = m_animals[index.row()];
    if (role == TypeRole)
        return animal.animalType();
    else if (role == SizeRole)
        return animal.animalSize();
    else if (role == Done)
        return animal.done();
    return QVariant();
}

bool AnimalModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() > m_animals.size())
        return false;

    const Animal &oldanimal = m_animals[index.row()];
    bool animalDone = oldanimal.done();
    QString animalType = oldanimal.animalType();
    QString animalSize = oldanimal.animalSize();

    //    qDebug() << "animalDone:" << animalDone;
    //    qDebug() << "index:" << index.row();
    //    qDebug() << "role:" << role;

    Animal animal(false, "", "");
    bool flag = false;

    switch (role) {
    case Done:
        if (animalDone != value.toBool() && (animalType != "" || animalSize != "")) {
            Animal t_animal(value.toBool(), animalType, animalSize);
            animal = t_animal;
            flag = true;
            //qDebug() << "here:" << role;
        }
        break;
    case TypeRole:
        if (animalType != value.toString()) {
            Animal t_animal(animalDone, value.toString(), animalSize);
            animal = t_animal;
            flag = true;
        }
        break;
    case SizeRole:
        if (animalSize != value.toString()) {
            Animal t_animal(animalDone, animalType, value.toString());
            animal = t_animal;
            flag = true;
        }
        break;
    }

    if (flag) {
        //        qDebug() << "New:" << animal.done() << "  " << animal.animalType() << "  "
        //                 << animal.animalSize();

        m_animals.replace(index.row(), animal);
        flag = false;

        return true;
    } else {
        return false;
    }
}

QHash<int, QByteArray> AnimalModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Done] = "done";
    roles[TypeRole] = "type";
    roles[SizeRole] = "size";
    return roles;
}
