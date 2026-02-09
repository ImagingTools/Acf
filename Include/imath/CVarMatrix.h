// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// ACF includes
#include <istd/istd.h>
#include <istd/TArray.h>
#include <istd/CIndex2d.h>


namespace iser
{
	class IArchive;
}


namespace imath
{


class CVarVector;


/**
	Implementation of variable-size mathematical matrix with double precision elements.
	
	\section VarMatrixPurpose Purpose
	CVarMatrix is a dynamic-size matrix class that provides mathematical matrix operations
	with runtime-determined dimensions. Built on top of istd::TArray<double, 2>, it supports
	common matrix operations like addition, multiplication, transpose, decomposition, and
	various matrix norms. The matrix stores double precision floating-point values in a
	2D array structure.
	
	\section VarMatrixVsFixed CVarMatrix vs TMatrix
	- **CVarMatrix**: Dynamic size determined at runtime, flexible resizing, heap allocation
	- **TMatrix**: Fixed size at compile time, stack allocation, compile-time type safety
	
	Use CVarMatrix when:
	- Matrix dimensions are determined at runtime (e.g., from file, algorithm output)
	- You need to resize matrices dynamically during computation
	- Working with matrices of varying sizes in the same algorithm
	- Interfacing with external libraries that provide runtime-sized data
	
	Use TMatrix when:
	- Matrix dimensions are known at compile time (e.g., 3x3 rotation matrices)
	- Performance is critical and dimensions are fixed
	- Want compile-time dimension checking for safety
	
	\section VarMatrixUsageExamples Usage Examples
	\code
	// Create 3x3 matrix
	imath::CVarMatrix mat1(istd::CIndex2d(3, 3));
	mat1.InitToIdentity(3);  // Set to identity matrix
	
	// Create from vector (column or row matrix)
	imath::CVarVector vec(3);
	vec.SetElement(0, 1.0);
	vec.SetElement(1, 2.0);
	vec.SetElement(2, 3.0);
	imath::CVarMatrix mat2(vec, false);  // 3x1 column matrix
	imath::CVarMatrix mat3(vec, true);   // 1x3 row matrix
	
	// Matrix operations
	imath::CVarMatrix mat4(istd::CIndex2d(3, 3));
	mat4.SetElementAt(0, 0, 2.0);
	mat4.SetElementAt(1, 1, 3.0);
	mat4.SetElementAt(2, 2, 4.0);
	
	// Matrix multiplication
	imath::CVarMatrix result;
	mat1.GetMultiplied(mat4, result);
	
	// Transpose
	imath::CVarMatrix transposed;
	mat4.GetTransposed(transposed);
	
	// Matrix norms
	double trace = mat4.GetTrace();  // Sum of diagonal elements
	double frobNorm = mat4.GetFrobeniusNorm();  // Euclidean norm
	
	// Decomposition
	imath::CVarMatrix triangular;
	bool success = mat4.GetTriangleDecomposed(triangular);
	\endcode
	
	\section VarMatrixOperations Common Operations
	- **Construction**: Default, size-based, copy, from vector
	- **Element Access**: GetElementAt(), GetElementRef(), SetElementAt()
	- **Initialization**: Clear(), InitToIdentity()
	- **Properties**: GetMaxElement(), GetMinElement(), GetTrace()
	- **Arithmetic**: GetAdded(), GetSubstracted(), GetMultiplied(), GetScaled(), GetNegated()
	- **Transform**: GetTransposed(), Transpose()
	- **Norms**: GetFrobeniusNorm(), GetFrobeniusNorm2()
	- **Decomposition**: GetTriangleDecomposed(), GetDecompositionQDQ()
	- **Comparison**: operators (==, !=)
	- **Serialization**: Serialize()
	
	\section VarMatrixStorage Storage Format
	The matrix uses column-major storage (compatible with OpenGL and many scientific libraries):
	- Elements are stored column by column
	- Element at (x, y) is in column x, row y
	- Access via GetElementAt(x, y) or underlying TArray interface
	
	\sa imath::TMatrix, imath::CVarVector, istd::TArray
	
	\ingroup LinearAlgebra
*/
class CVarMatrix: public istd::TArray<double, 2>
{
public:
	typedef istd::TArray<double, 2> BaseClass;

	/**
		Create empty matrix.
	*/
	CVarMatrix();

	/**
		Copy constructor.
	*/
	CVarMatrix(const CVarMatrix& matrix);

	/**
		Create matrix with specified size.
	*/
	explicit CVarMatrix(istd::CIndex2d size);

	/**
		Create matrix from vector.
	*/
	explicit CVarMatrix(const CVarVector& vector, bool isTransposed = false);

	/**
		Set all matrix cells to zero.
	*/
	void Clear();

	/**
		Create identity matrix.
	*/
	void InitToIdentity(int size);

	/*
		Get a matrix element at the given position.
	*/
	double GetElementAt(int x, int y) const;

	/*
		Get the reference of a matrix element at the given position.
	*/
	double& GetElementRef(int x, int y);

	/**
		Get maximal element.
	*/
	double GetMaxElement() const;

	/*
		Get minimal element.
	*/
	double GetMinElement() const;

	/**
		Get result matrix with negated all elements.
	*/
	void GetNegated(CVarMatrix& result);

	/**
		Get sum of two matrices.
	*/
	void GetAdded(const CVarMatrix& matrix, CVarMatrix& result) const;

	/**
		Get result of substraction of two matrices.
	*/
	void GetSubstracted(const CVarMatrix& matrix, CVarMatrix& result) const;

	/**
		Get result of multiplication of two matrices.
	*/
	void GetMultiplied(const CVarMatrix& matrix, CVarMatrix& result) const;

	/**
		Get result of multiplication of two matrices.
	*/
	CVarMatrix GetMultiplied(const CVarMatrix& matrix) const;

	/**
		Get result of multiplication of this matrix with scalar value.
	*/
	void GetScaled(double value, CVarMatrix& result) const;

	/**
		Get transposed matrix.
	*/
	void GetTransposed(CVarMatrix& result) const;

	/**
		Get transposed matrix.
	*/
	CVarMatrix GetTransposed() const;

	/**
		Transpose matrix.
	*/
	void Transpose();

	/**
		Get trace of this matrix.
	*/
	double GetTrace() const;

	/*
		Get square of euclidean norm.
	*/
	double GetFrobeniusNorm2() const;

	/*
		Get euclidean norm.
	*/
	double GetFrobeniusNorm() const;

	/**
		Transform matrix to upper triangle form using method of Householder reflexions.
		\param	result		triangle matrix.
		\param	matrix2Ptr	optional matrix will be transformed using the same reflexions.
							To realize QR decomposition you can use identity matrix.
		\param	maxColumns	optional maximal number of transformed columns. If it is negative value whole matrix will be transformed.
		\param	minHhNorm	minimal Hausholder reflexion vector length. If any reflexion vector is shorter this method fails.
		\return				true if success.
	*/
	bool GetTriangleDecomposed(
				CVarMatrix& result,
				CVarMatrix* matrix2Ptr = NULL,
				int maxColumns = -1,
				double minHhNorm = I_BIG_EPSILON) const;

	/**
		Transform this matrix in place.
	*/
	bool TransformR(int firstPartWidth);

	/**
		Solving of linear system with triangle matrix.
		Rx = y, result = x.
		\return	true if linear equation system was solved.
	*/
	bool GetSolvedTriangle(const CVarMatrix& vector, CVarMatrix& result, double accuracy = I_BIG_EPSILON) const;

	/**
		Solve 'Least Square Problem'.
		Solve linear Least Square Problem for equation AX = Y, where A is a N * M matrix, N >= M, X is n * k matrix and Y is m * k matrix.
	 */
	bool GetSolvedLSP(const CVarMatrix& vector, CVarMatrix& result, double minHhNorm = I_BIG_EPSILON) const;

	/**
		Calculate decomposition in form of QDQ where \c Q is orthogonal matrix and \c D is diagonal one.
		It works for square matrix only.
	*/
	bool GetDecompositionQDQ(CVarMatrix& matrixQ, CVarVector& diagonalD, double tolerance = I_BIG_EPSILON, int maxIterations = 100) const;

	/**
		Get single column as vector.
	*/
	void GetColumnVector(int columnIndex, CVarVector& result);
	/**
		Get single row as vector.
	*/
	void GetRowVector(int rowIndex, CVarVector& result);

	bool Serialize(iser::IArchive& archive);

	// operators
	CVarMatrix operator+(const CVarMatrix& b) const;
	CVarMatrix operator-(const CVarMatrix& b) const;
	CVarMatrix operator-();
	CVarMatrix operator*(const CVarMatrix& b) const;
	CVarMatrix operator*(double value) const;

	bool operator==(const CVarMatrix& matrix) const;
	bool operator!=(const CVarMatrix& matrix) const;

	// static methods
	/**
		Solve 'Least Square Problem' using robust algorithm.
		Solve linear Least Square Problem for equation AX = Y, where A is a \em {n * m} matrix, X is \em {m * k} matrix and Y is \em {n * k} matrix.
		This implementation solve LSP in place, it transforms internal matrix A into R = HA and matrix Y into Y' = HY.
		Then it solves equation in form RX = Y', where R is 'quasi' triangle matrix.
		\param	matrixA	input matrix A in equation AX = Y.
						It will be destroyed by this operation (triangle matrix R = HA will be calculated in place).
		\param	matrixY	input matrix Y in equation AX = Y.
						It will be destroyed by this operation (matrix Y' = HY will be calculated in place).
						Please note, that height of this matrix must be the same as height of matrix A.
		\param	matrixX result matrix X in equation AX = Y. It will be initialized be this function.
						The output size of this matrix will be set to \em {m * k} where \em m is width of matrix A and \em k is height of matrix Y.
	 */
	static void SolveRobustLSP(CVarMatrix matrixA, CVarMatrix& matrixY, CVarMatrix& matrixX, double minHhNorm = I_BIG_EPSILON);
};


// inline methods

inline CVarMatrix CVarMatrix::GetMultiplied(const CVarMatrix& matrix) const
{
	CVarMatrix result;

	GetMultiplied(matrix, result);

	return result;
}


inline void CVarMatrix::Transpose()
{
	CVarMatrix result;

	GetTransposed(result);

	*this=result;
}


inline CVarMatrix CVarMatrix::operator+(const CVarMatrix& matrix) const
{
	CVarMatrix result;

	GetAdded(matrix, result);

	return result;
}


inline CVarMatrix CVarMatrix::operator-(const CVarMatrix& matrix) const
{
	CVarMatrix result;

	GetSubstracted(matrix, result);

	return result;
}


inline CVarMatrix CVarMatrix::operator-()
{
	CVarMatrix result;

	GetNegated(result);

	return result;
}


inline CVarMatrix CVarMatrix::operator*(const CVarMatrix& matrix) const
{
	CVarMatrix result;

	GetMultiplied(matrix, result);

	return result;
}


inline CVarMatrix CVarMatrix::operator*(double value) const
{
	CVarMatrix result;

	GetScaled(value, result);

	return result;
}


inline CVarMatrix operator*(double value, const imath::CVarMatrix& matrix)
{
	return matrix * value;
}


} // namespace imath


