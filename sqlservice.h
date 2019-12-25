#ifndef SQLSERVICE_H
#define SQLSERVICE_H
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QString>
#include <QVariantMap>

class SqlService
{
public:
    SqlService();

    void setLastError(const QString &lastError);
    QSqlQuery &getSqlQuery() { return m_sqlQuery; }
    QSqlDatabase &getSqlDatabase() { return m_sqlDatabase; }

    bool isTableExist(QString table);
    bool isItemExist(QVariantMap map);
    bool prepareSql(const QString &query);
    bool exec(const QString &query);
    bool exec();


    bool openDatabase(QString name, const QString &type = "QSQLITE");
    bool createTable(QString table, QMap<QString, QString> map);
    bool insertItem(QString table, QVariantMap map);
    bool updateItem(QString table, QString name, QVariantMap map);
    bool deleteItem(QString table, QVariantMap map);
    QVariantMap findItem(QString table, QVariantMap map);
    QVector<QVariantMap> getItems(int pageFrom, int pageNum);
    QVector<QVariantMap> getItems();
    void initDb(QString dbname, QString table, QMap<QString, QString> map);

    QString getLastErrors() const;

private:
    QString m_lastError;
    QSqlQuery m_sqlQuery;
    QSqlDatabase m_sqlDatabase;

    QMap<QString, QString> getTableInfo(QString table);
};

#endif // SQLSERVICE_H