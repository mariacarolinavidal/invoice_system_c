# Invoice system in C

This project consists of an invoice system developed in the C programming language to manage a retail inventory, a customer shopping cart, and invoice generation. Implemented for the course Introduction to Algorithms and Data Structures at Instituto Superior Técnico.

##  Features

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
- Full Dynamic Allocation. Without global variables and zero memory leaks.
- Custom sorting algorithms (native `qsort` was intentionally bypassed to build custom sorting logic).
- Robust input parsing capable of handling multi-byte UTF-8 characters and dynamic string inputs with whitespaces (e.g., parsing `"Client Name"`).

## Structure

- `cart.c` - Shopping cart operations.
- `commands.c` - Acts as the command router and input parser.
- `functions.h` - Centralized repository for all forward declarations and module function prototypes.
- `invoice.c` - Invoice management.
- `system.c` - Initializes the core data structures. Ensures a clean program termination by calling memory cleanup routines.
- `products.c` - Handles the product catalog database.
- `project.c` - The entry point of the application. It contains only the main function.
- `project.h` - Global constants and structs. 
- `validations.c` - The core validation layer of the architecture.

## Compilation

gcc -O3 -Wall -Wextra -Werror -Wno-unused-result -o proj *.c

