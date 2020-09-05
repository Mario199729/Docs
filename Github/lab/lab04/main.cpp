#include<iostream>
#include<stack>


using namespace std;

int main(){

    stack<int> Stack;
    int a[5], n = 0;


    while(n < 5){
        cin >> a[n];
        if(a[n] < 10)
            Stack.push(a[n]);

        n++;
    }

    cout << "\nStack's size: " << Stack.size() << endl;

    while(!Stack.empty()){
        cout << Stack.top() << " ";
        Stack.pop();
    }

    cout << "\nStack's size: " << Stack.size() << endl;
    return 0;
}