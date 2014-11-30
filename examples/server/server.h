#ifndef SERVER_H
#define SERVER_H

// CORE includes
#include <QDateTime>
#include <QObject>
#include <QVariant>

// fwd
class MaiaXmlRpcServer;

class Server : public QObject
{
    Q_OBJECT

public:
    Server( QObject *parent = 0 );

private slots:
    QString callState( int i );
    QVariant birne( int x );
    void nix();
    QDateTime plusOneYear( QDateTime sometime );

private:
    MaiaXmlRpcServer *server;

};

#endif
