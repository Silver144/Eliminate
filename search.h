#pragma once

#include "user.h"
#include "socket.h"

class Search : public QWidget
{
	Q_OBJECT

public:
	explicit Search(QWidget *parent = Q_NULLPTR);
	~Search() {}

private:
	QLineEdit *user_name;
	QPushButton *search_btn;
	QPushButton *return_btn;
	QLabel *info_lab;
	QLabel *name_lab;
	QLabel *score_lab;
	QLabel *exp_lab;
	QLabel *level_lab;

	void ui_init();
	void closeEvent(QCloseEvent *event);
	void info_show(int exp, int level, int hiscore, int flag);

public slots:
	void search_user();
	void search_done(QByteArray t_array);

signals:
	void close_window();
};