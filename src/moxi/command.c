#include <string.h>
#include <stdlib.h>

#include "moxi/command.h"

static void *copy_bytes(const void *src, size_t n)
{
    void *dst;
    if (n == 0) {
        return NULL;
    }
    dst = malloc(n);
    memcpy(dst, src, n);
    return dst;
}

void reset_command(command_t *cmd) 
{
    switch (cmd->type) {
        case CMD_SET_LOGIC:
            break;
        case CMD_DECLARE_ENUM_SORT:
            free(cmd->data.declare_enum_sort.str);
            for (size_t i = 0; i < cmd->data.declare_enum_sort.nvalues; i++) {
                free(cmd->data.declare_enum_sort.values[i]);
            }
            free(cmd->data.declare_enum_sort.values);
            break;
        case CMD_DECLARE_FUN:
            free(cmd->data.declare_fun.str);
            free(cmd->data.declare_fun.args);
            break;
        case CMD_DECLARE_SORT:
            free(cmd->data.declare_sort.str);
            break;
        case CMD_DEFINE_SORT:
            free(cmd->data.define_sort.str);
            free(cmd->data.define_sort.args);
            break;
        case CMD_DECLARE_CONST:
            free(cmd->data.declare_const.str);
            break;  
        case CMD_DEFINE_FUN:
            free(cmd->data.define_fun.str);
            free(cmd->data.define_fun.args);
            break;
        case CMD_DEFINE_SYSTEM:
            free(cmd->data.define_system.str);
            free(cmd->data.define_system.input);
            free(cmd->data.define_system.input_next);
            free(cmd->data.define_system.output);
            free(cmd->data.define_system.output_next);
            free(cmd->data.define_system.local);
            free(cmd->data.define_system.local_next);
            for (size_t i = 0; i < cmd->data.define_system.nsubsys; i++) {
                free(cmd->data.define_system.subsys[i]);
                free(cmd->data.define_system.subsys_type[i]);
                free(cmd->data.define_system.subsys_input[i]);
                free(cmd->data.define_system.subsys_output[i]);
            }
            if (cmd->data.define_system.nsubsys > 0) {
                free(cmd->data.define_system.subsys);
                free(cmd->data.define_system.subsys_type);
                free(cmd->data.define_system.nsubsys_input);
                free(cmd->data.define_system.nsubsys_output);
                free(cmd->data.define_system.subsys_input);
                free(cmd->data.define_system.subsys_output);
            }
            break;
        case CMD_CHECK_SYSTEM: {
            check_system_cmd_t *cs = &cmd->data.check_system;
            free(cs->str);
            free(cs->input);
            free(cs->input_next);
            free(cs->output);
            free(cs->output_next);
            free(cs->local);
            free(cs->local_next);
            for (size_t i = 0; i < cs->nassumption; i++) {
                free(cs->assumption_name[i]);
            }
            free(cs->assumption_name);
            free(cs->assumption);
            for (size_t i = 0; i < cs->nfairness; i++) {
                free(cs->fairness_name[i]);
            }
            free(cs->fairness_name);
            free(cs->fairness);
            for (size_t i = 0; i < cs->nreachable; i++) {
                free(cs->reachable_name[i]);
            }
            free(cs->reachable_name);
            free(cs->reachable);
            for (size_t i = 0; i < cs->ncurrent; i++) {
                free(cs->current_name[i]);
            }
            free(cs->current_name);
            free(cs->current);
            for (size_t i = 0; i < cs->nquery; i++) {
                free(cs->query[i]);
            }
            free(cs->query);
            free(cs->query_name);
            free(cs->query_size);
            break;
        }
        case CMD_NONE:
        default:
            break;
    }
    cmd->type = CMD_NONE;
}

void set_set_logic_cmd(command_t *cmd, logic_type_t logic) 
{
    reset_command(cmd);
    cmd->type = CMD_SET_LOGIC;
    cmd->data.set_logic.logic = logic;
}

void set_declare_enum_sort_cmd(command_t *cmd, char *str, size_t nvalues,
                               char **values) 
{
    reset_command(cmd);
    cmd->type = CMD_DECLARE_ENUM_SORT;

    cmd->data.declare_enum_sort.str = malloc(strlen(str) * sizeof(char) + 1);
    memcpy(cmd->data.declare_enum_sort.str, str, strlen(str) * sizeof(char) + 1);

    cmd->data.declare_enum_sort.nvalues = nvalues;
    cmd->data.declare_enum_sort.values = malloc(nvalues * sizeof(char *));
    for (size_t i = 0; i < nvalues; i++) {
        cmd->data.declare_enum_sort.values[i] = malloc(strlen(values[i]) * sizeof(char) + 1);
        memcpy(cmd->data.declare_enum_sort.values[i], values[i], strlen(values[i]) * sizeof(char) + 1);
    }
}

void set_declare_fun_cmd(command_t *cmd, char *str, size_t nargs,
                          sort_t *args, sort_t ret)
{
    reset_command(cmd);
    cmd->type = CMD_DECLARE_FUN;
    
    cmd->data.declare_fun.str = malloc(strlen(str) * sizeof(char) + 1);
    memcpy(cmd->data.declare_fun.str, str, strlen(str) * sizeof(char) + 1);

    cmd->data.declare_fun.nargs = nargs;
    cmd->data.declare_fun.args = copy_bytes(args, nargs * sizeof(sort_t));
    cmd->data.declare_fun.ret = ret;
}

void set_declare_sort_cmd(command_t *cmd, char *str, size_t arity)
{
    reset_command(cmd);
    cmd->type = CMD_DECLARE_SORT;

    cmd->data.declare_sort.str = malloc(strlen(str) * sizeof(char) + 1);
    memcpy(cmd->data.declare_sort.str, str, strlen(str) * sizeof(char) + 1);

    cmd->data.declare_sort.arity = arity;
}

void set_declare_const_cmd(command_t *cmd, char *str, sort_t sort)
{
    reset_command(cmd);
    cmd->type = CMD_DECLARE_CONST;

    cmd->data.declare_const.str = malloc(strlen(str) * sizeof(char) + 1);
    memcpy(cmd->data.declare_const.str, str, strlen(str) * sizeof(char) + 1);

    cmd->data.declare_const.sort = sort;
}

void set_define_fun_cmd(command_t *cmd, char *str, size_t nargs,
                         term_t *args, sort_t ret, term_t term)
{
    reset_command(cmd);
    cmd->type = CMD_DEFINE_FUN;

    cmd->data.define_fun.str = malloc(strlen(str) * sizeof(char) + 1);
    memcpy(cmd->data.define_fun.str, str, strlen(str) * sizeof(char) + 1);

    cmd->data.define_fun.nargs = nargs;
    cmd->data.define_fun.args = copy_bytes(args, nargs * sizeof(term_t));
    cmd->data.define_fun.ret = ret;
    cmd->data.define_fun.term = term;
}

void set_define_sort_cmd(command_t *cmd, char *str, size_t nargs,
                         sort_t *args, sort_macro_t body)
{
    reset_command(cmd);
    cmd->type = CMD_DEFINE_SORT;

    cmd->data.define_sort.str = malloc(strlen(str) * sizeof(char) + 1);
    memcpy(cmd->data.define_sort.str, str, strlen(str) * sizeof(char) + 1);

    cmd->data.define_sort.nargs = nargs;
    cmd->data.define_sort.args = copy_bytes(args, nargs * sizeof(sort_t));
    cmd->data.define_sort.body = body;
}

void set_define_system_cmd(command_t* cmd, char* str, 
                           size_t ninput, term_t* input, term_t* input_next,
                           size_t noutput, term_t* output, term_t* output_next,
                           size_t nlocal, term_t* local, term_t* local_next,
                           term_t init, term_t trans, term_t inv)
{
    reset_command(cmd);
    cmd->type = CMD_DEFINE_SYSTEM;

    cmd->data.define_system.str = malloc(strlen(str) * sizeof(char) + 1);
    memcpy(cmd->data.define_system.str, str, strlen(str) * sizeof(char) + 1);

    cmd->data.define_system.ninput = ninput;
    cmd->data.define_system.input = copy_bytes(input, ninput * sizeof(term_t));
    cmd->data.define_system.input_next = copy_bytes(input_next, ninput * sizeof(term_t));

    cmd->data.define_system.noutput = noutput;
    cmd->data.define_system.output = copy_bytes(output, noutput * sizeof(term_t));
    cmd->data.define_system.output_next = copy_bytes(output_next, noutput * sizeof(term_t));

    cmd->data.define_system.nlocal = nlocal;
    cmd->data.define_system.local = copy_bytes(local, nlocal * sizeof(term_t));
    cmd->data.define_system.local_next = copy_bytes(local_next, nlocal * sizeof(term_t));
    
    cmd->data.define_system.init = init;
    cmd->data.define_system.trans = trans;
    cmd->data.define_system.inv = inv;

    cmd->data.define_system.nsubsys = 0;
    cmd->data.define_system.subsys = NULL;
    cmd->data.define_system.subsys_type = NULL;
    cmd->data.define_system.nsubsys_input = NULL;
    cmd->data.define_system.subsys_input = NULL;
    cmd->data.define_system.nsubsys_output = NULL;
    cmd->data.define_system.subsys_output = NULL;
}

void set_define_system_subsys_cmd(command_t* cmd, char* str,
                                  size_t ninput, term_t* input, term_t* input_next,
                                  size_t noutput, term_t* output, term_t* output_next,
                                  size_t nlocal, term_t* local, term_t* local_next,
                                  term_t init, term_t trans, term_t inv,
                                  size_t nsubsys, char** subsys, char** subsys_type,
                                  size_t* nsubsys_input, term_t** subsys_input,
                                  size_t* nsubsys_output, term_t** subsys_output)
{
    set_define_system_cmd(cmd, str, ninput, input, input_next, noutput, output,
                          output_next, nlocal, local, local_next, init, trans,
                          inv);

    define_system_cmd_t *define_system = &cmd->data.define_system;

    define_system->nsubsys = nsubsys;
    define_system->subsys = malloc(nsubsys * sizeof(char *));
    memcpy(define_system->subsys, subsys, nsubsys * sizeof(char *));
    for (size_t i = 0; i < nsubsys; i++) {
        define_system->subsys[i] = malloc(strlen(subsys[i]) * sizeof(char) + 1);
        strcpy(define_system->subsys[i], subsys[i]);
    }

    define_system->subsys_type = malloc(nsubsys * sizeof(char *));
    memcpy(define_system->subsys_type, subsys_type, nsubsys * sizeof(char *));
    for (size_t i = 0; i < nsubsys; i++) {
        define_system->subsys_type[i] = malloc(strlen(subsys_type[i]) * sizeof(char) + 1);
        strcpy(define_system->subsys_type[i], subsys_type[i]);
    }

    define_system->nsubsys_input = malloc(nsubsys * sizeof(size_t));
    memcpy(define_system->nsubsys_input, nsubsys_input, nsubsys * sizeof(size_t));

    define_system->nsubsys_output = malloc(nsubsys * sizeof(size_t));
    memcpy(define_system->nsubsys_output, nsubsys_output, nsubsys * sizeof(size_t));

    define_system->subsys_input = malloc(nsubsys * sizeof(term_t *));
    define_system->subsys_output = malloc(nsubsys * sizeof(term_t *));

    for (size_t i = 0; i < nsubsys; i++) {
        define_system->subsys_input[i] = malloc(nsubsys_input[i] * sizeof(term_t));
        memcpy(define_system->subsys_input[i], subsys_input[i], nsubsys_input[i] * sizeof(term_t));
    }

    for (size_t i = 0; i < nsubsys; i++) {
        define_system->subsys_output[i] = malloc(nsubsys_output[i] * sizeof(term_t));
        memcpy(define_system->subsys_output[i], subsys_output[i], nsubsys_output[i] * sizeof(term_t));
    }
}

void set_check_system_cmd(command_t* cmd, char* str, size_t ninput,
                          term_t* input, term_t* input_next, size_t noutput,
                          term_t* output, term_t* output_next, size_t nlocal,
                          term_t* local, term_t* local_next, size_t nassumption,
                          char** assumption_name, term_t* assumption,
                          size_t nfairness, char** fairness_name,
                          term_t* fairness, size_t nreachable,
                          char** reachable_name, term_t* reachable,
                          size_t ncurrent, char** current_name, term_t* current,
                          size_t nquery, char** query_name, size_t* query_size,
                          char*** query) 
{
    reset_command(cmd);
    cmd->type = CMD_CHECK_SYSTEM;

    cmd->data.check_system.str = malloc(strlen(str) * sizeof(char) + 1);
    memcpy(cmd->data.check_system.str, str, strlen(str) * sizeof(char) + 1);

    cmd->data.check_system.ninput = ninput;
    cmd->data.check_system.input = copy_bytes(input, ninput * sizeof(term_t));
    cmd->data.check_system.input_next = copy_bytes(input_next, ninput * sizeof(term_t));

    cmd->data.check_system.noutput = noutput;
    cmd->data.check_system.output = copy_bytes(output, noutput * sizeof(term_t));
    cmd->data.check_system.output_next = copy_bytes(output_next, noutput * sizeof(term_t));

    cmd->data.check_system.nlocal = nlocal;
    cmd->data.check_system.local = copy_bytes(local, nlocal * sizeof(term_t));
    cmd->data.check_system.local_next = copy_bytes(local_next, nlocal * sizeof(term_t));

    cmd->data.check_system.nassumption = nassumption;
    cmd->data.check_system.assumption_name = malloc(nassumption * sizeof(char*));
    cmd->data.check_system.assumption = copy_bytes(assumption, nassumption * sizeof(term_t));
    for (size_t i = 0; i < nassumption; i++) {
        cmd->data.check_system.assumption_name[i] = malloc(strlen(assumption_name[i]) * sizeof(char) + 1);
        strcpy(cmd->data.check_system.assumption_name[i], assumption_name[i]);
    }

    cmd->data.check_system.nfairness = nfairness;
    cmd->data.check_system.fairness_name = malloc(nfairness * sizeof(char*));
    cmd->data.check_system.fairness = copy_bytes(fairness, nfairness * sizeof(term_t));
    for (size_t i = 0; i < nfairness; i++) {
        cmd->data.check_system.fairness_name[i] = malloc(strlen(fairness_name[i]) * sizeof(char) + 1);
        strcpy(cmd->data.check_system.fairness_name[i], fairness_name[i]);
    }

    cmd->data.check_system.nreachable = nreachable;
    cmd->data.check_system.reachable_name = malloc(nreachable * sizeof(char*));
    cmd->data.check_system.reachable = copy_bytes(reachable, nreachable * sizeof(term_t));
    for (size_t i = 0; i < nreachable; i++) {
        cmd->data.check_system.reachable_name[i] = malloc(strlen(reachable_name[i]) * sizeof(char) + 1);
        strcpy(cmd->data.check_system.reachable_name[i], reachable_name[i]);
    }

    cmd->data.check_system.ncurrent = ncurrent;
    cmd->data.check_system.current_name = malloc(ncurrent * sizeof(char*));
    cmd->data.check_system.current = copy_bytes(current, ncurrent * sizeof(term_t));
    for (size_t i = 0; i < ncurrent; i++) {
        cmd->data.check_system.current_name[i] = malloc(strlen(current_name[i]) * sizeof(char) + 1);
        strcpy(cmd->data.check_system.current_name[i], current_name[i]);
    }

    cmd->data.check_system.nquery = nquery;
    cmd->data.check_system.query_name = malloc(nquery * sizeof(char*));
    cmd->data.check_system.query_size = malloc(nquery * sizeof(size_t));
    cmd->data.check_system.query = malloc(nquery * sizeof(char**));
    memcpy(cmd->data.check_system.query_name, query_name, nquery * sizeof(char*));
    memcpy(cmd->data.check_system.query_size, query_size, nquery * sizeof(size_t));
    for (size_t i = 0; i < nquery; i++) {
        cmd->data.check_system.query[i] = malloc(query_size[i] * sizeof(char*));
        memcpy(cmd->data.check_system.query[i], query[i], query_size[i] * sizeof(char*));
    }
}