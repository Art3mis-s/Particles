#include "Matrices.h"
#include <stdexcept>
#include <cmath>

namespace Matrices
{
	//Constructor
	Matrix::Matrix(int _rows, int _cols)
	{
		rows = _rows;
		cols = _cols;
		a.resize(rows);

		for (int i = 0; i < rows; i++)
		{
			a[i].resize(cols, 0.0); //initialize each element to 0
		}
	}

	//Adding matrices
	Matrix operator+(const Matrix& lhs, const Matrix& rhs)
	{
		if (lhs.getCols() != rhs.getCols() || lhs.getRows() != rhs.getRows())
		{
			throw runtime_error("Error: dimensions must agree");
		}

		Matrix result(lhs.getRows(), lhs.getCols());
		for (int i = 0; i < lhs.getRows(); i++)
		{
			for (int j = 0; j < lhs.getCols(); j++)
			{
				result(i, j) = lhs(i, j) + rhs(i, j);
			}
		}

		return result;
	}

	//Multiplication
	Matrix operator*(const Matrix& lhs, const Matrix& rhs)
	{
		if (lhs.getCols() != rhs.getRows())
		{
			throw runtime_error("Error: dimensions must agree");
		}

		Matrix result(lhs.getRows(), rhs.getCols());

		for (int i = 0; i < lhs.getRows(); i++)
		{
			for (int k = 0; k < rhs.getCols(); k++)
			{
				double sum = 0.0;

				for (int j = 0; j < lhs.getCols(); j++)
				{
					sum += lhs(i, j) * rhs(j, k);
				}
				result(i, k) = sum;
			}
		}
		return result;
	}

	//Equality
	bool operator==(const Matrix& lhs, const Matrix& rhs)
	{
		if (lhs.getRows() != rhs.getRows() || lhs.getCols() != rhs.getCols())
		{
			return false;
		}

		for (int i = 0; i < lhs.getRows(); i++) {
			for (int j = 0; j < lhs.getCols(); j++) {
				if (fabs(lhs(i, j) - rhs(i, j)) >= 0.001) {
					return false;
				}
			}
		}
		return true;
	}

	//Inequality
	bool operator!=(const Matrix& lhs, const Matrix& rhs)
	{
		return !(lhs == rhs);
	}

	//Output
	ostream& operator<<(ostream& os, const Matrix& m)
	{
		for (int i = 0; i < m.getRows(); i++) {
			for (int j = 0; j < m.getCols(); j++) {
				os << setw(10) << m(i, j) << " ";
			}
			os << "\n";
		}
		return os;
	}

	RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)
	{
		// Call the parent constructor Matrix(2, 2)

		// Assign elements based on the formula:
		
		// a(0, 0) = cos(theta)
		(*this)(0, 0) = cos(theta);

		// a(0, 1) = -sin(theta)
		(*this)(0, 1) = -sin(theta);

		// a(1, 0) = sin(theta)
		(*this)(1, 0) = sin(theta);

		// a(1, 1) = cos(theta)
		(*this)(1, 1) = cos(theta);
	}
	
	
	ScalingMatrix::ScalingMatrix(double scale) : Matrix(2, 2)
	{
		//Calling parent constructor Matrix(2, 2)

		//Assign first element and last element with scaler
		//a(0,0) = scale
		(*this)(0, 0) = scale;

		//a(1,1) = scale
		(*this)(1, 1) = scale;
	}
	
	TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols) : Matrix(2, nCols)
	{
		//Calling parent constructor Matrix(2, nCols)

		//Assign elements, Row 0 is xShift, Row 1 is yShift
		for (int j = 0; j < nCols; ++j)
		{
			//a(0, j) = xShift
			(*this)(0, j) = xShift;

			//a(1, j) = yShift
			(*this)(1, j) = yShift;
		}
	}
}