# MoXI Sort Checker

A reference sort checker for MoXI. The following programs are required:

- C compiler
- make
- gperf
- Python (3.9 or newer)
- [Yices2](https://yices.csl.sri.com/) (2.7.0 or newer)

To build, run

    make

This produces the sort checker `build/moxisc`, the shared library
`build/libmoxiparser.so` (`libmoxiparser.dylib` on macOS), and the public header
`build/moxiparser.h`.

The generated program `moxisc` takes a single file as input

    moxisc <filename>

To run the test suite, run

    python3 test/test.py

## Parser API

`libmoxiparser` parses a MoXI file one command at a time and returns
sort-checked Yices terms and types. Link a client against the parser library and
Yices:

    clang -Ibuild client.c -Lbuild -lmoxiparser -lyices -o client

The library lives in `build/`, so set `LD_LIBRARY_PATH` (or `DYLD_LIBRARY_PATH`
on macOS) or pass `-Wl,-rpath,...` when linking.

Initialize Yices before creating a parser. `new_moxi_parser` returns `NULL` if
the file cannot be opened. `moxi_parser_next_command` returns the next command,
or `NULL` at end of file or on a parse/sort error. The command pointer is owned
by the parser and is overwritten on the next call.

    #include <yices.h>
    #include "moxiparser.h"

    yices_init();
    moxi_parser_t *parser = new_moxi_parser("file.moxi");
    moxi_command_t *cmd;

    while ((cmd = moxi_parser_next_command(parser))) {
        switch (cmd->type) {
        case MOXI_SET_LOGIC:         /* cmd->str, cmd->logic */
        case MOXI_DECLARE_SORT:      /* cmd->str, cmd->arity */
        case MOXI_DECLARE_ENUM_SORT: /* cmd->str, cmd->strings */
        case MOXI_DEFINE_SORT:       /* cmd->str, cmd->sorts, cmd->sort */
        case MOXI_DECLARE_CONST:     /* cmd->str, cmd->sort */
        case MOXI_DECLARE_FUN:       /* cmd->str, cmd->sorts, cmd->sort */
        case MOXI_DEFINE_FUN:        /* cmd->str, cmd->terms, cmd->sort, cmd->term */
        case MOXI_DEFINE_SYSTEM:     /* cmd->str, cmd->system */
        case MOXI_CHECK_SYSTEM:      /* cmd->str, cmd->check_system */
        default:
            break;
        }
    }

    delete_moxi_parser(parser);
    yices_exit();

Sorts and formulas are Yices `type_t` and `term_t` values. For
`MOXI_DEFINE_FUN`, `cmd->terms` holds the arguments, `cmd->sort` the return
sort, and `cmd->term` the body.

For `MOXI_DEFINE_SYSTEM`, `cmd->system` has `input`, `output`, and `local`
variable vectors, the `init_term`, `trans_term`, and `inv_term` formulas, and
any `subsystems`. `moxi_term_map_find(system->next_vars, t)` returns the primed
next-state variable for `t`. `MOXI_CHECK_SYSTEM` uses the same variable layout
on `cmd->check_system`, plus named `assumption`, `fairness`, `reachable`, and
`current` formulas and `query` (named lists of those identifiers).

See `examples/echo.c` for a complete client and `src/moxiparser.h` for the
command layout.

We use Yices for term management and therefore sort checking of terms.

Much of the internals are adapted from Yices, including parsing. See [their
source code](https://github.com/SRI-CSL/yices2) (especially the SMTLIB2
sections) for reference.
