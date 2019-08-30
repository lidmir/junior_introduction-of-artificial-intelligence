//旅行商问题的遗传算法
//交配方法：1、父代交配后为最优的结果（路径长度较短）    2、常规交配法（生成的路径长度较长）
//变异方法：基于次序的变异规则

#include <iostream>  
#include <string.h>  
#include <fstream>  
#include <iterator>  
#include <algorithm>  
#include <limits.h>  
#include <math.h>  
#include <stdlib.h>  
#include<time.h> 
using namespace std;  
  
const int CityNum = 99; //城市的个数 
const double PC = 0.9; //交配概率  
double PM = 0.1; //变异概率  
double PS = 0.6;//保留概率  
int MaxGen = 50; //最大代数  
const int UnitNum = 5000; //群体规模为50  
double length_table[CityNum][CityNum];//记录每两个城市之间的距离
  
//城市  
struct node  
{  
    int num;//城市的编号  
    int x;//横坐标  
    int y;//纵坐标  
  
}nodes[CityNum];  

struct unit  
{  
    double length;//总长度  
    int path[CityNum];//路径   
    bool operator < ( const struct unit &other) const //用于群体的排序  
    {  
        return length < other.length;  
    }  
};  
   
//所有的种群
struct unit group[UnitNum];  
//代价最少的种群
unit best = {INT_MAX, {0} };  

//**主要函数声明**
//打开文件，记录城市坐标，同时产生一个二维数组记录每两个城市之间的距离 
void Initialization();     
//随机产生种群规模个数的种群 
void Initial_group( unit group[]);  
  //种群进化：交配，变异  
void Evolution_group(unit group[]);  
 
int main(int argc, char* argv[])  
{  
    srand((unsigned)time(NULL));  
	//打开文件，记录城市坐标，同时产生一个二维数组记录每两个城市之间的距离
    Initialization();    
    //随机产生种群规模个数的种群
    Initial_group( group );  
    //种群进化：选择，交配，变异  
    Evolution_group( group );  
	//打印
	cout << "交配概率PC = " << PC << endl;  
    cout << "变异概率PM = " << PM << endl;  
    cout << "保留概率PS = " << PS << endl;  
    cout << "最大代数 ＝ " << MaxGen << endl;  
    cout << "群体规模 ＝ " << UnitNum  << endl;  
	cout << "代价是： = " << best.length << endl;  
    cout << "路径是：";  
    copy(best.path,best.path + CityNum, ostream_iterator<int>(cout, " -> "));  
    cout <<best.path[0] << endl;   
}  
  
//打开文件，记录城市坐标，同时产生一个二维数组记录每两个城市之间的距离  
void Initialization()  
{  
    int i, j;    
    ifstream in("source.txt");    
    for (i = 0; i < CityNum; i++)    //读文件
    {  
        in >> nodes[i].num >> nodes[i].x >> nodes[i].y;  
    }    
    for (i = 0; i < CityNum; i++)   //将每两个城市之间的距离 放到二维数组length_table里
    {  
        length_table[i][i] = (double)INT_MAX;  
        for (j = i + 1; j < CityNum; j++)  
        {  
            length_table [i][j] = length_table[j][i] =sqrt(   
                    (nodes[i].x - nodes[j].x) * (nodes[i].x - nodes[j].x) +  
                    (nodes[i].y - nodes[j].y) * (nodes[i].y - nodes[j].y) );  
        }    
    }    
}  

//****************随机产生种群规模个数的种群，初始化种群*****************
//函数对象，给generate 调用  *生成器函数，该函数无参数，且返回值能够转换成迭代器所指向的值的类型
class GeneratorF 
{  
    public:  
        GeneratorF (int _seed = 0): seed(_seed) {}    
        int operator()() {  return seed += 1; }  
    private:  
        int seed;  
}; 
//计算一个群组需要的代价（旅行商走过的路线上所有城市的距离）  
void CalCulate_length(unit &temp)  
{  
    int j = 0;   
    temp.length = 0;    
    for (j = 1; j < CityNum; j++)   
        temp.length += length_table[ temp.path[j-1] ][ temp.path[j] ];      
    temp.length += length_table[ temp.path[CityNum - 1] ][temp.path[0] ];  //最后一个城市到起点 
}  
//随机产生种群规模个数的种群
void Initial_group( unit group[])  
{  
    int i;  
    unit temp;     
    //generate按顺序产生长度为CityNum的序列并存储至容器 
    generate(temp.path, temp.path + CityNum, GeneratorF());     
    // 产生种群 
    for (i = 0; i < UnitNum; i++)   
    {  
        //random_shuffle对一个元素序列进行随机排序 
        random_shuffle(temp.path, temp.path + CityNum);  
        memcpy(&group[i], &temp, sizeof(temp));    //把生成的新种群放到存放所有种群的数组group中
        CalCulate_length(group[i]);       //计算新种群的代价
    }  
} 
//********************************

//++++++++++++++++++种群进化：选择，交配，变异+++++++++++++++

//基于次序的变异，随机的选择一个群体，然后随机选择两个点，交换它们的位置  
void VarationGroup(unit group[])     
{  
    int i, j, k;  
    double temp;  //临时变量，用于交换
    //变异的数量，即，群体中的个体以PM的概率变异,变异概率不宜太大  
    double num = UnitNum * PM;  //变异次数 
    while (num--)   
    {   
        //确定发生变异的种群  
        k = rand() % UnitNum;    
        //确定发生变异的位  
        i = rand() % CityNum;  
        j = rand() % CityNum;   
        //交换 
        temp  = group[k].path[i];  
        group[k].path[i] = group[k].path[j];   
        group[k].path[j] = temp;  
		//重新计算种群的代价
        CalCulate_length(group[k]);  
    }  
}  
 //*****************交配***********************
//查找id （代表城市） 在当前解中的位置，用于两个解的交配 
int SearchCity( int path[], int len, int city)     //没有这个city返回-1,有返回i
{  
    if (city <= 0 || city > CityNum)   
    {  
        cout << "city outfiled, city = " << city << endl;  
        return -1;  
    }  
    int i = 0;  
    for (i = 0; i < len; i++)   
        if (path[i] == city)   
            return i;  
    return -1;  
}  
  
//*******54321→32154*****   
//其中m=3 只改变起点，有利于下次交叉的时候
//原来的两个相同代价、不同路径的解能和其他解交叉出不同的结果，有利于找到更好的解
void Reverse(int path[], int b, int e)  //交换路径
{  
    int temp;  
  
    while (b < e)   
    {  
        temp = path[b];   
        path[b] = path[e];  
        path[e] = temp;  
  
        b++;  
        e--;  
    }  
}  
void Rotate(int path[],int len, int m)  
{  
    if( m < 0 )  
		return;  
    if (m > len)    
        m %= len;  
    Reverse(path, 0, m -1);  
    Reverse(path, m, len -1);  
    Reverse(path, 0, len -1);  
}  
 //******************
//交配方法：二者交配得到最优的结果（代价最小）且二者的子代代价一致，开头不同
//依次交换城市，得到代价最小的路径
void Cross_group( unit &p, unit &q)  
{  
    int i = 0;  
    int pos1, pos2;  
	int len = CityNum;  
    int first;  
  
    double len1 = length_table[p.path[0] ][ p.path[1] ];  
    double len2 = length_table[q.path[0] ][ q.path[1] ];  
  
    if (len1 <= len2)   
    {  
        first = p.path[0];  
    }  
    else  
    {  
        first = q.path[0];  
    }  

    pos1 = SearchCity( p.path + i, len, first);  
    pos2 = SearchCity( q.path + i, len, first);  
  
    Rotate(p.path + i, len, pos1);  
    Rotate(q.path + i, len, pos2);  
  
    while ( --len > 1)   
    {  
        i++;  
        double span1  = length_table[ p.path[i - 1] ][ p.path[i] ];  
        double span2  = length_table[ q.path[i - 1] ][ q.path[i] ];  
        if ( span1 <= span2 )  
        {  
            pos2 = SearchCity( q.path + i, len, p.path[i]);  
            Rotate(q.path + i, len, pos2);  
        }  
        else  
        {  
            pos1 = SearchCity( p.path + i, len, q.path[i]);  
            Rotate(p.path + i, len, pos1);  
        }  
    }   
	Rotate(q.path, CityNum, rand() % CityNum);    
    CalCulate_length(p);  
    CalCulate_length(q);  
}  

//void Cross_group( unit &p, unit &q)   //常规交配法
//{
//	srand((unsigned)time(0));
//	unit son1;
//	unit son2;
//	memcpy(&son1,&p,  sizeof(unit)); 
//	memcpy(&son2,&q, sizeof(unit)); 
//	//generate(son1.path, son1.path + CityNum, GeneratorF()); 
//	//generate(son2.path, son2.path + CityNum, GeneratorF());
//	bool flag1[CityNum];       //标志是否出现
//	bool flag2[CityNum];
//	for(int j=0;j<CityNum;j++)  //标志位初始化0
//	{
//		flag1[j]=false;
//		flag2[j]=false;
//	}
//	int Position=rand()%CityNum;     //交配位
//	for(int j=0;j<=Position;j++)
//	{
//		//son1.path[j]=p.path[j];
//		//son2.path[j]=q.path[j];
//		flag1[p.path[j]-1]=true;
//		flag2[q.path[j]-1]=true;
//	}
//	for(int j=0,k=1,l=1;j<CityNum;j++)
//	{
//		if(!flag1[q.path[j]-1]) {son1.path[Position+k]=q.path[j];k++;}
//		if(!flag2[p.path[j]-1]) {son2.path[Position+l]=p.path[j];l++;}
//	}
//	CalCulate_length(son1);
//	CalCulate_length(son2);
//	memcpy(&p, &son1, sizeof(unit)); 
//	memcpy(&q, &son2, sizeof(unit)); 
//}
//*******************************************************************
//进化，在本函数中执行群体中个体的交配和变异  
void Evolution_group(unit group[])  
{  
    int j;  
    int n = MaxGen;  
    int num1, num2;  
    //以PS 的概率选择前 num2 个解，抛弃其后的num1 个解。  
    num1 = UnitNum * ( 1-PS);  
    num2 = UnitNum * PS;  
  
    //迭代几次，即繁衍多少代  
    while (n-- ) //循环GEN-MAX次  
    {  
		srand((unsigned)time(0));
        //选择部分优秀的种群  
        sort(group, group + UnitNum);  
        if (group[0].length < best.length)   
            memcpy(&best, &group[0], sizeof(unit));  
        for (j = 0; j <=  num1 - 1; j++)    //后num1个解,用num2的解去覆盖 
            memcpy(&group[num2 + j], &group[j], sizeof(unit));  
        //交配 
        for (j = 0; j < UnitNum / 2; j+= 1)   //开头和结尾依次交配
		{
			srand((unsigned)time(0));
			double r=rand()%10000*0.0001;
			if(r<PC)       
				Cross_group(group[j], group[ UnitNum - j -1]);  
		}
        //变异  
        VarationGroup(group);  
    }  
    //保存已找最好的解  
    sort(group, group + UnitNum);  
    if (group[0].length < best.length)   
    {  
        memcpy(&best, &group[0], sizeof(unit));  
    }  
}  
//*******************