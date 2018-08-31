#include"../../std_lib_facilities.h"
#include"Token.h"                      //必须要加，否则连类都没有，再它的成员函数就是胡扯


const char number = '8';               //基本功能下要用的
const char quit = 'q';
const char print = ';';
const string prompt = "> ";
const string result = "= ";
//**************引入变量的情况下要用的
const char name = 'a';                 //就算是let和name也要用L和a来做它们作为单词的t.kind,这是因为我们之前在未引入变量时的处理方法就是把t.kind作处理核心的，所以在此申明
const char let = 'L';
const string declkey = "let";


void Token_stream::putback(Token t)    //第1个功能“放回去”
{
	if (full)error("缓存区是满的，得先使用缓存区现有的数据，再往缓存区存入新的数据");
	buffer = t;
	full = true;
}




void Token_stream::ignore(char c)    //第3个功能：为了实现“错误恢复”所需的“略去”功能
{
	if (full&&c == buffer.kind) {    //这里之所以不需要说明是下面已经申请好的ts的buffer是出于“我买了洗衣机时配套好了延长保险，并一起购买（所以洗衣机和延长保险会匹配）”的逻辑；而不是“使用先买洗衣机再选延长保险，这时就需要将延长保险选定为你先前买的洗衣机” 
		full = false;
		return;
	}
	full = false;

	char ch = 0;
	while (cin >> ch)                //从输入流使用剩下的数据不断改写ch，相当于不断的吃数据，直到出错的计算式的参与部分被吃干净
		if (ch == c)return;          //比如（在加入primary里的对负号和正号的处理之前才算做的错误）："1++2*3;4+5;":第二个加号令expression（）里的197行出错，导致使用cin中剩下的“2*3;4+5”运行215行
}


Token Token_stream::get()                       //第2个功能（最基本功能）“从cin中获取单词”
{
	if (full)
	{
		full = false;
		return buffer;
	}
	char ch;                                          //先用字符承接这个数据
	cin >> ch;                                        //">>"会滤过所有空白符
	switch (ch)
	{
	case print:
	case quit:
	case'(':
	case')':
	case'+':
	case'-':
	case'*':
	case'/':
	case'%':
	case'=':                                          //*********为了变量的“let name = expression（）”而加的对于"="的获取
		return Token{ ch };                           //以Token的形式输出，并只用也只需用Token的kind来存储这些运算符号


	case'.':                                          //为喜欢"0.2"用".2"写法的人准备
	case'0 ': case'1': case'2': case'3': case'4':
	case'5':case'6': case'7': case'8': case'9':
	{
		cin.putback(ch);                              //调用cin流的成员函数putback(),来把独读取到的一个字符放回cin中去
		double val;
		cin >> val;                                   //发现这个单词将不会是运算符号后，改用浮点数承接这个数据
		return Token{ number,val };
	}

	default:                                         //isalpha()是为了查看输入的是不是英文字符 *****************所以变量名首字母必须为英文字母
		if (isalpha(ch)) {                           //*********为了变量的“let name = expression（）”而为'let' 'name' 两个“字符串”单词创建的后门
			string s;                                //98与99行是由于Token最多只存在“Token(char ch, string n) :kind{ ch }, name{ n } { }”的原因，针对单字母组成的变量名，强行将这个单字母变成字符串好装进这种Token里传出去
			s += ch;
			while (cin.get(ch) && isalpha(ch) || isdigit(ch))s += ch;    //cin.get()连空格符都不放过的从cin里拿字符，但是拿到的字符要满足“不是英文字符就是数字（首字母不行）”的条件我才算作名字
			cin.putback(ch);
			if (s == declkey)return Token(let);      //实际上“let”是返回“Token（'L'）”, const string declkey = "let";
			return Token{ name,s };                  //“name”才需要(也是实际上)返回的是“Token（'a',股票公司名）”
		}
		error("超出get（）的获取数据的种类范围");
	}
}

