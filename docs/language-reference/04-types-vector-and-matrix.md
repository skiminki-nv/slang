Vector Types
------------

A `vector<T, N>` represents a vector of `N` elements of type `T` where:
- `T` is a [fundamental scalar type](04-types-fundamental.md)
- `N` is a [specialization-time constant integer](TODO-Generics.md) in range [1, 4] denoting the number of elements.

The default values for `T` and `N` are `float` and `4`. This is for backwards compatibility.

An element of a vector is accessed as follows:
- Using the subscription operator `[]` (index 0 denotes the first element)
- Using the member access operator `.` where the elements are named `x`, `y`, `z`, `w`.

Example:
```hlsl
vector<int, 4> v = { 1, 2, 3, 4 };

int tmp;

tmp = v[0]; // tmp is 1
tmp = v.w;  // tmp is 4
v[1] = 9;   // v is { 1, 9, 3, 4 };
```

Multiple elements may be referenced by specifying two or more elements after the member access operator. This can be used to:
- Extract multiple elements. The resulting type is a vector with the size equal to the number of selected elements. The same element may be specified multiple times.
- Assign multiple elements using a vector with the size equal to the number of selected elements. The elements must be unique.

Example:
```hlsl
vector<int, 4> v = { 1, 2, 3, 4 };

int2 tmp2;
int3 tmp3;

tmp2 = v.xy;                   // tmp2 is { 1, 2 }
tmp3 = v.xww;                  // tmp3 is { 1, 4, 4 }
v.xz = vector<int, 2>(-1, -3); // v becomes { -1, 2, -3, 4 }
```

When applying an unary arithmetic operator, the operation applies to all vector elements.

Example:
```hlsl
vector<int, 4> v = { 1, 2, 3, 4 };

vector<int, 4> tmp;
tmp = -v;   // tmp is { -1, -2, -3, -4 };
```

When applying a binary arithmetic operator where the other operand is scalar, the operation applies to all vector elements with the scalar parameter.

Example:
```hlsl
vector<int, 4> v = { 1, 2, 3, 4 };

vector<int, 4> tmp;
tmp = v - 1;   // tmp is { 0, 1, 2, 3 };
tmp = 4 - v;   // tmp is { 4, 3, 2, 1 };
```

When applying a binary assignment operator where the right-hand operand is scalar, the assignment applies to all vector element with the scalar parameter.

Example:
```hlsl
vector<int, 4> v = { 1, 2, 3, 4 };

v += 1;     // v becomes { 2, 3, 4, 5 };
v = 42;     // v becomes { 42, 42, 42, 42 };
```

When applying a binary arithmetic, assignment, or comparison operator with two vectors of same length, the operator is applied element-wise.

Example:
```hlsl
vector<int, 4> v1 = { 1, 2, 3, 4 };
vector<int, 4> v2 = { 5, 6, 7, 8 };

vector<int, 4> tmp;
tmp = v1;       // tmp is { 1, 2, 3, 4 };
tmp = v1 + v2;  // tmp is { 6, 8, 10, 12 };
tmp = v1 * v2;  // tmp is { 5, 12, 21, 32 };

vector<bool, 4> cmpResult;
cmpResult = (v1 == vector<int, 4>(1, 3, 2, 4)); // cmpResult is { true, false, false, true }

v1 -= v2;       // v1 becomes { -4, -4, -4, -4 };
```

The memory layout of a vector type is `N` contiguous values of type `T` with no padding.

The alignment of a vector type is target-defined. The alignment of `vector<T, N>` is at least the alignment of `T` and at most `N` times the alignment of `T`.

Slang provides type aliases for all vectors between size 1 and 4 for fundamental scalar types. The type alias has name `<fundamental_type>N` where `<fundamental_type>` is one of the fundamental types and `N` is the vector length.

Example:
```hlsl
float4 v = { 1.0f, 2.0f, 3.0f, 4.0f }; // vector<float, 4>
int32_t2 i2 = { 1, 2 }; // vector<int, 2>
bool3 b3 = { true, false, false }; // vector<bool, 3>
```


Matrix Types (TODO)
-------------------

A matrix type is written as `matrix<T, R, C>` and represents a matrix of `R` rows and `C` columns, with elements of type `T`.
The element type `T` must be one of the built-in scalar types.
The _row count_ `R` and _column count_ `C` must be specialization-time constant integers.
The row count and column count must each be between 2 and 4, respectively.

A matrix type allows subscripting of its rows, similar to an `R`-element array of `vector<T,C>` elements.
A matrix type also supports element-wise arithmetic.

Matrix types support both _row-major_ and _column-major_ memory layout.
Implementations may support command-line flags or API options to control the default layout to use for matrices.

> Note: Slang currently does *not* support the HLSL `row_major` and `column_major` modifiers to set the layout used for specific declarations.

Under row-major layout, a matrix is laid out in memory equivalently to an `R`-element array of `vector<T,C>` elements.

Under column-major layout, a matrix is laid out in memory equivalent to the row-major layout of its transpose.
This means it will be laid out equivalently to a `C`-element array of `vector<T,R>` elements.

As a convenience, Slang defines built-in type aliases for matrices of the built-in scalar types.
E.g., declarations equivalent to the following are provided by the Slang core module:

```hlsl
typealias float3x4 = matrix<float, 3, 4>;
typealias int64_t4x2 = matrix<int64_t, 4, 2>;
```

> Note: For programmers using OpenGL or Vulkan as their graphics API, and/or who are used to the GLSL language,
> it is important to recognize that the equivalent of a GLSL `mat3x4` is a Slang `float3x4`.
> This is despite the fact that GLSL defines a `mat3x4` as having 3 *columns* and 4 *rows*, while a Slang `float3x4` is defined as having 3 rows and 4 columns.
> This convention means that wherever Slang refers to "rows" or "columns" of a matrix, the equivalent terms in the GLSL, SPIR-V, OpenGL, and Vulkan specifications are "column" and "row" respectively (*including* in the compound terms of "row-major" and "column-major")
> While it may seem that this choice of convention is confusing, it is necessary to ensure that subscripting with `[]` can be efficiently implemented on all target platforms.
> This decision in the Slang language is consistent with the compilation of HLSL to SPIR-V performed by other compilers.


### Matrix Operations

Matrix types support several operations:

* Element-wise operations (addition, subtraction, multiplication) using the standard operators (`+`, `-`, `*`). These operations require matrices of the same dimensions.
* Algebraic matrix-matrix multiplication using the `mul()` function, which supports matrices of compatible dimensions (e.g., `float2x3 * float3x4`).
* Matrix-vector multiplication using `mul()`, where the vector can be interpreted as either a row or column vector depending on the parameter order:
  * `mul(v, m)` - v is interpreted as a row vector
  * `mul(m, v)` - v is interpreted as a column vector

For proper matrix multiplication, always use the `mul()` function. The `operator*` performs element-wise multiplication and should only be used when you want to multiply corresponding elements of same-sized matrices.

> Note: This differs from GLSL, where the `*` operator performs matrix multiplication. When porting code from GLSL or CUDA to Slang, you'll need to replace matrix multiplications using `*` with calls to `mul()`.

### Legacy Syntax

For compatibility with older codebases, the generic `matrix` type includes default values for `T`, `R`, and `C`, being declared as:

```hlsl
struct matrix<T = float, let R : int = 4, let C : int = 4> { ... }
```

This means that the bare name `matrix` may be used as a type equivalent to `float4x4`:

```hlsl
// All of these variables have the same type
matrix a;
float4x4 b;
matrix<float, 4, 4> c;
```
