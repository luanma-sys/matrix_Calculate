#include "mymatrix.h"

matrix::matrix()
{
}

matrix::matrix(const char* buff)
{
	getmatrix(buff);
}

matrix::~matrix()
{
	for (auto it:_matrix)
	{
		for (int i = 0; i < it.row; i++) { delete[] it.array[i]; }
	}
}

bool matrix::getmatrix(const char* buff)//该函数最后字符为空格
{
	int l_row, r_row, l_col, r_col;
	char* buffer = (char*)buff;
	l_row = r_row = l_col = r_col = 0;
	if (strlen(buff)==0)	return false;

	while (buffer[0] != 0)
	{
		//if (buffer[0] == '|') { buffer = buffer + 1; continue; }
		int row = 0, col = 0;	char number[31];	char* end;

		end = strstr(buffer, "|");	if (end == 0)	return false;
		strncpy(number, buffer, end - buffer);	number[end - buffer] = 0;
		row = atoi(number);	buffer = end + 1;

		end = strstr(buffer, "|");	if (end == 0)	return false;
		strncpy(number, buffer, end - buffer);	number[end - buffer] = 0;
		col = atoi(number);	buffer = end + 1;

		struct rownum element;
		element.array = new double* [row];	element.row = row;	element.col = col;
		for (int ridx = 0; ridx < row; ridx++)
		{
			element.array[ridx] = new double[col];
			for (int cidx = 0; cidx < col; cidx++)
			{
				end = strstr(buffer, "|");	if (end == 0)	return false;
				strncpy(number, buffer, end - buffer);	number[end - buffer] = 0;
				element.array[ridx][cidx] = atof(number);	buffer = end + 1;
			}
		}
		_matrix.push_back(element);
	}
	return true;
}

const char* matrix::matrix_multip()
{
	char result[result_len];	memset(result, 0, result_len);
	std::vector<std::vector<double>> transfer;	//矩阵运算主对象
	int row= _matrix[0].row,  col=_matrix[0].col;

	for (int i = 0; i < _matrix[0].row; i++)
	{
		std::vector<double> temp;
		for (int j = 0; j < _matrix[0].col; j++)		{temp.push_back(_matrix[0].array[i][j]);}
		transfer.push_back(temp);
	}

	for (int idx = 1; idx < _matrix.size(); idx++)
	{
		if (col != _matrix[idx].row)		return (const char*)"execute matrix_multip() failed";

		std::vector<std::vector<double>> temp(transfer);
		for (int i = 0; i < row; i++)//清空主矩阵，不让里面的每一代原住民运算
		{
			for (int j = 0; j < col; j++)
			{
				transfer[i][j] = 0;
			}
		}
		col = _matrix[idx].col;	//运算后矩阵主对象的列数等于要处理矩阵的列数
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < _matrix[idx].col; j++)
			{	//运算时矩阵主对象的列数等于要处理矩阵的行数
				for (int k = 0; k < _matrix[idx].row; k++) { transfer[i][j] += temp[i][k] * _matrix[idx].array[k][j]; }//矩阵相乘公式
			}
		}
	}

	matrix_cache.assign(transfer.begin(), transfer.end());
	char temp[128];
	memset(temp, 0, 128);	itoa(row, temp, 10);	strcat(result, temp);	strcat(result, " ");
	memset(temp, 0, 128);	itoa(col, temp, 10);	strcat(result, temp);	strcat(result, " ");

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			memset(temp, 0, 128);
			sprintf(temp, "%g", transfer[i][j]);
			strcat(result, temp);		strcat(result, " ");
		}
	}
	strcat(result, "\0");
	return result;
}

const char* matrix::matrix_add()
{
	char result[result_len];	memset(result, 0, result_len);
	int row = _matrix[0].row, col = _matrix[0].col;
	for (auto it : _matrix)
	{
		if (row != it.row || col != it.col)	return (const char*)"execute matrix_add() failed";
	}

	std::vector<std::vector<double>> transfer;
	for (int i = 0; i < row; i++)
	{
		std::vector<double> temp;
		for (int j = 0; j < col; j++) { temp.push_back(0); }
		transfer.push_back(temp);
	}

	for (auto it : _matrix)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++) { transfer[i][j] += it.array[i][j]; }
		}
	}

	matrix_cache.assign(transfer.begin(), transfer.end());
	char temp[128];
	memset(temp, 0, 128);	itoa(row, temp, 10);	strcat(result, temp);	strcat(result, " ");
	memset(temp, 0, 128);	itoa(col, temp, 10);	strcat(result, temp);	strcat(result, " ");

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			memset(temp, 0, 128);
			sprintf(temp, "%g", transfer[i][j]);
			strcat(result, temp);		strcat(result, " ");
		}
	}
	strcat(result, "\0");
	return result;
}

const char* matrix::matrix_echelon()
{
	char result[result_len];	memset(result, 0, result_len);
	int row = _matrix[0].row, col = _matrix[0].col;
	std::vector<std::vector<double>> transfer;

	for (int i = 0; i < row; i++)
	{
		std::vector<double> temp;
		for (int j = 0; j < col; j++) { temp.push_back(_matrix[0].array[i][j]); }
		transfer.push_back(temp);
	}

	//化成阶梯型矩阵，但不进行缩放，因为这会影响特征值和矩阵的空间大小
	for (int i = 0; i < row; i++)	//遍历每一行，将碰到的首个非零数当作秩
	{
		int rankidx = -1;
		for (int j = 0; j < col; j++) 
		{ 
			if (transfer[i][j] != 0) { rankidx = j; break; }
		}
		if (rankidx == -1)	continue;	//如果没找到就说明该行全零，跳到下一行

		for (int ii = i+1; ii < row; ii++)	//目前遍历这行不受影响，要处理的是该行下面的行
		{
			double div = transfer[ii][rankidx] / transfer[i][rankidx];
			for (int jj = rankidx; jj < col; jj++)	
			{ transfer[ii][jj] -= transfer[i][jj] * div; }	//消去该行第一个非零元素
		}
	}
	//对vector里的vector数组进行排序(排的是数组不是值)	按vector里从第一列开始有多少有多少个连续的0进行排序
	std::sort(transfer.begin(), transfer.end(),		
		[=](const std::vector<double>& v1, const std::vector<double>& v2)->bool
		{for (int i = 0; i < v1.size(); i++)
		{
		if (v1[i] == 0 && v2[i] == 0)	continue;	//如果该列都为零跳过
		if (v1[i] != 0 || v2[i] != 0)	return abs(v1[i]) > abs(v2[i]);	//如果该列为RANK A则进行排序
		}
		return v1[0]>v2[0];	//如果这两列都全是零则返回0比较0
		});

	matrix_cache.assign(transfer.begin(), transfer.end());
	char temp[128];
	memset(temp, 0, 128);	itoa(row, temp, 10);	strcat(result, temp);	strcat(result, " ");
	memset(temp, 0, 128);	itoa(col, temp, 10);	strcat(result, temp);	strcat(result, " ");

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			memset(temp, 0, 128);	sprintf(temp, "%g", transfer[i][j]);
			strcat(result, temp);		strcat(result, " ");
		}
	}
	strcat(result, "\0");
	return result;
}


const char* matrix::matrix_transpose()
{
	char result[result_len];	memset(result, 0, result_len);
	int row = _matrix[0].col, col=_matrix[0].row;

	char temp[128];
	memset(temp, 0, 128);	itoa(row, temp, 10);	strcat(result, temp);		strcat(result, " ");
	memset(temp, 0, 128);	itoa(col, temp, 10);	strcat(result, temp);		strcat(result, " ");

	for (int j = 0; j < col; j++)
	{
		for (int i = 0; i < row; i++)
		{
			memset(temp, 0, 128);	sprintf(temp, "%g", _matrix[0].array[i][j]);
			strcat(result, temp);		strcat(result, " ");
		}
	}
	strcat(result, "\0");
	return result;
}

int matrix::matrix_dimensions()
{
	int rank = 0;
	if(matrix_cache.empty())	matrix_echelon();

	for (auto vectors : matrix_cache)
	{
		for (auto matrix_value : vectors)
		{
			if (matrix_value != 0) { rank++; break; }//矩阵已化成阶梯型，根据阶梯型矩阵特征计算rank
		}
	}
	return rank;
}

double matrix::matrix_area()
{
	if (matrix_cache.empty())	matrix_echelon();

	int row = matrix_cache.size()-1, col = matrix_cache[0].size()-1;
	if (row != col)	return 0;
	if (matrix_cache[row][col] == 0)	return 0;

	int area = matrix_cache[0][0];
	for (int i = 1; i <= row; i++) { area *= matrix_cache[i][i]; }
	return area;
}

const char* matrix::matrix_rank_vectors()
{
	char result[result_len];	memset(result, 0, result_len);
	char temp[128];	memset(temp, 0, 128);
	if (_matrix[0].row != _matrix[0].col) return (const char*)"matrix_rank_vectors() execute failed, because matrix is not square";

	memset(temp, 0, 128);	sprintf(temp, "%d", _matrix[0].row);
	strcat(result, temp);	strcat(result, " ");
	memset(temp, 0, 128);	sprintf(temp, "%d", _matrix[0].col);
	strcat(result, temp);	strcat(result, " ");

	Eigen::MatrixXd A(_matrix[0].row, _matrix[0].col);
	for (int m_row = 0; m_row < _matrix[0].row; m_row++)
	{
		for (int m_col = 0; m_col < _matrix[0].col; m_col++)
		{
			A(m_row, m_col) = _matrix[0].array[m_row][m_col];
		}
	}

	Eigen::EigenSolver<Eigen::MatrixXd> es(A);
	if (es.info() != Eigen::Success)//如果不满足特征值分解条件会中止进程
		abort();

	Eigen::MatrixXd V = es.pseudoEigenvectors();
	for (long long m_row = 0; m_row < _matrix[0].row; m_row++)
	{
		for (long long m_col = 0; m_col < _matrix[0].col; m_col++)
		{
			memset(temp, 0, 128);	sprintf(temp, "%.2f", V((long)_matrix[0].row * m_row + m_col));
			strcat(result, temp);	strcat(result, " ");
		}
	}
	strcat(result, "\0");
	return result;
}

const char* matrix::matrix_rank(int mul_count)
{
	char result[result_len];	memset(result, 0, result_len);
	char temp[128];	memset(temp, 0, 128);
	if (_matrix[0].row != _matrix[0].col) return (const char*)"matrix_rank() execute failed, because matrix is not square";

	Eigen::MatrixXd A(_matrix[0].row, _matrix[0].col);
	for (int m_row = 0; m_row < _matrix[0].row; m_row++)
	{
		for (int m_col = 0; m_col < _matrix[0].col; m_col++)
		{
			A(m_row, m_col) = _matrix[0].array[m_row][m_col];
		}
	}
	Eigen::EigenSolver<Eigen::MatrixXd> es(A);
	if (es.info() != Eigen::Success)//如果不满足特征值分解条件会中止进程
		abort();

	Eigen::MatrixXd D = es.pseudoEigenvalueMatrix();
	Eigen::MatrixXd mtemp(_matrix[0].row, _matrix[0].col);		mtemp = D;
	for (int i = 1; i < mul_count; i++) D =D*mtemp;

	for (long long m_row = 0; m_row < _matrix[0].row; m_row++)
	{
		double ima = 0;
		for (long long m_col = 0; m_col < _matrix[0].col; m_col++)
		{
			if (m_row == m_col)
			{
				memset(temp, 0, 128);	sprintf(temp, "%2g", D((long)_matrix[0].row * m_row + m_col));
				strcat(result, temp);	strcat(result, " ");
			}
			if (m_row != m_col)
			{
				if (D((long)_matrix[0].row * m_row + m_col) != 0)	ima = D((long)_matrix[0].row * m_row + m_col);
			}
		}
		if (ima != 0)
		{
			result[strlen(result) - 1] = 0;
			if (ima > 0) { memset(temp, 0, 128);	sprintf(temp, "+");		strcat(result, temp); }
			memset(temp, 0, 128);	sprintf(temp, "%2gi", ima);
			strcat(result, temp);	strcat(result, " ");
		}
	}

	strcat(result, "\0");
	return result;
}

const char* matrix::linearRelation()
{
	char result[result_len];	memset(result, 0, result_len);
	char temp[128];	memset(temp, 0, 128);
	if(_matrix.size()<2||_matrix[0].row!=_matrix[1].row)	
		return	(const char*)"linearRelation() execute failed, because matrix is conflicting";
	Eigen::MatrixXd A(_matrix[0].row, _matrix[0].col);
	Eigen::MatrixXd b(_matrix[1].row, _matrix[1].col);
	Eigen::MatrixXd X(_matrix[0].col, _matrix[1].col);
	Eigen::ColPivHouseholderQR<Eigen::MatrixXd> CPQR;

	memset(temp, 0, 128);	sprintf(temp, "%d", _matrix[0].col);
	strcat(result, temp);	strcat(result, " ");
	memset(temp, 0, 128);	sprintf(temp, "%d", _matrix[1].col);
	strcat(result, temp);	strcat(result, " ");

	for (int m_row = 0; m_row < _matrix[0].row; m_row++)
	{
		for (int m_col = 0; m_col < _matrix[0].col; m_col++)
		{
			A(m_row, m_col) = _matrix[0].array[m_row][m_col];
		}
	}
	for (int m_row = 0; m_row < _matrix[1].row; m_row++)
	{
		for (int m_col = 0; m_col < _matrix[1].col; m_col++)
		{
			b(m_row, m_col) = _matrix[1].array[m_row][m_col];
		}
	}
	CPQR.compute(A);	X = CPQR.solve(b);
	/*std::cout << A << "\n" << b << "\n\n\n";*/
	
	for (long long idx = 0; idx < X.size(); idx++)
	{
			memset(temp, 0, 128);	sprintf(temp, "%.2f", X(idx));
			strcat(result, temp);	strcat(result, " ");
	}
	strcat(result, "\0");

	return result;
}


char* itoa(int value, char* str, int base)
{
	sprintf(str, "%d", value);
	return str;
}



void Eig()
{
	Eigen::MatrixXd A(3,3);
	A << 1, 3, 3,
		-3, -5, -3,
		3, 3, 1;
	std::cout << "Here is a 4x4 matrix, A:" << std::endl << A << std::endl << std::endl;
	Eigen::EigenSolver<Eigen::MatrixXd> es(A);

	Eigen::MatrixXd D = es.pseudoEigenvalueMatrix();
	Eigen::MatrixXd V = es.pseudoEigenvectors();
	std::cout << "The pseudo-eigenvalue matrix D is:" << std::endl << D << std::endl;
	std::cout << "The pseudo-eigenvector matrix V is:" << std::endl << V << std::endl;
	std::cout << "Finally, V * D * V^(-1) = " << std::endl << V * D * V.inverse() << std::endl;
}