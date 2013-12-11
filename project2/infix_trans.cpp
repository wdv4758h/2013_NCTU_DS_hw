#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

#define SIZE 100

class infix {
public:

    infix(char *filename){
        op_now = 0;
        pre_now = 0;
        post_now = 0;
        total = 0;
        load(filename);
    }

    void postfix(){

        cout << "Postfix：";
        for(unsigned int i = 0; i < post_now; i++){
            cout << post[i];
        }

        cout << endl;
    }

    void result(){
        cout << "Result：" << total << endl;
    }

private:

    int cal(int a, int b, char op){
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

    void mk_postfix(std::ifstream &file){
        char c;
        while(file >> c){
            if(c == '('){

                operators[op_now++] = c;

            } else if(c == '+' || c == '-' || c == '*' || c == '/'){

                while(priority(c) <= priority(operators[op_now - 1])){
                    post[post_now++] = operators[--op_now];
                }

                operators[op_now++] = c;

            } else if(c == ')'){

                while(operators[op_now - 1] != '('){
                    post[post_now++] = operators[--op_now];
                }

                op_now--;

            } else {

                post[post_now++] = c;
            }
        }

        while(op_now > 0){
            post[post_now++] = operators[--op_now];
        }
    }

    void mk_result(){
        int tmp[SIZE];
        unsigned tmp_now = 0;
        for(unsigned int i = 0; i < post_now; i++){
            if(post[i] <= '9' && post[i] >= '0'){
                tmp[tmp_now++] = post[i] - '0';
            } else {
                tmp[tmp_now - 2] = cal(tmp[tmp_now - 2], tmp[tmp_now - 1], post[i]);
                tmp_now--;
            }
        }

        total = tmp[0];
    }

    void load(char *filename){
        //open file
        std::ifstream file(filename);

        if(file.is_open()){
            mk_postfix(file);
            mk_result();
            file.close();
        }
        else{
            cout << "Can not open file" << endl;
            exit(1);
        }
    }

    char operators[SIZE];
    char post[SIZE];
    unsigned int op_now;
    unsigned int post_now;
    int total;
};

int main(int argc, char* argv[]){

    //need to be change
    char *filename;
    if(argc > 1)
        filename = argv[1];
    else
        std::cin >> filename;

    class infix in(filename);
    in.postfix();
    in.result();

    return 0;
}
