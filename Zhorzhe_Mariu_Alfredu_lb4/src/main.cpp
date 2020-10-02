#include <iostream>

using namespace std;

class CustomStack
{

private:
    int sizes;
    int sizeStack;

protected:
    int *mData;

public:
    CustomStack()
    {
        sizes = 0;
        sizeStack = 100;
        mData = new int[sizeStack];
    }
    ~CustomStack()
    {
        delete[] mData;
    }

    bool empty()
    {
        if (sizes == 0)
        {
            return 1;
        }
        return 0;
    }

    size_t size()
    {
        return sizes;
    }

    void pop()
    {
        if (this->empty())
        {
            this->~CustomStack();
            cout << "error" << endl;
            exit(0);
        }

        sizes--;
    }
    void push(int val)
    {
        mData[sizes] = val;
        sizes++;
    }

    int top()
    {
        if (this->empty())
        {
            this->~CustomStack();
            cout << "error" << endl;
            exit(0);
        }

        return mData[sizes - 1];
    }

    void out()
    {
        if (this->size() != 1)
        {
            this->~CustomStack();
            cout << "error" << endl;
            exit(0);
        }

        cout << mData[0];
    }

    int Operator(char st)
    {
        int b = this->top();
        this->pop();
        int a = this->top();
        this->pop();

        switch (st)
        {
        case '*':
            return (a * b);
            break;

        case '/':
            return (a / b);
            break;

        case '+':
            return (a + b);
            break;

        case '-':
            return (a - b);
            break;
        }
    }
};

int check(char sym)
{
    if ((sym == '+') || (sym == '-') || (sym == '/') || (sym == '*'))
        return 1;
    return -1;
}

char *Initial()
{

    char *str = new char[10];
    char st;
    int i = 0;

    while (((st = getchar()) != EOF) && (st != ' ') && (st != '\n'))
    {
        str[i] = st;
        i++;
    }

    if (i == 0)
    {
        return NULL;
    }
    str[i] = '\0';
    return str;
}

int main()
{

    CustomStack Stack;
    char *str;

    while ((str = Initial()) != NULL)
    {
        if ((check(*str) > 0 && (str[1] == '\0')))
        {
            Stack.push(Stack.Operator(*str));
        }
        else
        {
            Stack.push(atoi(str));
        }
        delete[] str;
    }

    Stack.out();
    return 0;
}

