#pragma once
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include<algorithm>
#include <cmath>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>


#define result_len 1024

class matrix
{
private:
	
public:
	std::vector<struct rownum> _matrix;//储存客户端发过来的矩阵
	std::vector<std::vector<double>> matrix_cache;	//上一次进行数组计算后的结果缓存
	//在程序里进行计算的是函数内部的矩阵，在该矩阵计算完毕后赋值给矩阵缓存当作上一次计算结果。
	//不直接使用该矩阵进行计算是为了防止其他线程读缓存矩阵时因为缓存矩阵被使用而读到错误。
	//但系统设计是每一个用户连接都分配一个matrix对象，所以该设计目前没用

	matrix();
	matrix(const char* buff);
	~matrix();

	//传入的字符串参数:第一个数字为行数，第二个数字为列数，剩下数字从行开始传入,数字间用|隔开，最后一个数字后也要有|
	//例如row|col|A11|A12|A13|......|A21|.....|Ann|		(Aij代表i行j列)
	bool getmatrix(const char* buff);

	const char* matrix_multip();	//是否可以进行矩阵相乘由客户端判断，这里失败返回failed;
	const char* matrix_add();		//矩阵加法，如果容器内矩阵的行和列不符合条件会返回failed;
	const char* matrix_echelon();		//将矩阵化简成阶梯型	(vector容器里的第一个矩阵)
	const char* matrix_transpose();	//返回转置矩阵

	int matrix_dimensions();	//返回矩阵的空间维数
	double matrix_area();	//返回矩阵构成空间的面积
	const char* matrix_rank_vectors();	//返回矩阵rank中的向量
	const char* matrix_rank(int mul_count=1);	//返回矩阵的特征值
	const char* linearRelation();

	//bool 
};

struct rownum
{
	double** array;
	int col;
	int row;
};



void Eig();


char* itoa(int value, char* str, int base);