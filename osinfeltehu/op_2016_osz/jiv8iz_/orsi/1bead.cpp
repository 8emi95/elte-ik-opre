#include <fstream>
#include <iostream>
#include <vector>
#include <future> // ASYNC, FUTURE
#include <map>

std::tuple<std::string,int,int> szamol(std::multimap<std::string,int> adat,std::string azon) {
	int percek=0;
	int db =0;

	for(auto a : adat){
		if(a.first == azon) {
			percek+=a.second;
			db++;
		}		
	}
	int atlag = percek/db;
	std::tuple<std::string,int,int> t= std::make_tuple(azon,percek,atlag);
	
	return t ;
}

int main() {
	std::ifstream input("input.txt");
	
	unsigned int N,M; 
	std::string neptun,jatekazon;
	int perc;
	std::multimap<std::string,int> jatekok;
	
	input>>N >>M;
	
	for(int i=0;i<N;i++){
		input>>neptun>>jatekazon>>perc;
		jatekok.insert(std::pair<std::string,int> (jatekazon,perc));
	}	
	
	input.close();
		
	std::vector<std::future<std::tuple<std::string,int,int>>> eredmeny;
	
	for(std::multimap<std::string,int>::iterator it = jatekok.begin(); it != jatekok.end();it=jatekok.upper_bound(it->first)){
	
		eredmeny.push_back(std::async(szamol,jatekok,it->first));
	
	}
	
	std::ofstream output("output.txt");
	for(std::future<std::tuple<std::string,int,int>>& f : eredmeny) {
		f.wait();
		std::string azon;
		int ossz,atl;
		std::tie(azon,ossz,atl) = f.get();
				
		output<< azon<<" "<<ossz<<" "<<atl<<std::endl;

	}
	
	output.close();
	
	return 0;

}