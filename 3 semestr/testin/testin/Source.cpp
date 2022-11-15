#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include <omp.h>
#include<set>
#include<algorithm>

/// Поиск за количесво слов в ответе. Вставка за О(n^3*с) где n числов букв в шаблоне, с константа зависящая от того 
/// сколько слов попали в одну ячейку, и удаление примерно дольше В "c" раз чем  вставка. Вставку и удаление можно улучшить
/// полностью или частично исключив константу с ;
/// 
/// 
/// 

bool check(const std::string& str) {
	size_t count = 0;
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == '?') {
			++count;
		}
	}
	return count == 0 ? true : false;
}
class dictionary {
	std::unordered_map<std::string,std::vector<std::string>> dict; 
public:
	dictionary() : dict(){}
public:
	void insert(std::string str) {
		std::vector<std::string> vec;
		std::string copy = str;
		std::string copy1 = str;
		vec.push_back(copy);
		for (size_t k = 0; k < str.size(); ++k) {
			str[k] = '?';
			copy1 = str;
			for (size_t j = 0; j < str.size(); ++j) {
				for (size_t i = j; i < str.size(); ++i) {
					str[i] = '?';
					//std::cout << str << " ";
					if (dict.find(str) != dict.end()) {
						dict[str].push_back(copy);
					}
					else {
						dict.insert({ str, vec });
					}
					auto res = std::unique(dict[str].begin(), dict[str].end());
					dict[str].erase(res, dict[str].end());
				}
				str = copy1;
			}
			str = copy;
		}
	}
	void find(const std::string& str) {
		if (check(str)) {
			std::cout << (*dict.find(str)).second[0]<<" ";
		}
		else {
			for (auto el : (*dict.find(str)).second) std::cout << el << " ";
		}
	}
	void erase(std::string str) {
		dict.erase(str);
		for (auto el = dict.begin(); el != dict.end(); ++el) {
			if (((*el).second).size() == 1) {
				((*el).second).erase((*el).second.begin());
			}
			else {
				for (auto it = ((*el).second).begin(); it < ((*el).second).end(); ++it) {
					if (*it == str) {
						((*el).second).erase(it);
						break;
					}
				}
			}
		}
	}
};
int main() {
	dictionary a;
	a.insert("apple");
	a.find  ("a?p?e");
	a.insert("appppp");
	a.find("a?p??p");
	a.insert("apsdf");
	a.insert("appab");
	a.find("ap???");
	a.find("?????");
	a.erase("apsdf");
	a.find("ap???");
	a.insert("aqwertyuiopfhkjhgfn");
	a.insert("sdfgfdsasdfgfdsasdn");
	a.insert("nbvcvbnbvcvbnbvcvbn");
	a.find  ("??????????????????n");
	//a.find("????");
}
