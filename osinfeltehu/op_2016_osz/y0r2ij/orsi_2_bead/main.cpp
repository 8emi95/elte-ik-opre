#include <iostream>
#include <fstream>
#include <vector>
#include <future> // ASYNC, FUTURE
#include <cmath> // FLOOR
#include <numeric>
#include <string>

void BubbleSort(std::vector<std::string>& data)
{
	int i;
	int j;
	std::string swap;

	for (i = data.size() - 1; i > 0; --i)
	{
		for (j = 0; j < i; ++j)
		{
			if (data[j] > data[i])
			{
				swap = data[i];
				data[i] = data[j];
				data[j] = swap;
			}
		}
	}
}

std::vector<std::string> MergeSort(std::vector<std::string>& data)
{
	int threshold = 5;
	if (data.size() > threshold)
	{
		int k = floor(data.size() / 2);
		std::vector<std::string> data0;
		std::vector<std::string> data1;
		for (int i = 0; i < k; i++)
		{
			data0.push_back(data[i]);
		}
		for (int i = k; i < data.size(); i++)
		{
			data1.push_back(data[i]);
		}
		std::future<std::vector<std::string> > ndata0;
		std::future<std::vector<std::string> > ndata1;
		ndata0 = std::async(std::launch::async, MergeSort, std::ref(data0));
		ndata1 = std::async(std::launch::async, MergeSort, std::ref(data1));

		ndata0.wait();
		ndata1.wait();

		data0 = ndata0.get();
		data1 = ndata1.get();

		int i = 0;
		int j = 0;
		std::vector<std::string> result;
		while (result.size() == data0.size() + data1.size())
		{
			if (data0[i] < data1[j])
			{
				result.push_back(data0[i]);
				i++;
			}
			else
			{
				result.push_back(data1[j]);
				j++;
			}
		}
		return result;
	}
	else
	{
		BubbleSort(data);
		return data;
	}


}

int main()
{
	std::ifstream input("input_1.txt");

	unsigned int N;

	input >> N;

	// Read all the data into one array
	std::vector<std::string> data;

	for (int i = 0; i < N; i++)
	{
		std::string dummy;
		input >> dummy;
		data.push_back(dummy);
	}
	input.close();

	// Start calculating length for every vector.
	std::vector<std::string> results;
	results = MergeSort(data);

	// Wait for the function to return, write the result into a file
	std::ofstream output("output.txt");
	for (int i = 0; i < results.size(); i++)
	{
		output << results[i] << std::endl;
	}

	output.close();

	return 0;
}