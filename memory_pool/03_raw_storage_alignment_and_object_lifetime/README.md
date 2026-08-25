# Raw Storage, Alignment and Object Lifetime

This stage evolves the templated memory pool from managing preconstructed `T` objects to managing raw memory and explicit object lifetimes.

The previous implementation used:

```cpp
std::unique_ptr<T[]>
```

which meant every `T` object was constructed when the pool itself was created.

That raised an important question:

> What should `release()` do with the state of a generic object?

A generic `MemoryPool<T>` cannot assume every type provides a `reset()` function.

This stage solves that by separating **storage lifetime** from **object lifetime**.

## Previous Design

```text
Pool created
      ↓
T T T T T
      ↓
All T objects already exist
      ↓
acquire()
      ↓
Return an existing T
      ↓
release()
      ↓
T still exists
```

## New Design

```text
Pool created
      ↓
Raw aligned storage

[ ][ ][ ][ ][ ]

      ↓
acquire()
      ↓
Placement new
      ↓
T constructed in a slot
      ↓
release()
      ↓
T destructor called
      ↓
Slot becomes raw storage again
```

The same memory slot can therefore be reused for multiple independent object lifetimes.

## Concepts Introduced

- Raw memory allocation
- `std::byte`
- `sizeof(T)`
- `alignof(T)`
- `std::align_val_t`
- Aligned `operator new`
- Placement `new`
- Variadic templates
- Perfect forwarding
- Explicit destructor calls
- Object lifetime
- Storage reuse


## Key Idea

Reusing memory is not the same thing as reusing an object.

The pool keeps ownership of the raw memory while individual `T` objects are constructed and destroyed inside that memory.
