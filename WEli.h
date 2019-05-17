#pragma once

#include "search.h"
#include "Game.h"
#include "rank.h"
#include "loginW.h"
#include "socket.h"

class WEli : public QWidget
{
	Q_OBJECT

public:
	explicit WEli(QWidget *parent = Q_NULLPTR);
	~WEli() {}
	void keyPressEvent(QKeyEvent *k);

	QLabel *start_lab;
	QLabel *search_lab;
	QLabel *rank_lab;
	QLabel *exit_lab;

	QLabel *start_lab2;
	QLabel *search_lab2;
	QLabel *rank_lab2;
	QLabel *exit_lab2;

	loginW *login_window;
	Game *game_win;
	Search *search_win;
	Rank *rank_win;

	int state;

private:
	void ui_init();
	void init(std::string c_name);
	void init_challenger(QString c_name);
	void init_examiner(QString c_name);

public slots:
	void word_done(QByteArray t_array);
	void start_game();
	void start_search();
	void start_rank();
	void ls_from(std::string c_name);
	void challenger_done(QByteArray t_array);
	void examiner_done(QByteArray t_array);
};

//extern std::vector<word_inf> word_db;
//
//extern std::mt19937 rand_num;