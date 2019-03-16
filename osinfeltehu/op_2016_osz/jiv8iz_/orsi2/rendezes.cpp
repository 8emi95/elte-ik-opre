#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>    // std::sort


int main(){
    std::ifstream input("input.txt");

    unsigned int N;
    std::string neptun;
    std::vector<std::string> neptunkodok;
    std::ofstream output("output.txt");

    input>>N;
    for(int i=0;i<N;i++){
        input>>neptun;
        neptunkodok.push_back(neptun);
    }
    input.close();

    std::sort(neptunkodok.begin(),neptunkodok.end());
    //neptunkodok.sort();

    for(const std::string& n: neptunkodok){
            output<<n<<std::endl;
    }
    output.close();

    std::cout<<"Sikerult a fajlba kiiras.";


}
