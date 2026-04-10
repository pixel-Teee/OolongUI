#pragma once

namespace Oolong
{
		
#define VECTOR_MEMBERS(T, n)\
		/*convert to c array style*/ \
		typedef T (&array_t)[n];\
		operator array_t ()\
		{ return reinterpret_cast<array_t>(*data()); }\
		typedef const T (&const_array_t)[n];\
		operator const_array_t()const\
		{ return reinterpret_cast<const_array_t>(*data()); }\
		T& operator [] (int i)\
		{ return data()[i]; }\
		const T& operator [] (int i) const\
		{ return data()[i]; }\
		static constexpr unsigned int const DIM = n;\
		vector(){}\
		vector(const T* v)\
		{ for(int i = 0; i < n; ++i) data()[i] = v[i];}\
		private: operator bool();

	template<typename T, int n>
	struct vector
	{
		cassert(n > 4);
		T m_data[n];

		vector(T a)
		{ 
			for (int i = 0; i < n; ++i)
				m_data[i] = a;
		}

		T* data() { return m_data; }
		const T* data() const { return m_data; }

		template<typename U>
		explicit vector(const vector<U, n>& v)
		{
			for (int i = 0; i < n; ++i)
			{
				m_data[i] = static_cast<T>(v.m_data[i]);
			}
		}

		VECTOR_MEMBERS(T, n)
	};

	template <typename T>
	struct vector<T, 2>
	{
		T x, y;

		T* data() { return &x; }
		const T* data() const { return &x; }

		constexpr vector(T a) : x(a), y(a) { }
		constexpr vector(T _x, T _y) : x(_x), y(_y) { }
		template<typename U>
		explicit constexpr vector(const vector<U, 2>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) { }
		constexpr vector(const vector<T, 3>& v) : x(v.x), y(v.y) { }
		constexpr vector(const vector<T, 4>& v) : x(v.x), y(v.y) { }
		constexpr static vector zero() { return vector(static_cast<T>(0)); }

		VECTOR_MEMBERS(T, 2)
	};

	template <typename T>
	struct vector<T, 3>
	{
		T x, y, z;

		T* data() { return &x; }
		const T* data() const { return &x; }

		vector<T, 2>& xy() { return *reinterpret_cast<vector<T, 2>*>(&x); }
		const vector<T, 2>& xy() const { return *reinterpret_cast<const vector<T, 2>*>(&x); }

		constexpr vector(T a) : x(a), y(a), z(a) { }
		constexpr vector(T _x, T _y, T _z) : x(_x), y(_y), z(_z) { }
		constexpr vector(const vector<T, 2>& xy, T _z) : x(xy.x), y(xy.y), z(_z) { }
		constexpr vector(const vector<T, 4>& v) : x(v.x), y(v.y), z(v.z) { }
		template<typename U>
		explicit constexpr vector(const vector<U, 3>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) { }
		constexpr static vector zero() { return vector(static_cast<T>(0)); }

		VECTOR_MEMBERS(T, 3)
	};

	template <typename T>
	struct vector<T, 4>
	{
		T x, y, z, w;

		T* data() { return &x; }
		const T* data() const { return &x; }

		vector<T, 2>& xy() { return *reinterpret_cast<vector<T, 2>*>(&x); }
		const vector<T, 2>& xy() const { return *reinterpret_cast<const vector<T, 2>*>(&x); }
		vector<T, 2>& zw() { return *reinterpret_cast<vector<T, 2>*>(&z); }
		const vector<T, 2>& zw() const { return *reinterpret_cast<const vector<T, 2>*>(&z); }
		vector<T, 3>& xyz() { return *reinterpret_cast<vector<T, 3>*>(&x); }
		const vector<T, 3>& xyz() const { return *reinterpret_cast<const vector<T, 3>*>(&x); }

		constexpr vector(T a) : x(a), y(a), z(a), w(a) { }
		constexpr vector(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }
		constexpr vector(const vector<T, 2>& xy, T _z, T _w) : x(xy.x), y(xy.y), z(_z), w(_w) { }
		constexpr vector(const vector<T, 2>& xy, const vector<T, 2>& zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
		constexpr vector(const vector<T, 3>& xyz, T _w) : x(xyz.x), y(xyz.y), z(xyz.z), w(_w) { }
		template<typename U>
		explicit constexpr vector(const vector<U, 4>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) { }
		constexpr static vector zero() { return vector(static_cast<T>(0)); }

		VECTOR_MEMBERS(T, 4)
	};

#undef VECTOR_MEMBERS

#define DEFINE_CONCRETE_VECTORS(type) \
		typedef vector<type, 2> type##2; \
		typedef vector<type, 3> type##3; \
		typedef vector<type, 4> type##4;

		DEFINE_CONCRETE_VECTORS(float);
		DEFINE_CONCRETE_VECTORS(double);
		DEFINE_CONCRETE_VECTORS(int);
		DEFINE_CONCRETE_VECTORS(uint);
		DEFINE_CONCRETE_VECTORS(bool);
		DEFINE_CONCRETE_VECTORS(short);

#undef DEFINE_CONCRETE_VECTORS


	//重载数学操作(一元运算符)
#define DEFINE_UNARY_OPERATOR(op) \
	template<typename T> constexpr vector<T, 2> operator op (const vector<T, 2>& a) \
	{ return vector<T, 2>(op a.x, op a.y); } \
	template<typename T> constexpr vector<T, 3> operator op (const vector<T, 3>& a) \
	{ return vector<T, 3>(op a.x, op a.y, op a.z); } \
	template<typename T> constexpr vector<T, 4> operator op (const vector<T, 4>& a) \
	{ return vector<T, 4>(op a.x, op a.y, op a.z, op a.w); }

	//二元运算符
#define DEFINE_BINARY_OPERATORS(op) \
	/* Vector-vector op */ \
	template<typename T> constexpr vector<T, 2> operator op (const vector<T, 2>& a, const vector<T, 2>& b) \
	{ return vector<T, 2>(a.x op b.x, a.y op b.y); } \
	template<typename T> constexpr vector<T, 3> operator op (const vector<T, 3>& a, const vector<T, 3>& b) \
	{ return vector<T, 3>(a.x op b.x, a.y op b.y, a.z op b.z); } \
	template<typename T> constexpr vector<T, 4> operator op (const vector<T, 4>& a, const vector<T, 4>& b) \
	{ return vector<T, 4>(a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w); } \
	/* Scalar-vector op */ \
	template<typename T> constexpr vector<T, 2> operator op (T a, const vector<T, 2>& b) \
	{ return vector<T, 2>(a op b.x, a op b.y); } \
	template<typename T> constexpr vector<T, 3> operator op (T a, const vector<T, 3>& b) \
	{ return vector<T, 3>(a op b.x, a op b.y, a op b.z); } \
	template<typename T> constexpr vector<T, 4> operator op (T a, const vector<T, 4>& b) \
	{ return vector<T, 4>(a op b.x, a op b.y, a op b.z, a op b.w); } \
	/* Vector-scalar op */ \
	template<typename T> constexpr vector<T, 2> operator op (const vector<T, 2>& a, T b) \
	{ return vector<T, 2>(a.x op b, a.y op b); } \
	template<typename T> constexpr vector<T, 3> operator op (const vector<T, 3>& a, T b) \
	{ return vector<T, 3>(a.x op b, a.y op b, a.z op b); } \
	template<typename T> constexpr vector<T, 4> operator op (const vector<T, 4>& a, T b) \
	{ return vector<T, 4>(a.x op b, a.y op b, a.z op b, a.w op b); }

	//就地运算符，直接返回修改的当前对象
#define DEFINE_INPLACE_OPERATORS(op) \
	/* Vector-vector op */ \
	template<typename T> vector<T, 2> operator op (vector<T, 2>& a, const vector<T, 2>& b) \
	{ a.x op b.x; a.y op b.y; return a; } \
	template<typename T> vector<T, 3> operator op (vector<T, 3>& a, const vector<T, 3>& b) \
	{ a.x op b.x; a.y op b.y; a.z op b.z; return a; } \
	template<typename T> vector<T, 4> operator op (vector<T, 4>& a, const vector<T, 4>& b) \
	{ a.x op b.x; a.y op b.y; a.z op b.z; a.w op b.w; return a; } \
	/* Vector-scalar op */ \
	template<typename T> vector<T, 2> operator op (vector<T, 2>& a, T b) \
	{ a.x op b; a.y op b; return a; } \
	template<typename T> vector<T, 3> operator op (vector<T, 3>& a, T b) \
	{ a.x op b; a.y op b; a.z op b; return a; } \
	template<typename T> vector<T, 4> operator op (vector<T, 4>& a, T b) \
	{ a.x op b; a.y op b; a.z op b; a.w op b; return a; }

	//关系运算符
#define DEFINE_RELATIONAL_OPERATORS(op) \
	/* Vector-vector op */ \
	template<typename T> constexpr vector<bool, 2> operator op (const vector<T, 2>& a, const vector<T, 2>& b) \
	{ return vector<bool, 2>(a.x op b.x, a.y op b.y); } \
	template<typename T> constexpr vector<bool, 3> operator op (const vector<T, 3>& a, const vector<T, 3>& b) \
	{ return vector<bool, 3>(a.x op b.x, a.y op b.y, a.z op b.z); } \
	template<typename T> constexpr vector<bool, 4> operator op (const vector<T, 4>& a, const vector<T, 4>& b) \
	{ return vector<bool, 4>(a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w); } \
	/* Scalar-vector op */ \
	template<typename T> constexpr vector<bool, 2> operator op (T a, const vector<T, 2>& b) \
	{ return vector<bool, 2>(a op b.x, a op b.y); } \
	template<typename T> constexpr vector<bool, 3> operator op (T a, const vector<T, 3>& b) \
	{ return vector<bool, 3>(a op b.x, a op b.y, a op b.z); } \
	template<typename T> constexpr vector<bool, 4> operator op (T a, const vector<T, 4>& b) \
	{ return vector<bool, 4>(a op b.x, a op b.y, a op b.z, a op b.w); } \
	/* Vector-scalar op */ \
	template<typename T> constexpr vector<bool, 2> operator op (const vector<T, 2>& a, T b) \
	{ return vector<bool, 2>(a.x op b, a.y op b); } \
	template<typename T> constexpr vector<bool, 3> operator op (const vector<T, 3>& a, T b) \
	{ return vector<bool, 3>(a.x op b, a.y op b, a.z op b); } \
	template<typename T> constexpr vector<bool, 4> operator op (const vector<T, 4>& a, T b) \
	{ return vector<bool, 4>(a.x op b, a.y op b, a.z op b, a.w op b); }

DEFINE_BINARY_OPERATORS(+);
DEFINE_BINARY_OPERATORS(-);
DEFINE_UNARY_OPERATOR(-);
DEFINE_BINARY_OPERATORS(*);
DEFINE_BINARY_OPERATORS(/);
DEFINE_BINARY_OPERATORS(&);
DEFINE_BINARY_OPERATORS(|);
DEFINE_BINARY_OPERATORS(^);
DEFINE_UNARY_OPERATOR(!);
DEFINE_UNARY_OPERATOR(~);

DEFINE_INPLACE_OPERATORS(+=);
DEFINE_INPLACE_OPERATORS(-=);
DEFINE_INPLACE_OPERATORS(*=);
DEFINE_INPLACE_OPERATORS(/=);
DEFINE_INPLACE_OPERATORS(&=);
DEFINE_INPLACE_OPERATORS(|=);
DEFINE_INPLACE_OPERATORS(^=);

DEFINE_RELATIONAL_OPERATORS(==);
DEFINE_RELATIONAL_OPERATORS(!=);
DEFINE_RELATIONAL_OPERATORS(<);
DEFINE_RELATIONAL_OPERATORS(>);
DEFINE_RELATIONAL_OPERATORS(<=);
DEFINE_RELATIONAL_OPERATORS(>=);

#undef DEFINE_UNARY_OPERATOR
#undef DEFINE_BINARY_OPERATORS
#undef DEFINE_INPLACE_OPERATORS
#undef DEFINE_RELATIONAL_OPERATORS

}