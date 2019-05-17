#pragma once

#include "user.h"
#include "socket.h"

class Game : public QWidget
{
	Q_OBJECT

public:
	explicit Game(QWidget *parent = Q_NULLPTR);
	~Game() {}
	void keyPressEvent(QKeyEvent *k);
	challenger *player;
	provider *examiner;
	bool cp;
	bool exit_flag;
	bool is_typing;
	void user_init();

private:
	std::string rt_word;
	void word_show();
	void in_stage_c();
	void in_stage_p();
	void word_lab_show();
	void check_fail();
	void closeEvent(QCloseEvent *event);
	void stage_init();
	void stage_init_c();
	void stage_init_p();
	void word_submit();
	void exit_player();
	void exit_examiner();

	QString choose_word();
	QLabel *rt_char;
	QPushButton *return_btn;
	QTimer *fresh;
	QTimer *score_timer;
	QTimer *ui_timer;
	QLabel *info_lab;
	QLabel *name_lab;
	QLabel *stage_lab;
	QLabel *exp_lab;
	QLabel *score_lab;
	QLabel *level_lab;
	QLabel *check_lab;
	std::vector<QLabel *> live_label;
	int c_stage;
	int lives;

private slots:
	void Game_exit();
	void type_word();
	// void widget_refresh(int *r_time, QLabel *time_lab, QLabel *word_lab);
	void widget_refresh();
	void word_cert();
	void showEvent(QShowEvent *e);

signals:
	void close_window();
};
