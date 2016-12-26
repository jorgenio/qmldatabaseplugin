#include "database.h"

#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtQml/qqml.h>
#include <QtQml/QQmlContext>

class Database::Private : public QObject
{
    Q_OBJECT
public:
    Private(Database *parent);

private:
    Database *q;

public:
    QList<QObject *> contents;
    bool open;
};

Database::Private::Private(Database *parent)
    : QObject(parent)
    , q(parent)
    , open(false)
{
}

Database::Database(QObject *parent)
    : QObject(parent)
    , m_hostName("localhost")
    , d(new Private(this))
{
}

QQmlListProperty<QObject> Database::contents()
{
    return QQmlListProperty<QObject>(this, d->contents);
}


bool Database::open()
{
    if (!d->open) {
        if (m_databaseName.isNull()) return false;
        if (m_connectionName.isEmpty()) {
            QQmlContext *context = qmlContext(this);
            connectionName(context->nameForObject(this));
        }

        if (!QSqlDatabase::contains(m_connectionName)) {
            QSqlDatabase db = QSqlDatabase::addDatabase(m_type, m_connectionName);
            db.setHostName(m_hostName);
            db.setDatabaseName(m_databaseName);
            db.setUserName(m_userName);
            db.setPassword(m_password);
            db.setPort(m_port.toInt(0,10));
            if (db.open()) {
                open(true);
            } else {
                qDebug() << Q_FUNC_INFO << __LINE__ << db.lastError().text();
            }
        } else {
            open(QSqlDatabase::database(m_connectionName).isOpen());
        }
    }
    return d->open;
}

bool Database::isOpen() const
{
    return d->open;
}

void Database::open(bool open)
{
    if (d->open == open) return;
    d->open = open;
    emit openChanged(open);
}

bool Database::transaction()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    return db.transaction();
}

bool Database::commit()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    return db.commit();
}

bool Database::rollback()
{
    QSqlDatabase db = QSqlDatabase::database(m_connectionName);
    return db.rollback();
}

#include "release/database.moc"
