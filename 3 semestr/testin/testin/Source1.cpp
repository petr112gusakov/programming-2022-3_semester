#include<iostream>
#include<vector>
#include<string>
#include<unordered_set>
#include<set>

// В полях класса лежит вектор к.ч деревьев из строк (mp),и хэш таблица из строки(как ключа). Если мне нужно вставить слово,
// то оно вставится в un_set за О(1) и в нужную ячейку вектора за О(log n), где n число слов той же длинны, что и вставляемое.(61 строка)
// поиск и удаление за тоже время.





class dictionary {
	std::vector<std::set<std::string>> mp;
	std::unordered_set<std::string> dict;
public:
	dictionary() : dict(), mp() {}
private:
	std::pair<bool, std::pair<std::vector<char>, std::vector<size_t>>> check(const std::string& str) {
		std::vector<size_t> cv;
		std::vector<char> v;
		for (size_t i = 0; i < str.size(); ++i) {
			if (str[i] != '?') {
				v.push_back(str[i]);
				cv.push_back(i);
			}
		}
		if (v.size() == str.size()) return { false,{std::move(v),std::move(cv)} };
		else return { true,{std::move(v),std::move(cv)} };
	}
	std::vector<std::string> find_w_question(const std::string& str) {
		std::vector<std::string> res;
		std::pair<std::vector<char>, std::vector<size_t>> res_pair = (check(str).second);
		size_t f = res_pair.first.size();
		std::vector<size_t> cv = std::move(res_pair.second);
		std::vector<char> v = std::move(res_pair.first);
		size_t c = 0;
		for (auto el : mp[str.size()]) {
			if (f == 0) res.push_back(el);
			else {
				for (size_t i = 0; i < f; ++i) {
					if (el[cv[i]] == v[i]) {
						++c;
					}
				}
				if (c == f) {
					res.push_back(el);
					c = 0;
				}
				else c = 0;
			}
		}
		return res;
	}
public:
	void insert(const std::string& str) {
		mp.resize(str.size() + 100);
		mp[str.size()].insert(str);
		dict.insert( str);
	}
	void find(const std::string& str) {
		if (check(str).first) {
			for (auto el : find_w_question(str)) {
				std::cout << el << " ";
			}
		}
		else {
			std::cout << *dict.find(str) << " ";
		}
		std::cout << std::endl;
	}
	void erase(const std::string& str) {
		dict.erase(str);
		mp[str.size()].erase(str);
	}
};
int main() {
	dictionary a;

	a.insert("edplet");
	a.insert("apkle");
	a.insert("apple");
	a.insert("apppp");
	a.find("?p??e");
	a.find("???let");
	a.insert("asdt");
	a.insert("asdr");
	a.insert("asde");
	a.insert("asdw");
	a.insert("asdq");
	a.find("????");
	a.find("asdr");
	a.find("app??");
	a.erase("apple");
	//a.find("apple");// исключение от метода find контейнера unordered_map;
	a.find("a??le");// ok;
	a.insert("awgrytpplrsetrdyttrewedrgtje");
	a.find("???????????e????????w???????");
}
