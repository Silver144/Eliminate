#include "Game.h"

// int yd = 0;

QString r_word;

QLabel *word_lab;
QLabel *time_lab;
QFontMetrics *fMetrics;

extern std::vector<word_inf> word_db;
extern std::mt19937 rand_num;
extern WSocket *tcp_socket;

int r_time;

int bonus_time;

bool first_in = true;

bool cursor_on = 0;

Game::Game(QWidget *parent)
{
	user_init();
}

void Game::showEvent(QShowEvent *event)
{
	if (first_in)
		stage_init();
}

void Game::closeEvent(QCloseEvent *event)
{
	emit close_window();
}

void Game::word_submit()
{
	bool flag = true;
	for (int i = 0; i < word_db.size(); i++)
		if (word_db[i].word == rt_char->text().toStdString())
		{
			flag = false;
			break;
		}
	if (flag)
	{
		check_lab->setText("Success");
		check_lab->setGeometry(701 - 20 * check_lab->text().length(), 190, 48 * check_lab->text().length(), 80);

		c_stage++;
		examiner->word_cnt++;
		word_db.push_back(word_inf(rt_char->text().toStdString()));
		examiner->word_buf.push_back(rt_char->text().toStdString());
		
		int lab_width;
		stage_lab->setText(QString::number(c_stage));
		lab_width = fMetrics->width(stage_lab->text());
		stage_lab->setGeometry(283 - lab_width / 2, 125, stage_lab->text().size() * 24, 30);

		score_lab->setText(QString::number(score_lab->text().toInt() + 1 + rt_char->text().length() / 3));
		lab_width = fMetrics->width(score_lab->text());
		score_lab->setGeometry(283 - lab_width / 2, 215, score_lab->text().size() * 24, 30);
	}
	else
	{
		check_lab->setText("Fail");
		check_lab->setGeometry(701 - 20 * check_lab->text().length(), 190, 48 * check_lab->text().length(), 80);
	}
	rt_char->setText("");
}

void Game::check_fail()
{
	lives--;
	delete live_label[live_label.size() - 1];
	live_label.pop_back();
	if (lives == 0)
		Game_exit();
	else
		type_word();
}

void Game::Game_exit()
{
	exit_flag = 1;
	first_in = true;
	return_btn->setEnabled(false);
	/*if (score_timer != NULL)
		delete score_timer;*/
	if (cp)
		exit_player();
	else
		exit_examiner();

}

void Game::exit_examiner()
{
	check_lab->setText("Exp +" + QString::number(int(1.0 * score_lab->text().toInt() / 10 + c_stage - 1)));
	check_lab->setGeometry(701 - 20 * check_lab->text().length(), 190, 48 * check_lab->text().length(), 80);

	int t_level = level_lab->text().toInt();
	int t_exp = examiner->exp + (int)(1.0 * score_lab->text().toInt() / 10 + c_stage - 1);
	while (t_exp > t_level * (t_level * 100 - 50))
	{
		t_exp -= t_level * (t_level * 100 - 50);
		t_level++;
	}
	examiner->exp = t_exp;
	examiner->level = t_level;
	examiner->word_cnt = c_stage;

	QString write_data;
	write_data.append(QChar(10));
	
	write_data.append(int(examiner->name.size()));
	write_data.append(QString::fromStdString(examiner->name));

	write_data.append(QString::number(t_exp).size());
	write_data.append(QString::number(t_exp));

	write_data.append(QString::number(t_level).size());
	write_data.append(QString::number(t_level));

	write_data.append(QString::number(c_stage).size());
	write_data.append(QString::number(c_stage));

	for (int i = 0; i < examiner->word_cnt; i++)
	{
		write_data.append(int(examiner->word_buf[i].size()));
		write_data.append(QString::fromStdString(examiner->word_buf[i]));
	}

	tcp_socket->socket->write(write_data.toUtf8().data());
}

void Game::exit_player()
{
	while (!live_label.empty())
	{
		QLabel *t_live = live_label[live_label.size() - 1];
		live_label.pop_back();
		delete t_live;
	}
	score_timer->stop();
	delete ui_timer;

	if (!is_typing)
	{
		delete fresh;
		word_lab->hide();
		time_lab->hide();
	}
	else
		rt_char->setText("");
	is_typing = 0;

	check_lab->setText("Exp +" + QString::number(int(1.0 * score_lab->text().toInt() / 10 + c_stage - 1)));
	check_lab->setGeometry(701 - 20 * check_lab->text().length(), 190, 48 * check_lab->text().length(), 80);
	check_lab->show();

	int hi_score = std::max(score_lab->text().toInt(), player->high_score);
	int t_level = level_lab->text().toInt();
	int t_exp = player->exp + (int)(1.0 * score_lab->text().toInt() / 10 + c_stage - 1);
	while (t_exp > t_level * (t_level * 100 - 50))
	{
		t_exp -= t_level * (t_level * 100 - 50);
		t_level++;
	}
	player->exp = t_exp;
	player->high_score = hi_score;
	player->level = t_level;
	QString s_exp = QString::number(t_exp);
	QString s_level = QString::number(t_level);
	QString s_high_score = QString::number(hi_score);
	
	QString write_data;
	write_data.append(QChar(7));
	
	write_data.append(QChar(int(player->name.size())));
	write_data.append(QString::fromStdString(player->name));

	write_data.append(QChar(s_exp.size()));
	write_data.append(s_exp);

	write_data.append(QChar(s_high_score.size()));
	write_data.append(s_high_score);

	write_data.append(QChar(s_level.size()));
	write_data.append(s_level);

	tcp_socket->socket->write(write_data.toUtf8().data());
}

void Game::stage_init()
{
	exit_flag = 0;
	check_lab = new QLabel(this);
	QFont font("Menlo", 48);
	check_lab->setFont(font);
	check_lab->setText("");

	ui_timer = new QTimer(this);
	connect(ui_timer, &QTimer::timeout, [this]
	{
		if (return_btn->geometry().contains(this->mapFromGlobal(QCursor::pos())))
		{
			cursor_on = 1;
			QPixmap icon1(":/image/flake_b.png");
			return_btn->setIcon(icon1);
			return_btn->setIconSize(QSize(128, 128));
		}
		if (cursor_on && !return_btn->geometry().contains(this->mapFromGlobal(QCursor::pos())))
		{
			cursor_on = 0;
			QPixmap icon1(":/image/flake.png");
			return_btn->setIcon(icon1);
			return_btn->setIconSize(QSize(128, 128));
		}
	}
	);
	ui_timer->start(50);

	setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
	int width = GetSystemMetrics(SM_CXFULLSCREEN);
	int height = GetSystemMetrics(SM_CYFULLSCREEN);
	move((width - 960) / 2, (height - 720) / 2);

	this->setFixedSize(960, 720);

	if (cp)
		stage_init_c();
	else
		stage_init_p();
}

void Game::stage_init_p()
{
	return_btn->setEnabled(true);


	c_stage = examiner->word_cnt;
	check_lab->show();
	info_lab->setText("Name\nTotal\nLevel\nScore\nExp");
	name_lab->setText(QString::fromStdString(examiner->name.substr(1, examiner->name.length() - 1)));
	stage_lab->setText(QString::number(examiner->word_cnt));
	score_lab->setText(QString::number(0));
	exp_lab->setText(QString::number(examiner->exp) + "/" + QString::number(examiner->level * (examiner->level * 100 - 50)));
	level_lab->setText(QString::number(examiner->level));

	QFont font("Gigi", 24, QFont::Bold);

	int lab_width;

	info_lab->setFont(font);
	info_lab->setGeometry(97, 72, 97, 225);

	font.setPixelSize(24);

	fMetrics = new QFontMetrics(QFont("A"));
	lab_width = fMetrics->width(name_lab->text());
	name_lab->setFont(font);
	name_lab->setGeometry(283 - lab_width / 2, 74, name_lab->text().size() * 24, 48);
	// name_lab->show();

	lab_width = fMetrics->width(stage_lab->text());
	stage_lab->setFont(font);
	stage_lab->setGeometry(283 - lab_width / 2, 125, stage_lab->text().size() * 24, 30);
	// stage_lab->show();

	lab_width = fMetrics->width(level_lab->text());
	level_lab->setFont(font);
	level_lab->setGeometry(283 - lab_width / 2, 170, level_lab->text().size() * 24, 30);
	// level_lab->show();

	lab_width = fMetrics->width(score_lab->text());
	score_lab->setFont(font);
	score_lab->setGeometry(283 - lab_width / 2, 215, score_lab->text().size() * 24, 30);

	lab_width = fMetrics->width(exp_lab->text());
	exp_lab->setFont(font);
	exp_lab->setGeometry(283 - lab_width / 2, 260, exp_lab->text().size() * 24, 30);

	in_stage_p();
}

void Game::in_stage_p()
{
	type_word();
}

void Game::stage_init_c()
{
	c_stage = 1;
	lives = 3;
	first_in = false;
	return_btn->setEnabled(true);

	for (int i = 0; i < 3; i++)
	{
		QLabel *t_lab = new QLabel(this);
		live_label.push_back(t_lab);
		QPixmap tp(":/image/player.png");
		t_lab->setPixmap(tp);
		t_lab->setGeometry(115 + 48 * i, 331, 48, 48);
		t_lab->show();
	}

	info_lab->setText("Name\nStage\nLevel\nScore\nExp");
	name_lab->setText(QString::fromStdString(player->name.substr(1, player->name.length() - 1)));
	stage_lab->setText(QString::number(1));
	exp_lab->setText(QString::number(player->exp) + "/" + QString::number(player->level * (player->level * 100 - 50)));
	score_lab->setText(QString::number(0));
	level_lab->setText(QString::number(player->level));

	QFont font("Gigi", 24, QFont::Bold);

	int lab_width;

	info_lab->setFont(font);
	info_lab->setGeometry(97, 72, 97, 225);
	// info_lab->show();

	font.setPixelSize(24);

	fMetrics = new QFontMetrics(QFont("A"));
	lab_width = fMetrics->width(name_lab->text());
	name_lab->setFont(font);
	name_lab->setGeometry(283 - lab_width / 2, 74, name_lab->text().size() * 24, 48);
	// name_lab->show();

	lab_width = fMetrics->width(stage_lab->text());
	stage_lab->setFont(font);
	stage_lab->setGeometry(283 - lab_width / 2, 125, stage_lab->text().size() * 24, 30);
	// stage_lab->show();

	lab_width = fMetrics->width(level_lab->text());
	level_lab->setFont(font);
	level_lab->setGeometry(283 - lab_width / 2, 170, level_lab->text().size() * 24, 30);
	// level_lab->show();

	lab_width = fMetrics->width(score_lab->text());
	score_lab->setFont(font);
	score_lab->setGeometry(283 - lab_width / 2, 215, score_lab->text().size() * 24, 30);
	// score_lab->show();

	lab_width = fMetrics->width(exp_lab->text());
	exp_lab->setFont(font);
	exp_lab->setGeometry(283 - lab_width / 2, 260, exp_lab->text().size() * 24, 30);
	// exp_lab->setGeometry(273 - QString::number(player->exp).size() * 24, 260, exp_lab->text().size() * 24, 30);
	// exp_lab->show();

	in_stage_c();
}

void Game::in_stage_c()
{
	return_btn->setEnabled(false);

	c_stage++;
	fresh = new QTimer(this);
	int t_time = int(1.0 * 1000 / pow(1.5, floor(1.0 * c_stage / 5)));
	r_word = choose_word();
	r_time = r_word.length();

	is_typing = 0;

	// QProgressBar *t_bar = new QProgressBar(this);
	// t_bar->setRange(1, word.length());
	word_lab_show();

	QFont font("Gigi", 48);
	time_lab->setFont(font);
	time_lab->setText(QString::number(r_time));
	time_lab->setGeometry(600, 500, 64, 64);

	time_lab->show();
	fresh->start(t_time);

	bool a = connect(fresh, SIGNAL(timeout()), this, SLOT(widget_refresh()));


}

void Game::user_init()
{

	QPixmap bg(":/image/gbg.png");
	QLabel *background = new QLabel(this);
	background->setPixmap(bg);
	// background->show();

	score_timer = new QTimer(this);
	connect(score_timer, &QTimer::timeout, [this] { qDebug() << bonus_time << "\n"; bonus_time++; });
	score_timer->start(1000);
	score_timer->stop();

	time_lab = new QLabel(this);
	word_lab = new QLabel(this);

	QPixmap icon1(":/image/flake.png");
	return_btn = new QPushButton(this);
	return_btn->setGeometry(832, 592, 128, 128);
	return_btn->setIcon(icon1);
	return_btn->setIconSize(QSize(128, 128));
	return_btn->setFlat(true);
	return_btn->setStyleSheet("QPushButton{border-radius:5px;border-width:0px;}");
	return_btn->setCursor(QCursor(Qt::PointingHandCursor));
	return_btn->setFocusPolicy(Qt::NoFocus);

	// return_btn->setEnabled(false);
	// return_btn->show();
	
	rt_char	  = new QLabel(this);
	info_lab  = new QLabel(this);
	// name_lab = new QLabel(QString::fromStdString(player->name), this);
	name_lab  = new QLabel(this);
	stage_lab = new QLabel(this);
	exp_lab   = new QLabel(this);
	score_lab = new QLabel(this);
	level_lab = new QLabel(this);

	connect(return_btn, SIGNAL(clicked()), this, SLOT(Game_exit()));
}

void Game::word_lab_show()
{
	word_lab->setText(r_word);

	QFont font("Menlo", 48, QFont::Bold);
	word_lab->setFont(font);

	QPalette pa;
	pa.setColor(QPalette::WindowText, Qt::white);
	word_lab->setPalette(pa);

	word_lab->setGeometry(701 - 20 * r_word.size(), 190, 48 * r_word.size(), 80);
	word_lab->show();
}

void Game::type_word()
{
	if (cp)
	{
		word_lab->hide();
		time_lab->hide();
		rt_char->setText("");

		bonus_time = 0;
		score_timer->start();
	}
	
	/*while (!(rt_char->text()))
	{
		delete player->rt_char.back();
		player->rt_char.pop_back();
	}*/
	// delete fresh;
	return_btn->setEnabled(true);
	is_typing = 1;
	// return_btn->setEnabled(true);
	// setFocusPolicy(Qt::StrongFocus);
}

void Game::widget_refresh()
{
	// qDebug() << "1\n";
	if (r_time == 0)
	{	
		delete fresh;
		type_word();
	}
	else
	{
		r_time--;

		QFont font("Gigi", 48);
		time_lab->setFont(font);
		time_lab->setText(QString::number(r_time));
		time_lab->show();
	}
}

void Game::word_cert()
{
	/*delete score_timer;
	score_timer = NULL;*/
	score_timer->stop();
	// return_btn->setEnabled(false);

	if (rt_char->text() == r_word)
	{
		rt_char->setText("");
		word_show();

		int lab_width;
		int bonus = (int)c_stage * 4 * 1.0 / (0.5 + 0.25 * (bonus_time / 2));

		score_lab->setText(QString::number(score_lab->text().toInt() + bonus));
		lab_width = fMetrics->width(score_lab->text());
		qDebug() << "score: " << score_lab->text().toInt() << "\n" << "time: " << bonus_time << "\n";
		score_lab->setGeometry(283 - lab_width / 2, 215, score_lab->text().size() * 24, 30);

		stage_lab->setText(QString::number(stage_lab->text().toInt() + 1));
		lab_width = fMetrics->width(stage_lab->text());
		stage_lab->setGeometry(283 - lab_width / 2, 125, stage_lab->text().size() * 24, 30);

		in_stage_c();
	}
	else
		check_fail();
		// exit(0);
}

QString Game::choose_word()
{
	std::vector<int> tmp;
	for (int i = 0; i < word_db.size(); i++)
		if (!word_db[i].used)
			tmp.push_back(i);
	if (tmp.empty())
	{
		for (int i = 0; i < word_db.size(); i++)
			word_db[i].used = 0;
		int rc = rand_num() % word_db.size();
		word_db[rc].used = 1;
		return QString::fromStdString(word_db[rc].word);
	}
	int r = rand_num() % tmp.size();
	word_db[tmp[r]].used = 1;
	return QString::fromStdString(word_db[tmp[r]].word);
	// return word_db[r].used ? choose_word() : QString::fromStdString(word_db[r].word);
	// return "topology";
	// return QString::fromStdString(word_db[rand_num() % word_db.size()].first);
}

void Game::keyPressEvent(QKeyEvent *k)
{
	qDebug() << "string: " << rt_char->text() << "\n";
	if (exit_flag)
	{
		delete check_lab;
		emit close_window();
		return;
	}
	if (!check_lab->text().isEmpty())
		check_lab->setText("");
	int t_key = k->key();
	if (!is_typing)
	{
		if (t_key == Qt::Key_Space)
		{
			r_time = 0;
			widget_refresh();
		}
		return;
	}
	// qDebug() << "keyEvent\n";
	if (('A' <= t_key && t_key <= 'Z') || ('a' <= t_key && t_key <= 'z'))
	{
		if (cp)
			if (rt_char->text().size() == r_word.size())
				return;
		QString Q_str = k->text();
		Q_str = rt_char->text() + Q_str;
		qDebug() << "Q_str " << Q_str;
		rt_char->setText(Q_str);
	}
	else if (t_key == Qt::Key_Backspace && rt_char->text().size())
	{
		QString Q_str = rt_char->text().left(rt_char->text().length() - 1);
		rt_char->setText(Q_str);
	}
	else if (t_key == Qt::Key_Return)
	{
		if (cp)
			word_cert();
		else
			word_submit();
	}
	else;
	word_show();
}

void Game::word_show()
{
	if (!rt_char->text().size())
		return;
	int w_size = rt_char->text().size();
	QFont font("Menlo", 48);
	
	QPalette pa;
	pa.setColor(QPalette::WindowText, QColor(189, 170, 170));
	rt_char->setPalette(pa);
	
	int x_type = cp ? 700 : 580;

	rt_char->setFont(font);
	rt_char->setGeometry(x_type - w_size * 24, 480, 48 * w_size, 100);
	rt_char->show();
}
