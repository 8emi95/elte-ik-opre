#include <iostream>

using namespace std;

int main()
{
    /*task 1

    int first_array[9];

    for(int i=0; i<9; ++i){
        cout << "please, give give the values of "<< i <<"th pos.: ";
        cin >> first_array[i];
    }

    for(int i=0; i<9; ++i)
        cout << i << "th pos.: " <<first_array[i] << " \n";
    */


    //task 2

    int max_l = 30;
    int second_array[max_l];
    int studentNumber;

    cout << "how many students are in this class ? ";
    cin >> studentNumber;

    for(int i=0; i<studentNumber; ++i){
        cout << "please give the height of the " << i << " th student: ";
        cin >> second_array[i];
    }
    /*for(int i=0; i<studentNumber; ++i)
        cout << i << "th student is: " <<second_array[i] << " cm. \n";
    */

    // task 3
    /*int s=0;
    for(int i=0; i<studentNumber; ++i){
        if(second_array[i]>180)
            s++;

    }
    cout << "The number of students taller than 180cm is: " << s;
    */
    // task 4

    int a =0;
    int b =0;
    for(int i=0; i<studentNumber; ++i){
        if (second_array[i] > a){
            a = second_array[i];
            b=i+1;
        }
    }
    cout<<"the tallest student is the "<< b <<"th student, and his height is: " << a;




    return 0;
}
