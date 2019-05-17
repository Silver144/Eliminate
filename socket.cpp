#include "socket.h"

WSocket *tcp_socket;

WSocket::WSocket()
{
	socket = new QTcpSocket(this);
	
	connect(socket, &QTcpSocket::connected, [this]
	{
		QMessageBox socket_message(QMessageBox::NoIcon, "系统消息", "connect success", QMessageBox::Yes);
		if (socket_message.exec() == QMessageBox::Yes);
		connect(socket, SIGNAL(readyRead()), this, SLOT(read_socket()));
	});
}

void WSocket::read_socket()
{
	QByteArray t_array;
	t_array = socket->readAll();
	int t_switch = t_array.at(0);
	switch (t_switch)
	{
	case LOGIN_MSG:
		emit login_m(t_array);
		break;

	case WORD_ADD:
		emit word_m(t_array);
		break;

	case SEARCH_MSG:
		emit search_m(t_array);
		break;
	
	case RANK_PLAYER:
		emit rank_mp(t_array);
		break;

	case INIT_PLAYER:
		emit ip_m(t_array);
		break;

	case USER_REGI:
		emit regi_m(t_array);
		break;

	case INIT_EXAMINER:
		emit ie_m(t_array);
		break;

	case RANK_EXAMINER:
		emit rank_me(t_array);
		break;
	}
}