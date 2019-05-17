#pragma once

#include <user.h>
#include <QtNetWork/QtNetWork>

#define LOGIN_MSG 1
#define WORD_ADD 2
#define SEARCH_MSG 3
#define RANK_PLAYER 4
#define INIT_PLAYER 5
#define USER_REGI 6
#define WRITE_PLAYER 7
#define INIT_EXAMINER 8
#define RANK_EXAMINER 9
#define WRITE_EXAMINER 10

class WSocket : public QObject
{
	Q_OBJECT

public:
	explicit WSocket();
	QTcpSocket *socket;

public slots:
	void read_socket();

signals: 
	void word_m(QByteArray);
	void login_m(QByteArray);
	void search_m(QByteArray);
	void rank_mp(QByteArray);
	void ip_m(QByteArray);
	void regi_m(QByteArray);
	void ie_m(QByteArray);
	void rank_me(QByteArray);
};