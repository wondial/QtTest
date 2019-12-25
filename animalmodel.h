#ifndef ANIMALMODEL_H
#define ANIMALMODEL_H

#include <QAbstractListModel>
#include "animal.h"

class AnimalModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AnimalModel(QObject *parent = nullptr);

    enum AnimalRoles {
        TypeRole = Qt::UserRole + 1,
        Done,
        SizeRole
    };

    //外部接口 QML调用 添加数据
    Q_INVOKABLE void m_pushdata(const bool &done, const QString &animalType, const QString &animalSize);
    //外部接口 QML调用 添加数据在指定行
    Q_INVOKABLE void m_insert(int index, const bool &done, const QString &animalType, const QString &animalSize);
    //外部接口 删除指定行
    Q_INVOKABLE void m_removeOne(int index);
    Q_INVOKABLE void m_removeCompleted();
    //外部接口 清除 model
    Q_INVOKABLE void m_clear();
    //外部接口 C++调用 添加数据
    Q_INVOKABLE void m_addAnimal();

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;


protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Animal> m_animals;
};

#endif // ANIMALMODEL_H
