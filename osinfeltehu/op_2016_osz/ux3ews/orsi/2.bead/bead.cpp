#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <future>
#include <cmath> 
#include <numeric>  
#include <chrono>

int x = 40;

std::vector<std::string>::iterator merge(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
{

	std::vector<std::string> v(std::distance(first, last)+1);
	std::copy(first, last+1, v.begin());
	std::vector<std::string>::iterator mid = v.begin();
	std::advance(mid, (std::distance(first, last) / 2));

	std::vector<std::string>::iterator first1 = v.begin();
	std::vector<std::string>::iterator first2 = mid+1;
	std::vector<std::string>::iterator last1 = mid+1;
	std::vector<std::string>::iterator last2 = v.end();
	{
		while (true) {
			if (first1 == last1) return std::copy(first2, last2, first);
			if (first2 == last2) return std::copy(first1, last1, first);
			*first++ = (*first2<*first1) ? *first2++ : *first1++;
		}
	}

}

void bubblesort(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
{
	bool swapped = true;
	std::string str;
	while (swapped) {
		swapped = false;
		for (std::vector<std::string>::iterator it = first; it != last; ++it) {
			if ( *it > *(it+1)) {
				str = *it;
				*it = *(it+1);
				*(it+1) = str;
				swapped = true;
			}
		}
	}

}

void mergesort(std::vector<std::string>::iterator first, std::vector<std::string>::iterator last)
{
	if (first < last)
	{
		
		std::vector<std::string>::iterator mid = first;
		std::advance(mid, (std::distance(mid, last) / 2));
		if ( last - first > x )
		{
			std::thread t1(mergesort, first, mid);
			std::thread t2(mergesort, mid + 1, last);
			t1.join();
			t2.join();
		}
		else
		{
			bubblesort(first, last);
		}
		merge(first,last);	
	}
}

int main()
{
	std::ifstream input("input.txt");

	unsigned int N, M;

	input >> N;

	std::vector<std::string> neptuns (N);

	for (int i = 0; i < N; i++)
	{
		input >> neptuns[i];
	}
	typedef std::chrono::high_resolution_clock Clock;
	auto t1 = Clock::now();
	mergesort(neptuns.begin(), neptuns.end()-1);
	auto t2 = Clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()  << '\n';
	input.close();

	std::ofstream output("output.txt");

	for (std::string& f : neptuns)
	{
		output << f << std::endl;		
	}

	output.close();


	return 0;
}