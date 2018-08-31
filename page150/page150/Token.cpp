#include"../../std_lib_facilities.h"
#include"Token.h"                      //����Ҫ�ӣ��������඼û�У������ĳ�Ա�������Ǻ���


const char number = '8';               //����������Ҫ�õ�
const char quit = 'q';
const char print = ';';
const string prompt = "> ";
const string result = "= ";
//**************��������������Ҫ�õ�
const char name = 'a';                 //������let��nameҲҪ��L��a����������Ϊ���ʵ�t.kind,������Ϊ����֮ǰ��δ�������ʱ�Ĵ��������ǰ�t.kind��������ĵģ������ڴ�����
const char let = 'L';
const string declkey = "let";


void Token_stream::putback(Token t)    //��1�����ܡ��Ż�ȥ��
{
	if (full)error("�����������ģ�����ʹ�û��������е����ݣ����������������µ�����");
	buffer = t;
	full = true;
}




void Token_stream::ignore(char c)    //��3�����ܣ�Ϊ��ʵ�֡�����ָ�������ġ���ȥ������
{
	if (full&&c == buffer.kind) {    //����֮���Բ���Ҫ˵���������Ѿ�����õ�ts��buffer�ǳ��ڡ�������ϴ�»�ʱ���׺����ӳ����գ���һ��������ϴ�»����ӳ����ջ�ƥ�䣩�����߼��������ǡ�ʹ������ϴ�»���ѡ�ӳ����գ���ʱ����Ҫ���ӳ�����ѡ��Ϊ����ǰ���ϴ�»��� 
		full = false;
		return;
	}
	full = false;

	char ch = 0;
	while (cin >> ch)                //��������ʹ��ʣ�µ����ݲ��ϸ�дch���൱�ڲ��ϵĳ����ݣ�ֱ������ļ���ʽ�Ĳ��벿�ֱ��Ըɾ�
		if (ch == c)return;          //���磨�ڼ���primary��ĶԸ��ź����ŵĴ���֮ǰ�������Ĵ��󣩣�"1++2*3;4+5;":�ڶ����Ӻ���expression�������197�г�������ʹ��cin��ʣ�µġ�2*3;4+5������215��
}


Token Token_stream::get()                       //��2�����ܣ���������ܣ�����cin�л�ȡ���ʡ�
{
	if (full)
	{
		full = false;
		return buffer;
	}
	char ch;                                          //�����ַ��н��������
	cin >> ch;                                        //">>"���˹����пհ׷�
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
	case'=':                                          //*********Ϊ�˱����ġ�let name = expression���������ӵĶ���"="�Ļ�ȡ
		return Token{ ch };                           //��Token����ʽ�������ֻ��Ҳֻ����Token��kind���洢��Щ�������


	case'.':                                          //Ϊϲ��"0.2"��".2"д������׼��
	case'0 ': case'1': case'2': case'3': case'4':
	case'5':case'6': case'7': case'8': case'9':
	{
		cin.putback(ch);                              //����cin���ĳ�Ա����putback(),���Ѷ���ȡ����һ���ַ��Ż�cin��ȥ
		double val;
		cin >> val;                                   //����������ʽ�������������ź󣬸��ø������н��������
		return Token{ number,val };
	}

	default:                                         //isalpha()��Ϊ�˲鿴������ǲ���Ӣ���ַ� *****************���Ա���������ĸ����ΪӢ����ĸ
		if (isalpha(ch)) {                           //*********Ϊ�˱����ġ�let name = expression��������Ϊ'let' 'name' �������ַ��������ʴ����ĺ���
			string s;                                //98��99��������Token���ֻ���ڡ�Token(char ch, string n) :kind{ ch }, name{ n } { }����ԭ����Ե���ĸ��ɵı�������ǿ�н��������ĸ����ַ�����װ������Token�ﴫ��ȥ
			s += ch;
			while (cin.get(ch) && isalpha(ch) || isdigit(ch))s += ch;    //cin.get()���ո�������Ź��Ĵ�cin�����ַ��������õ����ַ�Ҫ���㡰����Ӣ���ַ��������֣�����ĸ���У����������Ҳ���������
			cin.putback(ch);
			if (s == declkey)return Token(let);      //ʵ���ϡ�let���Ƿ��ء�Token��'L'����, const string declkey = "let";
			return Token{ name,s };                  //��name������Ҫ(Ҳ��ʵ����)���ص��ǡ�Token��'a',��Ʊ��˾������
		}
		error("����get�����Ļ�ȡ���ݵ����෶Χ");
	}
}

