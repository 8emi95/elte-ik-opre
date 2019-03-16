#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <future>
#include <string>
#include <numeric>

std::tuple<std::string,unsigned int, unsigned int> SumMinutes(const std::string& gameName, const std::multimap<std::string, int>& data)
{
	typedef std::multimap<std::string, int>::const_iterator const_it;
	typedef std::pair<std::string, int> pair_t;
	const_it citBegin = data.lower_bound(gameName);
	const_it citEnd = data.upper_bound(gameName);
	
	unsigned int sum = std::accumulate(citBegin, citEnd, 0, [](const int& a,const pair_t& b){return a + b.second;});
	unsigned int db = std::distance(citBegin, citEnd);
	return std::make_tuple(gameName, sum, sum/db);
}

int main()
{
	std::ifstream input("input.txt");

	unsigned int N, M;

	input >> N >> M;

	std::multimap<std::string, int> data;
	std::set<std::string> gameNames;
	std::string neptun;
	std::string gameName;
	int mins;

	for (unsigned int i = 0; i < N; ++i)
	{
		input >> neptun;
		input >> gameName;
		input >> mins;
		data.insert(std::make_pair(gameName, mins));
		gameNames.insert(gameName);
	}
	input.close();

	std::vector< std::future< std::tuple< std::string,unsigned int, unsigned int>>> result;

	for(auto cit = gameNames.begin(); cit != gameNames.end(); ++cit)
	{
		result.push_back(std::async(std::launch::async, SumMinutes, *cit, data));
	}

	std::ofstream output("output.txt");
	
	typedef std::tuple<std::string,unsigned int, unsigned int> result_t;
	
	for (std::future <result_t>& res : result)
	{
		res.wait();
		result_t act = res.get();
		output << std::get<0>(act) << " " << std::get<1>(act) << " " << std::get<2>(act) << std::endl;
	}
	output.close();

    return 0;
}