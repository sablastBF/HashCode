#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <fstream>

using namespace std;

class State
{
    public:
    State(vector<char> A, vector<char> B, vector<char> C):a(A), b(B), c(C){

    }
    State(State *state){
        carry = state->carry;
        a = state->a;
        b = state->b;
        c = state->c;
        restA = state->restA;
        restB = state->restB;
        restC = state->restC;
        carryNow = state->carryNow;
        carry = state->carry;
    }


    vector<bool> carry;
    vector<char> a,b,c; // brojeve koje treba obraditi,, po njiba se prepoznaje stanje
    vector<char> restA, restB, restC; // brojeve koje smo obradili 
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

void print_sol(pair<int,State*> p){
    cout << "Broj koraka: "<<p.first<<endl;
    int m = max({p.second->restA.size(),p.second->restB.size(),p.second->restC.size()});
    for (int i = 0; i < m; i++){
        int ia = i,
        ib = i,
        ic = i;

        if (ia <  p.second->restA.size() && ib< p.second->restB.size()&& ic < p.second->restC.size()){
           // cout <<p.second->restA[ia]<<" "<< p.second->restB[ib]<<" "<<p.second->restC[ic]<<endl;
            bool carry = false;
            carry = p.second->carry[i];
 
            if (p.second->restA[ia] == 'X' && p.second->restB[ib] != 'X'  && p.second->restC[ic] != 'X'){
                p.second->restA[ia] = p.second->restC[ic] -'0' - (p.second->restB[ib] -'0') - carry;
                p.second->restA[ia] = (p.second->restA[ia] + 10)%10 +'0';
            }
            if (p.second->restA[ia] != 'X' && p.second->restB[ib] == 'X'  && p.second->restC[ic] != 'X'){
                p.second->restB[ib] = p.second->restC[ic] -'0' - (p.second->restA[ia] -'0') - carry;
                p.second->restB[ib] =  (p.second->restB[ib]+10)%10 +'0';
            }
            if (p.second->restA[ia] != 'X' && p.second->restB[ib] != 'X'  && p.second->restC[ic] == 'X'){
                p.second->restC[ic] = (p.second->restA[ia] - '0')+(p.second->restB[ib] -'0') + carry;
                p.second->restC[ic] = (p.second->restC[ic]+10)%10 + '0';
            }
            //cout <<p.second->restA[ia]<<" "<< p.second->restB[ib]<<" "<<p.second->restC[ic]<<endl;
        }else if (ia <  p.second->restA.size() && ic < p.second->restC.size()){
            bool carry = false;
            carry = p.second->carry[i];
            if (p.second->restA[ia] == 'X' && p.second->restC[ic] != 'X'){
                p.second->restA[ia] = p.second->restC[ic] - carry - '0';
                p.second->restA[ia] = (p.second->restA[ia] + 10)%10+'0';
            }
        } else if (ib <  p.second->restB.size() && ic < p.second->restC.size()){
            bool carry = false;
            carry = p.second->carry[i];
            if (p.second->restB[ia] != 'X' && p.second->restC[ic] == 'X'){
                p.second->restC[ia] = p.second->restB[ib] + carry - '0';
                p.second->restC[ia] = (p.second->restC[ia] + 10)%10+'0';
            }
        }
    }
    
    for (int i =  p.second->restA.size()-1; ~i; i--){
        cout << p.second->restA[i];
    }
    cout <<" + "; 

    for (int i =  p.second->restB.size()-1; ~i; i--){
        cout << p.second->restB[i];
    }
        
   cout <<" = ";
    for (int i =  p.second->restC.size()-1; ~i; i--){
        cout << p.second->restC[i];
    }
    cout << endl;
    cout <<"############"<<endl;
}

bool sumsUpToC(State *st){
    int k = st->a.back() - '0' + st->b.back() - '0' + st->carryNow;
    k = k% 10;
    return k == st->c.back() -'0';
}

void sol(string &_a, string &_b, string &_c){
  
    vector<char> A(_a.begin(), _a.end());
    vector<char> B(_b.begin(), _b.end());
    vector<char> C(_c.begin(), _c.end());

    State *pocetnoStanje = new State(A,B,C);

    priority_queue<pair<int, State*>, vector<pair<int, State*>>, myComp> pq;
    pq.push({0,pocetnoStanje});
    set<vector<vector<char>>> bio;

    while(!pq.empty()){
        pair<int,State*> p = pq.top();
        pq.pop();
        State *t = p.second;
        print_sol(p);

        if (bio.count({t->a, t->b,t->c, {t->carryNow}}) == 1){
            continue;
        }
        bio.insert({t->a, t->b,t->c, {t->carryNow}});
        if (t->a.size() == 0 && t->b.size() == 0 && t->c.size() == 0 && t->carryNow == 0){
            print_sol(p);            
            break;
        }
        if (t->a.size() == 0 && t->b.size() == 0 && t->c.size() == 0 && t->carryNow == 1){
            State *ns = new State(t);
            ns->carryNow = 0;
            ns->carry.push_back(1);
            ns->restC.push_back('1');
            pq.push({p.first+1, ns});
            continue;
        }

        // a + b = c
        // ne moramo nista samo prenesmo brojke
        if (t->a.size() > 0 &&  t->b.size() > 0 &&  t->c.size() && sumsUpToC(t)){
             State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = (ns->a.back() - '0' + ns->b.back() - '0' + ns->carryNow)>=10;
            ns->restA.push_back(ns->a.back());
            ns->a.pop_back();
            ns->restB.push_back(ns->b.back());
            ns->b.pop_back();
            ns->restC.push_back(ns->c.back());
            ns->c.pop_back();
            pq.push({p.first, ns});// nismo nista mjenjali
            continue;
        }

        //5 razlicih mjesta
        // 1    1x
        // 1 -> 1
        // 3    3

        if (t->b.size() > 0 && t->c.size() > 0){
            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = t->b.back() +ns->carryNow> t->c.back();
            ns->restA.push_back('X');

            ns->restB.push_back(ns->b.back());
            ns->b.pop_back();
            ns->restC.push_back(ns->c.back());
            ns->c.pop_back();
          
            pq.push({p.first+1, ns});
        }
        // 1    1
        // 1 -> 1x
        // 3    3

        if (t->a.size() > 0 && t->c.size() > 0){
            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = t->a.back() + ns->carryNow > t->c.back();
            ns->restB.push_back('X');

            ns->restA.push_back(ns->a.back());
            ns->a.pop_back();
            ns->restC.push_back(ns->c.back());
            ns->c.pop_back();
          
            pq.push({p.first+1, ns});
        }

        // 1    1x
        // 1 -> 1x
        // 3    3

        if (t->c.size() > 0){
            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = 0;
            ns->restA.push_back('X');
            ns->restB.push_back('X');
            ns->restC.push_back(ns->c.back());
            ns->c.pop_back();
          
            pq.push({p.first+2, ns});
        }

        // 1    1x
        // 1 -> 1x
        // 3    3
        if (t->c.size() > 0){
            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = 1;
            ns->restA.push_back('X');
            ns->restB.push_back('X');
            ns->restC.push_back(ns->c.back());
            ns->c.pop_back();
          
            pq.push({p.first+2, ns});
        }

        // 1    1
        // 1 -> 1
        // 3    3x
        if (t->a.size() > 0 && t->b.size() > 0){
            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = t->a.back() - '0' + t->b.back() - '0' +ns->carryNow >= 10;

            ns->restA.push_back(ns->a.back());
            ns->a.pop_back();
            ns->restB.push_back(ns->b.back());
            ns->b.pop_back();
            ns->restC.push_back('X');
            pq.push({p.first+1, ns});
        }
        // rubni slucajevi
        // a -> prazna
        // b -> prazan
        // c -> ima jos
        if (t->a.size() == 0 && t->b.size() == 0 && t->c.size()>0){

            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = 0;

            ns->restA.push_back('X');
        
            ns->restC.push_back(ns->c.back());
            ns->c.pop_back();

            pq.push({p.first+1, ns});
        }
        // a -> ima jos              
        // b -> prazan         
        // c -> prazan                  
        if (t->a.size() > 0 && t->b.size() == 0 && t->c.size() == 0){

            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = ns->carryNow + ns->a.back() - '0'>= 10;

            ns->restA.push_back(ns->a.back());
            ns->a.pop_back();

            ns->restC.push_back('X');

            pq.push({p.first+1, ns});
        }
        // a -> prazna           
        // b -> ima jos             
        // c -> prazan           

        if (t->a.size() == 0 && t->b.size() > 0 && t->c.size() == 0){

            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = ns->carryNow + ns->b.back() - '0'>= 10;

            ns->restB.push_back(ns->b.back());
            ns->b.pop_back();

            ns->restC.push_back('X');

            pq.push({p.first+1, ns});
        }

        // a prazan
        // b prazan
        // ima keri
        // i c je jedan ne treba dodavati nista

        if (t->a.size() == 0 && t->b.size() == 0 && t->carryNow == 1 && t->c.size() == 1 && t->c.back() == '1'){

            State *ns = new State(t);
            ns->carry.push_back(ns->carryNow);
            ns->carryNow = 0;

            ns->restC.push_back(ns->c.back());
            ns->c.pop_back();
            pq.push({p.first, ns});
        }
    }
}

void preProces(){
    for (int i = 0; i <= 9; i++){
        for (int j = 0; j <= 9; j++){
            string starter;
            starter.push_back(i + '0');
            starter.push_back(j + '0');
            starter.push_back((i + j) % 10 + '0');
            cout << starter << endl;
        }
    }
    
}

int main(){
    string s;
    ifstream inFile;
    inFile.open("in.txt");
    preProces();
    while(getline (inFile, s)){
        vector<string> v;
        string a;
        cout << s<< endl;
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