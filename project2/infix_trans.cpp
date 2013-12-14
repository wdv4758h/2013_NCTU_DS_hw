#include <iostream>
#include <fstream>
#include <time.h>
#include <cstdlib>  //for exit function

using std::cout;
using std::endl;

#define SIZE 200

class infix {
public:

    infix(char *filename){
        op_now = 0;
        total = 0;
        bet = new tree();
        load(filename);
    }

    void prefix(){
        cout << "Prefix：";
        _prefix(bet->left);
        cout << endl;
    }

    void postfix(){
        cout << "Postfix：";
        _postfix(bet->left);
        cout << endl;
    }

    void result(){
        cout << "Result：" << total << endl;
    }

private:

    class tree {
    public:
        tree(){
            left = NULL;
            right = NULL;
            op = ' ';
            data = 0;
            op_or_data = 0;
        }

        tree(char c){
            left = NULL;
            right = NULL;
            op = c;
            data = 0;
            op_or_data = 0;
        }

        tree(double n){
            left = NULL;
            right = NULL;
            op = ' ';
            data = n;
            op_or_data = 1;
        }

        tree(char c, class tree *l, class tree *r){
            left = l;
            right = r;
            op = c;
            data = 0;
            op_or_data = 0;
        }

        class tree *left;
        class tree *right;
        char op;
        double data;
        unsigned int op_or_data;
    };

    void _postfix(class tree *now){
        if(now != NULL){
            _postfix(now->left);
            _postfix(now->right);

            cout << " ";
            if(now->op_or_data){
                cout << now->data;
            } else {
                cout << now->op;
            }
        }
    }

    void _prefix(class tree *now){
        if(now != NULL){
            cout << " ";
            if(now->op_or_data){
                cout << now->data;
            } else {
                cout << now->op;
            }

            _prefix(now->left);
            _prefix(now->right);
        }
    }

    double cal(double a, double b, char op){
        if(op == '+')
            return a + b;
        else if(op == '-')
            return a - b;
        else if(op == '*')
            return a * b;
        else
            return a / b;
    }

    unsigned short priority(char &c){
        switch(c){
            case '+':
            case '-': return 1;
            case '*':
            case '/': return 2;
            default : return 0;
        }
    }

    void is_valid(unsigned int stack_now, unsigned int op_now){
        if(stack_now < 2 || op_now < 1){
            cout << "This formula is invalid" << endl;
            exit(1);
        }
    }

    void mk_tree(std::ifstream &file){

        class tree *stack[SIZE];
        unsigned int stack_now = 0;

        double tmp_data;

        double total_count[SIZE];
        unsigned int count_now = 0;

        unsigned int op_det = 0;

        char c;

        while(c = file.peek(), !file.eof()){

            if(c <= '9' && c >= '0'){
                file >> tmp_data;
                stack[stack_now++] = new tree(tmp_data);

                total_count[count_now++] = tmp_data;

                if(count_now > op_det + 1){
                    cout << "This formula is invalid" << endl;
                    exit(1);
                }

            } else {
                file.get(c);

                if(c == '('){

                    operators[op_now++] = c;

                } else if(c == '+' || c == '-' || c == '*' || c == '/'){

                    while(priority(c) <= priority(operators[op_now - 1])){

                        is_valid(stack_now, op_now);

                        stack[stack_now - 2] = new tree(operators[--op_now], stack[stack_now - 2], stack[stack_now - 1]);
                        stack_now--;

                        total_count[count_now - 2] = cal(total_count[count_now - 2], total_count[count_now - 1], operators[op_now]);
                        count_now--;

                        op_det--;
                    }

                    operators[op_now++] = c;

                    op_det++;

                    if(count_now > op_det + 1 || op_det > count_now){
                        cout << "This formula is invalid" << endl;
                        exit(1);
                    }

                } else if(c == ')'){

                    while(operators[op_now - 1] != '('){

                        is_valid(stack_now, op_now);

                        stack[stack_now - 2] = new tree(operators[--op_now], stack[stack_now - 2], stack[stack_now - 1]);
                        stack_now--;

                        total_count[count_now - 2] = cal(total_count[count_now - 2], total_count[count_now - 1], operators[op_now]);
                        count_now--;

                        op_det--;
                    }

                    op_now--;
                }
            }
        }

        while(stack_now > 1){

            is_valid(stack_now, op_now);

            stack[stack_now - 2] = new tree(operators[--op_now], stack[stack_now - 2], stack[stack_now - 1]);
            stack_now--;

            total_count[count_now - 2] = cal(total_count[count_now - 2], total_count[count_now - 1], operators[op_now]);
            count_now--;

            op_det--;
        }

        if(op_now != 0 || stack_now != 1){
            cout << "This formula is invalid" << endl;
            exit(1);
        }

        bet->left = stack[0];

        total = total_count[0];
    }

    void load(char *filename){
        //open file
        std::ifstream file(filename);

        if(file.is_open()){
            mk_tree(file);
            file.close();
        } else {
            cout << "Can not open file" << endl;
            exit(1);
        }
    }

    class tree *bet;
    char operators[SIZE];
    unsigned int op_now;
    double total;
};

int main(int argc, char* argv[]){

    //start the clock
    clock_t start,end;
    start = clock();

    //need to be change
    char *filename;
    if(argc > 1)
        filename = argv[1];
    else
        std::cin >> filename;

    class infix in(filename);
    in.prefix();
    in.postfix();
    in.result();

    //end the clock
    end = clock();
    cout << (end - start)/(double)(CLOCKS_PER_SEC) * 1000 << "ms" << endl;

    return 0;
}
