#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>
#include <omp.h>
#include<set>
#include<algorithm>

/// Поиск за количесво слов в ответе. Вставка за О(2^n) где n числов букв в слове.
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
bool NextSet(int* a, int n, int m, std::string& str, std::string& str1)
{
	std::string copy = str1;
	int j = m - 1;
	while (j >= 0 && a[j] == n) j--;
	if (j < 0) return false;
	if (a[j] >= n)
		j--;
	a[j] ++;
	str[j] = '?';

	//if (j == m - 1) return true;
	for (int k = j + 1; k < m; k++) {
		a[k] = 1;
		str[k] = copy[k];
	}

	return true;
}
class dictionary {
	std::unordered_map<std::string,std::vector<std::string>> dict; 
public:
	dictionary() : dict(){}
public:
	void insert(std::string str) {
		std::vector<std::string> vec;
		std::string copy = str;
		vec.push_back(copy);
		int n = 2;
		size_t m = str.size();
		int *a=new int [str.size()];
		for (size_t i = 0; i < str.size(); i++)
			a[i] = 1;
		while (NextSet(a, n, m, str, copy)) {
			if (dict.find(str) != dict.end()) {
				dict[str].push_back(copy);
			}
			else {
				dict.insert({ str, vec });
			}
		}
	}
	void find(const std::string& str) {
		if (check(str)) {
			std::cout << (*dict.find(str)).second[0]<<" ";
		}
		else {
			for (auto el : (*dict.find(str)).second) std::cout << el << " ";
			std::cout << std:: endl;
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
	a.insert("aqwertyuior");
	a.insert("sdfgffre");
	a.insert("nbvcvbnbvct");
	a.find  ("?bv?v?n?v?t");
	a.find  ("?dfg????");
	a.find  ("???????????");
}
