#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <fstream>
#include <map>

using namespace std;

class State
{
    public:
    State(vector<char> A, vector<char> B, vector<char> C){
        in = {A,B,C};
        result.resize(3);
    }

    State(State *state){
        in = state->in;
        result = state->result;
        carryNow = state->carryNow;
    }

    vector<vector<char>> in; 
    vector<vector<char>> result;
    bool carryNow = false;
};

struct myComp {
bool operator()(
    pair<int, State*>& a,
    pair<int, State*>& b)
{
    return a.first > b.first;
}
};

void pr(vector<char> &c){
    for (int i = 0; i < c.size(); i++){
        cout << c[i] <<" ";
    }
    cout << endl;
    return ;
}


map<string, string> mapSolverNoCarry;
map<string, string> mapSolverWithCarry;

map<string, string> mapSolverNoCarryWithCarryNow;
map<string, string> mapSolverWithCarryWithCarryNow;

void printSol(pair<int,State*> p){
    vector<pair<int, State*> > sol;

    State* state = p.second;
    int numberOfInsertions = p.first;
    cout << numberOfInsertions << endl;
    for (int i = 0; i < state->result.size(); i++){
        for (int j = state->result[i].size() -1 ; ~j ; j--){
            cout << state->result[i][j];
        }
        if (i == 0){
            cout <<" + ";
        } else if (i == 1){
            cout <<" = ";
        }
    }
    cout << endl;
}

bool allEmptyVector(vector<vector<char>> &v){
    for (int i = 0; i < v.size(); i++){
        if (!v[i].empty()) return 0;
    }
    return 1;
}

int countInsertions(string &str){
    int count = 0;
    for (char &c: str){
        if (c == 'X') {
            count++;
        }
    }
    return count;
}

void insertNextEqu(State* state, string &equ, string &insert){
    for (int i = 0; i < 3; i++){
        if ((equ[i] >='0' && equ[i] <='9') || equ[i] == 'P'){
            state->in[i].pop_back();
        } 
        if ((equ[i] >='0' && equ[i] <='9') || equ[i] == 'X'){
            state->result[i].push_back(insert[i]);
        }
    }
}

vector<pair<int, State*> >  getNextStates(pair<int, State*> p, string &equ){
    vector<pair<int, State*> > sol;

    State* state = p.second;
    int numberOfInsertions = p.first;
    string newEqu = equ;
    //cout << equ << endl;
    for (int i = 0; i < newEqu.length(); i++){
        if (newEqu[i] == 'P' || newEqu[i] == 'O'){
            newEqu[i] = '0';
        }
    }
  
    int numberOfNewInsertions = countInsertions(newEqu);
    if (state->carryNow == true){
        if (mapSolverNoCarryWithCarryNow.find(newEqu) != mapSolverNoCarryWithCarryNow.end()){
            State* nState = new State(state);
            nState->carryNow = false;
            insertNextEqu(nState, equ, mapSolverNoCarryWithCarryNow[newEqu]);
            sol.push_back({numberOfInsertions + numberOfNewInsertions, nState});
        }
        if (mapSolverWithCarryWithCarryNow.find(newEqu) != mapSolverWithCarryWithCarryNow.end()){
            State* nState = new State(state);
            nState->carryNow = true;
            insertNextEqu(nState, equ, mapSolverWithCarryWithCarryNow[newEqu]);
            sol.push_back({numberOfInsertions + numberOfNewInsertions, nState});
        }
    } else {
        if (mapSolverNoCarry.find(newEqu) != mapSolverNoCarry.end()){
            State* nState = new State(state);
            nState->carryNow = false;
            insertNextEqu(nState, equ, mapSolverNoCarry[newEqu]);
            sol.push_back({numberOfInsertions + numberOfNewInsertions, nState});
        }
        if (mapSolverWithCarry.find(newEqu) != mapSolverWithCarry.end()){
            State* nState = new State(state);
            insertNextEqu(nState, equ, mapSolverWithCarry[newEqu]);
            nState->carryNow = true;
            sol.push_back({numberOfInsertions + numberOfNewInsertions, nState});
        }
    }
    return sol;
}

void sol(string &_a, string &_b, string &_c){
    _a.insert(0, "P");
    _b.insert(0, "P");
    _c.insert(0, "P");

    vector<char> A(_a.begin(), _a.end());
    vector<char> B(_b.begin(), _b.end());
    vector<char> C(_c.begin(), _c.end());

    State *startState = new State(A,B,C);
    startState->carryNow = 0;
    priority_queue<pair<int, State*>, vector<pair<int, State*>>, myComp> pq;
    pq.push({0, startState});
    set<pair<vector<vector<char>>, bool>> memoization;
    vector<pair<int, State*> > nextStates;
    while(!pq.empty())
    {
        pair<int, State*> p = pq.top();
        pq.pop();
        int numberOfInsertions = p.first;
        State* state = p.second;
        // end state
        if (allEmptyVector(state->in) && state->carryNow == 0){
            printSol(p);
            break;
        } 
        // chech if we wisited this state
        if (memoization.count({state->in, state->carryNow}) == 1){
            continue;
        }
        memoization.insert({state->in, state->carryNow});

        for (int i = 0; i < (1 << 3); i++){
            int i_indx = i;
            string nextState = "";
            for (int j = 0; j < 3; j++){
                 if (state->in[j].empty()){
                        nextState.push_back('O');
                } else  if (i_indx%2 == 0){
                    nextState.push_back('X');
                } else {
                    nextState.push_back(state->in[j].back());
                }
                i_indx = i_indx / 2;
            }  
            nextStates = getNextStates(p, nextState);
            for ( pair<int, State*> &nextState: nextStates){
                pq.push(nextState);
            }         
        }

    }
}

void preProces(){
    for (int i = 0; i <= 9; i++){
        for (int j = 0; j <= 9; j++){
            { 
                string starter;
                starter.push_back(i + '0');
                starter.push_back(j + '0');
                starter.push_back((i + j) % 10 + '0');
                for (int z = 0; z < (1 << 3); z++){
                    int index_z = z;
                    string ender = "";
                    for (int q = 0; q < 3; q++){
                        if (index_z%2 == 0){
                            ender.push_back('X');
                        } else {
                            ender.push_back(starter[q]);
                        }
                        index_z = index_z / 2;
                    }
                     if (i + j >= 10){
                        mapSolverWithCarry[ender] = starter;
                    } else {
                        mapSolverNoCarry[ender] = starter;
                    }
                }
            }
            {
                string starter;
                starter.push_back(i + '0');
                starter.push_back(j + '0');
                starter.push_back((i + j + 1) % 10 + '0');
                for (int z = 0; z < (1 << 3); z++){
                    int index_z = z;
                    string ender = "";
                    for (int q = 0; q < 3; q++){
                        if (index_z%2 == 0){
                            ender.push_back('X');
                        } else {
                            ender.push_back(starter[q]);
                        }
                        index_z = index_z / 2;
                    }
                    if (i + j + 1 >= 10){
                        mapSolverWithCarryWithCarryNow[ender] = starter;
                    } else {
                        mapSolverNoCarryWithCarryNow[ender] = starter;
                    }
                }
            }
        }
    }
}


int main(){
    string s;
    ifstream inFile;
    inFile.open("in_test_1.txt");
    preProces();
    
    while(getline (inFile, s)){
        vector<string> v;
        string a;
        for (int i = 0; i < s.length(); i++){
            if (s[i] >= '0' && s[i] <= '9'){
                a.push_back(s[i]);
            } else {
                if (a.length())
                    v.push_back(a);
                a.clear();
            }
        }
        if (a.size())
        v.push_back(a);
    

        sol(v[0], v[1], v[2]);
        a.clear();
        v.clear();
    }
    return 0;
}




//              . --- .
//            /        \
//           |  O  _  O |
//           |  ./   \. |
//           /  `-._.-'  \
//         .' /         \ `.
//     .-~.-~/           \~-.~-.
// .-~ ~    |             |    ~ ~-.
// `- .     |             |     . -'
//      ~ - |             | - ~
//          \             /
//        ___\           /___
//        ~;_  >- . . -<  _i~
//           `'         `'