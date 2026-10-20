/**
 *
 */
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdbool.h>
#include <yices.h>

#include "moxi/logic.h"
#include "moxi/command.h"
#include "parse/token.h"
#include "util/ptr_stack.h"
#include "util/str_map.h"
#include "util/str_vector.h"

// A MoXI sort is a Yices type
// typedef type_t sort_t;

typedef enum {
    INPUT_VAR,
    OUTPUT_VAR,
    LOCAL_VAR,
    LOGIC_VAR // Let-bound, quantified, and define-fun variables (can not be
              // primed)
} var_kind_t;

// Stores a variable's kind and sort
typedef struct var_table_entry {
    var_kind_t kind;
    term_t term;
    bool is_primed;
} var_table_entry_t;

static inline bool symbol_is_primed(const char *str, size_t len)
{
    return str[len - 1] == '\'';
}

typedef struct sys_table_entry {
    char *name;
    size_t len;
    size_t ninput;
    term_t *input;
    term_t *input_next;
    size_t noutput;
    term_t *output;
    term_t *output_next;
    size_t nlocal;
    term_t *local;
    term_t *local_next;
} sys_table_entry_t;

/**
 * Stores information on symbols and their interpretations.
 *
 * The context stores the following tables:
 * - sys_table: maps strings to the system signatures
 */
typedef struct moxi_context {
    int status;
    uint32_t sort_var_id;
    const logic_t *logic;
    bool active_theory_symbols[NUM_THEORY_SYMBOLS];

    // Table for defined systems
    str_map_t sys_table;

    // Maps variable symbols to their kind (input, output, local) and sort.
    str_map_t var_table;

    // Stack of string vectors for storing scoped variables. When a new scope is
    // entered, a new string vector is pushed onto the stack. When a scope is
    // exited, the top string vector is popped off the stack and all variables
    // in the top vector are removed from the context.
    ptr_stack_t scope_stack;

    // Stores the symbols of all sort variables
    str_vector_t sort_var_symbols;

    // Most recently-processed command type
    cmd_type_t cur_cmd_type;

    // Most recently-processed command
    command_t cur_cmd;
} moxi_context_t;

void init_context(moxi_context_t *ctx);
void delete_context(moxi_context_t *ctx);

void moxi_set_logic(moxi_context_t *ctx, char *symbol);

bool is_active_theory_term(const moxi_context_t *ctx, const char *symbol);
bool is_active_theory_sort(const moxi_context_t *ctx, const char *symbol);
bool is_active_term(const moxi_context_t *ctx, const char *symbol);
bool is_active_sort(const moxi_context_t *ctx, const char *symbol);
theory_symbol_type_t get_theory_symbol_type(const moxi_context_t *ctx, const char *symbol);

/*******************************************************************************
 * Scope Management
 *
 * - Push/pop scopes.
 * - Add variables to the context and current scope.
 * - Find variables in the context.
 ******************************************************************************/

/**
 * Pushes a new scope for variables onto the context.
 */
void moxi_push_scope(moxi_context_t *ctx);

/**
 * Pops the top scope for variables from the context.
 */
void moxi_pop_scope(moxi_context_t *ctx);

/**
 * Returns the top scope for variables in the context.
 */
str_vector_t *moxi_get_scope(moxi_context_t *ctx);

/**
 * Adds `symbol` as a named term to the term table, variable table, and the current scope.
 */
void moxi_add_named_term(moxi_context_t *ctx, char *str, term_t term, var_kind_t kind);

/**
 * Finds a variable in the context. Returns `NULL` if the variable is not found.
 */
const var_table_entry_t *moxi_find_var(moxi_context_t *ctx, char *symbol);
void moxi_add_named_sort(moxi_context_t *ctx, char *str, sort_t sort);
void moxi_clear_sort_vars(moxi_context_t *ctx);

/*******************************************************************************
 * Sort Management
 *
 * - Add sorts to the context.
 * - Find sorts in the context.
 ******************************************************************************/

/**
 * Declares a new sort in the context.
 */
void moxi_declare_sort(moxi_context_t *ctx, char *str, size_t arity);

/**
 * Declares a new enum sort in the context.
 */
void moxi_declare_enum_sort(moxi_context_t *ctx, char *str, size_t nvalues, char **values);

/**
 * Defines a new sort in the context.
 */
void moxi_define_sort(moxi_context_t *ctx, char *str, size_t nargs,
                      sort_t *args, sort_t body);

/*******************************************************************************
* Term Management
*
* - Add functions/constants to the context.
* - Find terms in the context.
******************************************************************************/

/**
 * Declares a new constant in the context.
 */
void moxi_declare_const(moxi_context_t *ctx, char *str, sort_t sort);

/**
* Declares a new function in the context.
*/
void moxi_declare_fun(moxi_context_t *ctx, char *symbol, size_t nargs,
                    sort_t *args, sort_t ret);

/**
* Defines a new function in the context.
*/
void moxi_define_fun(moxi_context_t *ctx, char *symbol, size_t nargs,
                    sort_t *args, sort_t ret, term_t body);

/*******************************************************************************
 * System Management
 ******************************************************************************/

/**
 * Defines a new system in the context.
 */
void moxi_define_system(moxi_context_t *ctx, char *str, 
                        size_t ninput, term_t *input, term_t *input_next,
                        size_t noutput, term_t *output, term_t *output_next,
                        size_t nlocal, term_t *local, term_t *local_next,
                        term_t init, term_t trans, term_t inv);

/**
 * Defines a new system with subsystems in the context.
 */
void moxi_define_system_subsys(moxi_context_t *ctx, char *str, 
                               size_t ninput, term_t *input, term_t *input_next,
                               size_t noutput, term_t *output, term_t *output_next,
                               size_t nlocal, term_t *local, term_t *local_next,
                               term_t init, term_t trans, term_t inv, 
                               size_t nsubsys, char **symbols, char **subsys_type,
                               size_t *nsubsys_input, term_t **subsys_input, 
                               size_t *nsubsys_output, term_t **subsys_output);

/**
 * Checks a system in the context.
 */
void moxi_check_system(moxi_context_t* ctx, char* str, size_t ninput,
                       term_t* input, term_t* input_next, size_t noutput,
                       term_t* output, term_t* output_next, size_t nlocal,
                       term_t* local, term_t* local_next, size_t nassumption, char **assumption_name,   
                       term_t* assumption, size_t nfairness, char **fairness_name, term_t* fairness,
                       size_t nreachable, char **reachable_name, term_t* reachable, size_t ncurrent,
                       char **current_name, term_t* current, size_t nquery, char** query_name,
                       size_t* query_size, char*** query);

/**
 * Finds a system in the context. Returns `NULL` if the system is not found.
 */
const sys_table_entry_t *moxi_find_system(moxi_context_t *ctx, char *str);

#endif
