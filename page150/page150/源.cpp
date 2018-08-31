#include"../../std_lib_facilities.h"
#include"Token.h"


const char number = '8';               //基本功能下要是用的
const char quit = 'q';
const char print = ';';
const string prompt = "> ";
const string result = "= ";
//**************引入变量的情况下要用的
const char name = 'a';                 //就算是let和name也要用L和a来做它们作为单词的t.kind,这是因为我们之前在未引入变量时的处理方法就是把t.kind作处理核心的，所以在此申明
const char let = 'L';
const string declkey = "let";




Token_stream ts;                      //创建一个名为"ts"的提供get（）和putback()功能的“Token_stream ”类型
									  //********这是个提前声明，因为下面的的primary、term和expression函数都需要名为ts的Token_stream类来提供ts.get()功能


double expression();                  //********提前声明 
double get_value(string s);           //********提前声明,因为primary里用了这个函数


double primary()                      //primary是真正获得数值，ts的get函数是将数据加个标签并将该数据一起打包，primary才是根据标签来获得真正的值
{

	Token t = ts.get();                //primary直接获取单词（单词已经是最小的了，所以primary没法像expression将获得数值的任务甩给term函数），并将它分成数值还是括号
									   //这是term、primary、expression抓取值的核心步骤
	switch (t.kind)                    //虽然primary作为最基本获得单词的模块看似好像需要提供所有读取cin流可能遇到的情况，但可以理解为只写了两种情况。主要是因为使用计算器第一位的输入的必须为“（”或者是个数值，否则就是用户输入错误 而加、减、乘、除号会在获得一个数值后连续获得并使用（在term、expression中）
	{
	case'（':
	{                                  //在switch定义并初始化变量需要把他们放到一个语句块内
		double e = expression();       //获得下一位的表达式级别的数据
		t = ts.get();                  //获得下一位单词来查看是不是“）”
		if (t.kind != ')') error("没反括号啊 大哥");
		return e;
	}

	case number:
		return t.value;                //因为是数值，所以只返回数值就够了
	case'-':
		return -primary();             //只允许“-数值”或“-（”两种情况通过，比如"-*"或"-/"这种非法的连续两个运算符号搭配不允许通过
	case'+':
		return primary();              //只允许“+数值”或“+（”两种情况通过
	case name:
		return get_value(t.name);      // **********return the existed variable's value，使用这步来获取已经定义的变量的值**********

	default:
		error("非primary形式的单词");   //"1++2*3;4+5;"里第二个加号会最先在这里出现错误
	}

}



double term()                            //乘除法会在这里优先进行，用primary的形式计算好了（primary_1*primary_2或primary_1/primary_2）再升格为term传出去；如果采到下一个符号是加、减法，会装作没看见，直接将该primary值(primary_1)升格为term传出去，在expression再计算加、减法
{                                        //*******不需要提前声明，因为第136行虽然使用了primary 但primary的定义在我们这个term函数之前，即到这里（指term）时计算机已经认识primary了
	double left = primary();             //第一个数值的获取靠primary，或者说term只处理primary级别的数值
	Token t = ts.get();                  //获取下一位单词（一般是为了获得计算符号）


	while (1)                            //上两句为引入门的两句 之后primary级别的计算全在这个while（1）循环里进行
	{
		switch (t.kind)
		{
		case'*':
			left *= primary();           //获取下一个primary，并使用这个primary去计算新的left
			t = ts.get();                //获取下一个单词（一般是为了获得计算符号）
			break;
		case'/':
		{
			double e = primary();
			if (e == 0) error("divide by zero");               //考虑会不会除以0
			left /= e;
			t = ts.get();
			break;
		}
		case'%':
		{
			double d = primary();
			if (d == 0)error("除以0了");
			left = fmod(left, d);                   //fmod（） 浮点取模函数
			t = ts.get();
			break;
		}
		default:
			ts.putback(t);
			return left;                 //升格，输出term形式的结果

		}
	}
}


double expression()                   //在这个函数里计算加减法                                                            
{
	double left = term();             //第一个数值的获取靠term，或者说expression只处理term级别的数值
	Token t = ts.get();               //获取下一位单词（一般是为了获得计算符号）


	while (1)                         //上两句为引入门的两句 之后term级别的计算全在这个while（1）循环里进行
	{
		switch (t.kind)
		{
		case'+':
			left += term();           //获取（同时也调用了）下一个term，并使用这个term去计算新的left
			t = ts.get();             //获取下一个单词（一般是为了获得计算符号）
			break;
		case'-':
			left -= term();
			t = ts.get();
			break;
		default:
			ts.putback(t);
			return left;              //输出结果

		}
	}
}
/*********************************以上为正常计算器（无变量及常量）所需*********************************************************/


class Variable {                       //定义"变量"
public:
	string name;
	double value;
};

vector<Variable>var_table;             //申请一个名为var_table的Varible类的vector，给变量们一个单独的存贮空间

double get_value(string s) {           //1.读变量：返回name为字符串s的Varible对应的值 
	for (const Variable x : var_table)
		if (x.name == s)return x.value;
	error("压根没找到要查找的变量，所以也求不得它的值");
}

void set_value(string s, double d) {  //2.写变量
	for ( Variable x : var_table) {
		if (s == x.name) x.value = d;
		return;
	}
	error("您正在给一个不存在的变量赋值");
}

bool is_declared(string var)           //3.1（3的辅助功能）检测var是否已经在var_table中？
{
	for (const Variable&v : var_table)if (v.name == var) return true;
	return false;
}

double define_name(string var, double val)          //3.加一个变量
{
	if (is_declared(var))error("这个变量已经有了，不能重复定义");
	Variable v = { var,val };
	var_table.push_back(v);
	return val;

}
double declaration() {                            //处理看见let之后的情况（使用上面1、2、3所有的东西）
	Token t = ts.get();                           //在获得let后，进一步的去获得下一个单词
	if (t.kind != name)error("name expected in declaration");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=')error("=missing in declaration of", var_name);

	double d = expression();
	define_name(var_name, d);                    
	return d;
}
/*******************以上为变量计算的具体实现************************************/
//分类器（选择我们现在是进行常量计算还是变量计算）
double statement() {                               //作为处理"常量计算"和处理"变量计算"的分类器（使用上面所有东西（1、2、3+declaration（）））
	Token t = ts.get();
	switch (t.kind) {
	case let:                                      //处理变量(未定义过的变量才算作<处理变量>的情况)，即这里的情况只负责处理变量的定义
		return declaration();
	default:                                       //处理常量(定义过的变量算作<处理常量>的情况)
		ts.putback(t);                             
		return expression();                       //虽然我们的设计初衷是不会一次输入多个要计算的计算式，但真的要一次性输入多个计算式的首个计算式时，一旦输入的第一个单词不是";"或"q"，则会启动这句话，且一直会把所输入的要求计算的式子（比如"123+23+345;"中的"123+23+345"）全计算出结果，即令val妥妥的一次性等于"123+23+345"的结果
	}
}

/*********************************以下为正常计算器（无变量及常量）所需**********************************************************/
void clean_up_mess()
{
	ts.ignore(print);
}

void calculate()                                            //带有错误恢复功能的计算函数
{
	while (cin)                                             //代表输入状态（cin的状态）只要是允许输入的状态就算满足循环条件 所以即使一开始不输入任何东西 这个允许的状态也是满足的;实际上因为没说往那种基本类型里输入，只是“while（cin）”（而不是“int a=0;while（cin>>a）”）所以这样写的话，输入状态是基本不会错的
		try {
		cout << prompt;
		Token t = ts.get();                                 //检查单词看是否为需要立即显示的计算作业或者是要关闭计算器

		while (t.kind == print)t = ts.get();                //吃掉";"(因为使用的是while，所已可以吃掉连续的";",比如"1+2;;;"里的";")


		if (t.kind == quit) {
			return;
		};


		ts.putback(t);


		cout << result << statement() << "\n";               //为了加入变量处理功能，将expression（）改成了statement（）
		                                                     
	}
	catch (exception&e) {
		cerr << e.what() << '\n';
		clean_up_mess();                                     //输入错误与运行错误是两个概念，我们虽然输入的"1++2*3;4+5;"是会导致运行错误 但是就算是输入完毕后，也不是造成输入错误，但此时运行会出错（因为第二个加号），于是我们运次那个calculate里的处理运行错误的部分，清楚残余数据，但因为此时197里的while（cin）依旧保持肯定状态，所以计算会继续
	}
}



int main()
try
{
	calculate();
	keep_window_open();
	return 0;
}
catch (exception&e)                                       //********捕捉所有上述代码中安插的error函数
{
	cerr << e.what() << '\n';                             //这条语句能在“开始执行”的运行条件下将error函数的话能引用过来，从而发现是哪一块的error函数出了问题（如果每一块的error函数的话不一样的话）
	keep_window_open("~~");                               //输入特定的“~~”才可以关闭窗口，这么做的原因在page139页
	return 1;
}
catch (...)
{
	cerr << "exception\n";
	keep_window_open("~~");
	return 2;
}