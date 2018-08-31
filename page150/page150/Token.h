#pragma once

class Token {                                      // ********这也是个提前声明 因为下面定义“Token_stream”类型时先需要定义Token类型
public:
	char kind;
	double value;
	/*********以下为为了实现变量计算而多加的Token内容**********/
	string name;

	Token(char ch) :kind{ ch } { }                            //三个构造函数将Token搞成不同的形态 （实际上也只是多加了一种{'a',string}结构）
	Token(char ch, double val) :kind{ ch }, value{ val } { }
	Token(char ch, string n) :kind{ ch }, name{ n } { }
	Token() {}                                                        //*********设置一个（无参数的）默认构造函数 为了buffer
};


class Token_stream {                              // ********这也是个提前声明（包括定义Token_stream的两个public函数的部分都算） 因为下面声明一个名为ts的Token_stream类型时需要先定义这个所谓的“Token_stream类”
public:

	Token get();                      //返回值为Token（“单词”）类型的函数
	void putback(Token t);            //无返回值的函数
	void ignore(char c);              //忽略直到字符c(包括c)的所有字符
private:
	bool full{ false };               //这个private里的成员函数在private就初始化为false（并未在public里穿件构造函数来初始化它），后期也只是通过public来访问从而实现更改
	Token buffer;                     //这个写法很明显需要一个无参的默认构造函数，因为这种形式的声明对象需要一个默认构造函数
};






