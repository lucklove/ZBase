#include "matrix.hh"
#include "retval.hh"

namespace matrix {

std::tuple<matrix_t<double>, matrix_t<double>>
eig(matrix_t<double> mat)
{
        constexpr double eps = 0.00001;
        constexpr size_t loop_control = 2048;
	if(mat.size1() != mat.size2())
		DEBUG_THROW(MatrixError, "mat.size1() != mat.size2()");
        matrix_t<double> eigenvalue = mat;
        matrix_t<double> eigenvector(mat.size1(), mat.size2());
        size_t p,q; 
        size_t loop_count = 0;
        for (size_t i = 0; i < mat.size1(); ++i) { 
                eigenvector(i, i) = 1.0;
                for (size_t j = 0; j < mat.size1(); ++j) { 
                        if (i != j) { 
                                eigenvector(i, j) = 0.0;
                        }   
                }   
        }   
        while (true) { 
                double fm = 0.0;
                for(size_t i = 0; i < mat.size1(); ++i) { 
                        for (size_t j = 0; j < mat.size1(); j++) { 
                                if(i != j && fabs(eigenvalue(i, j)) > fm) { 
                                        fm = fabs(eigenvalue(i, j));
                                        p = i;  
                                        q = j;  
                                }   
                        }   
                }   
                if(fm < eps || loop_count > loop_control)
                        return zbase::makeRet(eigenvector, eigenvalue);
                ++loop_count;
                double x = -eigenvalue(p, q);
                double y = (eigenvalue(q, q) - eigenvalue(p, p)) / 2.0;
                double omega= x / sqrt(x * x + y * y);
                if(y < 0.0)
                        omega = -omega;
                double sn = 1.0 + sqrt(1.0 - omega * omega);
                sn = omega / sqrt(2.0 * sn);
                double cn = sqrt(1.0 - sn * sn);
                fm = eigenvalue(p, p);
                eigenvalue(p, p) = fm * cn * cn+ eigenvalue(q, q) * sn * sn + eigenvalue(p, q) * omega;
                eigenvalue(q, q) = fm * sn * sn + eigenvalue(q, q) * cn * cn - eigenvalue(p, q) * omega;
                eigenvalue(p, q) = 0.0;
                eigenvalue(q, p) = 0.0;
                for (size_t j = 0; j < mat.size1(); ++j) {
                        if (j != p && j != q) {
                                fm=eigenvalue(p, j);
                                eigenvalue(p, j) = fm * cn + eigenvalue(q, j) * sn;
                                eigenvalue(q, j) = -fm * sn + eigenvalue(q, j) * cn;
                        }
                }
                for (size_t i = 0; i < mat.size1(); i++) {
                        if (i != p && i != q) {
				if(i == 2 && p == 0 && q == 1)
                                fm = eigenvalue(i, p);
                                eigenvalue(i, p) = fm * cn + eigenvalue(i, q) * sn;
                                eigenvalue(i, q)= -fm * sn + eigenvalue(i, q) * cn;
                        }
                }
                for (size_t i = 0; i < mat.size1(); i++) {
                        fm = eigenvector(i, p);
                        eigenvector(i, p) = fm * cn + eigenvector(i, q) * sn;
                        eigenvector(i, q) = -fm * sn + eigenvector(i, q) * cn;
                }
	}
}

matrix_t<double>
zscore(matrix_t<double> mat)
{
	for(size_t i = 0; i < mat.size2(); ++i) {
		matrix_col_t<matrix_t<double>> mc(mat, i);
		double me = mean(mc);		/**< 求均值 */
		double val = sqrt(var(mc));
		for(auto & it : mc)
			it = (it - me) / val;
	}
	return mat;
}

matrix_t<double>
cov(matrix_t<double> mat)
{
	matrix_t<double> ret(mat.size2(), mat.size2());
	for(size_t i = 0; i < mat.size2(); ++i) {
		for(size_t j = 0; j < mat.size2(); ++j) {
			double sum = 0.0;
			for(size_t k = 0; k < mat.size1(); ++k)
				sum += mat(k, i) * mat(k, j);
			ret(i, j) = sum / (mat.size1() - 1);
		}
	}
	return ret;
}
		
		
}		/**< namespace matrix */
