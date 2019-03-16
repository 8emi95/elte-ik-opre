#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <future> // ASYNC, FUTURE
#include <cmath> // FLOOR
#include <numeric> // ACCUMULATE

/*void BubbleSort(std::vector<std::pair<std::string, std::vector<int> > >& data)
{
	int i;
	int j;
	std::pair<std::string, std::vector<int> > swap;

	for(i = data.size()-1; i > 0; --i)
	{
		for(j = 0; j < i; ++j)
		{
			if(data[j].first > data[i].first)
			{
				swap = data[i];
				data[i] = data[j];
				data[j] = swap;
			}	
		} 
	}
}*/

std::tuple<std::string, int , int> TimePerGame(const std::pair<std::string, std::vector<int> >& data)
{
	int sum = std::accumulate(data.second.begin(), data.second.end(), 0);	
	int average = sum / data.second.size();
	
	std::tuple<std::string, int , int> actGame(data.first, sum, average);
	return actGame;
}

int main()
{
	std::ifstream input("input.txt");
	
	unsigned int N, M;
	
	input >> N >> M;
	
	// Read all the data into one array
	std::vector<std::pair<std::string, std::vector<int> > > olddata;
	std::map<std::string, std::vector<int> > data;
	for (int i = 0; i < N; i++)
	{
		std::string dummy;
		std::string game;
		int time;
		
		input >> dummy >> game >> time;
		int j = 0;
		while (j < data.size() && game != data[game].first)
		{
			j++;
		}
		if (j == data.size())
		{
			std::pair<std::string, std::vector<int> > actGame;
			actGame.first = game;
			actGame.second.push_back(time);
			data.insert(actGame);
		}
		else
		{
			data[game].second.push_back(time);
		}
	}
	input.close();
	
	//BubbleSort(data);
	
	// Start calculating length for every vector.
	std::vector<std::future<std::tuple<std::string, int , int> > > results;
	for (size_t i = 0; i < M; i++)
	{
		results.push_back(std::async(std::launch::async, TimePerGame, data[i]));
	}
	
	// Wait for the function to return, write the result into a file
	std::ofstream output("output.txt");
	for (std::future<std::tuple<std::string, int , int> >& f : results)
	{
		f.wait();
		std::tuple<std::string, int , int> actGame = f.get();
		output << std::get<0>(actGame) << " " << std::get<1>(actGame) << " " << std::get<2>(actGame) << std::endl;
	}

	output.close();
	
	return 0;
}