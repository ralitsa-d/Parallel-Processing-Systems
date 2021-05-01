#include<iostream>
#include<thread>
#include<future>
#include<string>
#include<cstring>
#include<climits>
#include<stack>
#include<queue>
#include<math.h>
#include<iomanip>
#include<fstream>
#include<istream>
#include<ostream>
#include<chrono>
#include <mutex>
#include"ap.h"
#include"apint.h"
#include"apfloat.h"
#include"baseint.h"
#include"bigint.h"
#include"modint.h"
#include"raw.h"
#include"apint.cpp"
#include"Tree.h"
#include"Tree.cpp"
using namespace std;

mutex mtx;
int runningThreads = 1;

typedef Tree<string> strTree;
strTree CreateTreeForm() {
	strTree t;
	string str; cout << "Enter integer or operation: "; cin >> str;
	stack<int> st;
	long long int num = 0;
	if (str[0] >= '0' && str[0] <= '9') {
		int i = 0;
		while (str[i] >= '0' && str[i] <= '9') {
			st.push(str[i] - '0');
			i++;
		}
		int k = 0;
		while (st.empty() == false) {
			num = num + st.top()*pow(10, k);
			st.pop();
			k++;
		}
		strTree t1;
		t.createTr(str, t1, t1);
	}
	else if (str[0] == '+' || str[0] == '-' || str[0] == '*' || str[0] == '/') {
		strTree t1, t2;
		t1 = CreateTreeForm();
		t2 = CreateTreeForm();
		t.createTr(str, t1, t2);
	}
	return t;
}

bool IsForm(const strTree& t) {
	if (t.empty()) return false;
	string str = t.rootTree();
	if (str[0] >= '0'&& str[0] <= '9') {
		int i = 0;
		bool b = t.leftTree().empty() && t.rightTree().empty();
		while (i < size(str)) {
			b = b && (str[i] >= '0' && str[i] <= '9');
			i++;
		}
		return b;
	}
	if ((str[0] != '+' && str[0] != '-' && str[0] != '*' && str[0] != '/') && size(str)==1) return false;
	return IsForm(t.leftTree()) && IsForm(t.rightTree());
}

long long int value(const strTree& t, int max_threads, promise<long long int> && prms) {
	string str = t.rootTree();
	stack<int> st;
	long long int num = 0;
	if (str[0] >= '0' && str[0] <= '9') {
		int i = 0;
		while (str[i] >= '0' && str[i] <= '9') {
			st.push(str[i] - '0');
			i++;
		}
		int k = 0;
		while (st.empty() == false) {
			num = num + st.top()*pow(10, k);
			st.pop();
			k++;
		}
		prms.set_value(num);
		//future<long long int> ftr = prms.get_future();
		//long long int result = ftr.get();
		//cout << "num: " << num << endl;
		return num;
	}
	long long int valLeft, valRight, result;
	long long int denom;
	if (size(str) == 1 && (str == "+" || str == "-" || str == "*" || str == "/")) {

		bool newThread = false;
		mtx.lock();
		if (runningThreads < max_threads) {
			//cout << "(++) running threads, max threads: " << runningThreads << " " << max_threads << endl;
			runningThreads++;
			newThread = true;
		}
		mtx.unlock();

		if (newThread) {
			promise<long long int> new_prms;
			future<long long int> ftr = new_prms.get_future();
			thread th1(&value, t.leftTree(), max_threads, move(new_prms));
			promise<long long int> prms1;
			valRight = value(t.rightTree(), max_threads, move(prms1));
			th1.join();
			valLeft = ftr.get();
			mtx.lock();
			//cout << "(--) running threads, max threads: " << runningThreads << " " << max_threads << endl;
			runningThreads--;
			mtx.unlock();
		}
		else {
			promise<long long int> prms2;
			valLeft = value(t.leftTree(), max_threads, move(prms2));
			promise<long long int> prms3;
			valRight = value(t.rightTree(), max_threads, move(prms3));
		}
		
		switch (str[0]) {
		case'+':
			result = valLeft + valRight;
			/*return value(t.leftTree(), max_threads, current_threads + 2, move(prms)) 
				+ value(t.rightTree(), max_threads, current_threads + 2, move(prms));*/
			break;
		case'-':
			result = valLeft - valRight;
			/*return value(t.leftTree(), max_threads, current_threads + 2, move(prms)) 
				- value(t.rightTree(), max_threads, current_threads + 2, move(prms));*/
			break;
		case'*':
			result = valLeft * valRight;
			/*return value(t.leftTree(), max_threads, current_threads + 2, move(prms)) 
				* value(t.rightTree(), max_threads, current_threads + 2, move(prms));*/
			break;
		case'/':
			if (valRight != 0) result = valLeft / valRight;
			/*denom = value(t.rightTree(), max_threads, current_threads + 2, move(prms));
			if (denom != 0) {
				return value(t.leftTree(), max_threads, current_threads + 2, move(prms)) / denom;
			}*/
			else {
				cout << "Error";
				result = 0;
			}
			break;
		}
		prms.set_value(result);
		return result;
	}
	cout << "Error1";
	return LLONG_MAX;
	
}


int weight(char op) {
	int w;
	switch (op) {
	case'+':w = 2; break;
	case'-':w = 2; break;
	case'*':
	case'/':w = 1; break;
	case '^': w = 0; break;
	default:w = -1; break;
	}
	return w;
}

void translate(char* exp, char* result) {
	stack<char> st;
	int n = strlen(exp);
	int j = 0;
	int i = 0;
	st.push('(');


	while (i < n) {
		if (exp[i] >= '0' && exp[i] <= '9'/* || exp[i]>='a' && exp[i]<='z'*/) {
			result[j++] = exp[i];
			if (exp[i + 1] < '0' || exp[i] > '9') result[j++] = ' ';
		}
		else if (exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/' || exp[i] == '^') {
			while ((weight(st.top()) <= weight(exp[i])) && st.top() != '(') {
				result[j++] = st.top();
				st.pop();
			}
			st.push(exp[i]);
		}
		else if (exp[i] == '(') {
			st.push(exp[i]);
		}
		else if (exp[i] == ')') {
			while (st.top() != '(') {
				result[j++] = st.top();
				st.pop();
			}
			st.pop();
		}
		i++;
	}

	while (st.top() != '(') {
		result[j++] = st.top();
		st.pop();
	}
	st.pop();
	result[j++] = '\0';
}

strTree fromPolishNotationToTree(string expr) {
	strTree nullt;
	//strTree t;
	stack<strTree> treeStack;
	queue<char> charQueue;
	long long int num = 0;
	string strNum = "";
	int n = size(expr);
	int i = 0;
	while (i < n) {
		if (expr[i] >= '0' && expr[i] <= '9') {
			while (expr[i] >= '0' && expr[i] <= '9') {
				charQueue.push(expr[i]);
				i++;
			}
			int k = 0;
			while (charQueue.empty() == false) {
				strNum = strNum + charQueue.front();
				charQueue.pop();
				k++;
			}
			strTree strtree;
			strtree.createTr(strNum, nullt, nullt);
			treeStack.push(strtree);
			//t = strtree;
			strNum = "";
		}

		if (expr[i]=='+' || expr[i]=='-' || expr[i]=='*' || expr[i]=='/') {
			strTree b = treeStack.top();
			treeStack.pop();
			strTree a = treeStack.top();
			treeStack.pop();
			string newstr(1, expr[i]);
			strTree newtree;
			newtree.createTr(newstr, a, b);
			treeStack.push(newtree);
		}

		i++;
	}
	return treeStack.top();
}




int main() {
	/*Tree<char> t;
	t.create();
	t.print();
	Tree<char> t1(t);
	t1.print();
	Tree<char> t2 = t1;
	t2.print();
	Tree<char> tr;
	tr.print();
	std::cout << tr.empty() << std::endl;
	Tree<char> trl = t.leftTree();
	trl.print();
	Tree<char> trr = t.rightTree();
	trr.print();

	t.leftTree().print();
	t.rightTree().print();

	char ch = t.rootTree();
	std::cout << ch << std::endl;

	std::cout << std::endl;
	tr.createTr('a', trl, trr);
	std::cout << tr.empty() << std::endl;
	tr.print();*/

	/*Tree<string> TR;
	TR.create();
	TR.print();*/

	/*Tree<char> tree = Create_Tree_Form();
	cout << isForm(tree) << endl;
	tree.print();
	if (isForm(tree)) cout << expr(tree) << endl;*/


	///Листата на дървото са числа.
	///Примерни входни данни:
	///Enter integer or operation: +
	///Enter integer or operation: *
	///Enter integer or operation: 51
	///Enter integer or operation: 2
	///Enter integer or operation: /
	///Enter integer or operation: 206
	///Enter integer or operation: 2

	///Тези входни данни представят израза (51*2)+(206/2)
	///Това е коректен израз
	///В обратен полски запис изглежда така: 51 2 * 206 2 / +
	///Стойността му е 205


	/*Tree<string> tree = CreateTreeForm();
	cout << "It is a correct expression: " << boolalpha << IsForm(tree) << endl;	
	tree.print();
	if (IsForm(tree)) cout << value(tree) << endl;*/


	char filename[50];
	ifstream fin;
	cout << "Enter filename to read from: ";
	cin.getline(filename, 50);

	int maxThreads; cout << "Enter maximum threads: "; cin >> maxThreads;

	unsigned long long t_beginning = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	//cout << "Time in milliseconds at the beginning: ";
	//cout << t_beginning << endl;

	fin.open(filename);
	if (!fin.is_open()) {
		exit(1);
	}
	char s[100000];
	ofstream fout;
	fout.open("file2.txt", ios::out);

	long long int result;

	while (fin >> s) {
		
		char s1[100000];
		translate(s, s1);
		strTree strtree;
		strtree = fromPolishNotationToTree(s1);

		promise<long long int> prms;
		//future<long long int> ftr = prms.get_future();

		result = value(strtree, maxThreads, move(prms));
		fout << result << '\n';
	}
	fin.close();
	fout.close();

	unsigned long long t_end = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	//cout << "Time in milliseconds in the end: ";
	//cout << t_end << endl;

	cout << "How long did it take to calculate(in milliseconds)? -> ";
	cout << t_end - t_beginning << endl;

	system("pause");
	return 0;
}