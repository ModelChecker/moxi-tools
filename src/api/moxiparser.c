#include <stdlib.h>
#include <string.h>

#include "moxiparser.h"
#include "moxi/command.h"
#include "parse/parse.h"
#include "util/str_vector.h"
#include "util/int32_vector.h"
#include "util/int32_map.h"
#include "moxi/logic.h"

static char *dup_str(const char *s)
{
    size_t n;
    char *d;
    if (s == NULL) {
        return NULL;
    }
    n = strlen(s) + 1;
    d = malloc(n);
    memcpy(d, s, n);
    return d;
}

/**
 * A parser is a parser for the MoXI language.
 */
 struct moxi_parser {
    parser_t internal_parser;  // Internal parser
    moxi_command_t *command;   // Current command being parsed
    size_t index;              // Index of the current command
    moxi_error_t error;        // Error message
};


static moxi_logic_t moxi_logic_map[] = {
    MOXI_AX, // AX
    MOXI_BV, // BV
    MOXI_IDL, // IDL
    MOXI_LIA, // LIA
    MOXI_LRA, // LRA
    MOXI_LIRA, // LIRA
    MOXI_NIA, // NIA
    MOXI_NRA, // NRA
    MOXI_NIRA, // NIRA
    MOXI_RDL, // RDL
    MOXI_UF, // UF
    MOXI_ABV, // ABV
    MOXI_ALIA, // ALIA
    MOXI_ALRA, // ALRA
    MOXI_ALIRA, // ALIRA
    MOXI_ANIA, // ANIA
    MOXI_ANRA, // ANRA
    MOXI_ANIRA, // ANIRA
    MOXI_AUF, // AUF
    MOXI_UFBV, // UFBV
    MOXI_UFBVLIA, // UFBVLIA
    MOXI_UFIDL, // UFIDL
    MOXI_UFLIA, // UFLIA
    MOXI_UFLRA, // UFLRA
    MOXI_UFLIRA, // UFLIRA
    MOXI_UFNIA, // UFNIA
    MOXI_UFNRA, // UFNRA
    MOXI_UFNIRA, // UFNIRA
    MOXI_UFRDL, // UFRDL
    MOXI_AUFBV, // AUFBV
    MOXI_AUFBVLIA, // AUFBVLIA
    MOXI_AUFBVNIA, // AUFBVNIA
    MOXI_AUFLIA, // AUFLIA
    MOXI_AUFLRA, // AUFLRA
    MOXI_AUFLIRA, // AUFLIRA
    MOXI_AUFNIA, // AUFNIA
    MOXI_AUFNRA, // AUFNRA
    MOXI_AUFNIRA, // AUFNIRA
    MOXI_QF_AX, // QF_AX
    MOXI_QF_BV, // QF_BV
    MOXI_QF_IDL, // QF_IDL
    MOXI_QF_LIA, // QF_LIA
    MOXI_QF_LRA, // QF_LRA
    MOXI_QF_LIRA, // QF_LIRA
    MOXI_QF_NIA, // QF_NIA
    MOXI_QF_NRA, // QF_NRA
    MOXI_QF_NIRA, // QF_NIRA
    MOXI_QF_RDL, // QF_RDL
    MOXI_QF_UF, // QF_UF
    MOXI_QF_ABV, // QF_ABV
    MOXI_QF_ALIA, // QF_ALIA
    MOXI_QF_ALRA, // QF_ALRA
    MOXI_QF_ALIRA, // QF_ALIRA
    MOXI_QF_ANIA, // QF_ANIA
    MOXI_QF_ANRA, // QF_ANRA
    MOXI_QF_ANIRA, // QF_ANIRA
    MOXI_QF_AUF, // QF_AUF
    MOXI_QF_UFBV, // QF_UFBV
    MOXI_QF_UFBVLIA, // QF_UFBVLIA
    MOXI_QF_UFIDL, // QF_UFIDL
    MOXI_QF_UFLIA, // QF_UFLIA
    MOXI_QF_UFLRA, // QF_UFLRA
    MOXI_QF_UFLIRA, // QF_UFLIRA
    MOXI_QF_UFNIA, // QF_UFNIA
    MOXI_QF_UFNRA, // QF_UFNRA
    MOXI_QF_UFNIRA, // QF_UFNIRA
    MOXI_QF_UFRDL, // QF_UFRDL
    MOXI_QF_AUFBV, // QF_AUFBV
    MOXI_QF_AUFBVLIA, // QF_AUFBVLIA
    MOXI_QF_AUFBVNIA, // QF_AUFBVNIA
    MOXI_QF_AUFLIA, // QF_AUFLIA
    MOXI_QF_AUFLRA, // QF_AUFLRA
    MOXI_QF_AUFLIRA, // QF_AUFLIRA
    MOXI_QF_AUFNIA, // QF_AUFNIA
    MOXI_QF_AUFNRA, // QF_AUFNRA
    MOXI_QF_AUFNIRA, // QF_AUFNIRA
    MOXI_LOGIC_NONE, // LOGIC_NONE
    MOXI_LOGIC_ALL, // LOGIC_ALL
    MOXI_FP, // FP
    MOXI_STRING, // STRING
    MOXI_DT, // DT
    MOXI_SEQ, // SEQ
    MOXI_LOGIC_UNKNOWN, // LOGIC_UNKNOWN
};

/**
 * String vector functions.
 */

/**
 * Initializes a string vector. A moxi_string_vector_t is a str_vector_t.
 */
void init_moxi_string_vector(moxi_string_vector_t *vector)
{
    init_str_vector((str_vector_t *) vector, 0);
}

void delete_moxi_string_vector(moxi_string_vector_t *vector)
{
    delete_str_vector((str_vector_t *) vector);
}

void reset_moxi_string_vector(moxi_string_vector_t *vector)
{
    str_vector_reset((str_vector_t *) vector);
}

void append_moxi_string_vector(moxi_string_vector_t *vector, char *str)
{
    str_vector_append((str_vector_t *) vector, str);
}

/**
 * Term vector functions.
 */

void init_moxi_term_vector(moxi_term_vector_t *vector, uint32_t size)
{
    init_int32_vector((int32_vector_t *) vector, size);
}

void delete_moxi_term_vector(moxi_term_vector_t *vector)
{
    delete_int32_vector((int32_vector_t *) vector);
}

void reset_moxi_term_vector(moxi_term_vector_t *vector)
{
    int32_vector_reset((int32_vector_t *) vector);
}

void append_moxi_term_vector(moxi_term_vector_t *vector, int32_t i)
{
    int32_vector_append((int32_vector_t *) vector, i);
}

/**
 * Sort vector functions.
 */

void init_moxi_sort_vector(moxi_sort_vector_t *vector, uint32_t size)
{
    init_int32_vector((int32_vector_t *) vector, size);
}

void delete_moxi_sort_vector(moxi_sort_vector_t *vector)
{
    delete_int32_vector((int32_vector_t *) vector);
}

void reset_moxi_sort_vector(moxi_sort_vector_t *vector)
{
    int32_vector_reset((int32_vector_t *) vector);
}

void append_moxi_sort_vector(moxi_sort_vector_t *vector, int32_t i)
{
    int32_vector_append((int32_vector_t *) vector, i);
}

/**
 * Subsystem vector functions.
 */

void init_moxi_subsystem_vector(moxi_subsystem_vector_t *vector, size_t size)
{
    vector->data = malloc(size * sizeof(moxi_subsystem_t));
    vector->size = size;
}

void delete_moxi_subsystem(moxi_subsystem_t *subsystem)
{
    if (subsystem->name) {
        free(subsystem->name);
    }
    if (subsystem->system) {
        free(subsystem->system);
    }
    delete_moxi_term_vector(&subsystem->input);
    delete_moxi_term_vector(&subsystem->output);
    subsystem->name = NULL;
    subsystem->system = NULL;
}

void delete_moxi_subsystem_vector(moxi_subsystem_vector_t *vector)
{
    for (size_t i = 0; i < vector->size; i++) {
        delete_moxi_subsystem(&vector->data[i]);
    }
    if (vector->data) {
        free(vector->data);
    }
    vector->size = 0;
    vector->data = NULL;
}

/**
 * Named term vector functions.
 */

void init_moxi_named_term_vector(moxi_named_term_vector_t *vector, uint32_t capacity)
{
    vector->names = malloc(capacity * sizeof(char *));
    vector->data = malloc(capacity * sizeof(term_t));
    vector->capacity = capacity;
    vector->size = 0;
}

void delete_moxi_named_term_vector(moxi_named_term_vector_t *vector)
{
    for (size_t i = 0; i < vector->size; i++) {
        free(vector->names[i]);
    }
    free(vector->names);
    free(vector->data);
    vector->size = 0;
    vector->names = NULL;
    vector->data = NULL;
}

void reset_moxi_named_term_vector(moxi_named_term_vector_t *vector)
{
    vector->size = 0;
    vector->names = NULL;
    vector->data = NULL;
}

void append_moxi_named_term_vector(moxi_named_term_vector_t *vector, const char *name, term_t term)
{
    vector->names[vector->size] = malloc(strlen(name) + 1);
    strcpy(vector->names[vector->size], name);
    vector->data[vector->size] = term;
    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->names = realloc(vector->names, vector->capacity * sizeof(char *));
        vector->data = realloc(vector->data, vector->capacity * sizeof(term_t));
    }
    vector->size++;
}

/**
 * Named string list vector functions.
 */
void init_moxi_named_string_list_vector(moxi_named_string_list_vector_t *vector, uint32_t capacity)
{
    vector->names = malloc(capacity * sizeof(char *));
    vector->sizes = malloc(capacity * sizeof(uint32_t));
    vector->strings = malloc(capacity * sizeof(char **));
    vector->capacity = capacity;
    vector->size = 0;
}

void delete_moxi_named_string_list_vector(moxi_named_string_list_vector_t *vector)
{
    for (size_t i = 0; i < vector->size; i++) {
        free(vector->names[i]);
        for (size_t j = 0; j < vector->sizes[i]; j++) {
            free(vector->strings[i][j]);
        }
        free(vector->strings[i]);
    }
    free(vector->names);
    free(vector->sizes);
    free(vector->strings);
    vector->size = 0;
    vector->names = NULL;
    vector->sizes = NULL;
    vector->strings = NULL;
}

void reset_moxi_named_string_list_vector(moxi_named_string_list_vector_t *vector)
{
    vector->size = 0;
    vector->names = NULL;
    vector->sizes = NULL;
    vector->strings = NULL;
}

void append_moxi_named_string_list_vector(moxi_named_string_list_vector_t *vector, char *name, uint32_t nstrings, char **strings)
{
    vector->names[vector->size] = dup_str(name);
    vector->sizes[vector->size] = nstrings;
    vector->strings[vector->size] = malloc(nstrings * sizeof(char *));
    for (size_t j = 0; j < nstrings; j++) {
        vector->strings[vector->size][j] = malloc(strlen(strings[j]) + 1);
        strcpy(vector->strings[vector->size][j], strings[j]);
    }
    if (vector->size == vector->capacity) {
        vector->capacity *= 2;
        vector->names = realloc(vector->names, vector->capacity * sizeof(char *));
        vector->sizes = realloc(vector->sizes, vector->capacity * sizeof(uint32_t));
        vector->strings = realloc(vector->strings, vector->capacity * sizeof(char **));
    }
    vector->size++;
}

void init_moxi_term_map(moxi_term_map_t *map, uint32_t capacity)
{
    init_int32_map((int32_map_t *) map, capacity);
}

void delete_moxi_term_map(moxi_term_map_t *map)
{
    if (map == NULL) {
        return;
    }
    delete_int32_map((int32_map_t *) map);
    free(map);
}

term_t moxi_term_map_find(moxi_term_map_t *map, term_t term)
{
    return (term_t) int32_map_find((int32_map_t *) map, (int32_t) term);
}

void moxi_term_map_add(moxi_term_map_t *map, term_t term, term_t value)
{
    int32_map_add((int32_map_t *) map, (int32_t) term, (int32_t) value);
}

void reset_moxi_command(moxi_command_t *command)
{
    moxi_command_type_t type = command->type;

    free((void *)command->str);
    delete_moxi_string_vector(&command->strings);
    delete_moxi_term_vector(&command->terms);
    delete_moxi_sort_vector(&command->sorts);

    switch (type) {
    case MOXI_DEFINE_SYSTEM:
        delete_moxi_subsystem_vector(&command->system.subsystems);
        delete_moxi_term_vector(&command->system.input);
        delete_moxi_term_vector(&command->system.output);
        delete_moxi_term_vector(&command->system.local);
        delete_moxi_term_map(command->system.next_vars);
        break;
    case MOXI_CHECK_SYSTEM:
        delete_moxi_term_vector(&command->check_system.input);
        delete_moxi_term_vector(&command->check_system.output);
        delete_moxi_term_vector(&command->check_system.local);
        delete_moxi_term_map(command->check_system.next_vars);
        delete_moxi_named_term_vector(&command->check_system.assumption);
        delete_moxi_named_term_vector(&command->check_system.fairness);
        delete_moxi_named_term_vector(&command->check_system.reachable);
        delete_moxi_named_term_vector(&command->check_system.current);
        delete_moxi_named_string_list_vector(&command->check_system.query);
        break;
    default:
        break;
    }

    command->type = MOXI_NONE;
    command->logic = MOXI_LOGIC_NONE;
    command->str = NULL;
    command->term = NULL_TERM;
    command->sort = NULL_TYPE;
    command->sort_macro = 0;
    command->arity = 0;
    command->system = (moxi_system_t){0};
    command->check_system = (moxi_check_system_t){0};
    command->strings = (moxi_string_vector_t){0};
    command->terms = (moxi_term_vector_t){0};
    command->sorts = (moxi_sort_vector_t){0};
}

moxi_parser_t *new_moxi_parser(const char *filename)
{
    moxi_parser_t *parser = malloc(sizeof(moxi_parser_t));
    parser->internal_parser.filename = filename;
    int status = init_parser(&parser->internal_parser, filename);
    if (status) {
        free(parser);
        return NULL;
    }
    parser->index = 0;
    
    parser->command = calloc(1, sizeof(moxi_command_t));
    parser->command->type = MOXI_NONE;

    return parser;
}

void delete_moxi_parser(moxi_parser_t *parser)
{
    delete_parser(&parser->internal_parser);
    reset_moxi_command(parser->command);
    free(parser->command);
    free(parser);
} 

/**
 * Resets the current command in memory in `parser` then sets the values of the command in
 * `parser->command` based on the current command in the internal parser. Returns the command in
 * `parser->command`. Returns NULL if there is an error.
 */
moxi_command_t *moxi_parser_next_command(moxi_parser_t *parser)
{
    int status;
    status = parse_command(&parser->internal_parser);

    if (status != 0) {
        return NULL;
    }

    reset_moxi_command(parser->command);
    command_t *internal_cmd = &parser->internal_parser.pstack.ctx.cur_cmd;
    moxi_command_t *cmd = parser->command;
    moxi_system_t *system = &cmd->system;
    moxi_subsystem_vector_t *subsystems = &cmd->system.subsystems;

    switch (internal_cmd->type) {
        case CMD_SET_LOGIC:
            cmd->type = MOXI_SET_LOGIC;
            cmd->str = dup_str(logic_str[moxi_logic_map[internal_cmd->data.set_logic.logic]]);
            cmd->logic = moxi_logic_map[internal_cmd->data.set_logic.logic];
            break;
        case CMD_DECLARE_ENUM_SORT:
            cmd->type = MOXI_DECLARE_ENUM_SORT;
            cmd->str = dup_str(internal_cmd->data.declare_enum_sort.str);
            init_moxi_string_vector(&cmd->strings);
            for (size_t i = 0; i < internal_cmd->data.declare_enum_sort.nvalues; i++) {
                append_moxi_string_vector(&cmd->strings, internal_cmd->data.declare_enum_sort.values[i]);
            }
            break;
        case CMD_DECLARE_SORT:
            cmd->type = MOXI_DECLARE_SORT;
            cmd->str = dup_str(internal_cmd->data.declare_sort.str);
            cmd->arity = internal_cmd->data.declare_sort.arity;
            break;
        case CMD_DEFINE_SORT:
            cmd->type = MOXI_DEFINE_SORT;
            cmd->str = dup_str(internal_cmd->data.define_sort.str);
            cmd->arity = internal_cmd->data.define_sort.nargs;
            cmd->sort = internal_cmd->data.define_sort.body;
            cmd->sort_macro = internal_cmd->data.define_sort.body;
            init_moxi_sort_vector(&cmd->sorts, internal_cmd->data.define_sort.nargs);
            for (size_t i = 0; i < internal_cmd->data.define_sort.nargs; i++) {
                append_moxi_sort_vector(&cmd->sorts, internal_cmd->data.define_sort.args[i]);
            }
            break;
        case CMD_DECLARE_CONST:
            cmd->type = MOXI_DECLARE_CONST;
            cmd->str = dup_str(internal_cmd->data.declare_const.str);
            cmd->sort = internal_cmd->data.declare_const.sort;
            break;
        case CMD_DEFINE_FUN:
            cmd->type = MOXI_DEFINE_FUN;
            cmd->str = dup_str(internal_cmd->data.define_fun.str);
            cmd->term = internal_cmd->data.define_fun.term;
            cmd->sort = internal_cmd->data.define_fun.ret;
            init_moxi_term_vector(&cmd->terms, internal_cmd->data.define_fun.nargs);
            for (size_t i = 0; i < internal_cmd->data.define_fun.nargs; i++) {
                append_moxi_term_vector(&cmd->terms, internal_cmd->data.define_fun.args[i]);
            }
            break;
        case CMD_DECLARE_FUN:
            cmd->type = MOXI_DECLARE_FUN;
            cmd->str = dup_str(internal_cmd->data.declare_fun.str);
            cmd->sort = internal_cmd->data.declare_fun.ret;
            init_moxi_sort_vector(&cmd->sorts, internal_cmd->data.declare_fun.nargs);
            for (size_t i = 0; i < internal_cmd->data.declare_fun.nargs; i++) {
                append_moxi_sort_vector(&cmd->sorts, internal_cmd->data.declare_fun.args[i]);
            }
            break;
        case CMD_DEFINE_SYSTEM: 
        {
            cmd->type = MOXI_DEFINE_SYSTEM;
            cmd->str = dup_str(internal_cmd->data.define_system.str);

            init_moxi_term_vector(&system->input, internal_cmd->data.define_system.ninput);
            for (size_t i = 0; i < internal_cmd->data.define_system.ninput; i++) {
                append_moxi_term_vector(&system->input, internal_cmd->data.define_system.input[i]);
            }

            init_moxi_term_vector(&system->output, internal_cmd->data.define_system.noutput);
            for (size_t i = 0; i < internal_cmd->data.define_system.noutput; i++) {
                append_moxi_term_vector(&system->output, internal_cmd->data.define_system.output[i]);
            }

            init_moxi_term_vector(&system->local, internal_cmd->data.define_system.nlocal);
            for (size_t i = 0; i < internal_cmd->data.define_system.nlocal; i++) {
                append_moxi_term_vector(&system->local, internal_cmd->data.define_system.local[i]);
            }

            system->next_vars = malloc(sizeof(int32_map_t));
            init_moxi_term_map(system->next_vars, 0);
            for (size_t i = 0; i < internal_cmd->data.define_system.ninput; i++) {
                moxi_term_map_add(
                    system->next_vars, 
                    internal_cmd->data.define_system.input[i], 
                    internal_cmd->data.define_system.input_next[i]
                );
            }
            for (size_t i = 0; i < internal_cmd->data.define_system.noutput; i++) {
                moxi_term_map_add(
                    system->next_vars, 
                    internal_cmd->data.define_system.output[i],     
                    internal_cmd->data.define_system.output_next[i]
                );
            }
            for (size_t i = 0; i < internal_cmd->data.define_system.nlocal; i++) {
                moxi_term_map_add(
                    system->next_vars, 
                    internal_cmd->data.define_system.local[i], 
                    internal_cmd->data.define_system.local_next[i]
                );
            }

            init_moxi_subsystem_vector(&system->subsystems, internal_cmd->data.define_system.nsubsys);

            system->init_term = internal_cmd->data.define_system.init;
            system->trans_term = internal_cmd->data.define_system.trans;
            system->inv_term = internal_cmd->data.define_system.inv;

            system->subsystems.size = internal_cmd->data.define_system.nsubsys;
            for (size_t i = 0; i < internal_cmd->data.define_system.nsubsys; i++) {
                subsystems->data[i].name = dup_str(internal_cmd->data.define_system.subsys[i]);
                subsystems->data[i].system = dup_str(internal_cmd->data.define_system.subsys_type[i]);

                init_moxi_term_vector(&subsystems->data[i].input, internal_cmd->data.define_system.nsubsys_input[i]);
                for (size_t j = 0; j < internal_cmd->data.define_system.nsubsys_input[i]; j++) {
                    append_moxi_term_vector(&subsystems->data[i].input, internal_cmd->data.define_system.subsys_input[i][j]);
                }

                init_moxi_term_vector(&subsystems->data[i].output, internal_cmd->data.define_system.nsubsys_output[i]);
                for (size_t j = 0; j < internal_cmd->data.define_system.nsubsys_output[i]; j++) {
                    append_moxi_term_vector(&subsystems->data[i].output, internal_cmd->data.define_system.subsys_output[i][j]);
                }
            }
            break;
        }
        case CMD_CHECK_SYSTEM:
        {
            cmd->type = MOXI_CHECK_SYSTEM;
            cmd->str = dup_str(internal_cmd->data.check_system.str);

            moxi_check_system_t *check_system = &cmd->check_system;

            init_moxi_term_vector(&check_system->input, internal_cmd->data.check_system.ninput);
            for (size_t i = 0; i < internal_cmd->data.check_system.ninput; i++) {
                append_moxi_term_vector(&check_system->input, internal_cmd->data.check_system.input[i]);
            }

            init_moxi_term_vector(&check_system->output, internal_cmd->data.check_system.noutput);
            for (size_t i = 0; i < internal_cmd->data.check_system.noutput; i++) {
                append_moxi_term_vector(&check_system->output, internal_cmd->data.check_system.output[i]);
            }

            init_moxi_term_vector(&check_system->local, internal_cmd->data.check_system.nlocal);
            for (size_t i = 0; i < internal_cmd->data.check_system.nlocal; i++) {
                append_moxi_term_vector(&check_system->local, internal_cmd->data.check_system.local[i]);
            }

            check_system->next_vars = malloc(sizeof(int32_map_t));
            init_moxi_term_map(check_system->next_vars, 0);
            for (size_t i = 0; i < internal_cmd->data.check_system.ninput; i++) {
                moxi_term_map_add(check_system->next_vars, internal_cmd->data.check_system.input[i], internal_cmd->data.check_system.input_next[i]);
            }
            for (size_t i = 0; i < internal_cmd->data.check_system.noutput; i++) {
                moxi_term_map_add(check_system->next_vars, internal_cmd->data.check_system.output[i], internal_cmd->data.check_system.output_next[i]);
            }
            for (size_t i = 0; i < internal_cmd->data.check_system.nlocal; i++) {
                moxi_term_map_add(check_system->next_vars, internal_cmd->data.check_system.local[i], internal_cmd->data.check_system.local_next[i]);
            }

            init_moxi_named_term_vector(&check_system->assumption, internal_cmd->data.check_system.nassumption); 
            for (size_t i = 0; i < internal_cmd->data.check_system.nassumption; i++) {
                term_t term = internal_cmd->data.check_system.assumption[i];
                append_moxi_named_term_vector(
                    &check_system->assumption, 
                    internal_cmd->data.check_system.assumption_name[i],
                    term
                );
            }

            init_moxi_named_term_vector(&check_system->fairness, internal_cmd->data.check_system.nfairness);
            for (size_t i = 0; i < internal_cmd->data.check_system.nfairness; i++) {
                term_t term = internal_cmd->data.check_system.fairness[i];
                append_moxi_named_term_vector(
                    &check_system->fairness, 
                    internal_cmd->data.check_system.fairness_name[i],
                    term
                );
            }

            init_moxi_named_term_vector(&check_system->reachable, internal_cmd->data.check_system.nreachable);
            for (size_t i = 0; i < internal_cmd->data.check_system.nreachable; i++) {
                term_t term = internal_cmd->data.check_system.reachable[i];
                append_moxi_named_term_vector(
                    &check_system->reachable, 
                    internal_cmd->data.check_system.reachable_name[i],
                    term
                );
            }

            init_moxi_named_term_vector(&check_system->current, internal_cmd->data.check_system.ncurrent);
            for (size_t i = 0; i < internal_cmd->data.check_system.ncurrent; i++) {
                term_t term = internal_cmd->data.check_system.current[i];
                append_moxi_named_term_vector(
                    &check_system->current, 
                    internal_cmd->data.check_system.current_name[i],
                    term
                );
            }

            init_moxi_named_string_list_vector(&check_system->query, internal_cmd->data.check_system.nquery);
            for (size_t i = 0; i < internal_cmd->data.check_system.nquery; i++) {
                append_moxi_named_string_list_vector(
                    &check_system->query,
                    internal_cmd->data.check_system.query_name[i],
                    internal_cmd->data.check_system.query_size[i],
                    internal_cmd->data.check_system.query[i]
                );
            }
            break;
        }
        default:
            break;
    }

    return parser->command;
}
