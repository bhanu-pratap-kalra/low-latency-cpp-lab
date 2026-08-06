# Memory Pool

This module explores the design and implementation of memory pools in Modern C++.

Rather than jumping directly into a generic allocator, the implementation begins with a simple design dedicated to a single object type `BigObject`. This allows us to focus on the fundamental ideas behind memory pools, allocation, deallocation, memory reuse, ownership, and object lifetime.

---

## Planned Evolution

The implementation will evolve through the following stages:

- Fixed capacity `BigObject` memory pool
- Templated memory pool
- Object construction and destruction using placement `new`
- Alignment and raw storage
- Custom allocation hooks - `operator new` and `operator delete`
- Standard allocator integration
- Polymorphic Memory Resources - `std::pmr`
- Thread aware memory pools
- Benchmarking and performance analysis
- Applying memory pools to reusable objects in performance sensitive systems

