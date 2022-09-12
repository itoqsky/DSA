#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

#define sp(x) setprecision(x));

using namespace std;

bool isDigits(string x){
    int ok = 0;
    for(int i = 0; i < x.size(); i++){
        if(x[i] == '.'){
            if(ok == 1) {
                cout << x << '\n';
                return false;
            }
            if(ok == 0) ok = 1;
        }
        else if('0' <= x[i] && x[i] <= '9'){
            continue;
        }else{
            return false;
        }
    }
    return true;
}

long long toInt(string s){
    long long x = 0;
    for(int i = 0; i < s.size(); i++){
        if(s[i] == 13 || s[i] == 10) break;
        x = x * 10 + (s[i] - '0');
    }
    return x;
}

string fullAns(long double d){
    //cout << d << '\n';
    string s, str;
    str = to_string(d);
    char *p;
    int cnt = 0;
    p = strchr(&str[0], '.');
    while (cnt <= 2){
        cnt ++;
        if(*p == '\0') break;
        p ++;
    }
    *p-- = '\0';
    while (*p == '0'){
        *p-- = '\0';
    }

    if(*p == '.'){
        *p = '\0';
    }
    return str;
}
class User{ 
    string name;
    string PIN;
    long double balance;
    int number;
    long double last;
    int bank;
public:
    User() {}

    User(string name, string PIN, long double balance, long double last, int number, int bank){
        this->name = name;
        this->PIN = PIN;
        this->balance = balance;
        this->last = last;
        this->number = number;
        this->bank = bank;
    }
    
    bool isActivated(){
        if(isDigits(this->PIN)){
            return true;
        }else{
            return false;
        }
    }
    
    bool ask(){
        char ch;
        cout << endl << "Would you like to return to the main menu? (Y/N): ";
        cin >> ch;
        cout << endl;
        if(ch == 'Y'){
            return true;
        }else{
            return false;
        }
    }
    
    bool modifyPin(){
        string s;
        cout << "Enter new PIN code: " << endl;
        cin >> s;
        if(isDigits(s) == false) {
            cout << endl;
            cout << "The code should be pure 4-digit code" << endl;
            return modifyPin();
        }
        if(isDigits(this->PIN) == false){
            cout << "The card is activated" << endl;
        }else{
            cout << "The card is modified";
        }
        this->PIN = s;
        
        return ask();
    }
    
    bool showBalance(){
        cout << "Card owner: " << this->name << endl;
        cout << "Card number: " << this->number << endl;
        printf("Card balance in KZT: %s\n", fullAns(this->balance).c_str());
        cout << "Card balance in USD: " << this->balance/450 << endl;
        printf("Last transaction: %s\n", fullAns(this->last).c_str()); 
        return ask();
    }
    
    bool isPinSame(string pin){
        if(this->PIN == pin){
            return true;
        }else{
            return false;
        }
    }
    
    bool withdraw(){
        long double x;
        printf("\nCurrent balance: %s KZT\n", fullAns(this->balance).c_str());
        cout << "Enter amount to withdraw: ";
        string amn;
        cin >> amn;
        while (!isDigits(amn) || stold(amn) <= 0.0){
            if(stold(amn) == 0.0) cout << "Nothing to withdraw\n";
            else cout << "Invalid amount entered (amount must be a positive number)\n";
            cout << "Try again: "; 
            cin >> amn;
        }
        x = stold(amn);
        if(x > this->balance){
            cout << "The balance is insufficient" << endl;
            return withdraw();
        }else{
            cout << endl << "Cash withdrawn successfully!" << endl;
            this->balance -= x;
            last = -x;
            printf("Remaining balance: %s KZT\n", fullAns(this->balance).c_str());
            return ask();
        }
    }
    
    bool deposit() {
        long double x;
        printf("\nCurrent balance: %s KZT\n", fullAns(this->balance).c_str());
        cout << "Enter amount to put in: ";
        string amn;
        cin >> amn;
        while (!isDigits(amn) || stold(amn) <= (double)0){
            if(stold(amn) == (double)0) cout << "Nothing to put in\n";
            else cout << "Invalid amount entered (amount must be a positive number)\n";
            cout << "Try again: "; 
            cin >> amn;
        }
        x = stold(amn);
        this->balance += x;
        last = x;
        cout << endl << "Cash is in the card successfully!" << endl;
        printf("Remaining balance: %s KZT\n", fullAns(this->balance).c_str());
        return ask();
    }
    bool transfer();
}mp[1000000];

bool User::transfer() {
    int toNumber;
    long double sum;
    cout << endl << "**Commission for fund transfers over 20,000 KZT is 500 KZT" << endl;
    cout << endl << "Beneficiary’s card number: ";
    cin >> toNumber;
    
    while(!mp[toNumber].isActivated()) {
        cout << "The card is not activated." << endl << endl;
        cout << "Beneficiary’s card number: ";
        cin >> toNumber;
    }
    cout << "Transfer amount (in KZT): ";
    string amn;
    cin >> amn;
    while (!isDigits(amn) || stold(amn) <= (double)0){
        if(stold(amn) == (double)0) cout << "Nothing to transfer\n";
        else cout << "Invalid amount entered (amount must be a positive number)\n";
        cout << "Try again: "; 
        cin >> amn;
    }
    sum = stold(amn);

    if(sum > 20000) sum += 500;
    if(this->bank != mp[toNumber].bank){
        sum += 200;
    }
    
    while(this->balance < sum){
        cout << "The amount is insufficient" << endl << endl;
        cout << "Transfer amount (in KZT): ";
        cin >> sum;
        
        if(sum > 20000) sum += 500;
        if(this->bank != mp[toNumber].bank){
            sum += 200;
        }
    }
    
    cout << "Transfer successful!" << endl;
    this->balance -= sum;
    if(this -> bank != mp[toNumber].bank) sum -= 200;
    if(sum > 20500) sum -= 500;
    
    mp[toNumber].balance += sum;
    
    return ask();
}
    
void fail(){
    cout << endl << "The card number or PIN code is incorrect. Try again!" << endl;
}
    
int ask2(){
    int number;
    string pin;
    cout << "Card number (6 digits): ";
    cin >> number;
    
    if(!(100000 <= number && number <= 999999)){
        fail();
        return -1;
    }
    cout << "PIN code (4 digits): ";
    cin >> pin;
    if(pin.size() != 4){
        fail();
        return -1;
    }
    if(mp[number].isPinSame(pin)){
        if(!isDigits(pin)){
            cout << "The card is new and has to be activated";
            mp[number].modifyPin();
        } 
        return number;
    }
    fail();
    return - 1;
}

void output(string text, int length, bool left){
    if(left == true){
        cout << text;
        for(int i = 1; i <= length - text.size(); i++){
            cout << ' ';
        }
    }else{
        int spaces = (length - text.size() + 1) / 2;
        for(int i = 1; i <= spaces; i++){
            cout << ' ';
        }
        cout << text;
        for(int i = 1; i <= spaces; i++){
            cout << ' ';
        }
        cout << endl << endl;
    }
}

int main(){
    fstream myfile;
    myfile.open("./database.csv", ios::in);
    string line, word;
    vector<string> row;
    getline(myfile, line);
    cout << fixed;
    while(getline(myfile, line)){
        row.clear(); 

        stringstream ss(line);
        
        while(getline(ss, word, ',')){
            row.push_back(word);
        }
       // cout << int(row[5][1]) << endl;
        mp[stoi(row[2])] = User(row[1], row[3], stold(row[4]), stold(row[5].substr(0, row[5].size() - 2)), stoi(row[2]), stold(row[0]));
    }
    
    while(1){
        int conslen = 60;
        string title = "Welcome to the bank NUR";
    
        output(title, conslen, 0);
    
        int cardNumber = -1;
        
        while(1){
            int x = ask2();
            if(x == -1){
                continue;
            }else{
                cardNumber = x;
                cout << endl;
                break;
            }
        }

        while(1){
            
            bool cont;
            cout << "Select one of the following options: " << endl << endl;

            output("1. Withdraw Cash", conslen/2, 1);
            output("2. Deposit Cash", conslen/2, 1);
            cout << endl;
            output("3. Modify PIN Code", conslen/2, 1);
            output("4. Show Balance", conslen/2, 1);
            cout << endl;
            output("5. Transfer Between Accounts", conslen/2, 1);
            output("6. End Session", conslen/2, 1);
            cout << endl;
            
            int type;
            
            cout << endl << "Enter option number: ";
            cin >> type;
            
            if(type == 1){
                cont = mp[cardNumber].withdraw();
            } else if(type == 2){
                cont = mp[cardNumber].deposit();
            } else if(type == 3){
                cont = mp[cardNumber].modifyPin();
            } else if(type == 4){
                cont = mp[cardNumber].showBalance();
            } else if(type == 5){
                cont = mp[cardNumber].transfer();
            } else {
                cout << "Goodbye!" << endl;
                break;
            }

            if(cont == false) break;
        }
    }
}
