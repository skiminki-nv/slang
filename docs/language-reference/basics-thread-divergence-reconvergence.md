# Thread Divergence and Reconvergence Model

Invocations are said to be on a uniform path when either their execution has not diverged or it has
reconverged. In structured conditional control flow, divergence occurs when invocations take different control
flow paths based on conditional branches. Invocations reconverge when the conditional control flow branches
join.

Control flow uniformity is considered in the following scopes:
- Dispatch scope: whether all invocations in the dispatch are on a uniform path.
- Workgroup scope: whether all invocations in the workgroup are on a uniform path.
- Subgroup scope: whether all invocations in the subgroup are on a uniform path.
- Mutually convergent invocation sets (within a subgroup): the invocations that are on a mutually uniform
  path, i.e., on the same path.


> 📝 **Remark 1:** All invocations start on a uniform control flow on the shader entry point.

> 📝 **Remark 2:** In SPIR-V terminology: uniform control flow (or converged control flow) is the state when
> all invocations execute the same dynamic instance of an instruction.


## Divergence/Reconvergence in Structured Control Flow

**`if` statements:**

Divergence occurs when some invocations take the *then* branch and the rest take the *else*
branch. Reconvergence occurs when invocations exit the *then* and *else* blocks.

Example 1:
```hlsl
// divergence occurs when not all invocations evaluate
// the condition uniformly as either `true` or `false`
if (cond)
{
    // "then" path
}
else
{
    // "else" path
}
// reconvergence
```

Example 2:
```hlsl
// divergence occurs when not all invocations evaluate
// the condition uniformly as `true` or `false`
if (cond)
{
    // "then" path
}
// reconvergence
```


**`switch` statements:**

Divergence occurs when invocations jump to different case groups. Reconvergence occurs when invocations
exit the switch statement. Reconvergence between invocations on adjacent case label groups occurs on switch
case fall-through.

Case group is the set of case labels that precede the same non-empty statement.

Example 1:
```hlsl
// divergence occurs when not all invocations jump to
// same case label groups:
switch (value)
{
// first case group
case 0:
case 2:
    doSomething1();
    break;

// second case group
case 1:
    doSomething2();
    break;

// third case group
case 3:
default:
    doSomething3();
    break;
}
// reconvergence
```

Example 2:
```hlsl
// divergence occurs when not all invocations jump to
// same case label groups:
switch (value)
{
case 0: // first case group
    doSomething1();
    // fall-through

case 1: // second case group
    // reconvergence between first and second case group
    doSomething2();
    // fall-through

default: // third case group

    // reconvergence between second and third case group
    //
    // all invocations are now on the same path

    doSomething3();
    break;
}
// reconvergence
```

**`while` statements:**

Divergence occurs when some invocations exit the loop while the rest continue. Reconvergence occurs when all
invocations have exited the loop.

Example 1:
```hlsl
[numthreads(1,1,1)]
void computeMain(uint3 threadId : SV_DispatchThreadID)
{
    uint numLoops = 50 + (threadId.x & 1);
    for (uint i = 0; i < threadId.x; ++i)
    {
        // divergence after 50 loops:
        // - even-numbered threads exit the loop
        // - odd-numbered threads continue for one more iteration
    }
    // reconvergence
}
```


# Subgroup Execution Model

A subgroup is a set of up of invocations. All invocations in the subgroup execute in the single instruction,
multiple threads (SIMT) model.

Invocations in a subgroup can synchronize and share data efficiently using subgroup functions such as ballots,
reductions, and similar functions. For example, an atomic memory access to the same location by multiple
invocations in a subgroup can often be coalesced within the subgroup and then performed by a single
invocation. This can significantly reduce the number of atomic memory accesses, and thus, increasing
performance.

The subgroup size is defined by the target. It is a power of two in range [4, 128]. The invocations within a
subgroup belong in one of the following classes:
- *active invocation*---an invocation that participates in producing a result.
- *inactive invocation*---an invocation that does not produce any side effects. An invocation can be inactive
  because the subgroup could not be fully utilized when assigning invocations to subgroups. An invocation can
  also be inactive because an active invocation executed the `discard` statement (fragment shaders).
- *helper invocation*---an invocation that is used to compute derivatives, typically fragment quads. A helper
  invocation does not produce any other side effects, and it does not participate in subgroup functions unless
  otherwise stated.

Despite the SIMT execution model, Slang does not require that subgroup invocations execute synchronously,
unless they are on the same control flow path and they are executing subgroup-synchronous
functions. Subgroup-synchronous functions include functions that operate over subgroups, barriers, and
cooperative vectors.

> 📝 **Remark 1:** In SPIR-V terminology, subgroup-synchronous functions are *tangled instructions* with the
> subgroup scope.


# Examples

