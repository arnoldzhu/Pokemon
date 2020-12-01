#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>  //监听套接字（仅服务器）
#include <QTcpSocket>  //通信套接字
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QStringList>
#include <QDebug>
#include <cstdlib>
#include <ctime>
#include "pokemon.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWidget; }
QT_END_NAMESPACE

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();
    void Register(QStringList);
    void Login(QStringList);
    void SendUserinfo(QStringList);
    void RefreshPokemonInfo(QStringList);
    void SendAllPokemonInfo();
    void AddPokemon(QStringList);
    void UpdatePokemon(QStringList);
    void DeletePokemon(QStringList);
    void LastPokemon(QStringList&);
    void RefreshMatchesPlayed(QString, QString);
    void RefreshMatchesWon(QString, QString);
    void DefaultAction();
    int UserExists(QString&);
    void GiveNewUserPokemon(QString&);
    void InitFirePokemon(QStringList&);
    void InitFirePokemon(QString&, attribute);
    void InitAquaPokemon(QString&, attribute);
    void InitGrassPokemon(QString&, attribute);

private slots:

    void on_SendButton_clicked();

    void on_CloseButton_clicked();

private:
    Ui::ServerWidget *ui;

    QTcpServer *server = NULL; //监听套接字
    QTcpSocket *socket = NULL; //通信套接字

    QSqlDatabase db;

    QString username = "";
};
#endif // SERVERWIDGET_H
