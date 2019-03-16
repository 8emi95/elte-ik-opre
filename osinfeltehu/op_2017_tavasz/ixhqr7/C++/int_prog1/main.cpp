#include <iostream>

using namespace std;

float farToCel(float);

int main()
{
    float far = 0;
    float cel = 0;
    cel = farToCel(far);



    /*int64_t fact = 1;
    for (int i=2; i <= 15; ++i){
        fact = fact*i;
        cout <<i<< "! = " << fact << endl;
    }

    /*int s=0;

    for(int i=0; i<10; ++i){
        s+=i;
        cout << "the value of s is: " << s << endl;
    }

    /*int a;
    cin >> a;
    while(a>0){
        cout << "the value of a is: " << a << endl;
        //cin >> a;
        a-=7;
        cout << "the new value is: " << a <<endl;
    }


    /*int a;
    do{
        cout << "please give the value of a: " << endl;
        cin >> a;
        if (a<0){
            cout << "this is not a good a value. Please try again" << endl;
        }
    }while(a<0);
    return 0;
    */

}

float farToCel (float){
    float far;
    float cel;
    cout << "Please enter the degree in Farenhite" << endl;
    cin >> far;

    cel= ((far - 32) * 5)/9;

    cout << "the degree in Celsius: " << cel << endl;
    return cel;
}
