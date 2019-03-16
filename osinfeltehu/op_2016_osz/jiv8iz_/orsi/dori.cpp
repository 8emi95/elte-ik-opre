#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <future>
#include <cmath> 
#include <map>

using namespace std;
struct adatok {
	string jatekazonosito;
	int percekszama;
	int atlagpercek;
};

void szamol(multimap<string, int>& adat, string azon) {
	int percek = 0;
	int db = 0;
	for (multimap<string, int>::iterator it = adat.begin(); it != adat.end();++it) {
		if (it->first == azon) {
			percek += it->second;
			db++;
		}

	}
	int atlag = percek / db;
	cout << "A " << azon << " jatekot osszesen " << percek << " percig, es atlagosan " << atlag << " percig jatszottak.\n";

}

tuple<string, int, int> szamol2(multimap<string, int>& adat, string azon) {
	int percek = 0;
	int db = 0;
	adatok a;
	for (multimap<string, int>::iterator it = adat.begin(); it != adat.end();++it) {
		if (it->first == azon) {
			percek += it->second;
			db++;
		}

	}
	int atlag = percek / db;
	tuple<string, int, int> t;
	t = make_tuple(azon, percek, atlag);
	return t;

}

int main() {
	ifstream input("input.txt");

	unsigned int N, M; //N = sorok,M = játékok
					   //Bolvasom a fajlbol az adatokat a jatekok nevu map-be
	input >> N >> M;
	string neptun, jatekazon;
	int perc;
	multimap<string, int> jatekok;
	for (int i = 0;i<N;i++) {
		input >> neptun >> jatekazon >> perc;
		jatekok.insert(pair<string, int>(jatekazon, perc));
	}
	//kiirom a map elemeit	
	for (multimap<string, int>::iterator it = jatekok.begin(); it != jatekok.end();++it) {
		cout << it->first << " " << it->second << endl;
	}
	cout << "\n\n";
	input.close();



	multimap<string, pair<int, int>> eredmeny;
	std::vector<std::future<tuple<string, int, int>>> results;

	for (multimap<string, int>::iterator it = jatekok.begin(); it != jatekok.end();it = jatekok.upper_bound(it->first)) {
		results.push_back(async( szamol2, jatekok, it->first));
		cout << it->first;
		//tuple<string,int,int> t = szamol2();
		cout << get<0>(szamol2(jatekok, it->first));
	}

	ofstream output("output.txt");
	for (future<tuple<string, int, int>>& f : results) {
		f.wait();
		string azon;
		int ossz, atl;
		tie(azon, ossz, atl) = f.get();
		eredmeny.insert(pair<string,pair<int,int>>(azon,pair<int,int>(ossz,atl)));

		cout << azon << " " << ossz << " " << atl << endl;
	}

	std::map<std::string, std::pair<int, int>>::iterator it;
	for (it = eredmeny.begin(); it != eredmeny.end(); ++it) {
		output << it->first << " " << it->second.first << " " << it->second.second << std::endl;
	}


	output.close();
	/*
	map<string,adatok> ered;
	for(multimap<string,int>::iterator it = jatekok.begin(); it != jatekok.end();it=jatekok.upper_bound(it->first)){

	ered[it->first].percekszama = async(launch::async,szamol2,jatekok,it->first).percekszama;
	ered[it->first].atlagpercek = async(launch::async,szamol2,jatekok,it->first).atlagpercek;
	}

	*/
	/*
	while( it!=jatekok.end()){
	ja=it->first;
	db=0;
	for(multimap<string,int>::iterator it2 = it; it2!=jatekok.end();++it2){
	if(it2->first == ja) {
	db++;
	}
	}
	//cout<<ja<<" "<<sum<<" "<<sum/db<<endl;
	cout<<szamol2(jatekok,ja).percekszama<<" "<<szamol2(jatekok,ja).atlagpercek<<endl;


	eredmeny.insert(pair<string,adatok>(ja,szamol2(jatekok,ja)));
	for(int i=0;i<db;++i){
	it++;
	}

	}
	*/
	return 0;

	}