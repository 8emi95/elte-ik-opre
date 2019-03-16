#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <future>
#include <cmath> 
#include <map>

std::tuple<std::string, int, int> calculate_times(const std::multimap<std::string, int>& data, std::string key)
{
	int fullt = 0;

	auto range = data.equal_range(key);
	for (auto iterator = range.first; iterator != range.second;iterator++)
	{
		fullt = fullt + iterator->second;
	}

	int average = fullt / data.count(key);

	return std::make_tuple(key, fullt, average);;
}

int main()
{
	std::ifstream input("input.txt");

	unsigned int N, M;

	input >> N >> M;

	std::multimap<std::string, int> surveymap;

	std::string neptun;
	std::string game;
	int gametime;
	for (int i = 0; i < N; i++)
	{
		input >> neptun;
		input >> game;
		input >> gametime;
		surveymap.insert(std::pair<std::string, int>(game, gametime));

	}
	std::vector<std::future<std::tuple<std::string,int,int>>> results;
	for (auto it = surveymap.begin(), end = surveymap.end();
		it != end;
		it = surveymap.upper_bound(it->first)
		)
	{
			results.push_back(std::async(std::launch::async, calculate_times, surveymap, it->first));
	}
	input.close();

	std::ofstream output("output.txt");
	std::map< std::string, std::pair<int, int>> resultmap;
	for (std::future<std::tuple<std::string, int, int>>& f : results)
	{
		f.wait();
		std::string game;
		int ft;
		int at;
		std::tie(game, ft, at) = f.get();
		resultmap.insert(std::pair<std::string, std::pair<int, int>>(game, std::pair<int, int>(ft, at)));
		
	}

	std::map<std::string, std::pair<int, int>>::iterator it;
	for (it = resultmap.begin(); it != resultmap.end(); ++it) {
		output << it->first << " " << it->second.first << " " << it->second.second << std::endl;
	}


	output.close();


	return 0;
}