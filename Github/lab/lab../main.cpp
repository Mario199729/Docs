#include<iostream>
#include<string.h>
#include<stdio.h>

#define LEN 50
using namespace std;

class CostumStack {

    protected:
        int* mDate;

    private:
        int sizes;
        int sizestack;
        int sizestr;
        char s[LEN][LEN];

        int checkSymb(char* st){
            if(strcmp(st, "*") == 0 || strcmp(st, "-") == 0 || strcmp(st, "/") == 0 || strcmp(st, "+") == 0)
                return 1;
            return -1;
        }

        double Operate(char symb, int x, int y){
            switch(symb){
                case '+':
                    return x + y;
                case '-':
                    return x - y;
                case '*':
                    return x * y;
            }
            return x / y;
        }

    public:
        CostumStack(){
            sizes = 0;
            sizestr = 0;
            sizestack = 100;
            mDate = new int[sizestack];
        }
        ~CostumStack(){
            delete [] mDate;
        }

        size_t size(){
            return sizes;
        }
        bool empty(){
            if(sizes != 1)
                return 1;
            return 0;
        }

        void push(int val){
            mDate[sizes] = val;
            sizes++;
        }

        void pop(int k){
            for(int i = k; i <= sizes; i++)
                mDate[i] = mDate[i+1];

            sizes--;
        }

        int top(){
            if(this->empty()){
                this->~CostumStack();
                cout << "error" << endl;
                exit(0);
            }

            return mDate[sizes-1];
        }

        void Initial(char *str){
            int k = 0;
            for(int i = 0; i <= strlen(str); i++){
                if(str[i] == ' ' || str[i] == '\0'){
                    s[sizestr++][k] = '\0';
                    k = 0;
                }else {
                    s[sizestr][k++] = str[i];
                }
            }

            k = 0; 
            for (int i = 0; i < sizestr; i++){
                if(strlen(s[i]) > 1)
                    push(atoi(s[i]));
                else 
                    if(checkSymb(s[i]) < 0)
                        push(atoi(s[i]));
            }
        }

        void Main(){
            int dn = 0, k = 0;

            for(int i = 0; i <= sizestr; i++){
                
                if(checkSymb(s[i]) > 0){
                    if(dn >= 2){
                        //cout << mDate[k-2] << s[i][0] << mDate[k-1] << " = ";
                        mDate[k-1] = Operate(s[i][0], mDate[k-2], mDate[k-1]);
                        //cout << mDate[k-1] << endl;
                        pop(k-2); dn--;k--;
                    }else{
                        this->~CostumStack();
                        cout << "error" << endl;
                        exit(0);
                    }
                }else{
                    dn++; k++;
                }
            }
        }
};

int main(){
    CostumStack Stack;
    char symb;
    char *str = new char[LEN];
    int i = 0;

    while((symb = getchar()) != EOF && symb != '\n'){
        str[i++] = symb;
    }
   

    Stack.Initial(str);
    Stack.Main();
    cout << Stack.top() << endl;
    //cout << "size = " << Stack.size() << endl;

    return 0;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      