# Invoice system in C

This project consists of an invoice system developed in the C programming language to manage a retail inventory, a customer shopping cart, and invoice generation. Implemented for the course Introduction to Algorithms and Data Structures at Instituto Superior Técnico.

##  Features

The system is controlled via `stdin` using single-letter commands followed by arguments:

- Add or update a product in the catalog.
- List available products (supports `*` and `?` wildcard pattern matching).
- Manage items in the current shopping cart.
- Checkout cart, apply VAT, and issue a sequential invoice.
- List client's invoice storage sorted alphabetically and chronologically.
- Delete a specific invoice or reduce product stock.
- Print financial summaries and active VAT tables.
- Quit the system and safely free all allocated memory.
- Includes custom EAN-8/EAN-13 barcode check-digit validation.
- VAT classes loaded from an external file, or by default if not provided.


## Tech Stack & Architecture

- **Language:** C
- **Compilation:** GCC optimized (`-O3 -Wall -Wextra -Werror`)
- Full Dynamic Allocation. Without global variables and zero memory leaks.
- Custom sorting algorithms (native `qsort` was intentionally bypassed to build custom sorting logic).
- Robust input parsing capable of handling multi-byte UTF-8 characters and dynamic string inputs with whitespaces (e.g., parsing `"Client Name"`).

## Compilation

$ gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj *.c

