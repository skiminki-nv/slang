# Evaluation of Expressions

Expressions in a Slang program are evaluated during compilation, runtime, or both. When an expression can be
evaluated depends on its value evaluation class. All expressions are evaluated during runtime execution at the
latest.

In general, the compiler toolchain that consists of the Slang and the target compilers determines when an
expression is evaluated. Unless otherwise stated, the timing is implementation-defined. In general, the
compilers attempt to move evaluation to compile time when possible, as part of optimization.

An expression can be forced to be evaluated during Slang translation time by assigning it to a `constexpr`
variable, providing it as an argument to a `constexpr` parameter of a function, or providing it as an argument
to a generic value parameter.

When targeting SPIR-V, a global `const` variable can be forced to be a specialization constant with the
[\[SpecializationConstant\](../../../core-module-reference/attributes/specializationconstant-0e.html) or
[\[vk::specialization_constant\]](../../../core-module-reference/attributes/vk_specialization_constant.html)
modifier.


> 📝 **Remark:** Currently, there are no mechanisms to force a value to be evaluated at specific translation
> stage, other than link constant and codegen specialization constant. At this time, the other evaluation
> classes are defined as limits for optimization.


## Value Evaluation Classes

Value evaluation class determines the earliest stage when a value expression can be evaluated. The classes are
as follows:

1. Compile-time constants
   1. Module constants
   2. Link constants
   3. Codegen constants
2. Runtime values
   1. Rate-specified values
      1. Uniform values
      2. Values related to execution hierarchy
         1. Thread-group-rate values
         2. Wave-rate values
      3. Graphics pipeline rate-specified values
      4. Thread-rate values
   2. Non-rate-specified values


### Description

**Module constants**

A value expression can be fully evaluated during module translation. All Slang literals belong in this
category. Module constants do not depend on values in other modules.

**Link constants**

A value expression can be fully evaluated during linking. Link constants may depend on module and link
constants in other modules.

A link constant expression can be forced to be evaluated during linking. This is achieved by assigning the
expression to a `constexpr` variable or by providing it as an argument to a `constexpr` function parameter.
Similarly, the argument expression for a generic value parameter is always forced to be a link constant. In
case the compiler cannot evaluated the expression during linking, an error is raised.

**Codegen constants**

A value expression can be fully evaluated during target program compilation.

A special class of codegen constants are Vulkan specialization constants. See
[\[vk::specialization_constant\]](../../../core-module-reference/attributes/vk_specialization_constant.html)
for details.

**Uniform values**

Uniform values are runtime values. They are immutable over a graphics launch or a compute dispatch. Uniform
values are passed to a Slang program using
[ConstantBuffer\<T\>](../../../core-module-reference/types/constantbuffer-08/index.html),
[ParameterBlock\<T\>](../../../core-module-reference/types/parameterblock-09/index.html), non-static global
`const` variables, or `uniform` entry point parameters.

**Thread-group-rate values**

These values are defined per thread group instance. For example, the thread group identifier `SV_GroupIndex`.

**Wave-rate values**

These values are defined per wave instance. For example, the return value of `WaveGetActiveMask()` on the
[wave-uniform](basics-execution-divergence-reconvergence.md) path.

**Thread-rate values**

These values are defined per thread instance. For example, the thread identifiers `SV_DispatchThreadID` and
`SV_GroupThreadID`.

**Graphics pipeline rate-specified values**

The graphics pipeline has a number of rate-specified values, often related to a specific stage. For example,
inputs from a previous stage or system-value semantics such as `SV_Position`, `SV_IsFrontFace`, and
`SV_VertexID`. The rate-specified values are not further classified here.

**Non-rate-specified values**

This class is for values that do not belong in any of the aforementioned classes. For example, a read access
to a mutable shared variable that is not rate-specified results results in a non-rate-specified value.


> 📝 **Remark 1:** The semantics of Slang `constexpr` differ from C++ `constexpr`. In Slang, it is an error if
> a `constexpr`-variable cannot be evaluated as a link constant. The closest equivalent in C++ is `consteval`.

> 📝 **Remark 2:** Rate in "rate-specified" refers to the rate of change.

> 📝 **Remark 3:** Immutable (`const`) variables are separate from value classification. An immutable variable
> can hold a constant, rate-specified value, or a runtime value.

> 📝 **Remark 4:** TODO: Add references to system-value semantics.


### Value Evaluation Class Hierarchy

Value evaluation classes are partially ordered as per the table below, higher position signifying higher
"constant-ness". Ordering is not defined between values related to execution hierarchy (thread-group-rate,
wave-rate) and graphics pipeline rate-specified values.

<table>
<tr>
  <td rowspan=3>Constants</td>
  <td colspan=2>Module constants</td>
</tr>
<tr>
  <td colspan=2>Link constants</td>
</tr>
<tr>
  <td colspan=2>Codegen constants</td>
</tr>
<tr>
  <td rowspan=5>Runtime values</td>
  <td colspan=2>Uniform values</td>
</tr>
<tr>
  <td>Thread-group-rate values</td>
  <td rowspan=2>Graphics pipeline rate-specified values</td>
</tr>
<tr>
  <td>Wave-rate values</td>
</tr>
<tr>
  <td colspan=2>Thread-rate values</td>
</tr>
<tr>
  <td colspan=2>Non-rate-specified values</td>
</tr>
</table>

A value expression of a higher class is always eligible to be evaluated as a value expression of a lower
class.

> 📝 **Remark:** In the value evaluation class hierarchy, all module constants are also link constants. All
> link constants are also codegen constants. Are constants are also runtime values. The class of
> non-rate-specified values encompasses all other classes.


## Value Evaluation Class of an Expression

The value evaluation class of an expression depends on:
- the value evaluation classes of the inputs values
- the value evaluation classes of the functions and operations in the expression

The value evaluation class of an expression is the highest class in the hierarchy such that no part in the
expression has a lower value evaluation class.

> 📝 **Remark:** An expression that consists of wave-rate values, graphics pipeline rate-specified values, and
> compile-time evaluatable functions has the value evaluation class of thread-rate values. This is the highest
> class that is equal or lower than any component in the expression.

