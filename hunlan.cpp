#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include "hunlan.h"
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

class MyManager{
    int lineNum;
    vector<string> strVariableVec;
    vector<string> numVariableVec;
    vector<string> result;
    map<string , string> strVariable;
    map<string , int> numVariable;

    public:
    void taskAssign(vector<string> result1, int line){
        
        result = result1;
        lineNum = line;
        
       if(result[0] == "NUMBER" or result[0]== "STRING"){
           declare();
       } 
       
        if(result[1] == "=" and result.size()==3){
           initialize(result);
       }
        if(result[0] == "PRINT"){
            if(lineNum == 25){
                cout<<"test"<<endl;
            }
           print();
       }
       for(long long unsigned int i=0;i<result.size();i++){
           if(result[i]=="+" or result[i]=="-" or result[i]=="*"){
             
               calculation();
               break;
           }
       }
    
    }

    void declare(){
       
        if(result[0]== "NUMBER"){
            //int num = stoi(result[1]);
            if(result[1] == "NUMBER" or result[1] == "STRING" or result[1] == "PRINT"){
                cout << "ERROR: Bad instruction in Line "<<lineNum<<endl;
                exit(3);
            }
            else{
                //NUMBER num;
                numVariableVec.push_back(result[1]);
            }
        }
        else if(result[0]== "STRING"){
            if(result[1] == "NUMBER" or result[1] == "STRING" or result[1] == "PRINT"){
                cout << "ERROR: Bad instruction in Line "<<lineNum <<endl;
                exit(3);
            }
            else{
                strVariableVec.push_back(result[1]);
                
            }
            
        }    
    }

    void initialize(vector<string>result1){
        vector<string> result = result1;
        bool variableMatched = false;
     for(long long unsigned int i = 0; i< numVariableVec.size();i++){
        
         if(isNumber(result[2]) and (numVariableVec[i]==result[0])){
            int val = stoi(result[2]);
            numVariable.insert(pair<string,int>(result[0],val));
            variableMatched = true;
            
        }
        int value;
        if(!isNumber(result[2]) and (numVariableVec[i]==result[0])){
            for(long long unsigned int j = 0; j< numVariableVec.size();j++){
                
            if(numVariableVec[j]==result[2]){
                for(auto pair: numVariable){
                    if(pair.first == result[2]){
                        value = pair.second;
                        variableMatched = true;
                        
                    }
                }
            }
            if(variableMatched == true){
                numVariable.insert(pair<string,int>(result[0], value));
                
            }
            }
            
        }
         
     }
        for(long long unsigned int i = 0; i< strVariableVec.size();i++){ 
            
            if((variableMatched==false) and (strVariableVec[i] == result[0])){
                strVariable.insert(pair<string,string>(result[0],result[2]));
                variableMatched = true;
            }
        }
        if(variableMatched == false){
             cout << "ERROR: Bad instruction in Line "<<lineNum <<endl;
             exit(3);
        }  
    }

    void print(){
        string output = result[1];
     
        if(result.size()>2){
            cout << "ERROR: Bad instruction in Line "<<lineNum <<endl;
            exit(3);
        }
        else if(output[0]=='"' and output[output.size()-1] == '"'){
            cout << output.substr(1,output.size()-2)<<endl;
        }
        else if(isNumber(output)){
            cout<< output<<endl;
        }
        
        else{
            
            bool variableMatched = false;
            for(auto pair : numVariable){
                //cout<<pair.first<<endl;
                if(pair.first == result[1]){
                    
                    cout << pair.second<<endl;
                    variableMatched = true;
                }
            }
            for(auto pair : strVariable){
                if(pair.first == result[1] and variableMatched == false){
                    string out = pair.second;
                    cout << out.substr(1,out.size()-2)<<endl;
                    variableMatched = true;
                }
            
            }
            if(variableMatched == false){
                cout << "ERROR: Bad instruction in Line "<<lineNum <<endl;
                exit(3);
            }
        }
    }
    int precedence(string c){
        if(c == "*"){
            return 3;
        }
        else if(c == "+"){
            return 2;
        }
        else if(c == "-"){
            return 1;
        }
        else{
            return 0;
        }
    }
    bool isOperator(string oper){
        if(oper == "+" or oper == "-" or oper == "*"){
            return true;
        }
        else{
            return false;
        }
    }
    int Evaluation(string operand1, string operand2 , string operato){
        
    int value1 = stoi(operand1);
    int value2 = stoi(operand2);
    if(operato == "+"){
        return value1+value2;
    }
    else if(operato == "-"){
        return value1-value2;
    }
    else{
        return value1*value2; 
    }
    
}
string PostfixEvaluation(vector<string> postfix){
    
     stack<string> evalStack;
     for(int i=0; i<postfix.size();i++){
         if(isNumber(postfix[i])){
             
             evalStack.push(postfix[i]);
         }
         if(isOperator(postfix[i])){
             string temp = postfix[i];
             string operand2 = evalStack.top();
             evalStack.pop();
             string operand1 = evalStack.top();
             evalStack.pop();
             
             string sum = to_string(Evaluation(operand1,operand2,postfix[i]));
             evalStack.push(sum);
         }
        
     }
             return evalStack.top();

 }
string InfixToPostfix( vector<string> infix){ 
    stack<string> oper;
    vector<string> postfix;
    for(int i=0;i<infix.size();i++){
        if(isNumber(infix[i])){
            postfix.push_back(infix[i]);
        }
        else if(isOperator(infix[i])){
                string opero = infix[i];
                
                if(oper.empty()){
                    
                    oper.push(opero);
                }
                else{ 
                    if(precedence(oper.top())<precedence(opero)){
                        oper.push(opero);
                    }
                    else if(precedence(oper.top())==precedence(opero)){
                        oper.push(opero);
                    }
                    else if(precedence(oper.top())>precedence(opero)){
                        string temp = oper.top();
                        oper.pop();
                        postfix.push_back(temp);
                        oper.push(opero);
                    }
                    else{
                        cout << "ERROR: Bad instruction in Line "<<lineNum<<endl;
                        exit(3);
                     }
                }      
        }
    }
    while(!oper.empty()){
            postfix.push_back(oper.top());
            oper.pop();
    }
   
    postfix.erase(std::remove(postfix.begin(), postfix.end(), ""), postfix.end());
    return PostfixEvaluation(postfix);   
}
    void calculation(){
        bool variableMatch = false;
       
        for(long long unsigned int i = 2; i<result.size();i++){
            
            if(!isNumber(result[i]) or !isOperator(result[i])){
            for(auto pair: numVariable){
                if(result[i] == pair.first){
                    result[i] = to_string(pair.second);
                    variableMatch = true;
                }
                
            }
        }
         if(isNumber(result[i]) or isOperator(result[i])){
            variableMatch = true;
        }
        if(variableMatch == false){
            cout << "ERROR: Bad instruction in Line "<<lineNum<<endl;
            exit(3);
        }
        }
        
        vector<string> newResult;
         for(long long unsigned int i = 2; i<result.size();i++){
             newResult.push_back(result[i]);
         }
        string sum = InfixToPostfix(newResult);
        vector<string> initializeValue{result[0], result[1], sum};
        
        initialize(initializeValue);
    }

    bool isNumber(string str){
        for(long long unsigned int i=0; i<str.size();i++){
            if(!isdigit(str[i])){
                return false;
            }
        }
        return true;
    }

};

class Processing{
    MyManager manager;
    public:
    vector<string> checkDoubleQuoteStr(vector<string> result){
        string newItem = "";
        vector<string> newresult;
        bool openingQuote = false;
        bool strComplete = false;
        for(long long unsigned int i=0;i<result.size();i++){
            string str = result[i];
            /*
            if(str[0] == '"' and str[str.size()-1]=='"'){
                return result;
            }
            */
            if(str[0] == '"' and openingQuote == false and strComplete== false){
                
                 newItem= result[i];
                 openingQuote = true;
            }
            else if(str[str.size()-1]!='"' and openingQuote == true and strComplete== false){
                newItem = newItem +" "+ str;
                
            }
            else if(str[str.size()-1]=='"' and openingQuote == true and strComplete== false){
                newItem = newItem + " "+str;
                newresult.push_back(newItem);
                //openingQuote = false;
                strComplete = true;
                
            }
            else{
                newresult.push_back(str);
                
            }
            
        }
        return newresult;
    }
    void parse(string input, int lineNum){
        
        vector<string> result;
        vector<string> result1;
        istringstream iss(input);
        string item, item1;
        string line= "";
        vector<char> seperator{' ', '\t'};

        while(getline(iss,item1,'\t')){
            
            line = line+ item1;   
        }
        while(line[0]== ' '){
            line = line.substr(1);
        }
        
        string str;
  
    for(int i=0; i<line.length();  ){
        
        if(line[i] == ' '){
           
            while(line[i+1] == ' ')
                i++;
        }
        
        str =str+ line[i++];
    }
        istringstream iss1(str);

        while(getline(iss1,item,' ')){
            vector<string> checkItem = checkString(item);
            if(checkItem.size()!=0){
                for(long long unsigned int i=0;i<checkItem.size();i++){
                    result.push_back(checkItem[i]);
                }
            }
            else{
                result.push_back(item);
            }
            
        }
    
        for(long long unsigned int i=1; i<result.size();i++){
            if(result[i]=="#"){
                result.resize(i);
            }
        }
        result.erase(std::remove(result.begin(), result.end(), " "), result.end());
        /*
        for(int i=0; i<result.size();i++){
            if(result[i]!=" "){
                result1.push_back(result[i]);
            }
        }
        */
        
        
        vector<string> newresult = checkDoubleQuoteStr(result);
        if(newresult.size()<result.size()){
            manager.taskAssign(newresult,lineNum);
        }
        else{
            manager.taskAssign(result,lineNum);
        }
        
    }

    void readfile(string filename){
        string line;
        ifstream myfile;
        int lineNum=1;
        myfile.open(filename);
        
        if(myfile.is_open()){ 
            while(myfile){
                getline(myfile , line);
                
                while(line.size()==0 or line[0]== '#'){
                    getline(myfile,line);
                    lineNum++;
                }
                parse(line, lineNum);
                lineNum++;
            }
        
            myfile.close();
        }
        else{
            cout<<"File not open"<<endl;
        }
    }
    vector<string> checkString(string item){
        
        vector<string> seperatedStr;
        if(item.size()>1){
            for(long long unsigned int i=0;i<item.size();i++){
                if(item[i] == '+' or item[i] == '-' or item[i] == '*' or item[i] == '=' or item[i] == '#'){
                    string item1 = item.substr(0,i);
                    seperatedStr.push_back(item1);
                    string item2 = item.substr(i,1);
                    seperatedStr.push_back(item2);
                    string item3 = item.substr(i+1);
                    seperatedStr.push_back(item3);
                    return seperatedStr;
                }
            }
        }
        return {};
    }
    
};
int main(int argc, char** argv)
{
    Processing process;
    
    process.readfile(argv[1]);
  
    return 0;
}

