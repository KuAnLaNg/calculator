#include"../../std_lib_facilities.h"
#include"Token.h"


const char number = '8';               //����������Ҫ���õ�
const char quit = 'q';
const char print = ';';
const string prompt = "> ";
const string result = "= ";
//**************��������������Ҫ�õ�
const char name = 'a';                 //������let��nameҲҪ��L��a����������Ϊ���ʵ�t.kind,������Ϊ����֮ǰ��δ�������ʱ�Ĵ��������ǰ�t.kind��������ĵģ������ڴ�����
const char let = 'L';
const string declkey = "let";




Token_stream ts;                      //����һ����Ϊ"ts"���ṩget������putback()���ܵġ�Token_stream ������
									  //********���Ǹ���ǰ��������Ϊ����ĵ�primary��term��expression��������Ҫ��Ϊts��Token_stream�����ṩts.get()����


double expression();                  //********��ǰ���� 
double get_value(string s);           //********��ǰ����,��Ϊprimary�������������


double primary()                      //primary�����������ֵ��ts��get�����ǽ����ݼӸ���ǩ����������һ������primary���Ǹ��ݱ�ǩ�����������ֵ
{

	Token t = ts.get();                //primaryֱ�ӻ�ȡ���ʣ������Ѿ�����С���ˣ�����primaryû����expression�������ֵ������˦��term���������������ֳ���ֵ��������
									   //����term��primary��expressionץȡֵ�ĺ��Ĳ���
	switch (t.kind)                    //��Ȼprimary��Ϊ�������õ��ʵ�ģ�鿴�ƺ�����Ҫ�ṩ���ж�ȡcin��������������������������Ϊֻд�������������Ҫ����Ϊʹ�ü�������һλ������ı���Ϊ�����������Ǹ���ֵ����������û�������� ���ӡ������ˡ����Ż��ڻ��һ����ֵ��������ò�ʹ�ã���term��expression�У�
	{
	case'��':
	{                                  //��switch���岢��ʼ��������Ҫ�����Ƿŵ�һ��������
		double e = expression();       //�����һλ�ı��ʽ���������
		t = ts.get();                  //�����һλ�������鿴�ǲ��ǡ�����
		if (t.kind != ')') error("û�����Ű� ���");
		return e;
	}

	case number:
		return t.value;                //��Ϊ����ֵ������ֻ������ֵ�͹���
	case'-':
		return -primary();             //ֻ����-��ֵ����-�����������ͨ��������"-*"��"-/"���ַǷ�����������������Ŵ��䲻����ͨ��
	case'+':
		return primary();              //ֻ����+��ֵ����+�����������ͨ��
	case name:
		return get_value(t.name);      // **********return the existed variable's value��ʹ���ⲽ����ȡ�Ѿ�����ı�����ֵ**********

	default:
		error("��primary��ʽ�ĵ���");   //"1++2*3;4+5;"��ڶ����ӺŻ�������������ִ���
	}

}



double term()                            //�˳��������������Ƚ��У���primary����ʽ������ˣ�primary_1*primary_2��primary_1/primary_2��������Ϊterm����ȥ������ɵ���һ�������Ǽӡ���������װ��û������ֱ�ӽ���primaryֵ(primary_1)����Ϊterm����ȥ����expression�ټ���ӡ�����
{                                        //*******����Ҫ��ǰ��������Ϊ��136����Ȼʹ����primary ��primary�Ķ������������term����֮ǰ���������ָterm��ʱ������Ѿ���ʶprimary��
	double left = primary();             //��һ����ֵ�Ļ�ȡ��primary������˵termֻ����primary�������ֵ
	Token t = ts.get();                  //��ȡ��һλ���ʣ�һ����Ϊ�˻�ü�����ţ�


	while (1)                            //������Ϊ�����ŵ����� ֮��primary����ļ���ȫ�����while��1��ѭ�������
	{
		switch (t.kind)
		{
		case'*':
			left *= primary();           //��ȡ��һ��primary����ʹ�����primaryȥ�����µ�left
			t = ts.get();                //��ȡ��һ�����ʣ�һ����Ϊ�˻�ü�����ţ�
			break;
		case'/':
		{
			double e = primary();
			if (e == 0) error("divide by zero");               //���ǻ᲻�����0
			left /= e;
			t = ts.get();
			break;
		}
		case'%':
		{
			double d = primary();
			if (d == 0)error("����0��");
			left = fmod(left, d);                   //fmod���� ����ȡģ����
			t = ts.get();
			break;
		}
		default:
			ts.putback(t);
			return left;                 //�������term��ʽ�Ľ��

		}
	}
}


double expression()                   //��������������Ӽ���                                                            
{
	double left = term();             //��һ����ֵ�Ļ�ȡ��term������˵expressionֻ����term�������ֵ
	Token t = ts.get();               //��ȡ��һλ���ʣ�һ����Ϊ�˻�ü�����ţ�


	while (1)                         //������Ϊ�����ŵ����� ֮��term����ļ���ȫ�����while��1��ѭ�������
	{
		switch (t.kind)
		{
		case'+':
			left += term();           //��ȡ��ͬʱҲ�����ˣ���һ��term����ʹ�����termȥ�����µ�left
			t = ts.get();             //��ȡ��һ�����ʣ�һ����Ϊ�˻�ü�����ţ�
			break;
		case'-':
			left -= term();
			t = ts.get();
			break;
		default:
			ts.putback(t);
			return left;              //������

		}
	}
}
/*********************************����Ϊ�������������ޱ���������������*********************************************************/


class Variable {                       //����"����"
public:
	string name;
	double value;
};

vector<Variable>var_table;             //����һ����Ϊvar_table��Varible���vector����������һ�������Ĵ����ռ�

double get_value(string s) {           //1.������������nameΪ�ַ���s��Varible��Ӧ��ֵ 
	for (const Variable x : var_table)
		if (x.name == s)return x.value;
	error("ѹ��û�ҵ�Ҫ���ҵı���������Ҳ�󲻵�����ֵ");
}

void set_value(string s, double d) {  //2.д����
	for ( Variable x : var_table) {
		if (s == x.name) x.value = d;
		return;
	}
	error("�����ڸ�һ�������ڵı�����ֵ");
}

bool is_declared(string var)           //3.1��3�ĸ������ܣ����var�Ƿ��Ѿ���var_table�У�
{
	for (const Variable&v : var_table)if (v.name == var) return true;
	return false;
}

double define_name(string var, double val)          //3.��һ������
{
	if (is_declared(var))error("��������Ѿ����ˣ������ظ�����");
	Variable v = { var,val };
	var_table.push_back(v);
	return val;

}
double declaration() {                            //������let֮��������ʹ������1��2��3���еĶ�����
	Token t = ts.get();                           //�ڻ��let�󣬽�һ����ȥ�����һ������
	if (t.kind != name)error("name expected in declaration");
	string var_name = t.name;

	Token t2 = ts.get();
	if (t2.kind != '=')error("=missing in declaration of", var_name);

	double d = expression();
	define_name(var_name, d);                    
	return d;
}
/*******************����Ϊ��������ľ���ʵ��************************************/
//��������ѡ�����������ǽ��г������㻹�Ǳ������㣩
double statement() {                               //��Ϊ����"��������"�ʹ���"��������"�ķ�������ʹ���������ж�����1��2��3+declaration��������
	Token t = ts.get();
	switch (t.kind) {
	case let:                                      //�������(δ������ı���������<�������>�����)������������ֻ����������Ķ���
		return declaration();
	default:                                       //������(������ı�������<������>�����)
		ts.putback(t);                             
		return expression();                       //��Ȼ���ǵ���Ƴ����ǲ���һ��������Ҫ����ļ���ʽ�������Ҫһ��������������ʽ���׸�����ʽʱ��һ������ĵ�һ�����ʲ���";"��"q"�����������仰����һֱ����������Ҫ������ʽ�ӣ�����"123+23+345;"�е�"123+23+345"��ȫ��������������val���׵�һ���Ե���"123+23+345"�Ľ��
	}
}

/*********************************����Ϊ�������������ޱ���������������**********************************************************/
void clean_up_mess()
{
	ts.ignore(print);
}

void calculate()                                            //���д���ָ����ܵļ��㺯��
{
	while (cin)                                             //��������״̬��cin��״̬��ֻҪ�����������״̬��������ѭ������ ���Լ�ʹһ��ʼ�������κζ��� ��������״̬Ҳ�������;ʵ������Ϊû˵�����ֻ������������룬ֻ�ǡ�while��cin�����������ǡ�int a=0;while��cin>>a��������������д�Ļ�������״̬�ǻ���������
		try {
		cout << prompt;
		Token t = ts.get();                                 //��鵥�ʿ��Ƿ�Ϊ��Ҫ������ʾ�ļ�����ҵ������Ҫ�رռ�����

		while (t.kind == print)t = ts.get();                //�Ե�";"(��Ϊʹ�õ���while�����ѿ��ԳԵ�������";",����"1+2;;;"���";")


		if (t.kind == quit) {
			return;
		};


		ts.putback(t);


		cout << result << statement() << "\n";               //Ϊ�˼�����������ܣ���expression�����ĳ���statement����
		                                                     
	}
	catch (exception&e) {
		cerr << e.what() << '\n';
		clean_up_mess();                                     //������������д������������������Ȼ�����"1++2*3;4+5;"�ǻᵼ�����д��� ���Ǿ�����������Ϻ�Ҳ�������������󣬵���ʱ���л������Ϊ�ڶ����Ӻţ������������˴��Ǹ�calculate��Ĵ������д���Ĳ��֣�����������ݣ�����Ϊ��ʱ197���while��cin�����ɱ��ֿ϶�״̬�����Լ�������
	}
}



int main()
try
{
	calculate();
	keep_window_open();
	return 0;
}
catch (exception&e)                                       //********��׽�������������а����error����
{
	cerr << e.what() << '\n';                             //����������ڡ���ʼִ�С������������½�error�����Ļ������ù������Ӷ���������һ���error�����������⣨���ÿһ���error�����Ļ���һ���Ļ���
	keep_window_open("~~");                               //�����ض��ġ�~~���ſ��Թرմ��ڣ���ô����ԭ����page139ҳ
	return 1;
}
catch (...)
{
	cerr << "exception\n";
	keep_window_open("~~");
	return 2;
}