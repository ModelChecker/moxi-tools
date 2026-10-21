/**
 *
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "io/print.h"
#include "moxi/command.h"
#include "moxi/context.h"
#include "moxi/hash_logic.h"
#include "parse/hash_symbol.h"

void activate_core_symbols(moxi_context_t *ctx)
{
    ctx->active_theory_symbols[THY_SYM_BOOL] = true;
    ctx->active_theory_symbols[THY_SYM_TRUE] = true;
    ctx->active_theory_symbols[THY_SYM_FALSE] = true;
    ctx->active_theory_symbols[THY_SYM_NOT] = true;
    ctx->active_theory_symbols[THY_SYM_AND] = true;
    ctx->active_theory_symbols[THY_SYM_OR] = true;
    ctx->active_theory_symbols[THY_SYM_IMPLIES] = true;
    ctx->active_theory_symbols[THY_SYM_XOR] = true;
    ctx->active_theory_symbols[THY_SYM_EQ] = true;
    ctx->active_theory_symbols[THY_SYM_DISTINCT] = true;
    ctx->active_theory_symbols[THY_SYM_ITE] = true;
}

void activate_bitvec_symbols(moxi_context_t *ctx)
{
    ctx->active_theory_symbols[THY_SYM_BITVEC] = true;
    ctx->active_theory_symbols[THY_SYM_CONCAT] = true;
    ctx->active_theory_symbols[THY_SYM_EXTRACT] = true;
    ctx->active_theory_symbols[THY_SYM_REPEAT] = true;
    ctx->active_theory_symbols[THY_SYM_BVCOMP] = true;
    ctx->active_theory_symbols[THY_SYM_BVREDAND] = true;
    ctx->active_theory_symbols[THY_SYM_BVREDOR] = true;
    ctx->active_theory_symbols[THY_SYM_BVNOT] = true;
    ctx->active_theory_symbols[THY_SYM_BVAND] = true;
    ctx->active_theory_symbols[THY_SYM_BVOR] = true;
    ctx->active_theory_symbols[THY_SYM_BVNAND] = true;
    ctx->active_theory_symbols[THY_SYM_BVNOR] = true;
    ctx->active_theory_symbols[THY_SYM_BVXOR] = true;
    ctx->active_theory_symbols[THY_SYM_BVXNOR] = true;
    ctx->active_theory_symbols[THY_SYM_BVNEG] = true;
    ctx->active_theory_symbols[THY_SYM_BVADD] = true;
    ctx->active_theory_symbols[THY_SYM_BVSUB] = true;
    ctx->active_theory_symbols[THY_SYM_BVMUL] = true;
    ctx->active_theory_symbols[THY_SYM_BVUDIV] = true;
    ctx->active_theory_symbols[THY_SYM_BVUREM] = true;
    ctx->active_theory_symbols[THY_SYM_BVSDIV] = true;
    ctx->active_theory_symbols[THY_SYM_BVSREM] = true;
    ctx->active_theory_symbols[THY_SYM_BVSMOD] = true;
    ctx->active_theory_symbols[THY_SYM_BVSHL] = true;
    ctx->active_theory_symbols[THY_SYM_BVLSHR] = true;
    ctx->active_theory_symbols[THY_SYM_BVASHR] = true;
    ctx->active_theory_symbols[THY_SYM_ZERO_EXTEND] = true;
    ctx->active_theory_symbols[THY_SYM_SIGN_EXTEND] = true;
    ctx->active_theory_symbols[THY_SYM_ROTATE_LEFT] = true;
    ctx->active_theory_symbols[THY_SYM_ROTATE_RIGHT] = true;
    ctx->active_theory_symbols[THY_SYM_BVULT] = true;
    ctx->active_theory_symbols[THY_SYM_BVULE] = true;
    ctx->active_theory_symbols[THY_SYM_BVUGT] = true;
    ctx->active_theory_symbols[THY_SYM_BVUGE] = true;
    ctx->active_theory_symbols[THY_SYM_BVSLT] = true;
    ctx->active_theory_symbols[THY_SYM_BVSLE] = true;
    ctx->active_theory_symbols[THY_SYM_BVSGT] = true;
    ctx->active_theory_symbols[THY_SYM_BVSGE] = true;
}

void activate_array_symbols(moxi_context_t *ctx)
{
    ctx->active_theory_symbols[THY_SYM_ARRAY] = true;
    ctx->active_theory_symbols[THY_SYM_SELECT] = true;
    ctx->active_theory_symbols[THY_SYM_STORE] = true;
}

void activate_int_symbols(moxi_context_t *ctx)
{
    ctx->active_theory_symbols[THY_SYM_INT] = true;
    ctx->active_theory_symbols[THY_SYM_PLUS] = true;
    ctx->active_theory_symbols[THY_SYM_MINUS] = true;
    ctx->active_theory_symbols[THY_SYM_TIMES] = true;
    ctx->active_theory_symbols[THY_SYM_IDIV] = true;
    ctx->active_theory_symbols[THY_SYM_MOD] = true;
    ctx->active_theory_symbols[THY_SYM_ABS] = true;
    ctx->active_theory_symbols[THY_SYM_DIVISIBLE] = true;
    ctx->active_theory_symbols[THY_SYM_GT] = true;
    ctx->active_theory_symbols[THY_SYM_GE] = true;
    ctx->active_theory_symbols[THY_SYM_LT] = true;
    ctx->active_theory_symbols[THY_SYM_LE] = true;
}

void activate_real_symbols(moxi_context_t *ctx)
{
    ctx->active_theory_symbols[THY_SYM_REAL] = true;
    ctx->active_theory_symbols[THY_SYM_PLUS] = true;
    ctx->active_theory_symbols[THY_SYM_MINUS] = true;
    ctx->active_theory_symbols[THY_SYM_TIMES] = true;
    ctx->active_theory_symbols[THY_SYM_RDIV] = true;
    ctx->active_theory_symbols[THY_SYM_GT] = true;
    ctx->active_theory_symbols[THY_SYM_GE] = true;
    ctx->active_theory_symbols[THY_SYM_LT] = true;
    ctx->active_theory_symbols[THY_SYM_LE] = true;
}

void activate_int_real_symbols(moxi_context_t *ctx)
{
    ctx->active_theory_symbols[THY_SYM_TO_REAL] = true;
    ctx->active_theory_symbols[THY_SYM_TO_INT] = true;
}

static void default_delete_entry(void *entry)
{
    free(entry);
}

static void delete_var_stack_entry(void *entry) 
{ 
    str_vector_t *vec = (str_vector_t *)entry;
    delete_str_vector(vec);
    free(vec);
}

static void delete_sys_table_entry(void *entry) 
{ 
    sys_table_entry_t *sys = (sys_table_entry_t *) entry;
    free(sys->name);
    free(sys->input);
    free(sys->input_next);
    free(sys->output);
    free(sys->output_next);
    free(sys->local);
    free(sys->local_next);
    free(sys);
}

/**
 * Sets all non-core symbols in `active_symbols` to `false`.
 */
void reset_symbols(moxi_context_t *ctx)
{
    size_t i;
    for (i = 0; i < NUM_THEORY_SYMBOLS; ++i) {
        ctx->active_theory_symbols[i] = false;
    }
    activate_core_symbols(ctx);
}

void init_context(moxi_context_t *ctx)
{
    ctx->status = 0;
    ctx->sort_var_id = 0;
    init_str_map(&ctx->var_table, 0, default_delete_entry);
    init_str_map(&ctx->sys_table, 0, delete_sys_table_entry);
    init_ptr_stack(&ctx->scope_stack, delete_var_stack_entry);
    init_str_vector(&ctx->sort_var_symbols, 16);
    ctx->logic = &no_logic;
    activate_core_symbols(ctx);
    ctx->cur_cmd_type = CMD_NONE;
    ctx->cur_cmd.data.none.unused = NULL;
}

void delete_context(moxi_context_t *ctx)
{
    reset_command(&ctx->cur_cmd);
    delete_str_vector(&ctx->sort_var_symbols);
    delete_str_map(&ctx->var_table);
    delete_str_map(&ctx->sys_table);
    delete_ptr_stack(&ctx->scope_stack);
}

theory_symbol_type_t get_theory_symbol_type(const moxi_context_t *ctx, const char *symbol)
{
    const theory_symbol_t *thy_sym = find_moxi_thy_sym(symbol, strlen(symbol));
    if (thy_sym != NULL) {
        return thy_sym->type;
    }
    return THY_SYM_UNKNOWN;
}

bool is_active_theory_term(const moxi_context_t *ctx, const char *symbol)
{
    const theory_symbol_t *thy_sym = find_moxi_thy_sym(symbol, strlen(symbol));
    return (thy_sym != NULL && thy_sym->kind == SYM_KIND_TERM && ctx->active_theory_symbols[thy_sym->type]);
}

bool is_active_theory_sort(const moxi_context_t *ctx, const char *symbol)
{
    const theory_symbol_t *thy_sym = find_moxi_thy_sym(symbol, strlen(symbol));
    return (thy_sym != NULL && thy_sym->kind == SYM_KIND_SORT && ctx->active_theory_symbols[thy_sym->type]);
}

bool is_active_term(const moxi_context_t *ctx, const char *symbol)
{
    return is_active_theory_term(ctx, symbol) || (yices_get_term_by_name(symbol) != NULL_TERM);
}

bool is_active_sort(const moxi_context_t *ctx, const char *symbol)
{
    return is_active_theory_sort(ctx, symbol) || (yices_get_type_by_name(symbol) != NULL_TYPE);
}

void moxi_push_scope(moxi_context_t *ctx)
{
    str_vector_t *vec = malloc(sizeof(str_vector_t));
    init_str_vector(vec, 16);
    ptr_stack_push(&ctx->scope_stack, vec);
}

void moxi_pop_scope(moxi_context_t *ctx)
{
    str_vector_t *vec = ptr_stack_pop(&ctx->scope_stack);
    size_t i;
    for (i = 0; i < vec->size; ++i) {
        free(str_map_remove(&ctx->var_table, vec->data[i]));
        yices_remove_term_name(vec->data[i]);
    }
    delete_str_vector(vec);
    free(vec);
}

str_vector_t *moxi_get_scope(moxi_context_t *ctx)
{
    return ptr_stack_top(&ctx->scope_stack);
}

void moxi_clear_sort_vars(moxi_context_t *ctx)
{
    uint32_t i;
    str_vector_t *sort_var_symbols = &ctx->sort_var_symbols;
    for (i = 0; i < sort_var_symbols->size; ++i) {
        yices_remove_type_name(sort_var_symbols->data[i]);
    }
    str_vector_reset(sort_var_symbols);
    ctx->sort_var_id = 0; // FIXME: double check this
}

void moxi_add_named_term(moxi_context_t *ctx, char *symbol, term_t term, var_kind_t kind)
{
    if (is_active_theory_term(ctx, symbol)) {
        ctx->status = 1;
        return;
    }

    var_table_entry_t *new_var_entry = malloc(sizeof(var_table_entry_t));
    new_var_entry->kind = kind;
    new_var_entry->term = term;
    new_var_entry->is_primed = false;
    str_map_add(&ctx->var_table, symbol, strlen(symbol), new_var_entry);

    str_vector_t *scope = ptr_stack_top(&ctx->scope_stack);
    str_vector_append(scope, symbol);

    yices_set_term_name(term, symbol);

    if (kind == LOGIC_VAR) {
        return;
    }

    // All other var types get a primed version as well
    char *primed = malloc((strlen(symbol) + 2) * sizeof(char));
    sprintf(primed, "%s'", symbol);

    term_t primed_term = yices_new_uninterpreted_term(yices_type_of_term(term));
    if (primed_term == NULL_TERM) {
        yices_print_error(stderr);
        ctx->status = 1;
        free(primed);
        return;
    }
    yices_set_term_name(primed_term, primed);

    var_table_entry_t *primed_var_entry = malloc(sizeof(var_table_entry_t));
    primed_var_entry->kind = kind;
    primed_var_entry->term = primed_term;
    primed_var_entry->is_primed = true;
    str_map_add(&ctx->var_table, primed, strlen(primed), primed_var_entry);

    str_vector_append(scope, primed);
    free(primed);
}

void moxi_add_named_sort(moxi_context_t *ctx, char *str, sort_t sort)
{
    if (is_active_theory_sort(ctx, str)) {
        ctx->status = 1;
        return;
    }
    yices_set_type_name(sort, str);
}

const var_table_entry_t *moxi_find_var(moxi_context_t *ctx, char *symbol)
{
    return str_map_find(&ctx->var_table, symbol);
}

void moxi_set_logic(moxi_context_t *ctx, char *symbol)
{
    size_t len = strlen(symbol);
    const logic_t *logic = find_moxi_logic(symbol, len);
    if (logic == NULL) {
        PRINT_ERROR("unknown logic '%s'", symbol);
        ctx->logic = &unkown_logic;
        ctx->status = 1;
        return;
    }
    logic_type_t type = logic->type;
    ctx->logic = logic;

    if (!logic_is_supported[type]) {
        PRINT_ERROR("unsupported logic '%s'", logic->name);
        ctx->status = 1;
        return;
    }

    reset_symbols(ctx);

    if (logic_has_ints[type]) {
        activate_int_symbols(ctx);
    }
    if (logic_has_reals[type]) {
        activate_real_symbols(ctx);
    }
    if (logic_has_ints[type] && logic_has_reals[type]) {
        activate_int_real_symbols(ctx);
    }
    if (logic_has_bitvectors[type]) {
        activate_bitvec_symbols(ctx);
    }
    if (logic_has_arrays[type]) {
        activate_array_symbols(ctx);
    }

    command_t *cmd = &ctx->cur_cmd;
    set_set_logic_cmd(cmd, type);
}

void moxi_declare_enum_sort(moxi_context_t *ctx, char *str, size_t nvalues, char **values)
{
    if (is_active_theory_sort(ctx, str)) {
        ctx->status = 1;
        return;
    }
    sort_t sort = yices_new_scalar_type(nvalues);
    yices_set_type_name(sort, str);

    size_t i;
    term_t term;
    for (i = 0; i < nvalues; ++i) {
        if (is_active_theory_term(ctx, values[i])) {
            ctx->status = 1;
            continue;
        }
        term = yices_constant(sort, i);
        yices_set_term_name(term, values[i]);
    }

    command_t *cmd = &ctx->cur_cmd;
    set_declare_enum_sort_cmd(cmd, str, nvalues, values);
}

void moxi_declare_sort(moxi_context_t *ctx, char *symbol, size_t arity)
{
    if (is_active_theory_sort(ctx, symbol)) {
        ctx->status = 1;
        return;
    }
    if (arity == 0) {
        sort_t sort = yices_new_uninterpreted_type();
        yices_set_type_name(sort, symbol);
        return;
    }
    
    yices_type_constructor(symbol, arity);
    
    command_t *cmd = &ctx->cur_cmd;
    set_declare_sort_cmd(cmd, symbol, arity);
}

void moxi_define_sort(moxi_context_t *ctx, char *str, size_t nargs,
                      sort_t *args, sort_t body)
{
    if (is_active_theory_sort(ctx, str)) {
        ctx->status = 1;
        return;
    }
    if (nargs == 0) {
        yices_set_type_name(body, str);
    } else {
        yices_type_macro(str, nargs, args, body);
    }

    command_t *cmd = &ctx->cur_cmd;
    set_define_sort_cmd(cmd, str, nargs, args, body);
}

void moxi_declare_const(moxi_context_t *ctx, char *symbol, sort_t sort)
{
    moxi_declare_fun(ctx, symbol, 0, NULL, sort);
    command_t *cmd = &ctx->cur_cmd;
    set_declare_const_cmd(cmd, symbol, sort);
}

void moxi_declare_fun(moxi_context_t *ctx, char *symbol, size_t nargs,
                     sort_t *args, sort_t ret)
{
    if (is_active_term(ctx, symbol)) {
        ctx->status = 1;
        return;
    }

    sort_t sort;
    if (nargs == 0) {
        sort = ret;
    } else {
        sort = yices_function_type(nargs, args, ret);
    }
    term_t term = yices_new_uninterpreted_term(sort);
    if (term == NULL_TERM) {
        yices_print_error(stderr);
        ctx->status = 1;
        return;
    }
    yices_set_term_name(term, symbol);

    command_t *cmd = &ctx->cur_cmd;
    set_declare_fun_cmd(cmd, symbol, nargs, args, ret);
}

void moxi_define_fun(moxi_context_t *ctx, char *str, size_t nargs,
                     term_t *args, sort_t ret, term_t body)
{
    if (is_active_term(ctx, str)) {
        ctx->status = 1;
        return;
    }

    // yices_lambda requires a positive arity. A 0-ary define-fun is a named
    // constant, so attach the name to the body the same way 0-ary define-sort
    // attaches a name to its sort body.
    if (nargs == 0) {
        if (yices_set_term_name(body, str) < 0) {
            yices_print_error(stderr);
            ctx->status = 1;
            return;
        }
    } else {
        term_t lambda = yices_lambda(nargs, args, body);
        if (lambda == NULL_TERM) {
            yices_print_error(stderr);
            ctx->status = 1;
            return;
        }
        yices_set_term_name(lambda, str);
    }

    command_t *cmd = &ctx->cur_cmd;
    set_define_fun_cmd(cmd, str, nargs, args, ret, body);
}

void moxi_define_system(moxi_context_t *ctx, char *str, size_t ninput,
                        term_t *input, term_t *input_next, size_t noutput, term_t *output, term_t *output_next,
                        size_t nlocal, term_t *local, term_t *local_next, term_t init,
                        term_t trans, term_t inv)
{
    sys_table_entry_t *sys = malloc(sizeof(sys_table_entry_t));
    sys->name = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(sys->name, str);

    sys->ninput = ninput;
    sys->input = malloc(ninput * sizeof(term_t));
    memcpy(sys->input, input, ninput * sizeof(term_t));

    sys->input_next = malloc(ninput * sizeof(term_t));
    memcpy(sys->input_next, input_next, ninput * sizeof(term_t));

    sys->noutput = noutput;
    sys->output = malloc(noutput * sizeof(term_t));
    memcpy(sys->output, output, noutput * sizeof(term_t));

    sys->output_next = malloc(noutput * sizeof(term_t));
    memcpy(sys->output_next, output_next, noutput * sizeof(term_t));

    sys->nlocal = nlocal;
    sys->local = malloc(nlocal * sizeof(term_t));
    memcpy(sys->local, local, nlocal * sizeof(term_t));

    sys->local_next = malloc(nlocal * sizeof(term_t));
    memcpy(sys->local_next, local_next, nlocal * sizeof(term_t));

    str_map_add(&ctx->sys_table, str, strlen(str), sys);

    command_t *cmd = &ctx->cur_cmd;
    set_define_system_cmd(cmd, str, ninput, input, input_next, noutput, output,
                          output_next, nlocal, local, local_next, init, trans,
                          inv);
}

void moxi_define_system_subsys(moxi_context_t* ctx, char* str, 
                               size_t ninput, term_t* input, term_t* input_next,
                               size_t noutput, term_t* output, term_t* output_next,
                               size_t nlocal, term_t* local, term_t* local_next,
                               term_t init, term_t trans, term_t inv, 
                               size_t nsubsys, char** symbols, char** subsys_type,
                               size_t* nsubsys_input, term_t** subsys_input, 
                               size_t* nsubsys_output, term_t** subsys_output) 
{
    moxi_define_system(ctx, str, ninput, input, input_next, noutput, output, output_next, nlocal, local, local_next, init, trans, inv);

    command_t *cmd = &ctx->cur_cmd;
    set_define_system_subsys_cmd(cmd, str, ninput, input, input_next, noutput,
                                 output, output_next, nlocal, local, local_next,
                                 init, trans, inv, nsubsys, symbols,
                                 subsys_type, nsubsys_input, subsys_input,
                                 nsubsys_output, subsys_output);
}

void moxi_check_system(moxi_context_t* ctx, char* str, size_t ninput,
                       term_t* input, term_t* input_next, size_t noutput,
                       term_t* output, term_t* output_next, size_t nlocal,
                       term_t* local, term_t* local_next, size_t nassumption,
                       char** assumption_name, term_t* assumption,
                       size_t nfairness, char** fairness_name, term_t* fairness,
                       size_t nreachable, char** reachable_name,
                       term_t* reachable, size_t ncurrent, char** current_name,
                       term_t* current, size_t nquery, char** query_name,
                       size_t* query_size, char*** query, size_t nqueries,
                       size_t *queries_nquery, char ***queries_name,
                       size_t **queries_size, char ****queries)
{
    const sys_table_entry_t *sys = moxi_find_system(ctx, str);
    if (sys == NULL) {
        PRINT_ERROR("system %s not defined", str);
        ctx->status = 1;
        return;
    }

    command_t *cmd = &ctx->cur_cmd;
    set_check_system_cmd(cmd, str, ninput, input, input_next, noutput, output,
                         output_next, nlocal, local, local_next, nassumption,
                         assumption_name, assumption, nfairness, fairness_name,
                         fairness, nreachable, reachable_name, reachable,
                         ncurrent, current_name, current, nquery, query_name,
                         query_size, query, nqueries, queries_nquery,
                         queries_name, queries_size, queries);
}

const sys_table_entry_t *moxi_find_system(moxi_context_t *ctx, char *str)
{
    return str_map_find(&ctx->sys_table, str);
}
