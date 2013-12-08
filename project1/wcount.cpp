#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#define WSIZE 10000

using std::cout;
using std::cin;
using std::endl;
using std::string;

class wcount {
public:
    wcount(string &filename){
        size = 0;
        load(filename);
    }

    void load(string& filename){
        string word;

        std::ifstream file(filename);

        if(file.is_open()){
            int times = 1;

            while(file >> word){

                for(int i = 0, preadd = 0; i < word.length(); i++){

                    //to lower case
                    if(word[i]<='Z' && word[i]>='A'){
                        word[i] += 32;

                    //handle Punctuation : "Taiwan's" is "Taiwan" and "s"
                    } else if(!((word[i] >= '0' && word[i] <= '9') or (word[i] >= 'a' && word[i] <= 'z'))){
                        word.erase(i, 1);

                        //word index and word.length() will change
                        i--;

                        if(i >= preadd){
                            string tmp = word.substr(preadd, i - preadd + 1);
                            add_feq(tmp, times);
                            times++;
                            preadd = i + 1;
                        }
                    }

                    //the end of string
                    if(i + 1 == word.length()){
                        if(i >= preadd){
                            string tmp = word.substr(preadd, i - preadd + 1);
                            add_feq(tmp, times);
                            times++;
                        }
                    }
                }
            }

            file.close();
        }
        else
            cout << "Can not open file" << endl;
    }

    //output the greastest # rank
    void output(unsigned const int bound){

        unsigned int tmp[bound];
        for(unsigned int i = 0; i < bound; i++){
            tmp[i] = 0;
            for(unsigned int j = 0; j < size; j++){
                if(i > 0){
                    if((count[j] > tmp[i]) && (count[j] < tmp[i-1])){
                        tmp[i] = count[j];
                    }
                } else {
                    if(count[j] > tmp[i])
                        tmp[i] = count[j];
                }
            }
        }

        for(unsigned int i = 0; i < bound; i++){
            for(unsigned int j = 0; j < size; j++){

                if(count[j] == tmp[i])
                    cout << "(" << words[j] << "," << count[j] << "," << startPlace[j] << ")" << endl;
            }
        }
    }

private:

    void add_feq(string& word, const unsigned int& times){
        bool hascount = 0;

        for(unsigned int i = 0; i < size; i++){
            if(word == words[i]){
                count[i]++;
                hascount = 1;
                break;
            }
        }

        if(!hascount){
            words[size] = word;
            count[size] = 1;
            startPlace[size] = times;
            size++;
        }
    }

    unsigned int count[WSIZE];
    unsigned int startPlace[WSIZE];
    string words[WSIZE];
    unsigned int size;
};

int main(int argc, char* argv[]){

    //start the clock
    clock_t start,end;
    start = clock();

    //input data
    string s;
    unsigned int bound;

    if(argc > 1)
        s = argv[1];
    else
        cin >> s;

    if(argc > 2)
        sscanf(argv[2], "%d", &bound);
    else
        cin >> bound;

    //count the times
    class wcount w(s);
    w.output(bound);

    //end the clock
    end = clock();
    cout << (end - start)/(double)(CLOCKS_PER_SEC) * 1000 << "ms" << endl;

    return 0;
}
