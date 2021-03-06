#include "pch.h"
#include <iostream>
#include<string>
#include<iomanip>
using namespace std;
struct newdata
{
	char name;
	int a;
};
struct HNode
{
	int weight;
	int parent;
	int Lchild;
	int Rchild;
	bool flag;				//判断是否使用过该节点作为最小值
};
struct HCode
{
	char data;
	string code;
};
class Huffman 
{
private:
	HNode*HTree;
	newdata*data;
	int Height;
	int Wide;
	int *LevelWide;																		//每一层的宽度
	int order;																				//按层序遍历时使用的一个计数器
	bool *ifchild;																		//按层序遍历次序存放每一个节点是否有孩子
	int NumOfline;																		//得到最大宽度的中间变量
public:
	int N;																					//个数
	HCode*HCodeTable;
	void Init(string str);															  //初始化
	void CreatHTree();
	void CreatTable();																  //建立编码表
	string Encoding(int i);													      //编码
	void PrintCode(string);														//将输入的字符编码并输出
	string Decoding(string old);											     //译码
	void PrintInfor();																 //打印信息（图示方式显示权重）
	void SelectMin(int&x, int&y, int a, int b);
	void compreef();																	      //压缩效果
	void loop();																			     //循环解码
	void PrintTreeform();									   						    //打印哈夫曼树的信息
	void PrintTreeCross();															   //横向打印哈夫曼树(主调)
	void _PrintTreeCross(int T, int Layer, int way);						  //横向打印哈夫曼树(实际打印)
	void PrintTreeVertical();														 //竖向打印哈夫曼树(主调)
	void _PrintTreeVertical(int T,int level,int EachWide);			//竖向打印哈夫曼树(实际打印)
	void GetWide(int T, int level);												//获取宽度、按层序储存是否有孩子（主调）
	void _GetWide();																	//获取宽度、按层序储存是否有孩子（实际）
	int GetHeight(int R);																//获取高度
	~Huffman();
};
void Huffman::Init(string str)		//复杂度 O(n)				初始化
{
	int n = 0;
	int num[256] = { 0 };
	for (int i = 0;i < str.length();i++)
		num[(int)str[i]]++;						//统计次数
	for (int i = 0;i < 256;i++)
		if (num[i] != 0)
			n++;											//统计个数
	data = new newdata[n];
	int temp = 0;
	for (int i = 0;i < 256;i++)					//生成所需的映射
	{
		if (num[i] != 0)
		{
			data[temp].name = (char)(i);
			data[temp].a = num[i];
			temp++;
		}
		if (temp == n)
			break;
	}
	N = n;
}
void Huffman::SelectMin(int&x, int&y, int c, int b)		//挑选2个最小值		复杂度O(n)
{
	for (int i = c;i < b;i++)												//寻找一个没用过的
	{
		if (HTree[i].flag)
		{
			x = i;
			break;
		}
	}
	for (int i = c; i < b; i++)											//比较
	{
		if ((HTree[i].flag) && (HTree[i].weight < HTree[x].weight))
			x = i;
	}
	HTree[x].flag = false;												//修改
	for (int i = c;i < b;i++)												//与x相同方法处理
	{
		if (HTree[i].flag)
		{
			y = i;
			break;
		}
	}
	for (int i = c; i < b; i++)
	{
		if ((HTree[i].flag) && (HTree[i].weight < HTree[y].weight))
			y = i;
	}
	HTree[y].flag = false;
}
void Huffman::CreatHTree()		//建立哈夫曼树  p147				复杂度O(n)
{
	HCodeTable = new HCode[N];
	HTree = new HNode[2 * N - 1];
	for (int i = 0; i < N; i++)
	{
		HTree[i].weight = data[i].a;
		HTree[i].Lchild = HTree[i].Rchild = HTree[i].parent = -1;
		HTree[i].flag = true;
		HCodeTable[i].data = data[i].name;
	}
	int x, y;
	for (int i = N; i < 2 * N - 1; i++)
	{
		SelectMin(x, y, 0, i);
		HTree[x].parent = HTree[y].parent = i;
		HTree[i].weight = HTree[x].weight + HTree[y].weight;
		HTree[i].Lchild = x;
		HTree[i].Rchild = y;
		HTree[i].parent = -1;
	}
}
string Huffman::Encoding(int i)				//从下往上递归编码 这样可以避免再将编码反转		复杂度O(n)
{
	string a;
	if (HTree[i].parent == -1)						//终止条件为根节点
	{
		string blank = "";
		return blank;
	}
	if (HTree[HTree[i].parent].Lchild == i)			//如果该点为左孩子，则它的编码等于父节点编码+“0”
		a = Encoding(HTree[i].parent) + "0";
	else
		a = Encoding(HTree[i].parent) + "1";			//如果该点为右孩子，则它的编码等于父节点编码+“1”
	return a;														//返回这个点的编码
}
string Huffman::Decoding(string old)		//复杂度O(mn)
{
	cout << "Your code is:" << endl;
	cout << old << endl;
	cout << "Your decoding string is:" << endl;
	string newstr = "";
	int i = 0;
	while (i < old.length())									//大循环：字符串存在时
	{
		int temp = 2 * N - 2;
		while (HTree[temp].Lchild != -1)				//小循环：是否为叶子节点
		{
			if (old[i] == '0')
				temp = HTree[temp].Lchild;
			else
				temp = HTree[temp].Rchild;
			i++;
		}
		newstr += HCodeTable[temp].data;
	}
	cout << newstr << endl;
	return newstr;
}
void Huffman::CreatTable()				//配合Encoding使用	复杂度O(n)
{
	for (int i = 0;i < N;i++)
		HCodeTable[i].code = Encoding(i);
}
Huffman::~Huffman()				//复杂度O(1)			析构函数
{
	delete []data;
	data = NULL;
	delete []HTree;
	HTree = NULL;
	delete []HCodeTable;					//删除数组  得用delete[]  !!!
	HCodeTable = NULL;
}
void Huffman::PrintCode(string old)				//将输入的字符编码并输出   复杂度O(m*n)
{
	cout << "	" << "The code of your input string is:" << endl;
	for (int i = 0;i < old.length();i++)
	{
		for (int j = 0;j < N;j++)
		{
			if (old[i] == HCodeTable[j].data)
			{
				cout << HCodeTable[j].code;
				break;
			}
		}
	}
	cout << endl;
}
void Huffman::compreef()				//复杂度 O(n)
{
	double Huffef = 0, equalef = 0;
	int equalbit = ceil(log(N) / log(2));							//计算等长编码的每一位大小
	cout <<"equal length compress for one word:  "<< equalbit << endl;
	for (int i = 0;i < N;i++)
	{
		equalef += equalbit * HTree[i].weight;											//计算等长编码的长度
		Huffef += HTree[i].weight*HCodeTable[i].code.length();				//计算Huffman编码的长度
	}
	cout << "equale length compress :   "<<equalef << endl;
	cout << "Haffman compress :   " << Huffef << endl;
	cout << "ratio: " << Huffef/equalef * 100 << "%" << endl;			//计算比率
}
void Huffman::PrintInfor()                                                        //打印树的信息			复杂度O(n)
{
	cout << endl << endl;
	cout << "		" << "Print Information" << endl;
	for (int i = 0; i < N ; i++)
	{
		cout << setw(6)<<"char:  "<<HCodeTable[i].data<<" "<<"weight:"<<setw(5)<<HTree[i].weight<<"  ";
		for (int x = 0;x < HTree[i].weight;x++)						//用可视化的方式显示
			cout << "▓";
		cout<<endl;
	}
	for (int i = 0;i < N;i++)
		cout << "code:    " << setw(6)<<HCodeTable[i].code << "     " << "interpret:    " << HCodeTable[i].data << endl;
}
void Huffman::PrintTreeform()								//以表格方式打印树的子结点、父节点、权重		复杂度O(n)
{
	cout << endl << endl;
	cout << "			Huffman Tree:" <<endl;
	cout << setw(6) << "Node" << setw(8) << "Parent" << setw(12) << "LeftChild" << setw(12) << "RightChild" << setw(10) << "Weight" << endl;
	for (int i = 0;i < 2 * N - 1;i++)
	{
		cout << setw(6) << i;
		cout << setw(8)<<HTree[i].parent;
		cout << setw(12) << HTree[i].Lchild;
		cout << setw(12) << HTree[i].Rchild;
		cout << setw(10) << HTree[i].weight << endl;
	}
	cout << endl << endl;
	this->Height = this->GetHeight(2 * N - 2);
	cout << "	" << "The height of your tree is :		" << this->Height << endl;
	this->_GetWide();
	cout << "	" << "The wide of your tree is :		" << this->Wide << endl;
}
int Huffman::GetHeight(int R)				//课堂ppt		复杂度O(n)
{
	if (R == -1) 
		return 0;
	else
	{
		int m = GetHeight(HTree[R].Lchild);
		int  n = GetHeight(HTree[R].Rchild);
		return m > n ? m + 1 : n + 1;
	}
}
void Huffman::GetWide(int T, int level)			//复杂度O(n)
{
	if (T == -1 || level < 1)				// 空树或层级不合理
			return;
	if (level == 1)
	{
		if (HTree[T].Lchild != -1)
			ifchild[order] = true;				//按层序储存是否有孩子
		order++;
		NumOfline++;								//统计每一层的个数
		return;
	}
	GetWide(HTree[T].Lchild, level - 1);							// 左子树的 level - 1 级
	GetWide(HTree[T].Rchild, level - 1);							// 右子树的 level - 1 级
}
void Huffman::_GetWide()							//复杂度O(n)
{
	LevelWide = new int[Height];
	ifchild = new bool[2 * N - 1];
	order = 0;
	for (int i = 0;i < 2 * N - 1;i++)
		ifchild[i] = false;
	int T = 2 * N - 2;
	int depth = this->Height;
	int i;
	int temp = NumOfline;
	for (i = 1; i <= depth; i++)							//计算每一层的层宽，并修改最大值
	{
		NumOfline = 0;
		GetWide(T, i);
		LevelWide[i - 1] = NumOfline;
		if (NumOfline > temp)
			temp = NumOfline;
	}
	Wide = temp;
}
void Huffman::_PrintTreeVertical(int T, int level,int EachWide)				//复杂度O(n)
{
	if (T == -1 || level < 1)
		return;
	if (level==1)
	{
		cout <<setw(EachWide)<< HTree[T].weight<<"("<<ifchild[order]<<")";
		order++;
		return;
	}
	_PrintTreeVertical(HTree[T].Lchild, level - 1,EachWide);				//逐行递归打印  进入下一行
	_PrintTreeVertical(HTree[T].Rchild, level - 1,EachWide);
}
void Huffman::PrintTreeVertical()				//复杂度O(n)
{
	int T = 2 * N - 2;
	int depth = this->Height;
	int i;
	int j = 0;
	order = 0;
	cout << endl << endl << endl;
	cout << "This is your tree in vertical way:" << endl;
	cout << "(the numer '0' after the weight means no child,	number '1' means 2 children)" << endl << endl;
	for (i = 1; i <= depth; i++)
	{
		int EachWide = (Wide * 2 * 4) / (LevelWide[i - 1]+1);					//计算每一层的层宽
		_PrintTreeVertical(T, i,EachWide);
		cout << endl;
		int time = 0;
		for (int x = 0;x < LevelWide[i - 1];x++)
		{
			if (ifchild[j] == true)
			{
				cout << setw(EachWide + 3 + time) << "╭  ╮";						//如果有孩子 在打印了一行之后打印分支符号
				time++;
			}
			else if(ifchild[j]==false)
				cout << setw(EachWide + 4) << "   ";										//如果没有孩子则打印空格
			j++;
		}
		cout << endl;
	}
}
void Huffman::PrintTreeCross()					//复杂度O(1)
{
	cout << endl<<endl << "Your tree in crossed way:" << endl<<endl;
	_PrintTreeCross(2 * N - 2, 0, 0);
}
void Huffman::_PrintTreeCross(int T, int Layer,int way)						//按竖向树状打印的二叉树		复杂度O(n)
{
	int i;
	if (T ==-1)
		return;
	_PrintTreeCross(HTree[T].Rchild, Layer + 1,2);
	for (i = 0;i < Layer;i++)
		cout << "   ";
	if (way == 0)
		cout << HTree[T].weight << endl;
	if (way == 1)
		cout << "╰  " << HTree[T].weight << endl;
	if(way==2)
		cout <<"╭  "<< HTree[T].weight << endl;					//按LR顺序调用，最后会左中右输出结点，用层深决定结点的左右位置
	_PrintTreeCross(HTree[T].Lchild, Layer + 1,1);					//递归调用下一行
}
void Huffman:: loop()				//复杂度O(1)
{
	while (1)
	{
		int num;
		cout << "		" << "1.Decoding a code" << endl;
		cout << "		" << "2.Quit" << endl;
		cout << "		" << "Please input the numer before the conduction" << endl;
		cin >> num;
		if (num == 1)
		{
			string code;
			cin >> code;
			this->Decoding(code);
		}
		if (num == 2)
			return;
	}
}
int main()
{
	cout << endl << endl << endl << endl << endl << endl << endl;
	cout <<"		"<< "##     ##    ###    ######## ######## ##     ##    ###    ##    ## " << endl;
	cout << "		" << "##     ##   ## ##   ##       ##       ###   ###   ## ##   ###   ## " << endl;
	cout << "		" << "##     ##  ##   ##  ##       ##       #### ####  ##   ##  ####  ##" << endl;
	cout << "		" << "######### ##     ## ######   ######   ## ### ## ##     ## ## ## ##" << endl;
	cout << "		" << "##     ## ######### ##       ##       ##     ## ######### ##  #### " << endl;
	cout << "		" << "##     ## ##     ## ##       ##       ##     ## ##     ## ##   ### " << endl;
	cout << "		" << "##     ## ##     ## ##       ##       ##     ## ##     ## ##    ## " << endl;
	cout << endl << endl;
	cout << "		" << "1.Creat a Huffman Tree" << endl;
	cout << "		" << "2.Quit" << endl;
	cout << "		" << "Please input the numer before the conduction" << endl;
	int num;
	cin >> num;
	if (num == 1)
	{
		cout << "		" << "1.Use the sample" << endl;
		cout << "		" << "2.Input your string" << endl;
		cout << "		" << "Please input the numer before the conduction" << endl;
		cin >> num;
		if (num == 1)
		{
			Huffman sample;
			string test = "I love data Structure, I love Computer.I will try my best to study data Structure.";
			sample.Init(test);
			sample.CreatHTree();
			sample.CreatTable();
			sample.PrintInfor();
			sample.PrintTreeform();
			sample.PrintTreeCross();
			sample.PrintTreeVertical();
			sample.compreef();
			sample.PrintCode(test);
			sample.Decoding("01111111000101001111011011101001010100011001");
		}
		if (num == 2)
		{
			cout << "		" << "Please input a string:" << endl;
			string str;
			cin >> str;
			Huffman job;
			job.Init(str);
			job.CreatHTree();
			job.CreatTable();
			job.PrintInfor();
			job.PrintTreeform();
			job.PrintTreeCross();
			job.PrintTreeVertical();
			job.compreef();
			job.PrintCode(str);
			job.loop();
		}
	}
	if (num == 2)
	{
		cout << "Thanks for using!" << endl;
		return 0;
	}
	cout << "Thanks for using!" << endl;
	return 0;
}