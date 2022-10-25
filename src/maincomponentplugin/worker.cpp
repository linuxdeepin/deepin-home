#include "./worker.h"

Worker::Worker(QObject *parent)
    : QObject(parent)
{}

Worker::~Worker() {}

QString Worker::getNode()
{
    qDebug() << "get node";
    QDBusInterface daemon("com.deepin.Home.Daemon",
                          "/com/deepin/Home/Daemon",
                          "com.deepin.Home.Daemon",
                          QDBusConnection::sessionBus());
    QDBusReply<QString> reply = daemon.call("getNode");
    return reply;
};
QString Worker::getLanguage()
{
    qDebug() << "get language";
    QDBusInterface daemon("com.deepin.Home.Daemon",
                          "/com/deepin/Home/Daemon",
                          "com.deepin.Home.Daemon",
                          QDBusConnection::sessionBus());
    QDBusReply<QString> reply = daemon.call("getLanguage");
    return reply;
};
QString Worker::getMachineID()
{
    qDebug() << "get machine id";
    QDBusInterface daemon("com.deepin.Home.Daemon",
                          "/com/deepin/Home/Daemon",
                          "com.deepin.Home.Daemon",
                          QDBusConnection::sessionBus());
    QDBusReply<QString> reply = daemon.call("getMachineID");
    return reply;
};
void Worker::markRead(QString channel, QString topic, QString uuid)
{
    qDebug() << "mark read";
    QList<QVariant> args;
    args << channel << topic << uuid;
    QDBusInterface daemon("com.deepin.Home.Daemon",
                          "/com/deepin/Home/Daemon",
                          "com.deepin.Home.Daemon",
                          QDBusConnection::sessionBus());

    daemon.callWithArgumentList(QDBus::NoBlock, "markRead", args);
};
bool Worker::isRead(QString channel, QString topic, QString uuid)
{
    qDebug() << "is read";
    QList<QVariant> args;
    args << channel << topic << uuid;
    QDBusInterface daemon("com.deepin.Home.Daemon",
                          "/com/deepin/Home/Daemon",
                          "com.deepin.Home.Daemon",
                          QDBusConnection::sessionBus());
    QDBusReply<bool> reply = daemon.callWithArgumentList(QDBus::BlockWithGui, "isRead", args);
    return reply;
};