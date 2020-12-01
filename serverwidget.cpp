#include "serverwidget.h"
#include "ui_serverwidget.h"

Pokemon *mypokemon = NULL;
Pokemon *opponent = NULL;

ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    srand(time(NULL));

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("server_database.db");
    db.open();

    QSqlQuery q;

    //指定父对象，让其自动回收空间
    server = new QTcpServer(this);

    server->listen(QHostAddress::Any, 8888);

    setWindowTitle("服务器：8888");

    connect(server, &QTcpServer::newConnection,
            [=]()
            {
                //取出建立好连接的套接字
                socket = server->nextPendingConnection();

                //获取对方ip和端口
                QString ip = socket->peerAddress().toString();
                qint16 port = socket->peerPort();
                QString temp = QString("[%1:%2]:成功连接").arg(ip).arg(port);

                ui->ReadTextEdit->setText(temp);  //显示成功连接的信息

                connect(socket, &QTcpSocket::readyRead,
                        [=]()
                        {
                            //从通信套接字中取出内容
                            QByteArray array = socket->readAll();
                            QString str = array;
                            QStringList list = str.split(',');
                            QString mode = list[0];

                            qDebug() << "array:" << array;
                            qDebug() << "str:" << str;
                            qDebug() << "list:" << list;
                            qDebug() << "mode:" << mode;

                            //TODO:给list[i]变量加注释
                            if(mode == "register")
                                Register(list);
                            else if(mode == "login")
                                Login(list);
                            else if(mode == "askuserinfo")
                                SendUserinfo(list);
                            else if(mode == "winduelbattle") {
                                AddPokemon(list);
                                RefreshMatchesPlayed(list[1], list[12]);
                                RefreshMatchesWon(list[1], list[13]);
                            }
                            else if(mode == "winupgradebattle") {
                                UpdatePokemon(list);
                                RefreshMatchesPlayed(list[1], list[9]);
                                RefreshMatchesWon(list[1], list[10]);
                            }
                            else if(mode == "loseupgradebattle") {
                                RefreshMatchesPlayed(list[1], list[2]);
                            }
                            else if(mode == "refreshpokemoninfo")
                                RefreshPokemonInfo(list);
                            else if(mode == "askallpokemoninfo")
                                SendAllPokemonInfo();
                            else if(mode == "addpokemon")
                                AddPokemon(list);
                            else if(mode == "deletepokemon")
                                DeletePokemon(list);
                            else if(mode == "lastpokemon")
                                LastPokemon(list);
                            else
                                DefaultAction();

                            ui->ReadTextEdit->append(array);
                        }
                        );
            }
        );
}

ServerWidget::~ServerWidget()
{
    delete ui;
}

void ServerWidget::Register(QStringList list)
{
    QString username = list[1], password = list[2];
    QSqlQuery q;

    if(!UserExists(username)) {
        q.exec("insert into userinfo values('"+username+"', '"+password+"', 0, 0, 0, 0, 0)");

        GiveNewUserPokemon(username);

        //返回成功信息
        QString str = "registersuccess";

        //给对方发送数据，用socket套接字
        socket->write(str.toUtf8().data());
    }
    else {
        QString str = "registerfail";
        socket->write(str.toUtf8().data());
    }
}

int ServerWidget::UserExists(QString& str)
{
    QSqlQuery q;
    QString select_sql = "select username from userinfo";
    q.exec(select_sql);
    while(q.next())
        if(str == q.value(0).toString())
            return 1;

    return 0;
}


void ServerWidget::Login(QStringList list)
{
    QString username = list[1], password = list[2];

    QSqlQuery q;
    QString sql = "select * from userinfo where username='"+username+"'";
    q.exec(sql);

    while(q.next())
        if(password == q.value(1).toString()) {
            socket->write("loginsuccess");
            this->username = username;
            return ;
        }

    socket->write("loginfailed");
}

void ServerWidget::SendUserinfo(QStringList list)
{
    int i=0;
    QString str = "userlistinfo,";
    QString username = list[1];

    QSqlQuery q;
    q.exec("select * from pokemoninfo where username = '"+username+"'");

    //小精灵数据，格式为精灵的各属性值（号，名，属性，系，级，攻，防，速，生命，经）10条
    while(q.next()) {
        for(i=1; i<11; ++i) {
            ui->ReadTextEdit->append(q.value(i).toString());
            str.append(q.value(i).toString());
            str.append(",");
        }
        ui->ReadTextEdit->append(str);
    }

    //用户数据，格式：用户名|对战场数|队战胜场数|奖章数|金奖章数
    q.exec("select * from userinfo where username = '"+list[1]+"'");
    while(q.next()) {
        for(int j=0; j<7; ++j) {
            if(j!=1 && j!=2) {
                ui->ReadTextEdit->append(q.value(j).toString());
                str.append(q.value(j).toString());
                str.append(",");
            }
        }
        ui->ReadTextEdit->append(str);
    }

    //将数据发送出去
    socket->write(str.toUtf8().data());
}

void ServerWidget::AddPokemon(QStringList list)
{
    QSqlQuery q;

    //发送更新数据，格式为精灵的各属性值（主人名称，号，名，属性，系，级，攻，防，速，生命，经）
    QString user_name = list[1];
    QString number = list[2];
    QString name = list[3];
    QString attri = list[4];
    QString type = list[5];
    QString level = list[6];
    QString attack = list[7];
    QString defence = list[8];
    QString speed = list[9];
    QString max_life = list[10];
    QString exp = list[11];

    //写入数据库
    q.exec("insert into pokemoninfo values('"+user_name+"',"
     "'"+number+"', '"+name+"', '"+attri+"', '"+type+"', '"+level+"', "
     "'"+attack+"', '"+defence+"', '"+speed+"', '"+max_life+"', '"+exp+"')");
}

void ServerWidget::RefreshMatchesPlayed(QString username, QString matches_played)
{
    QSqlQuery q;

    //更新用户比赛总场数
    q.exec("UPDATE userinfo SET matches_played = '"+matches_played+"' where username = '"+username+"' ");
}

void ServerWidget::RefreshMatchesWon(QString username, QString matches_won)
{
    QSqlQuery q;

    //更新用户胜场
    q.exec("UPDATE userinfo SET matches_won = '"+matches_won+"' where username = '"+username+"' ");
}

void ServerWidget::RefreshPokemonInfo(QStringList list)
{
    int i = 0;
    QString str = "sendrefreshedpokemoninfo,";

    QString username = list[1];

    QSqlQuery q;
    q.exec("select * from pokemoninfo where username = '"+username+"'");

    //小精灵数据，格式为精灵的各属性值（号，名，属性，系，级，攻，防，速，生命，经）10条
    while(q.next()) {
        for(i=1; i<11; ++i) {
            ui->ReadTextEdit->append(q.value(i).toString());
            str.append(q.value(i).toString());
            str.append(",");
        }
        ui->ReadTextEdit->append(str);
    }

    socket->write(str.toUtf8().data());
}

void ServerWidget::SendAllPokemonInfo()
{
    //ui->ReadTextEdit->append("发送所有其他用户数据");
    int i=0;
    QString str = "allpokemoninfo,";

    //TODO:说明格式
    QSqlQuery q;
    q.exec("select * from pokemoninfo");
    while(q.next()) {
        for(i=0; i<10; ++i) {
            ui->ReadTextEdit->append(q.value(i).toString());
            str.append(q.value(i).toString());
            str.append(",");
        }
        socket->write(str.toUtf8().data());
        str = "pokemonlistinfo,";
    }
}

void ServerWidget::UpdatePokemon(QStringList list)
{
    //格式：(win, user, num, level, att, def, speed, life, exp, play, won) 11条
    QSqlQuery q;

    QString username = list[1];
    QString number = list[2];
    QString level = list[3];
    QString attack = list[4];
    QString defence = list[5];
    QString speed = list[6];
    QString life = list[7];
    QString exp = list[8];

    q.exec("UPDATE pokemoninfo SET level = '"+level+"', attack = '"+attack+"',"
           "defence = '"+defence+"', speed = '"+speed+"', max_life = '"+life+"', exp = '"+exp+"'  "
           "WHERE (username = '"+username+"' AND number = '"+number+"')");
}

void ServerWidget::DeletePokemon(QStringList list)
{
    ui->ReadTextEdit->append("删除精灵数据");
    QString user_name = list[1];
    QString number = list[2];
    ui->ReadTextEdit->append(user_name);
    ui->ReadTextEdit->append(number);

    QSqlQuery q;
    q.exec("DELETE FROM pokemoninfo WHERE (username = '"+user_name+"' AND number = '"+number+"')");
}

void ServerWidget::LastPokemon(QStringList& list)
{
    int i;
    //格式：(last, username, number,
    //username, number, name, att, ty, le, at, de, s, l, ex);
    ui->ReadTextEdit->append("lastpokemon\n");
    for(i = 0; i < 14; ++i)
        ui->ReadTextEdit->append(list[i]);
    DeletePokemon(list);

    for(i = 1; i < 12; ++i)
        list[i] = list[i+2];

    AddPokemon(list);
}

void ServerWidget::GiveNewUserPokemon(QString& username)
{
    int typenum;
    attribute attrinum;

    for(int i=0; i<3; ++i) {
        typenum = rand() % 3;
        attrinum = (attribute)(rand() % 4);
        switch (typenum) {
        case 0:
            InitFirePokemon(username, attrinum); break;
        case 1:
            InitAquaPokemon(username, attrinum); break;
        default:
            InitGrassPokemon(username, attrinum); break;
        }
    }
}

void ServerWidget::DefaultAction()
{
    ui->ReadTextEdit->append("default action");
}

void ServerWidget::on_SendButton_clicked()
{
    if(socket == NULL)
        return;

    //获取编辑区内容
    QString str = ui->WriteTextEdit->toPlainText();

    //给对方发送数据，用socket套接字
    socket->write(str.toUtf8().data());
}

void ServerWidget::on_CloseButton_clicked()
{
    if(socket == NULL)
        return;

    //主动和客户端断开连接
    socket->disconnectFromHost();
    socket->close();
    socket = NULL;
}

void ServerWidget::InitFirePokemon(QString& username, attribute attrinum)
{
    FirePokemon chedder((attribute)attrinum, "小火猴");
    QSqlQuery q;

    QString number = QString::number(chedder.GetNumber());
    QString name = chedder.GetName();
    QString attri = QString::number(chedder.GetAttribute());
    QString type = QString::number(chedder.GetType());
    QString level = QString::number(chedder.GetCurrentLevel());
    QString attack = QString::number(chedder.GetAttack());
    QString defence = QString::number(chedder.GetDefence());
    QString max_life = QString::number(chedder.GetMaxlife());
    QString speed = QString::number(chedder.GetSpeed());
    QString curr_life = QString::number(chedder.GetCurrentLife());
    QString exp =  QString::number(chedder.GetCurrentExp());

    ui->ReadTextEdit->append(u8"成功进入");

    //写入数据库
    q.exec("insert into pokemoninfo values('"+username+"',"
     "'"+number+"', '"+name+"', '"+attri+"', '"+type+"', '"+level+"', "
     "'"+attack+"', '"+defence+"', '"+speed+"', '"+max_life+"', '"+exp+"')");
}

void ServerWidget::InitAquaPokemon(QString& username, attribute attrinum)
{
    AquaPokemon chedder((attribute)attrinum, "波加曼");
    QSqlQuery q;

    QString number = QString::number(chedder.GetNumber());
    QString name = chedder.GetName();
    QString attri = QString::number(chedder.GetAttribute());
    QString type = QString::number(chedder.GetType());
    QString level = QString::number(chedder.GetCurrentLevel());
    QString attack = QString::number(chedder.GetAttack());
    QString defence = QString::number(chedder.GetDefence());
    QString max_life = QString::number(chedder.GetMaxlife());
    QString speed = QString::number(chedder.GetSpeed());
    QString curr_life = QString::number(chedder.GetCurrentLife());
    QString exp =  QString::number(chedder.GetCurrentExp());

    ui->ReadTextEdit->append(u8"成功进入");

    //写入数据库
    q.exec("insert into pokemoninfo values('"+username+"',"
     "'"+number+"', '"+name+"', '"+attri+"', '"+type+"', '"+level+"', "
     "'"+attack+"', '"+defence+"', '"+speed+"', '"+max_life+"', '"+exp+"')");
}

void ServerWidget::InitGrassPokemon(QString& username, attribute attrinum)
{
    GrassPokemon chedder((attribute)attrinum, "草苗龟");
    QSqlQuery q;

    QString number = QString::number(chedder.GetNumber());
    QString name = chedder.GetName();
    QString attri = QString::number(chedder.GetAttribute());
    QString type = QString::number(chedder.GetType());
    QString level = QString::number(chedder.GetCurrentLevel());
    QString attack = QString::number(chedder.GetAttack());
    QString defence = QString::number(chedder.GetDefence());
    QString max_life = QString::number(chedder.GetMaxlife());
    QString speed = QString::number(chedder.GetSpeed());
    QString curr_life = QString::number(chedder.GetCurrentLife());
    QString exp =  QString::number(chedder.GetCurrentExp());

    ui->ReadTextEdit->append(u8"成功进入");

    //写入数据库
    q.exec("insert into pokemoninfo values('"+username+"',"
     "'"+number+"', '"+name+"', '"+attri+"', '"+type+"', '"+level+"', "
     "'"+attack+"', '"+defence+"', '"+speed+"', '"+max_life+"', '"+exp+"')");
}
