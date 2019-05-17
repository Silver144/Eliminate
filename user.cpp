#include "user.h"

int user::cert_user(std::string user_passwd)
{
	std::string c_name, encrypt_passwd;
	std::ifstream user_data("name_passwd.txt");
	if (user_data.is_open())
	{
		while (!user_data.eof())
		{
			getline(user_data, c_name);
			if (c_name == "0" + name || c_name == "1" + name)
			{
				getline(user_data, encrypt_passwd);
				user_data.close();
				if (encrypt_passwd == user_passwd)
					return std::stoi(c_name.substr(0, 1));
				else
					return 2;
			}
			getline(user_data, c_name);
		}
	}
	return 3;
}

void user::init_word()
{
	std::random_device rd;
	rand_num.seed(rd());

	std::ifstream word_data("word.txt");
	std::string tmp_str;
	if (word_data.is_open())
	{
		while (!word_data.eof())
		{
			getline(word_data, tmp_str);
			std::string word_master = tmp_str;
			getline(word_data, tmp_str);
			int word_num = std::stoi(tmp_str);
			for (int i = 0; i < word_num; i++)
			{
				getline(word_data, tmp_str);
				word_db.push_back(word_inf(tmp_str));
			}
		}
		word_data.close();
	}
}

bool user::ac_cert()
{
	std::string t_name;
	std::ifstream user_data("name_passwd.txt");
	if (user_data.is_open())
	{
		while (!user_data.eof())
		{
			getline(user_data, t_name);
			if (t_name == "0" + name || t_name == "1" + name)
			{
				user_data.close();
				return true;
			}
			getline(user_data, t_name);
		}
		user_data.close();
	}
	return false;
}

void user::ac_write(std::string password, bool flag)
{
	std::ofstream user_data;
	user_data.open("name_passwd.txt", std::ios::app);
	if (flag == 0)
	{
		user_data << "\n" << "0" + name << "\n" << password;
		user_data.close();
		std::ofstream word_data;
		word_data.open("word.txt", std::ios::app);
		word_data << "\n" << "0" + name << "\n" << "0";
		word_data.close();
		std::ofstream score_data;
		score_data.open("score.txt", std::ios::app);
		score_data << "\n" << "0" + name << "\n" << "0" << "\n" << "1";
		score_data.close();
	}
	else
	{
		user_data << "\n" << "1" + name << "\n" << password;
		user_data.close();
		std::ofstream score_data;
		score_data.open("score.txt", std::ios::app);
		score_data << "\n" << "1" + name << "\n" << "0" << "\n" << "0" << "\n" << "1";
		score_data.close();
	}
}

std::tuple<int, int, int> challenger::get_c_info(std::string c_name)
{
	int exp, level, hiscore;
	std::string a_name;
	std::ifstream score_data("score.txt");
	if (score_data.is_open())
	{
		while (!score_data.eof())
		{
			getline(score_data, a_name);
			if (a_name == c_name)
			{
				getline(score_data, a_name);
				exp = std::stoi(a_name);
				getline(score_data, a_name);
				high_score = std::stoi(a_name);
				getline(score_data, a_name);
				level = std::stoi(a_name);
				getline(score_data, a_name);
				break;
			}
			else if (a_name[0] == '0')
			{
				getline(score_data, a_name);
				getline(score_data, a_name);
			}
			else
			{
				getline(score_data, a_name);
				getline(score_data, a_name);
				getline(score_data, a_name);
			}
		}
		score_data.close();
	}
	return std::tuple<int, int, int>(exp, hiscore, level);
}

void challenger::write_back()
{
	std::ifstream user_data("score.txt");
	std::vector<std::string> t_vec;
	while (!user_data.eof())
	{
		std::string t_str;
		getline(user_data, t_str);
		t_vec.push_back(t_str);
		if (t_str[0] == '0')
		{
			getline(user_data, t_str);
			t_vec.push_back(t_str);
			getline(user_data, t_str);
			t_vec.push_back(t_str);
		}
		else
		{
			if (t_str == name)
			{
				t_vec.push_back(std::to_string(exp));
				t_vec.push_back(std::to_string(high_score));
				t_vec.push_back(std::to_string(level));
				getline(user_data, t_str);
				getline(user_data, t_str);
				getline(user_data, t_str);
			}
			else
			{
				getline(user_data, t_str);
				t_vec.push_back(t_str);
				getline(user_data, t_str);
				t_vec.push_back(t_str);
				getline(user_data, t_str);
				t_vec.push_back(t_str);
			}
		}
	}
	user_data.close();
	std::ofstream new_data("score.txt");
	for (int i = 0; i < t_vec.size(); i++)
	{
		new_data << t_vec[i];
		if (i != t_vec.size() - 1)
			new_data << "\n";
	}
	new_data.close();
}

std::tuple<int, int, int, std::vector<std::string>> provider::get_p_info(std::string c_name)
{
	int exp, level, word_cnt;
	std::vector<std::string> word_v;
	std::ifstream word_data("word.txt");
	std::string a_name;
	std::string word;
	int word_num;
	if (word_data.is_open())
	{
		while (!word_data.eof())
		{
			getline(word_data, a_name);
			if (a_name == c_name)
			{
				getline(word_data, word);
				word_cnt = std::stoi(word);
				for (int i = 0; i < word_cnt; i++)
				{
					getline(word_data, word);
					word_v.push_back(word);
				}
				break;
			}
		}
		word_data.close();
	}
	std::ifstream score_data("score.txt");
	if (score_data.is_open())
	{
		while (!score_data.eof())
		{
			getline(score_data, a_name);
			if (a_name == c_name)
			{
				getline(score_data, a_name);
				exp = std::stoi(a_name);
				getline(score_data, a_name);
				level = std::stoi(a_name);
				score_data.close();
				break;
			}
			else if (a_name[0] == '1')
			{
				getline(score_data, a_name);
				getline(score_data, a_name);
				getline(score_data, a_name);
			}
			else
				getline(score_data, a_name);
		}
		score_data.close();
	}
	return std::tuple<int, int, int, std::vector<std::string>>(exp, level, word_cnt, word_v);
}

void provider::write_back()
{
	std::ifstream user_data("score.txt");
	std::vector<std::string> t_vec;
	while (!user_data.eof())
	{
		std::string t_str;
		getline(user_data, t_str);
		t_vec.push_back(t_str);
		if (t_str[0] == '1')
		{
			getline(user_data, t_str);
			t_vec.push_back(t_str);
			getline(user_data, t_str);
			t_vec.push_back(t_str);
			getline(user_data, t_str);
			t_vec.push_back(t_str);
		}
		else
		{
			if (t_str == name)
			{
				t_vec.push_back(std::to_string(exp));
				t_vec.push_back(std::to_string(level));
				getline(user_data, t_str);
				getline(user_data, t_str);
			}
			else
			{
				getline(user_data, t_str);
				t_vec.push_back(t_str);
				getline(user_data, t_str);
				t_vec.push_back(t_str);
			}
		}
	}
	user_data.close();
	std::ofstream new_data("score.txt");
	for (int i = 0; i < t_vec.size(); i++)
	{
		new_data << t_vec[i];
		if (i != t_vec.size() - 1)
			new_data << "\n";
	}
	new_data.close();

	while (!t_vec.empty())
		t_vec.pop_back();
	std::ifstream word_data("word.txt");
	while (!word_data.eof())
	{
		std::string t_str;
		getline(word_data, t_str);
		t_vec.push_back(t_str);
		if (t_str == name)
		{
			getline(word_data, t_str);
			t_vec.push_back(std::to_string(word_cnt));
			for (int i = 0; i < word_cnt; i++)
			{
				getline(word_data, t_str);
				t_vec.push_back(word_buf[i]);
			}
		}
		else
		{
			getline(word_data, t_str);
			t_vec.push_back(t_str);
			int num = std::stoi(t_str);
			for (int i = 0; i < num; i++)
			{
				getline(word_data, t_str);
				t_vec.push_back(t_str);
			}
		}
	}
	word_data.close();

	std::ofstream new_word("word.txt");
	for (int i = 0; i < t_vec.size(); i++)
	{
		new_word << t_vec[i];
		if (i != t_vec.size() - 1)
			new_word << "\n";
	}
	new_word.close();
}