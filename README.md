<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/mattiaisgro/uroboro"> <img alt="GitHub issues" src="https://img.shields.io/github/issues/mattiaisgro/uroboro">

# Uroboro Math Library
A general purpose, header-only, **C++ mathematical library**, focused on graphical and physical computation. Includes **real and complex analysis** functions with x86 Assembly enhancements, **linear algebra** operations, **quaternions** and **statistical** functions. Many other features are also supported, see [Functionalities](https://github.com/mattiaisgro/uroboro/blob/master/README.md#Functionalities) for a fuller overview of the functionalities provided by the library.

## How to use
The library is header-only, so it is only needed to include the proper header files in your program to use it. To simplify the usage of the library, the `uroboro.h` automatically includes common headers for real and complex analysis and linear algebra.

- To include **all header files** `UROBORO_INCLUDE_ALL` may be defined before including `uroboro.h`.
- All functions and classes are defined inside the `uroboro` namespace (and eventually other sub-namespaces). Another namespace alias is automatically defined as `umath` (same as using `uroboro`). If you want to disable the `umath` alias, define `UROBORO_NO_NAMESPACE` before including `uroboro.h`

**Matrices** (`algebra/mat.h`) are implemented inside the `mat<N, K>` class (where N and K are the number of columsn and rows respectively), while **vectors** (`algebra/vec.h`) in the `vec<N>` class (where N is the number of rows). **Quaternions** (`complex/quat.h`) and **complex** (`complex/complex.h`) numbers are implemented in the `quat` and `complex` classes. For example usage see [Examples](https://github.com/mattiaisgro/uroboro/blob/master/README.md#Examples).

## Functionalities
- Real analysis (`real_analysis.h`)
- Linear algebra (vector and matrix operations) (`algebra/vec.h`, `algebra/mat.h`)
- Complex (in algebraic and exponential form) and quaternion numbers (`complex/complex.h`, `complex/quat.h`, `complex/phasor.h`)
- Complex analysis (`complex/complex_analysis.h`)
- Dual numbers and Automatic Differentiation (`other/dual.h`, `other/dual_functions.h`)
- Statistical functions (including Least Squares Linearization) (`statistics/statistics.h`)
- Probability distribution functions (`statistics/distributions.h`)
- Approximation of roots and extrema of real functions (`approx.h`)
- Derivative approximation (`calculus/derivation.h`)
- Integral approximation, including Runge-Kutta of 4th order (`calculus/integration.h`)
- Polynomial operations, including derivation and integration (`polynomial.h`)
- Interpolation between vector data, including Bezier curves (`other/interpolation.h`)

## Examples
This section provides example usage of the library. See the `example.cpp` source file for more.

#### Using vectors
Vector operations can be used through the `vec<N, T = real>` class, where `N` specifies the number of rows and `T` specifies the element type (`real` is used as default). Common vector types are defined as `vec2`, `vec3` and `vec4`.

Declare a vector:
```c
// vec3 is the same as vec<3, real>
vec3 v1 = {1, 2, 3};

// You can initialize a vector from a bracketed list
vec3 v2 = {0, 1, 0};
```

Basic vector operations:
```c
// Compute the lenght of a vector
real l = v1.lenght();

// Access the i-th element (by reference)

// You can use the vec::at() function
v1.at(1) = 2;

// Or the [i] operator
v1[1] = 2;

// Get and set the i-th element
l = v1.get(1);
v1.set(1, 2);

// Multiply a vector by a scalar
v1 = v1 * 2.0;
v2 = v2 / 3.0;

// Normalize a vector

// normalized() returns the normalized vector
// without changing the initial vector
vec3 v1_n = v1.normalized();

// Meanwhile normalize() normalizes the vector itself
v1.normalize()
```

Dot product:
```c
// You can use the class method
real d = v1.dot(v2);

// Or the two-argument function
d = dot(v1, v2);

// Or simply the * operator
d = v1 * v2;
```

Cross product:
```c
// Same thing with the cross product
vec3 c;
c = v1.cross(v2);
c = cross(v1, v2);

// But there is no cross product operator!
```

#### Using matrices
Matrices are implemented in the `mat<N, K>` class, where N is the number of columns and K is the number of rows (lexicographical and column-first implementation). Common square matrix types are defines as `mat2`, `mat3`, `mat4`

Initialize a matrix:
```c
// 4x4 identity matrix
mat4 A1 = mat4::identity();

// Diagonal matrix with 3 on the diagonal
mat4 A2 = mat4(3.0);

// Zero matrix
mat3 B1 = mat3();
```

Basic matrix operations:
```c
// Access an element
B1.at(1, 1) = 2.0;

// Get or set and element
real a12 = A2.get(1, 2);
A1.set(a12, 1, 2);

// Scalar multiplication
A1 = (A2 * 2.0) / 3.0;
```

Vector and matrix multiplication:
```c
// Matrix-Vector multiplication

// You can use transform()
vec3 u1;
u1 = B1.transform(v1);

// Or simply the * operator
u1 = B1 * v1;

// Same with Matrix-Matrix multiplication
mat4 R1;
R1 = A1.transform(A2);
R1 = A1 * A2;
```

Matrix transposition:
```c
// transposed() returns the transposed matrix
// without modifying the original matrix
mat3 R2 = R1.transposed();

// Meanwhile transpose() transposes the matrix itself
R1.transpose();
```

Matrix inversion:
```c
// Compute the inverse of a matrix
mat4 I1 = A1.inverse();
```

(UNDER DEVELOPMENT) Matrix determinant:
```c
// Compute the determinant
real D = I1.det();
```

Transformations:
```c
// Get the 3x3 rotation matrix around an arbitrary axis
mat3 Rot1 = mat3::rotation_3x3(PI2, vec3({1, 2, 3}));

// Rotation around the x axis (same for y and z)
mat3 Rot2 = mat3::rotation_3x3_x(PI4);

// 4x4 rotation matrix around the z axis of 10 degrees
// (radians(real) converts degrees to radians, degrees does the opposite)
mat4 Rot3 = mat4::rotation_4x4_z(radians(10));

// 4x4 translation matrix
mat4 T1 = mat4::translation(1, 2, 3);

// perspective(), ortho(), scaling_3x3(), scaling_4x4(),
// lookAt() functions are also supported
```

#### Using complex numbers
Declare a complex number using the `complex` class.
```c
// You can initialize a complex number
// using a bracketed list
complex z1 = {1, 1};

// And you can also access its elements directly
z1.a = 1;
z2.b = 1;

// Or get its real and imaginary part
real x = z1.Re() + z1.Im();

// A complex rotor
complex z2 = complex::rotor(TAU);

// Complex multiplication
complex z3 = z1 * z2;

// Complex conjugate
complex z4 = z1.conjugate();

// Complex analysis functions
complex z5 = ln(z3) + atan(z3);
```
Common complex functions such as `sqrt`, `ln`, complex trigonometric functions and many more are also supported (`complex_analysis.h`).

#### Using quaternions
Declare a quaternion using the `quat` class.
```c
// Initialize a quaternion as 1 + 2i + 3j + 4k
quat q = {1, 2, 3, 4};

// Compute the inverse of a quaternion
quat p = q.inverse();

// Compute the normalized quaternion
quat p_n = p.normalized();

// Normalize the quaternion itself
p.normalize();

// Return a quaternion representing a rotation around an arbitrary axis
quat r = quat::rotation(PI, v);

// Rotate a vector around an arbitrary axis using quaternions
vec3 v_1 = quat::rotate(w, PI / 2.0, v);
```

#### Using polynomials
Polynomials are stored and manipulated using the `polynomial<T = real>` class, where `T` is the type of the coefficients of the polynomial (`real` by default).
```c
// Represents the polynomial 1 + 2x + 3x^2
polynomial<real> P1 = {1, 2, 3};

// x + 2x^3
polynomial<real> P2 = {0, 1, 0, 2};

// Access the polynomial's coefficients
P2[1] = 2;

// Polynomial operations
polynomial<real> Pr = (P1 * P2) + P1;

// Evaluate the polynomial for a given value

// You can use the eval function
real res;
res = Pr.eval(1.0);

// Or simply the () operator
res = Pr(1.0);

// Find the (effective) order of a polynomial
real order = Pr.find_order();

// Trim unneeded higher-order null coefficients
Pr.trim();
```

#### Using statistical functions
The header `statistics/statistics.h` defines many statistical functions of common use. The `vec_buff` type (alias for `std::vector<real>`) is used to store data.

## Progress
Most functionalities have already been implemented, but test cases and documentation are in need of writing. Benchmarking different algorithms may be also used to enchance the library. Core functions which are currently under development are:
- Matrix determinant
- powf, exp, sin/cos/tan (only the **architecture independent** versions, they already work properly on x86 machines)
- asin/acos/atan/atan2 (Inverse trig functions)

See [Future development](https://github.com/mattiaisgro/uroboro/blob/master/README.md#Future development) for information about new features which might get implemented.

## Macros
These are the macros that can be defined to change the library's behaviour:
- **UROBORO_INCLUDE_ALL** - Including `uroboro.h` will include _all_ header files
- **UROBORO_BRANCHLESS** - Branchless implementations will be preferred when possible (and computationally convenient)
- **UROBORO_FORCE_BRANCHLESS** - Branchless implementations will always be used whenever possible
- **UROBORO_THROW_EXCEPTIONS** - Exceptions will be thrown and errno set on error (by default errno is set and NaN is returned)
- **UROBORO_ONLY_EXCEPTIONS** - Exceptions will be thrown on error (without modifying errno)
- **UROBORO_NO_NAMESPACE** - The `umath` namespace alias will not be defined
- **UROBORO_X86** - Assembly x86 implementations will be used whenever possible
- **UROBORO_FLOAT_PREC** - Floating point precision (`float`) will be used for the `real` type (by default `double` is used)
- **UROBORO_LONG_DOUBLE_PREC** - Long double precision (`long double`) will be used
- **UROBORO_ARBITRARY_PREC** - Arbitrary precision will be used (NOT implemented yet)
- See `constants.h` for more specific defines

## Future development
#### In progress
These are features which are currently under development:
- Matrix determinant
- Column/row precedence independence for matrices (let the user decide whether to use column/row-first storage and colexicographical/lexicographical representation)

#### Planned
These are features which will be developed soon:
- Romberg integral approximation
- Pseudorandom Number Generation
- LU decomposition of matrices
- Matrix eigenvalue finding
- Montecarlo integral approximation
- Benchmarking (either implemented or integrated)

#### Potential
These are interesting features which might or might not be implemented in the future.
- Sorting algorithms
- Metropolis pseudorandom number generation
- SSE implementations of vector and matrix operations




