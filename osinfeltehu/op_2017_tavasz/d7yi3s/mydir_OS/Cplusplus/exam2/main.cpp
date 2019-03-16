#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Date{
    private:
        int year;
        int month;
        int day;
    public:
         Date(int year, int month, int day)
         :year(year), month(month), day(day){}

         Date(){};

    bool operator==(Date d){
        return (d.year == this->year) && (d.month == this->month) && (d.day == this->day);
    }

    bool operator<=(Date d){
        if(d.year <= this->year){
            if(d.month <= this->month){
                if(d.day <= this->day){
                    return true;
                }else return false;
            }else return false;
        }else return false;
    }



};

class Calendar{
    private:
        vector< pair<Date, string> > pairsVector;


    public:


        void insertEvent(Date d, string event){
            pairsVector.push_back(make_pair(d, event));
        }



        string getEventOnDate(Date d){
            for(unsigned int i=0; i<pairsVector.size(); i++){
                if(d == pairsVector[i].first){
                    return pairsVector[i].second;
                }
            }
            //supposing the date-event pair is already inserted
        }

        void printSortedEvents(){
            pair<Date, string> temp;

            //ordering the vector
            for(unsigned int i=0; i<pairsVector.size(); i++){
                if(pairsVector[i+1].first <= pairsVector[i].first){
                    temp = pairsVector[i+1];
                    pairsVector[i+1] = pairsVector[i];
                    pairsVector[i] = temp;
                }
            }


            cout << "The Events sorted: " << endl;
            for(unsigned int j=0; j<pairsVector.size(); j++){
                cout << j+1 << ". " << pairsVector[j].second << endl;
            }

        }







};

int main()
{

Calendar c;
Date d;
c.insertEvent(d, "New year");
}
