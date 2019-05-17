#pragma once
#pragma  execution_character_set("utf-8")

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QPushbutton>
#include <QLineEdit>
#include <QMessageBox>
#include <string>
#include <iostream>
#include <QDebug>
#include <fstream>
#include <QRadioButton>
#include <QButtonGroup>
#include <Windows.h>
#include <QKeyEvent>
#include <QPixmap>
#include <QPixmapCache>
#include <QTimer>
#include <random>
#include <chrono>

std::string sha256(std::string message);

class user
{
public:
	user(std::string in_name) : name(in_name) {}
	~user() {}
	std::string name;
	int exp;
	int level;
	int cert_user(std::string user_passwd);

	void init_word();
	bool ac_cert();
	void ac_write(std::string password, bool flag);
};

class challenger : public user
{
	friend bool operator<(const challenger a, const challenger b)
	{
		if (a.level < b.level)
			return false;
		else if (a.level == b.level)
		{
			if (a.exp < b.exp)
				return false;
			else if (a.exp == b.exp)
				if (a.high_score < b.high_score)
					return false;
		}
		return true;
	}
public:
	challenger(std::string in_name) : user(in_name) {}
	~challenger() {}
	int high_score;
	std::tuple<int, int, int> get_c_info(std::string c_name);
	void write_back();
};

class provider : public user
{
	friend bool operator<(const provider &a, const provider &b)
	{
		if (a.level < b.level)
			return false;
		else if (a.level == b.level)
		{
			if (a.word_cnt < b.word_cnt)
				return false;
		}
		return true;
	}
public:
	provider(std::string in_name) : user(in_name) {}
	~provider() {}
	int word_cnt;

	std::vector<std::string> word_buf;
	std::tuple<int, int, int, std::vector<std::string>> get_p_info(std::string c_name);
	void write_back();
};


class word_inf
{
public:
	word_inf(std::string s1)
	{
		word = s1;
		used = false;
		diff = s1.length() / 3 + 1;
	}
	~word_inf() {}

	std::string word;
	bool used;
	int diff;
};

extern std::vector<word_inf> word_db;
extern std::mt19937 rand_num;