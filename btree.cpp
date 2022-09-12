#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <queue>
#include <map>
#include <list>
#include <sstream>

const int mxn = 1e6 + 3;
const int mod = 1e9 + 7;
const int T = 9;

int ans;

template<class K, class V>
class RangeMap{
public:
    virtual int size() = 0;
    virtual bool isEmpty() = 0;
    virtual void add(K key, V value) = 0;
    virtual bool contains(K key) = 0;
    virtual V lookup(K key) = 0;
    virtual std::list<V> lookupRange(K from, K to) = 0; // lookup values for a range of keys
    
    // optional: remove an item from a map (+1% extra credit)
    // virtual void remove(K key);
};

class Date{
public:
    int year, month, day, cost;
    Date(){
        year = month = day = cost = 0;
    }
    Date(std::string s, int cost){
        this->year =  (s[0] - '0') * 1000 + (s[1] - '0') * 100 + (s[2] - '0') * 10 + (s[3] - '0');
        this->month = (s[5] - '0') * 10 + (s[6] - '0');
        this->day = (s[8] - '0') * 10 + (s[9] - '0');
        this->cost = cost;
    }
};

bool isMore(Date a, Date b){
    if(a.year > b.year) return 1;
    else if(a.year == b.year){
        if(a.month > b.month) return 1;
        else if (a.month == b.month){
            if(a.day > b.day) return 1;
        }
    }return 0; 
}
bool isEqual(Date a, Date b){
    if(a.year == b.year){
        if(a.month == b.month){
            if(a.day == b.day) return 1;
        }
    }
    return 0;
}
long long binpow(int a, int n){
    if(n == 0) return 1;
    if(n % 2 == 0){
        long long b = binpow(a, n/2) % mod;
        return b * b;
    }else{
        return binpow(a, n - 1) % mod * a;
    }
}

std::list<int> ls;

class Node{
    int n;
    Date keys[2 * T - 1];
    bool leaf;
    Node *c[2 * T];
    int height;
    int sum;
    std::list <int> ls;
public:

    Node(bool lleaf){
        leaf = lleaf;
        sum = n = 0;
    }

    void splitChild(int i){                             //SPLIT
        Node *y = c[i];
        Node *z = new Node(y->leaf);
        y->sum = z->sum = 0;
        y->n = z->n = T - 1;
        for (int j = 0; j < T - 1; j ++){
            z->keys[j] = y->keys[j + T];
            y->sum += y->keys[j].cost;
            z->sum += z->keys[j].cost;
        }
        if(!y->leaf){
            for (int j = 0; j < T; j ++){
                z->c[j] = y->c[j + T];
                y->sum += y->c[j]->sum;
                z->sum += z->c[j]->sum;
            }
        }
        for (int j = n; j >= i + 1; j --){
            c[j + 1] = c[j];
        }
        c[i + 1] = z;
        for (int j = n - 1; j >= i; j --){
            keys[j + 1] = keys[j];
        }
        keys[i] = y->keys[T - 1];
        n ++;
    }

    void insertNonFull(Date k, int v){                    //INSNON
        int i = n - 1;
        if(leaf){
            while (i >= 0 && isMore(keys[i], k)){
                keys[i + 1] = keys[i];
                i --;
            }
            n ++;
            keys[i + 1] = k;
            sum += v;
        }else{
            while (i >= 0 && isMore(keys[i], k)){i --;}
            if(c[i + 1]->n == 2 * T - 1){
                splitChild(i + 1);
                if(isMore(k, keys[i + 1])) i ++;
            }
            c[i + 1]->insertNonFull(k, v);
            sum += v;
        }
    }

    bool contains(Date key){                              //CONTAINS
        int i = 0;
        while (i < n && isMore(key, keys[i])) i ++;
        if(i < n && isEqual(key, keys[i])) return 1;
        if(leaf) return 0;
        return c[i]->contains(key);
    }
    int lookup(Date key){                                 //LOOKUP
        int i = 0;
        while (i < n && isMore(key, keys[i])) i ++;
        if(i < n && isEqual(key, keys[i])) return keys[i].cost;
        else if(leaf) return 0;
        else return c[i]->lookup(key);
    }
    
    std::list<int> lookupRange(Date from, Date to, bool ok = 0){//LOOKRANGE
        int i = 0;
        if(ok == 0){
            while (i < n && isMore(from, keys[i])) i ++;
            if(i < n && (isMore(keys[i], from) || isEqual(keys[i], from))){
                if(!ls.empty()) ls.clear();
                ls.push_back(keys[i].cost);
                ok = 1;
            }
            if(ok == 0){
                if(leaf) return ls;
                return c[i + 1]->lookupRange(from, to, 0);
            }
        }else{
            while (i < n && (isMore(to, keys[i]) || isEqual(keys[i], to))){
                ls.push_back(keys[i].cost);
                i ++;
            }
            if(i < n && (isMore(keys[i], to) || leaf)) return ls;
            return c[i+1]->lookupRange(from, to, 1);
        }
    }
    void traverse(){
        int i;
        for (int i = 0; i < n; i ++){
            if(!leaf)c[i]->traverse();
            std::cout << ' ' << keys[i].cost;  
        }
        if(!leaf)c[i]->traverse();
    }

    void Sum (Date from, Date to){
        for (int i = 0; i < n; i ++){
            if((isMore(keys[i], from) || isEqual(keys[i], from)) && (isMore(to, keys[i]) || isEqual(to, keys[i]))){
                ans += keys[i].cost;
            }
        }
        if(!leaf){
            for (int i = 0; i < n - 1; i ++){
                if(isMore(from, keys[i + 1]) || isMore(keys[i], to)) continue;
                if((isMore(keys[i], from) || isEqual(keys[i], from)) && (isMore(to, keys[i + 1]) || isEqual(to, keys[i + 1]))){
                    ans += c[i + 1]->sum;
                }else{
                    c[i + 1]->Sum(from, to);
                }
            }
            if(n > 0 && (isMore(keys[0], from) || isEqual(keys[0], from))) c[0]->Sum(from, to);
            if(n > 0 && (isMore(to, keys[n - 1]) || isEqual(to, keys[n - 1]))) c[n]->Sum(from, to);
        }
    }
template<class K, class V>
friend class BTree;
};

Node *root = NULL;
int height;

template<class K, class V>
class BTree : public RangeMap<K, V>{ //     IMPLEMENTATION
public:
    
    BTree(){

    }

    int size(){ return 2*binpow(T, height) - 1;}
    bool isEmpty(){
        if(root->leaf) return true;
        return false;
    }

    void add(Date key, int value){
        if(root == NULL){
            root = new Node(true);
            root->keys[0] = key;
            root->n = 1;
            root->sum = value;
        }else{
            if(root->n == 2 * T - 1){
                Node *s = new Node(false);
                s->c[0] = root;
                s->splitChild(0);
                int i = 0;
                if(isMore(key, s->keys[0]))i ++;
                s->c[i]->insertNonFull(key, value);
                s->sum += value;
                root = s;
            }else root->insertNonFull(key, value);
        }
    }
    bool contains(Date key){ return (root == NULL) ? 0 : root->contains(key);}
    int lookup(Date key){return (root == NULL) ? NULL : root->lookup(key);}
    std::list<int> lookupRange(Date from, Date to){ return (root == NULL) ? ls : root->lookupRange(from, to);}
    int report (Date from, Date to){
        int res = 0;
        if(root == NULL){
            return 0;
        }
        ans = 0;
        root->Sum(from, to);
        res = ans;
        return res;
    }
};

int main (){
    int q;
    std::cin >> q;
    BTree<Date, int> btree;
    while (q --){
        std::string str, op, fr, ch, to;
        int am;
		std::cin >> str;
        if(str == "REPORT"){
            std::cin >> str >> fr >> ch >> to;
			std::cout << btree.report(Date(fr, 0), Date(to, 0)) << '\n';
		}else{
			std::cin >> op >> am;
			if(op == "WITHDRAW") am *= -1;
            btree.add(Date(str, am), am);
		}
    }
    return 0;   
}
