# Fixed Capacity `BigObject` Pool

This first stage implements a simple pool dedicated to one type `BigObject`

The goal is to understand:

- Preallocation
- Slot tracking
- Acquisition and release
- Memory reuse
- Pool exhaustion
- Ownership validation

This version preconstructs every `BigObject` when the pool is created.

## Build and RUN

```bash
make help
```
