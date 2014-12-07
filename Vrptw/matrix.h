#ifndef _matrix_h_
#define _matrix_h_

/*
	逆下三角方阵（Inverse lower triangular matrix）
	0 1 1
	0 0 1
	0 0 0
	i 为行，j 为列
*/
template <class T>
class IltMatrix {
public:
	IltMatrix() :matrix_rows(2) {
		Create(matrix_rows);
	}

	explicit IltMatrix(size_t m) :matrix_rows(m) {
		Create(matrix_rows);
	}

	IltMatrix(const IltMatrix& im) {
		Create(im);
	}

	~IltMatrix() { delete[] matrix_array; }

	IltMatrix& operator=(const IltMatrix& im) {
		delete[] matrix_array;
		Create(matrix_array);
		return *this;
	}

	//根据方阵坐标取元素
	T Get_element(size_t i, size_t j) {
		if (i == j) { return 0; }
		return matrix_array[CalSubscript(i, j)];
	}

	//根据方阵坐标设元素
	void Set_element(size_t i, size_t j, T k) {
		if (Element_IsNull(i, j) || (k >= 0)) { matrix_array[CalSubscript(i, j)] = k; }
	}

	//判断元素是否为空值
	bool Element_IsNull(size_t i, size_t j) {
		if (i == j) { return false; }
		if (GetElement(i, j) == -1) { return true; }
		return false;
	}

private:
	T* matrix_array; //方阵数组
	size_t matrix_rows; //方阵行数
	size_t array_length; //数组长度

	//计算矩阵对应的数组下标
	size_t CalSubscript(size_t i, size_t j) const {
		size_t t = 0;
		if (i > j) { t = CalSubscript(j, i); }
		else { t = j + (i*matrix_rows) - (((i*i) + (3 * i)) / 2) - 1; }
		return t;
	}

	//根据方阵的行数创建对应的数组
	void Create(size_t rows) {
		matrix_rows = rows;
		array_length = (matrix_rows * (matrix_rows - 1)) / 2;
		matrix_array = new T[array_length];
		for (size_t i = 0; i != array_length; ++i) {
			matrix_array[i] = -1;
		}
	}

	//根据方阵创建对应的数组
	void Create(const IltMatrix& im) {
		Create(im.n);
		for (size_t i = 0; i != array_length; ++i) {
			matrix_array[i] = im.matrix_array[i];
		}
	}
};
#endif