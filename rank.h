#pragma once

#include "user.h"
#include "socket.h"

class Rank : public QWidget 
{
	Q_OBJECT

public:
	explicit Rank(int t_flag, QWidget *parent = Q_NULLPTR);
	~Rank() {}
	void handle_rank();

private:
	int flag;
	std::vector<QLabel*> id;
	std::vector<QLabel*> name;
	std::vector<QLabel*> level;
	std::vector<QLabel*> exp;
	std::vector<QLabel*> score;
	std::vector<challenger*> c_info;
	std::vector<provider*> p_info;

	QLabel *background;
	QPushButton *return_btn;

	void ui_init();
	void show_info(int size);

public slots:
	void rank_edone(QByteArray t_array);
	void rank_pdone(QByteArray t_array);

signals:
	void close_window();
};