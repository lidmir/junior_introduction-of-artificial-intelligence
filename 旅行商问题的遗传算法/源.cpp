//������������Ŵ��㷨
//���䷽����1�����������Ϊ���ŵĽ����·�����Ƚ϶̣�    2�����潻�䷨�����ɵ�·�����Ƚϳ���
//���췽�������ڴ���ı������

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
  
const int CityNum = 99; //���еĸ��� 
const double PC = 0.9; //�������  
double PM = 0.1; //�������  
double PS = 0.6;//��������  
int MaxGen = 50; //������  
const int UnitNum = 5000; //Ⱥ���ģΪ50  
double length_table[CityNum][CityNum];//��¼ÿ��������֮��ľ���
  
//����  
struct node  
{  
    int num;//���еı��  
    int x;//������  
    int y;//������  
  
}nodes[CityNum];  

struct unit  
{  
    double length;//�ܳ���  
    int path[CityNum];//·��   
    bool operator < ( const struct unit &other) const //����Ⱥ�������  
    {  
        return length < other.length;  
    }  
};  
   
//���е���Ⱥ
struct unit group[UnitNum];  
//�������ٵ���Ⱥ
unit best = {INT_MAX, {0} };  

//**��Ҫ��������**
//���ļ�����¼�������꣬ͬʱ����һ����ά�����¼ÿ��������֮��ľ��� 
void Initialization();     
//���������Ⱥ��ģ��������Ⱥ 
void Initial_group( unit group[]);  
  //��Ⱥ���������䣬����  
void Evolution_group(unit group[]);  
 
int main(int argc, char* argv[])  
{  
    srand((unsigned)time(NULL));  
	//���ļ�����¼�������꣬ͬʱ����һ����ά�����¼ÿ��������֮��ľ���
    Initialization();    
    //���������Ⱥ��ģ��������Ⱥ
    Initial_group( group );  
    //��Ⱥ������ѡ�񣬽��䣬����  
    Evolution_group( group );  
	//��ӡ
	cout << "�������PC = " << PC << endl;  
    cout << "�������PM = " << PM << endl;  
    cout << "��������PS = " << PS << endl;  
    cout << "������ �� " << MaxGen << endl;  
    cout << "Ⱥ���ģ �� " << UnitNum  << endl;  
	cout << "�����ǣ� = " << best.length << endl;  
    cout << "·���ǣ�";  
    copy(best.path,best.path + CityNum, ostream_iterator<int>(cout, " -> "));  
    cout <<best.path[0] << endl;   
}  
  
//���ļ�����¼�������꣬ͬʱ����һ����ά�����¼ÿ��������֮��ľ���  
void Initialization()  
{  
    int i, j;    
    ifstream in("source.txt");    
    for (i = 0; i < CityNum; i++)    //���ļ�
    {  
        in >> nodes[i].num >> nodes[i].x >> nodes[i].y;  
    }    
    for (i = 0; i < CityNum; i++)   //��ÿ��������֮��ľ��� �ŵ���ά����length_table��
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

//****************���������Ⱥ��ģ��������Ⱥ����ʼ����Ⱥ*****************
//�������󣬸�generate ����  *�������������ú����޲������ҷ���ֵ�ܹ�ת���ɵ�������ָ���ֵ������
class GeneratorF 
{  
    public:  
        GeneratorF (int _seed = 0): seed(_seed) {}    
        int operator()() {  return seed += 1; }  
    private:  
        int seed;  
}; 
//����һ��Ⱥ����Ҫ�Ĵ��ۣ��������߹���·�������г��еľ��룩  
void CalCulate_length(unit &temp)  
{  
    int j = 0;   
    temp.length = 0;    
    for (j = 1; j < CityNum; j++)   
        temp.length += length_table[ temp.path[j-1] ][ temp.path[j] ];      
    temp.length += length_table[ temp.path[CityNum - 1] ][temp.path[0] ];  //���һ�����е���� 
}  
//���������Ⱥ��ģ��������Ⱥ
void Initial_group( unit group[])  
{  
    int i;  
    unit temp;     
    //generate��˳���������ΪCityNum�����в��洢������ 
    generate(temp.path, temp.path + CityNum, GeneratorF());     
    // ������Ⱥ 
    for (i = 0; i < UnitNum; i++)   
    {  
        //random_shuffle��һ��Ԫ�����н���������� 
        random_shuffle(temp.path, temp.path + CityNum);  
        memcpy(&group[i], &temp, sizeof(temp));    //�����ɵ�����Ⱥ�ŵ����������Ⱥ������group��
        CalCulate_length(group[i]);       //��������Ⱥ�Ĵ���
    }  
} 
//********************************

//++++++++++++++++++��Ⱥ������ѡ�񣬽��䣬����+++++++++++++++

//���ڴ���ı��죬�����ѡ��һ��Ⱥ�壬Ȼ�����ѡ�������㣬�������ǵ�λ��  
void VarationGroup(unit group[])     
{  
    int i, j, k;  
    double temp;  //��ʱ���������ڽ���
    //���������������Ⱥ���еĸ�����PM�ĸ��ʱ���,������ʲ���̫��  
    double num = UnitNum * PM;  //������� 
    while (num--)   
    {   
        //ȷ�������������Ⱥ  
        k = rand() % UnitNum;    
        //ȷ�����������λ  
        i = rand() % CityNum;  
        j = rand() % CityNum;   
        //���� 
        temp  = group[k].path[i];  
        group[k].path[i] = group[k].path[j];   
        group[k].path[j] = temp;  
		//���¼�����Ⱥ�Ĵ���
        CalCulate_length(group[k]);  
    }  
}  
 //*****************����***********************
//����id ��������У� �ڵ�ǰ���е�λ�ã�����������Ľ��� 
int SearchCity( int path[], int len, int city)     //û�����city����-1,�з���i
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
  
//*******54321��32154*****   
//����m=3 ֻ�ı���㣬�������´ν����ʱ��
//ԭ����������ͬ���ۡ���ͬ·���Ľ��ܺ������⽻�����ͬ�Ľ�����������ҵ����õĽ�
void Reverse(int path[], int b, int e)  //����·��
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
//���䷽�������߽���õ����ŵĽ����������С���Ҷ��ߵ��Ӵ�����һ�£���ͷ��ͬ
//���ν������У��õ�������С��·��
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

//void Cross_group( unit &p, unit &q)   //���潻�䷨
//{
//	srand((unsigned)time(0));
//	unit son1;
//	unit son2;
//	memcpy(&son1,&p,  sizeof(unit)); 
//	memcpy(&son2,&q, sizeof(unit)); 
//	//generate(son1.path, son1.path + CityNum, GeneratorF()); 
//	//generate(son2.path, son2.path + CityNum, GeneratorF());
//	bool flag1[CityNum];       //��־�Ƿ����
//	bool flag2[CityNum];
//	for(int j=0;j<CityNum;j++)  //��־λ��ʼ��0
//	{
//		flag1[j]=false;
//		flag2[j]=false;
//	}
//	int Position=rand()%CityNum;     //����λ
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
//�������ڱ�������ִ��Ⱥ���и���Ľ���ͱ���  
void Evolution_group(unit group[])  
{  
    int j;  
    int n = MaxGen;  
    int num1, num2;  
    //��PS �ĸ���ѡ��ǰ num2 ���⣬��������num1 ���⡣  
    num1 = UnitNum * ( 1-PS);  
    num2 = UnitNum * PS;  
  
    //�������Σ������ܶ��ٴ�  
    while (n-- ) //ѭ��GEN-MAX��  
    {  
		srand((unsigned)time(0));
        //ѡ�񲿷��������Ⱥ  
        sort(group, group + UnitNum);  
        if (group[0].length < best.length)   
            memcpy(&best, &group[0], sizeof(unit));  
        for (j = 0; j <=  num1 - 1; j++)    //��num1����,��num2�Ľ�ȥ���� 
            memcpy(&group[num2 + j], &group[j], sizeof(unit));  
        //���� 
        for (j = 0; j < UnitNum / 2; j+= 1)   //��ͷ�ͽ�β���ν���
		{
			srand((unsigned)time(0));
			double r=rand()%10000*0.0001;
			if(r<PC)       
				Cross_group(group[j], group[ UnitNum - j -1]);  
		}
        //����  
        VarationGroup(group);  
    }  
    //����������õĽ�  
    sort(group, group + UnitNum);  
    if (group[0].length < best.length)   
    {  
        memcpy(&best, &group[0], sizeof(unit));  
    }  
}  
//*******************