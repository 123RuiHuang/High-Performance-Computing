#ifndef _SCOTTGS_MATRIX_MULTIPLY
#define _SCOTTGS_MATRIX_MULTIPLY

#include "FloatMatrix.hpp"

namespace scottgs 
{

class MatrixMultiply
{
public: 
	MatrixMultiply();
	~MatrixMultiply();

	///
	/// \brief Use naive element access
	/// \param lhs The left-hand operand to the multiplication
	/// \param rhs The right-hand operand to the multiplication
	/// \returns the result of the matrix multiplication
	scottgs::FloatMatrix operator()(const scottgs::FloatMatrix& lhs, const scottgs::FloatMatrix& rhs) const;

	///
	/// \brief Use boost built-in matrix multiplication
	/// \param lhs The left-hand operand to the multiplication
	/// \param rhs The right-hand operand to the multiplication
	/// \returns the result of the matrix multiplication
	scottgs::FloatMatrix multiply(const scottgs::FloatMatrix& lhs, const scottgs::FloatMatrix& rhs) const;

};

}; // end namespace scottgs
#endif // SCOTTGS_MATRIX_MULTIPLY

