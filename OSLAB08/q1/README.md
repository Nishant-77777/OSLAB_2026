# Q1 - Inspecting virtual-to-physical translation

**Student:** Nishant (2403MC02)

## Implementation summary

`walkpgdir` is exposed to the system-call path so `va2pa` can inspect the
calling process's page table. The kernel rejects absent mappings and mappings
without `PTE_U`. For a valid page it combines the frame address with the
12-bit page offset and prints the directory index, table index, offset, and
the P/W/U permission bits.

The user test examines one global object, one stack object, and one allocated
heap block. It also checks a deliberately unmapped address. After `fork()`,
the parent and child print the same virtual address for the shared object but
different physical addresses, showing that the child received its own copied
page.

## Run

```text
make clean && make qemu-nox
vatest
```

The output can be compared with the Q1 terminal screenshot in the parent
directory.
