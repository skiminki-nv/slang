# Generics

Generics in Slang enable compile-time parameterization of [structures](types-struct.md),
[interfaces](types-interface.md), [type aliases (TODO)](types-alias.md), [functions and member functions](TODO.md),
[subscript operators](types-struct.md#subscript-op), and
[constructors](types-struct.md#constructor). Parameterization is allowed for types and
`uint`/`int`/`bool`-typed values. In addition, Slang supports [generic structure
extension](types-extension.md#generic-struct) covered in [type extensions](types-extension.md).

When a generic type or function is instantiated, the compile-time parameters are bound. An instantiated
generic is a concrete type or function, which can be used as any other concrete type or
function. Conceptually, partial parameter binding can be done by defining a generic type alias for a generic
object, but this does not instantiate a generic.

Slang does not directly support specialization or partial specialization of generics. However, [generic struct
extension](types-extension.md#generic-struct) can be used to extend generic structures much to the same
effect.


## Syntax

Generic [structure](types-struct.md) syntax:
> **`'struct'`** [*`identifier`*] [*`generic-params-decl`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`':'`** *`bases-clause`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`'where'`** *`where-clause`*)\*<br>
> **`'{'`** *`member-list`* **`'}'`**

Generic [interface](types-interface.md) syntax:
> **`'interface'`** *`identifier`* [*`generic-params-decl`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`':'`** *`bases-clause`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`'where'`** *`where-clause`*)\*<br>
> **`'{'`** *`member-list`* **`'}'`**

Generic type alias syntax:
> **`'typealias'`** *`identifier`* [*`generic-params-decl`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'='`** *`simple-type-spec`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`'where'`** *`where-clause`*)\* **`';'`**

Generic function and member function declaration: (traditional syntax)
> *`simple-type-spec`* *`identifier`* [*`generic-params-decl`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'('`** *`param-list`* **`')'`**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`'where'`** *`where-clause`*)\*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`';'`** | **`'{'`** *`body-stmt`*\*  **`'}'`**)

Generic function and member function declaration: (modern syntax)
> **`'func'`** *`identifier`* [*`generic-params-decl`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'('`** *`param-list`* **`')'`**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`'throws'`** *`simple-type-spec`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`'->'`** *`simple-type-spec`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`'where'`** *`where-clause`*)\*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`';'`** | **`'{'`** *`body-stmt`*\*  **`'}'`**)

Generic [constructor](types-struct.md#constructor) declaration:
> **`'__init'`** [*`generic-params-decl`*] **`'('`** *`param-list`* **`')'`**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`'where'`** *`where-clause`*)\*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`';'`** | **`'{'`** *`body-stmt`*\*  **`'}'`**)

Generic [subscript operator](types-struct.md#subscript-op) declaration:
> **`'__subscript'`** [*`generic-params-decl`*] **`'('`** *`param-list`* **`')'`**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'->'`** *`simple-type-spec`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`'where'`** *`where-clause`*)\*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`';'`** | **`'{'`** *`body-stmt`*\*  **`'}'`**)

Generic parameters declaration:
> *`generic-params-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'<'`** [*`generic-param-decl`* (**`','`** *`generic-param-decl`*)\* ] **`'>'`**
>
> *`generic-param-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`generic-value-param-decl`* |<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`generic-value-param-trad-decl`* |<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`generic-type-param-decl`* |<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`generic-type-param-pack-decl`*
>
> *`generic-value-param-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'let'`** *`identifier`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`':'`** *`simple-type-spec`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`'='`** *`init-expr`*]<br>
>
> *`generic-value-param-trad-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`simple-type-spec`* <br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`identifier`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`'='`** *`init-expr`*]<br>
>
> *`generic-type-param-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`'typename'`**] *`identifier`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`':'`** *`simple-type-spec`*]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`'='`** *`simple-type-spec`*]<br>
>
> *`generic-type-param-pack-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'each'`** *`identifier`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`':'`** *`simple-type-spec`*]<br>


Generic parameter conformance clause:
> *`where-clause`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`'optional'`**]<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`simple-type-spec`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(*`generic-type-constraint-decl`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|*`generic-type-constraint-eq-decl`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|*`generic-type-constraint-coercion-decl`*)<br>
>
> *`generic-type-constraint-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`':'`**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`simple-type-spec`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;(**`','`** *`simple-type-spec`*)\*<br>
>
> *`generic-type-constraint-eq-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'=='`**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`simple-type-spec`*<br>
>
> *`generic-type-constraint-coercion-decl`* =<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`'('`**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`simple-type-spec`*<br>
> &nbsp;&nbsp;&nbsp;&nbsp;**`')'`**<br>
> &nbsp;&nbsp;&nbsp;&nbsp;[**`'implicit'`**]<br>

> 📝 **Remark:** Generic enumeration declarations are also supported by the parser. However, they are not
> currently useful. See GitHub issue [#10078](https://github.com/shader-slang/slang/issues/10078).

### Parameters

- *`generic-params-decl`* declares a list of generic parameters.
- *`generic-param-decl`* declares a generic value, type, or type parameter pack.
- *`generic-value-param-decl`* declares a generic value parameter.
- *`generic-value-param-trad-decl`* declares a generic value parameter using traditional syntax.
- *`generic-type-param-decl`* declares a generic type parameter.
- *`generic-type-param-pack-decl`* declares a generic type parameter pack.
- *`where-clause`* is a generic parameter conformance clause.
- *`generic-type-constraint-decl`* is a generic parameter conformance clause, requiring the declared parameter
  to be a subtype of one or more constraining type expressions.
- *`generic-type-constraint-eq-decl`* is a generic parameter conformance clause, requiring the declared parameter
  to be equal to the constraining type expression.
- *`generic-type-constraint-coercion-decl`* is a generic parameter conformance clause, requiring the declared parameter
  to be coercible. This constraint may be used only in [generic structure extensions](types-extension.md#generic-struct).
  See GitHub issue [#10087](https://github.com/shader-slang/slang/issues/10087).
- *`identifier`*: see the respective syntax for a description.
- *`bases-clause`*: see the respective syntax for a description.
- *`member-list`*: see the respective syntax for a description.
- *`simple-type-spec`*: see the respective syntax for a description.
- *`param-list`*: see the respective syntax for a description.
- *`body-stmt`*: see the respective syntax for a description.


## Description

A generic parameter declaration list *`generic-params-decl`* adds any number of compile-time parameters to
structures and functions. These parameterized constructs are called *generic structures* and *generic
functions*.

A generic parameter declaration is one of:

- Generic value parameter declaration *`generic-value-param-decl`*, which adds a value parameter with an optional
  default value. The value type must be one of `bool`, `int`, `uint`.
- Generic type parameter declaration *`generic-type-param-decl`*, which adds a type parameter with optional
  type constraint and default type. The keyword `typename` is optional.
- Generic type parameter pack declaration *`generic-type-param-pack-decl`*, which adds a type parameter
  pack. A type parameter pack is a variable-length list of types.

Types may be constrained by:

- Specifying an inline type constraint in *`generic-type-param-decl`* using the form `TypeParam :
  ParentType`. This adds a single conformance requirement such that `TypeParam` must be a subtype of
  `ParentType`.
- Specifying one or more `where` clauses (*`where-clause`*). A `where` clause adds a single requirement using
  one of the following forms:
  - Conformance declaration *`generic-type-constraint-decl`* adds a requirement that the left-hand-side type
    expression must conform to the right-hand-side type expression.
  - Equivalence declaration *`generic-type-constraint-eq-decl`* adds a requirement that the left-hand-side
    type expression must be equal to the right-hand-side type expression.
  - Coercion declaration *`generic-type-constraint-coercion-decl`* adds a requirement that the parenthesized
    type expression must be convertible to the left-hand-side type expression.

Conformance and equivalence requirements may be declared as optional. When optional, expression `ParamType is
ParentType` returns `true` when `ParamType` conforms to or equals `ParentType`. When the expression is used in
an `if` statement using the form `if (ParamType is ParentType) { ... }`, then any variable of type `ParamType` may
be used as type `ParentType` in the "then" branch.

The coercion requirement is usable only in generic structure extensions.

Value parameters cannot be constrained.

> 📝 **Remark 1:** In Slang, a conformance requirement `TypeParam : ParentType` means that `TypeParam` must inherit
> from `ParentType`, and `ParentType` must be an interface.

> 📝 **Remark 2:** Slang also has the `__generic` modifier, which can be used to declare generic parameters as
> an alternative for *`generic-params-decl`*. Using *`generic-params-decl`* is recommended.


### Type Parameter Packs {#type-param-packs}

A type parameter pack is declared using the `each TypeIdentifier` syntax. When a generic construct is instantiated, the
parameter pack is bound with a (possibly empty) sequence of types.

A type parameter pack is expanded using the `expand`/`each` construct using the following syntax:

> Expand-expression:<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`expand-expr`* = **`'expand'`** *`expr`*

> Each-expression:<br>
> &nbsp;&nbsp;&nbsp;&nbsp;*`each-expr`* = **`'each'`** *`expr`*

An each-expression evaluates to a type parameter pack, tuple, or a type-parameter-pack-typed variable.

There must be at least one each-expression within an expand-expression. An each-expression must always be
enclosed within an expand-expression except in a generic type declaration. If there are multiple
each-expressions within an expand-expression, they must all have an equal number of type parameters.

An expand-expression evaluates to a comma-separated value sequence whose length is the number of type
parameters of the embedded each-expressions. The sequence elements are the expand expressions with
each-expressions substituted by the Nth element of the evaluated tuple or a type-parameter-pack-typed variable.

That is, `expand-expr` is substituted with the following sequence:

```hlsl
expr, // every each-expr is substituted by pack element 0
expr, // every each-expr is substituted by pack element 1
expr, // every each-expr is substituted by pack element 2
...
expr  // every each-expr is substituted by pack element N-1
```

In function parameter lists, expand/each constructs must come after all other parameters. There may be
multiple declared expand/each parameters, in which case the type parameter packs must have equal lengths.


## Type Checking

Type checking of generic type parameters is performed based on their type constraints, and it is performed
before instantiation. In general, an operation on a generic type or a generic-typed variable is legal if it is
legal for all possible concrete types conforming to the declared constraints.

The rules are as follows:
- If a generic type parameter `T` has an equality constraint `T == U`, type `T` is considered to be type `U`
  for all intents and purposes.
- If a generic type parameter `T` has a type constraint `T : Parent`, type `T` is considered to be a subtype
  of `Parent`. That is, all inherited members of `Parent` are accessible via generic type parameter `T`.
- If a generic type parameter `T` has a type constraint `U(T)`, type `T` may be converted to type `U`.

Type constraints may be declared also for the associated types. For example, `where T : Parent where T.AssocT
== int` requires that `AssocT` is `int`. Note that `Parent` must declare associated type `AssocT`. (See
[interfaces](types-interface.md) for associated type declarations.)

No assumptions are made about generic value parameters other than their declared type.


> 📝 **Remark:** In contrast to C++ templates, type checking of Slang generics is performed before the
> compile-time parameters are bound. In C++, type checking is performed during template instantiation.


## Parameter Binding

Compile-time parameters for a generic can be bound either explicitly, implicitly, or as a combination of
both. Binding is done at the call site.

In explicit binding, the generic parameters are listed in angle brackets after the generic type or function
identifier. Explicit binding cannot be used in constructs that do not use a named identifier at call sites
(e.g., operator overloading).

In implicit binding, generic parameters are inferred. Inference is performed by matching the generic
parameters against the call site parameter types. It is an error if a parameter cannot be inferred from the
call site.

In case generic parameter inference is ambiguous for a type parameter, the following rules are used
to determine the type:
- In case all inferred types are [fundamental scalar types](types-fundamental.md#scalar) or
  [vector types](types-vector-and-matrix.md) of the same length, the element type with the highest promotion rank is
  used. The promotion ranks from the lowest to the highest are: `int8_t`, `uint8_t`, `int16_t`,
  `uint16_t`, `int32_t`, `uint32_t`, `int64_t`, `uint64_t`, `float`, `double`.
  - A fundamental type is promoted to a 1-dimensional vector if necessary.
- In all other cases, an ambiguous type parameter is an error.

It is an error when inference yields multiple options for a generic value parameter.

Mixing explicit and implicit parameter binding is allowed. The left-most generic parameters use the provided
explicit parameter bindings and the rest are inferred.

> 📝 **Remark:** In case the generic parameter inference is ambiguous and `bool` is inferred as a fundamental
> or element type, the behavior is currently undefined. See GitHub issue
> [#10164](https://github.com/shader-slang/slang/issues/10164) for details.


## Examples

### Generic structure with type and value parameters
```hlsl
RWStructuredBuffer<float> outputBuffer;

struct TestStruct<T, let size : uint>
{
    var arr : T[size];
}

[numthreads(10,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{

    TestStruct<float, 10> obj =
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    outputBuffer[id.x] = obj.arr[id.x];
}
```

### Generic type alias for partial type binding
```hlsl
struct ArrayOfElements<T, let size : uint>
{
    typealias ElementType = T;
    ElementType elems[size];
}

typealias ArrayOfFiveElements<T> = ArrayOfElements<T, 5>;

RWStructuredBuffer<float> outputBuffer;

[numthreads(5,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    ArrayOfFiveElements<float> myArray =
        { 1.0, 2.0, 3.0, 4.0, 5.0 };

    outputBuffer[id.x] = myArray.elems[id.x];
}
```

### Generic function with type parameter
```hlsl
RWStructuredBuffer<float> outputBuffer;

// Note: IFunc<ReturnType, Param1Type, Param2Type, ...>
func performOp<T>(IFunc<T, T, T> binaryOp, T a, T b) -> T
{
    return binaryOp(a, b);
}

func add2<T : IArithmetic>(T a, T b) -> T
{
    return a + b;
}

struct Adder : IFunc<int, int, int>
{
    int bias;

    int operator() (int a, int b)
    {
        return add2(add2(a, b), bias);
    }
}

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    Adder addTwoWithBias = { 5 };
    outputBuffer[id.x] = performOp(addTwoWithBias, 234, 456);
}
```

### Generic constructor
```hlsl
RWStructuredBuffer<float> outputBuffer;

struct TestStruct
{
    int val;

    __init<T : IInteger>(T initval1, T initval2)
    {
        T tmp = initval1 + initval2;
        val = tmp.toInt();
    }
}

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    TestStruct obj = { 123, 345 };
    outputBuffer[id.x] = obj.val;
}
```

### Generic subscript
```hlsl
struct TestStruct
{
    var arr : float[10];

    // declare a 1-parameter subscript operator
    __subscript<T> (T i) -> float where T : IInteger
    {
        get { return arr[i.toInt()]; }
        set { arr[i.toInt()] = newValue; }
    }
}
```

### Type constraint
```hlsl
RWStructuredBuffer<float> outputBuffer;

func addTwo<T>(T a, T b) -> T where T : IArithmetic
{
    return a + b;
}

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    outputBuffer[id.x]  = addTwo(1, 2);
    outputBuffer[id.x] += addTwo(1.1, 2.2);
}
```

### Optional type constraint
```hlsl
RWStructuredBuffer<float> outputBuffer;

func addTwoIfInts<T>(T a, T b) -> T
    where T : IArithmetic       // T must be IArithmetic
    where optional T : IInteger // T may also be IInteger
{
    if (T is IInteger)
        return a + b; // return sum if T is IInteger
    else
        return a - a; // return 0 otherwise
}

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    outputBuffer[id.x]  = addTwoIfInts(1, 2);
    outputBuffer[id.x] += addTwoIfInts(1.1, 2.2);
}
```

### Type equality constraint for associated type
```hlsl
RWStructuredBuffer<float> outputBuffer;

interface IFace
{
    associatedtype PropertyType;
    property prop : PropertyType;
}

struct IntProperty : IFace
{
    typealias PropertyType = int;
    PropertyType prop;
}

struct FloatProperty : IFace
{
    typealias PropertyType = float;
    PropertyType prop;
}

int addTwoInts<T:IFace>(T a, T b) where T.PropertyType == int
{
    return a.prop + b.prop;
}

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    IntProperty intObj1 = { 1 };
    IntProperty intObj2 = { 2 };

    FloatProperty floatObj1 = { 1.0 };
    FloatProperty floatObj2 = { 2.0 };

    outputBuffer[id.x]  = addTwoInts(intObj1, intObj2);

    // FloatProperty does not conform to equivalence requirement
    // "T.PropertyType == int". Hence, the following line will
    // not compile.
    // outputBuffer[id.x] += addTwoInts(floatObj1, floatObj2);
}
```

### Type coercion constraint
```hlsl
struct Foo<A>
{
}

// adds method to the struct if A is convertible to int
extension<A> Foo<A> where int(A)
{
    int extensionMethod(int x)
    {
        return x + 42;
    }
}
```

### Type parameter pack
```hlsl
RWStructuredBuffer<float> outputBuffer;

void sumHelper(inout int acc, int term)
{
    acc += term;
}

int sumInts<each T>(expand each T terms) where T == int
{
    int acc = 0;
    expand sumHelper(acc, each terms);

    // expands to:
    //
    // sumHelper(acc, term0),
    // sumHelper(acc, term1),
    // ...

    return acc;
}

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    outputBuffer[id.x] += sumInts(1, 2, 3, 4, 5, 6, 7);
}
```

### Multiple function parameter packs
```hlsl
void dotProductHelper(float a, float b, inout float ret)
{
    ret += a * b;
}

float dotProduct<each T>
    (expand each T a, expand each T b)
    where T == float
{
   float r = 0.0f;

   expand dotProductHelper(each a, each b, r);
   return r;
}

RWStructuredBuffer<double> outputBuffer;

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    float a[3] = { 4, 5, 6 };
    float b[3] = { 2, 1, 0 };

    outputBuffer[0] =
        dotProduct(
            a[0], a[1], a[2],
            b[0], b[1], b[2]);
}
```

### Generic struct extension for generic types
```hlsl
struct GenericStruct<T>
{
}

extension<T> GenericStruct<T> where T : IFloat
{
    int isInt() { return 0; }
}

extension<T> GenericStruct<T> where T : IInteger
{
    int isInt() { return 1; }
}

RWStructuredBuffer<int> outputBuffer;

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    GenericStruct<float> floatObj = { };
    GenericStruct<uint> uintObj = { };

    outputBuffer[0] = floatObj.isInt();
    outputBuffer[1] = uintObj.isInt();
}
```
> 📝 **Remark:** An extension cannot currently be used to override a more generic implementation.
> See GitHub issue [#10146](https://github.com/shader-slang/slang/issues/10146).

### Explicit and implicit parameter binding

```hlsl
// Return type Ret is listed first, since it cannot be
// inferred.
func diffSingle<Ret : IInteger, T : IInteger, U : IInteger>
    (T a, U b) -> Ret
{
    return Ret(a.toInt64() - b.toInt64());
}

RWStructuredBuffer<int> outputBuffer;

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    int8_t a = 3;
    int16_t b = 5;

    // Return type Ret is bound explicitly. Parameter
    // types T and U can be inferred from function
    // arguments
    outputBuffer[0] = diffSingle<int32_t>(a, b);
}
```

### Implicit parameter binding for type and value
```hlsl
ElementType sumElements<ElementType : IArithmetic, let N : uint>
    (ElementType arr[N])
{
    ElementType acc = arr[0];

    for (uint i = 1; i < N; ++i)
        acc = acc + arr[i];

    return acc;
}

RWStructuredBuffer<int> outputBuffer;

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    int elements[7] = { 1,2,3,4,5,6,7 };
    int empty[0] = { };

    // generic parameters ElementType and N are bound implicitly
    outputBuffer[0] = sumElements(elements);
}
```

### Implicit parameter binding, ambiguous value parameter
```hlsl
uint len<let N : uint>(int[N] arr, int[N] arr2)
{
    return N;
}

RWStructuredBuffer<uint> outputBuffer;

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    int arr1[7] = { };
    int arr2[6] = { };

    // error: generic parameter N cannot be inferred
    outputBuffer[0] = len(arr1, arr2);
}
```

### Implicit parameter binding, ambiguous type parameter
```hlsl
interface IBase
{
}

struct A : IBase
{
}

struct B : IBase
{
}

void testFunc<T>(T x, T y)
{
}

[numthreads(1,1,1)]
void main(uint3 id : SV_DispatchThreadID)
{
    A a = { };
    B b = { };

    // Explicit parameter type binding must be used,
    // since implicit type for T is ambiguous and
    // non-fundamental.
    testFunc<IBase>(a, b);
}
```
