﻿#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <clocale>
//#define See
//#define File

using namespace std;

void show_array(double** const &syst, const unsigned &n)
{
	cout << endl;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << syst[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl << endl;	
}

bool dia_dominance(double** const &syst, const unsigned &n)
{
	double* sum = new double[n];
	for (int i = 0; i < n; i++)
		sum[i] = 0;

	for (int i = 0; i < n; i++)
	{
		if (syst[i][i])
			for (int j = 0; j < n; j++)
			{
				if (j == i)				
					continue;				
				sum[i] += syst[i][j];
			}
		if (sum[i] <= abs(syst[i][i]))
			continue;
		else
		{
			delete[] sum;
			return false;
		}	
	}
	delete[] sum;
	return true;
}

bool test_0_rows(double** const &syst, const unsigned &n)
{
	for (int i = 0; i < n;)
	{
		for (int j = 0; j < n;)
		{
			if (syst[i][j])
			{
				if ((i == n - 1) && (j == 0))
					return true;
				++i;
			}
			else
			{
				if (j == n - 1)
					return false;
				++j;
			}
		}
	}
}

bool test_0_cols(double** const &syst, const unsigned &n)
{
	for (int j = 0; j < n;)
	{
		for (int i = 0; i < n;)
		{
			if (syst[i][j])
			{
				if ((j == n - 1) && (i == 0))
				{
					return true;
				}
				++j;
			}
			else
			{
				if (i == n - 1)
				{
					return false;
				}
				++i;
			}
		}
	}
}

bool test_0_dia(double** const &syst, const unsigned &n)
{
	for (int i = 0; i < n; i++)
	{
		if (syst[i][i] == 0)
		{
			continue;
		}
		else
		{
			return true;
		}
	}
	if (syst[n][n] == 0)
		return false;
}

void iteration(double**syst, double *x, unsigned &n, const double *b)
{
	double e;

	double *x_last = new double[n];
	for (int i = 0; i < n; i++)
		x_last[i] = x[i];

	double* sum = new double[n];
	for (int i = 0; i < n; i++)
		sum[i] = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			if (i == j)
				continue;
			else
				sum[i] += syst[i][j] * x[j];
		}

	for (int i = 0; i < n; i++)
		x[i] = (b[i] - sum[i]) / syst[i][i];

	e = abs(x[0] - x_last[0]);

	if (e < 0.001)
	{
		delete[] x_last;
		delete[] sum;
		x_last, sum = nullptr;
		return;
	}
	else
	{
		delete[] sum;
		delete[] x_last;
		iteration(syst, x, n, b);
	}
}

int main()
{
	unsigned n;
	cout << "Enter the dimension of the matrix: ";
	cin >> n;

	double **syst = new double *[n];
	for (int i = 0; i < n; i++)
		syst[i] = new double[n];

	double *b = new double[n];

	double *x = new double[n];
	for (int i = 0; i < n; i++)
		x[i] = 0;

	cout << "Enter the matrix:\n";
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> syst[i][j];

	cout << "Enter free elements:\n";
	for (int i = 0; i < n; i++)
		cin >> b[i];
#ifdef See	
	show_array(syst, n);
#endif // See

	double *buf = new double[n];
	double buf_b;
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (syst[j][i] > syst[i][i])
			{
				buf = syst[j];
				syst[j] = syst[i];
				syst[i] = buf;

				buf_b = b[j];
				b[j] = b[i];
				b[i] = buf_b;
			}
		}
	}	

#ifdef See	
	show_array(syst, n);
#endif // See

	if (test_0_cols(syst, n) && test_0_rows(syst, n) && test_0_dia(syst, n) && dia_dominance(syst, n))
	{
		iteration(syst, x, n, b);

		for (int i = 0; i < n; i++)
			cout << "x[" << i + 1 << "] = " << x[i] << endl;
#ifdef File
		ofstream fout;
		fout.open("xvector.txt");
		for (int i = 0; i < n; i++)
			fout << "x" << i + 1 << " = " << x[i] << endl;
		fout.close();
#endif // File
	}
	else
	{
		cout << "No solution!";
	}

	delete[] x;
	delete[]b;
	delete[] buf;
	x, b, buf = nullptr;

	for (int i = 0; i < n; i++)
		delete[] syst[i];
	syst = nullptr;
}
