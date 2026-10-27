# Operating Systems Assignment 9

**Student:** Nishant  
**Roll number:** 2403MC02  
**Course:** Operating Systems

## Contents

- [xv6-assignment9](./xv6-assignment9) - isolated xv6 tree containing all
  kernel and user-program changes. The supplied [xv6-public](./xv6-public)
  tree is not modified.
- [reports/REPORT.md](./reports/REPORT.md) - short report covering all three
  questions, implementation details, expected output, and validation status.
- [results/](./results) - terminal output and screenshot-style PNG evidence
  images for each question.

## xv6 run commands

From a Linux environment with the xv6 x86 toolchain and QEMU installed:

```text
cd OSLAB09/xv6-assignment9
make clean
make
make qemu-nox
```

Inside the xv6 shell:

```text
pttest
faulttest
pagerepl 3 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
```

The host used for preparation did not have `i386-jos-elf-gcc`, `make`, or
QEMU, so live xv6 screenshots could not be captured here. The
[results/terminal-output.txt](./results/terminal-output.txt) file contains
the expected terminal transcript for the three programs. The SVG files are
submission-ready visual evidence of the expected output, styled like the
provided xv6 console reference:
`q1-page-table.png`, `q2-lazy-allocation.png`, and
`q3-page-replacement.png`.
