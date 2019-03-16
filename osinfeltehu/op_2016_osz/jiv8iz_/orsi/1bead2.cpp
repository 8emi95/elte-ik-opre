#include <fstream>
#include <iostream>
#include <vector>
#include <future> // ASYNC, FUTURE
#include <map>

using namespace std;
struct adatok{
	//string jatekazonosito;
	int percekszama;
	int atlagpercek;
};
void szamol(multimap<string,int> adat,string azon) {
	int percek=0;
	int db =0;
	for(multimap<string,int>::iterator it = adat.begin(); it!=adat.end();++it){
		if(it->first == azon) {
			percek+=it->second;
			db++;
		}
		
	}
	int atlag = percek/db;
	cout<<"A "<<azon<<" jatekot osszesen "<<percek<<" percig, es atlagosan "<<atlag<<" percig jatszottak.\n";

}

adatok szamol2(multimap<string,int> adat,string azon) {
	int percek=0;
	int db =0;
	adatok a;
	for(multimap<string,int>::iterator it = adat.begin(); it!=adat.end();++it){
		if(it->first == azon) {
			percek+=it->second;
			db++;
		}
		
	}
	int atlag = percek/db;
	//a.jatekazonosito = azon;
	a.percekszama = percek;
	a.atlagpercek =atlag;
	return a ;

}

int main() {
	ifstream input("input.txt");
	
	unsigned int N,M; //N = sorok,M = játékok
	
	input>>N >>M;
	string neptun,jatekazon;
	int perc;
	multimap<string,int> jatekok;
	for(int i=0;i<N;i++){
		input>>neptun>>jatekazon>>perc;
		jatekok.insert(pair<string,int> (jatekazon,perc));
	}	
	
	for(multimap<string,int>::iterator it = jatekok.begin(); it!=jatekok.end();++it){
		cout<<it->first <<" "<<it->second<<endl;
	}
	cout<<"\n\n";
	
	
	
	
	multimap<string,adatok> eredmeny;

	string ja;
	multimap<string,int>::iterator it = jatekok.begin();
	int db,sum;
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
	
	ofstream output("output.txt");
	for(multimap<string,adatok>::iterator it= eredmeny.begin(); it!= eredmeny.end();++it){
		output << it->first<<" "<<it->second.percekszama<<" "<<it->second.atlagpercek<<endl;
	}
	
	return 0;

}