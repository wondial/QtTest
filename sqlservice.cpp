#include "sqlservice.h"
#include <QDebug>

SqlService::SqlService(){}

void SqlService::setLastError(const QString &lastError)
{
    m_lastError = lastError;
    qDebug() << m_lastError;
}

bool SqlService::prepareSql(const QString &querySql)
{
    return m_sqlQuery.prepare(querySql);
}

bool SqlService::exec(const QString &querySql)
{
    return m_sqlQuery.exec(querySql);
}

bool SqlService::exec()
{
    return m_sqlQuery.exec();
}

bool SqlService::openDatabase(QString dbname, const QString &type)
{
    if (QSqlDatabase::contains(dbname)) {
        m_sqlDatabase = QSqlDatabase::database(dbname);
    } else {
        m_sqlDatabase = QSqlDatabase::addDatabase(type);
        m_sqlDatabase.setDatabaseName(dbname);
    }

    if (!m_sqlDatabase.open()) {
        setLastError(m_sqlDatabase.lastError().text());
        return false;
    }
    m_sqlQuery = QSqlQuery(m_sqlDatabase);
    qDebug() << "Open database success!";
    return true;
}

bool SqlService::createTable(QString table, QMap<QString, QString> map) {
  QStringList tables = m_sqlDatabase.tables();
  if (m_sqlDatabase.contains(table)) {
    return false;
  }

  QString NEWTABLE_SQL = QString("CREATE TABLE IF NOT EXISTS %1 (").arg(table);
  QMapIterator<QString, QString> tmp(map);
  while (tmp.hasNext()) {
    tmp.next();
    NEWTABLE_SQL.append(QString("%1 %2").arg(tmp.key()).arg(tmp.value()));
    if (tmp.hasNext()) {
      NEWTABLE_SQL.append(",");
    }
  }
  NEWTABLE_SQL.append(")");
  qDebug() << NEWTABLE_SQL;
  return this->exec(NEWTABLE_SQL);
}

QVariant SqlService::insertItem(QString table, QVariantMap map)
{
    QMap<QString, QString> tableContentMap;

    // QStringList tables = m_sqlDatabase.tables();

    bool b = m_sqlQuery.exec(QString("select count(*) from sqlite_master where "
                                     "type='table' and name='%1'")
                                 .arg(table));
    if (!b) {
      setLastError(QString("没有发现 %1 表!").arg(table));
      return false;
    }

    tableContentMap = getTableInfo(table);

    if (tableContentMap.isEmpty())
      return false;

    QString INSERTITEM_SQL = QString("insert into %1 (").arg(table);
    QString values = QString("values (");
    QMapIterator<QString, QString> i(tableContentMap);
    while (i.hasNext()) {
        i.next();
        INSERTITEM_SQL.append(QString("%1").arg(i.key()));
        values.append("?");
        if (i.hasNext()) {
            INSERTITEM_SQL.append(",");
            values.append(",");
        }
    }
    INSERTITEM_SQL.append(")");
    values.append(")");
    INSERTITEM_SQL.append(values);
    qDebug() << INSERTITEM_SQL;
    qDebug() << values;
    if (!this->prepareSql(INSERTITEM_SQL)) {
      return false;
    }
    QMapIterator<QString, QString> ii(tableContentMap);
    while (ii.hasNext()) {
        ii.next();
        m_sqlQuery.addBindValue(map[ii.key()]);
    }

    this->exec();
    return m_sqlQuery.lastInsertId ();

}

bool SqlService::updateItem(QString table,QVariantMap map, QString name)
{
    QString content = QString("update %1 set ").arg(table);
    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext()) {
        i.next();
        if (i.hasNext())
            content += QString("%1 = '%2', ").arg(i.key()).arg(i.value().toString());
        else
            content += QString("%1 = '%2' ").arg(i.key()).arg(i.value().toString());
    }
    content += QString("where %1 = %2").arg(name).arg(map[name].toString());
    return this->exec(content);
}

bool SqlService::deleteItem(QString table, QVariantMap map)
{
    QString DELETE_SQL = QString("delete from %1 where %2 = %3").arg(table);
    QMapIterator<QString, QVariant> mi(map);
    while (mi.hasNext()) {
        mi.next();
        if (mi.hasNext())
            DELETE_SQL += QString("%1 = '%2', ").arg(mi.key()).arg(mi.value().toString());
        else
            DELETE_SQL += QString("%1 = '%2' ").arg(mi.key()).arg(mi.value().toString());
    }

    return this->exec(DELETE_SQL);
}

QVector<QVariantMap> SqlService::getItems(int pageFrom, int pageNum)
{
    QVector<QVariantMap> list;
    if (!m_sqlQuery.seek(pageFrom)) {
        setLastError("getValues error![The number of pages is beyond the limit]");
        return list;
    }
    do {
        QVariantMap map;
        for (int i = 0; i < m_sqlQuery.record().count(); i++)
            map.insert(m_sqlQuery.record().field(i).name(), m_sqlQuery.record().field(i).value());
        list.append(map);
    } while (m_sqlQuery.next() && --pageNum);
    return list;
}

QVector<QVariantMap> SqlService::getItems()
{
    QVector<QVariantMap> list;
    if (m_sqlQuery.size() < 0) {
        setLastError("getValues error![The number of pages is beyond the limit]");
        return list;
    }
    do {
        QVariantMap map;
        for (int i = 0; i < m_sqlQuery.record().count(); i++)
            map.insert(m_sqlQuery.record().field(i).name(), m_sqlQuery.record().field(i).value());
        list.append(map);
    } while (m_sqlQuery.next());
    return list;
}

bool SqlService::initDb(QString dbname, QString table,
                        QMap<QString, QString> map) {
  if (openDatabase(dbname))
    return createTable(table, map);
  else {
    setLastError(m_sqlDatabase.lastError().text());
    return false;
  }
}

QMap<QString, QString> SqlService::getTableInfo(QString table)
{
    QMap<QString, QString> result;
    const auto GET_TABLE_INFO_SQL =
        QString(R"(PRAGMA table_info( %1 ))").arg(table);
    qDebug() << GET_TABLE_INFO_SQL;
    m_sqlQuery.prepare(GET_TABLE_INFO_SQL);
    if (m_sqlQuery.exec()) {
      //        for (int i = 0; i < 2; i++)
      //            result.insert(m_sqlQuery.record().field(i).name(),
      //            m_sqlQuery.record().field(i).value().toString());
      while (m_sqlQuery.next()) {
        //        qDebug() << QString(QString::fromLocal8Bit("字段数:%1
        //        字段名:%2  字段类型:%3"))
        //                        .arg(query.value(0).toString())
        //                        .arg(query.value(1).toString())
        //                        .arg(query.value(2).toString());
        result.insert(m_sqlQuery.value(1).toString(),
                      m_sqlQuery.value(2).toString());
      }
    }
    return result;
}
