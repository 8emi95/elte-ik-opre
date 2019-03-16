#include <fstream>
#include <vector>
#include <future>
#include <string>
//#include <chrono>

void Merge(std::vector<std::string>& vec1, std::vector<std::string>& vec2, std::vector<std::string>& res)
{
	size_t i = 0, j = 0, m = 0;
	while (i < vec1.size() && j < vec2.size())
	{
		if (vec1.at(i) < vec2.at(j))
			res[m++] = vec1[i++];
		else
			res[m++] = vec2[j++];
	}

	while (i < vec1.size())
	{
		res[m++] = vec1[i++];
	}
	while (j < vec2.size())
	{
		res[m++] = vec2[j++];
	}
}

void BubbleSort(std::vector<std::string>& vec)
{
	for (int i = vec.size(); i > 1; --i)
	{
		for (int j = 0; j < i - 1; ++j)
		{
			if (vec[j] > vec[j + 1])
			{
				std::string temp = vec[j];
				vec[j] = vec[j + 1];
				vec[j + 1] = temp;
			}
		}
	}
	//return vec;
}

std::vector< std::string > AsyncMergeSort(std::vector< std::string >& vec, size_t u, size_t v)
{
	if (u < v)
	{
		size_t k = (u + v) / 2;
		if (u + 20 >= v)
		{
			if (vec.begin() + v == vec.end())
				--v;
			std::vector< std::string > toSort(vec.begin() + u, vec.begin() + v + 1);
			BubbleSort(toSort);
			return toSort;
		}
			
		std::future< std::vector < std::string >> resultFirstHalf = std::async(std::launch::async, AsyncMergeSort, std::ref(vec), u, k);
		std::future< std::vector < std::string >> resultSecondHalf = std::async(std::launch::async, AsyncMergeSort, std::ref(vec), k + 1, v);
		
		resultFirstHalf.wait();
		resultSecondHalf.wait();

		std::vector < std::string > Firsthalf = resultFirstHalf.get();
		std::vector < std::string > Secondhalf = resultSecondHalf.get();

		std::vector < std::string > mergedVec(Firsthalf.size() + Secondhalf.size());

		Merge(Firsthalf, Secondhalf, mergedVec);

		return mergedVec;
	}
	else
	{
		std::vector< std::string > ret;
		return ret;
	}
}

int main()
{
	std::ifstream input("input.txt");

	unsigned int N = 0;

	input >> N;

	if (N <= 0)
	{
		std::ofstream output("output.txt");
		return 0;
	}

	std::vector<std::string> data(N);
	for (size_t i = 0; i < N; ++i)
	{
		input >> data[i];
	}

	input.close();

	//auto start = std::chrono::system_clock::now();
	std::vector< std::string > res = AsyncMergeSort(data, 0, data.size());
	//auto end = std::chrono::system_clock::now();
	//std::chrono::duration<double> diff = end - start;


	std::ofstream output("output.txt");
	//output << diff.count() << std::endl;

	for (std::string& d : res)
	{
		output << d << "\n";
	}

	output.close();

	return 0;
}