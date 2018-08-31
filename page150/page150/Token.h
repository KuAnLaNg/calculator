#pragma once

class Token {                                      // ********��Ҳ�Ǹ���ǰ���� ��Ϊ���涨�塰Token_stream������ʱ����Ҫ����Token����
public:
	char kind;
	double value;
	/*********����ΪΪ��ʵ�ֱ����������ӵ�Token����**********/
	string name;

	Token(char ch) :kind{ ch } { }                            //�������캯����Token��ɲ�ͬ����̬ ��ʵ����Ҳֻ�Ƕ����һ��{'a',string}�ṹ��
	Token(char ch, double val) :kind{ ch }, value{ val } { }
	Token(char ch, string n) :kind{ ch }, name{ n } { }
	Token() {}                                                        //*********����һ�����޲����ģ�Ĭ�Ϲ��캯�� Ϊ��buffer
};


class Token_stream {                              // ********��Ҳ�Ǹ���ǰ��������������Token_stream������public�����Ĳ��ֶ��㣩 ��Ϊ��������һ����Ϊts��Token_stream����ʱ��Ҫ�ȶ��������ν�ġ�Token_stream�ࡱ
public:

	Token get();                      //����ֵΪToken�������ʡ������͵ĺ���
	void putback(Token t);            //�޷���ֵ�ĺ���
	void ignore(char c);              //����ֱ���ַ�c(����c)�������ַ�
private:
	bool full{ false };               //���private��ĳ�Ա������private�ͳ�ʼ��Ϊfalse����δ��public�ﴩ�����캯������ʼ������������Ҳֻ��ͨ��public�����ʴӶ�ʵ�ָ���
	Token buffer;                     //���д����������Ҫһ���޲ε�Ĭ�Ϲ��캯������Ϊ������ʽ������������Ҫһ��Ĭ�Ϲ��캯��
};






