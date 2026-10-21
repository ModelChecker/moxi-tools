#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <yices.h>
#include "moxi/logic.h"

typedef type_t sort_t; // yices type
typedef int32_t sort_macro_t; // yices type macro

/**
 * The type of a command.
 */
 typedef enum cmd_type {
    CMD_SET_LOGIC,
    CMD_DECLARE_ENUM_SORT,
    CMD_DECLARE_SORT,
    CMD_DEFINE_SORT,
    CMD_DECLARE_CONST,
    CMD_DECLARE_FUN,
    CMD_DEFINE_FUN,
    CMD_DEFINE_SYSTEM,
    CMD_CHECK_SYSTEM,
    CMD_NONE
} cmd_type_t;

/**
 * A command to set the logic.
 *
 * (set-logic <symbol>)
 */
typedef struct set_logic_cmd {
    logic_type_t logic; // Logic of the context
} set_logic_cmd_t;

/**
 * A command to declare an enum sort.
 *
 * (declare-enum-sort <symbol> ( <symbol>+ ))
 */
typedef struct declare_enum_sort_cmd {
    char *str;         // Name of the enum sort
    char **values;     // Values of the enum sort where for 0 <= i < nvalues, 
                       // `values[i]` is the null-terminated string value of the enum sort
    size_t nvalues;    // Number of values of the enum sort
} declare_enum_sort_cmd_t;

/**
 * A command to declare a sort.
 *
 * (declare-sort <symbol> <numeral>)
 */
typedef struct declare_sort_cmd {
    char *str;         // Name of the sort
    size_t arity;      // Arity of the sort
} declare_sort_cmd_t;

/**
 * A command to declare a constant.
 *
 * (declare-const <symbol> <sort>)
 */
typedef struct declare_const_cmd {
    char *str;        // Name of the constant
    sort_t sort;      // Sort of the constant
} declare_const_cmd_t;

/**
 * A command to declare a function.
 *
 * (declare-fun <symbol> ( <sort>* ) <sort>)
 */
typedef struct declare_fun_cmd {
    char *str;         // Name of the function
    size_t nargs;      // Number of arguments of the function
    sort_t *args;      // Arguments of the function where for 0 <= i < nargs, 
                       // `args[i]` is the argument of the function
    sort_t ret;        // Return sort of the function
} declare_fun_cmd_t;

/**
 * A command to define a function.
 *
 * (define-fun <symbol> ( <sorted-var>* ) <sort> <term>)
 */
typedef struct define_fun_cmd {
    char *str;         // Name of the function
    size_t nargs;      // Number of arguments of the function
    sort_t *args;      // Arguments of the function where for 0 <= i < nargs, 
                       // `args[i]` is the argument of the function
    sort_t ret;        // Return sort of the function
    term_t term;       // Body of the function
} define_fun_cmd_t;

/**
 * A command to define a sort macro.
 *
 * (define-sort <symbol> ( <symbol>* ) <sort-macro>)
 */
typedef struct define_sort_cmd {
    char *str;         // Name of the sort macro
    size_t nargs;      // Number of arguments of the sort macro
    sort_t *args;      // Arguments of the sort macro
    sort_macro_t body; // Body of the sort macro
} define_sort_cmd_t;

/**
 * A command to define a system.
 *
 * (define-system <symbol> 
 *   :input (( <symbol> <sort> )*)
 *   :output (( <symbol> <sort> )*)
 *   :local (( <symbol> <sort> )*)
 *   :init <term>
 *   :trans <term>
 *   :inv <term>
 *   :subsys (( <symbol> ( <symbol> <sort> )*)*)
 * )
 */
typedef struct define_system_cmd {
    /**
     * System signature data
     */
    char *str;  // Name of the system instance
    size_t len; // Length of the name of the system instance
    size_t ninput;  // Number of input variables of the system instance
    term_t *input;  // Input variables of the system instance
    term_t *input_next;  // Next input variables of the system instance
    size_t noutput; // Number of output variables of the system instance
    term_t *output; // Output variables of the system instance
    term_t *output_next;  // Next output variables of the system instance
    size_t nlocal;  // Number of local variables of the system instance
    term_t *local;  // Local variables of the system instance
    term_t *local_next;  // Next local variables of the system instance

    /**
     * System behavior data
     */
    term_t init;  // Initial term of the system instance
    term_t trans; // Transition term of the system instance
    term_t inv;   // Invariant term of the system instance

    /**
     * Subsystems data
     */
    size_t nsubsys;        // Number of subsystem instances
    char **subsys;         // Names of the subsystem instances where for 0 <= i < nsubsys, 
                           // `subsys[i]` is the name of the subsystem instance
    char **subsys_type;    // Names of the types of the subsystem instances where for 0 <= i < nsubsys, 
                           // `subsys_type[i]` is the name of the type of the subsystem instance
    size_t *nsubsys_input; // Number of input variables of the subsystem instances where for 0 <= i < nsubsys, 
                           // `nsubsys_input[i]` is the number of input variables of the subsystem instance
    term_t **subsys_input; // Input variables of the subsystem instances where for 0 <= i < nsubsys, 
                           // `subsys_input[i]` is an array of length `nsubsys_input[i]`
    size_t *nsubsys_output; // Number of output variables of the subsystem instances where for 0 <= i < nsubsys, 
                            // `nsubsys_output[i]` is the number of output variables of the subsystem instance
    term_t **subsys_output; // Output variables of the subsystem instances where for 0 <= i < nsubsys, 
                            // `subsys_output[i]` is an array of length `nsubsys_output[i]`
} define_system_cmd_t;

/**
 * A command to check a system.
 *
 * (check-system <symbol> 
 *   :input (( <symbol> <sort> )*)
 *   :output (( <symbol> <sort> )*)
 *   :local (( <symbol> <sort> )*)
 *   :assumption ( <symbol> <term> )
 *   :fairness ( <symbol> <term> )
 *   :reachable ( <symbol> <term> )
 *   :current ( <symbol> <term> )
 *   :query ( <symbol> ( <symbol>+ ) )
 *   :queries (( <symbol> ( <symbol>+ ) )+ )
 * )
 */
typedef struct check_system_cmd {
    char *str;  // Name of the system instance
    size_t len; // Length of the name of the system instance

    size_t ninput;  // Number of input variables of the system instance
    term_t *input;  // Input variables of the system instance
    term_t *input_next;  // Next input variables of the system instance
    size_t noutput; // Number of output variables of the system instance
    term_t *output; // Output variables of the system instance
    term_t *output_next;  // Next output variables of the system instance
    size_t nlocal;  // Number of local variables of the system instance
    term_t *local;  // Local variables of the system instance
    term_t *local_next;  // Next local variables of the system instance
    
    size_t nassumption; // Number of assumptions of the system instance
    char **assumption_name; // Names of the assumptions of the system instance
    term_t *assumption; // Assumptions of the system instance where for 0 <= i < nassumption, 
                        // `assumption[i]` is the assumption of the system instance

    size_t nfairness;   // Number of fairness variables of the system instance
    char **fairness_name; // Names of the fairness variables of the system instance
    term_t *fairness;   // Fairness variables of the system instance

    size_t nreachable;  // Number of reachable variables of the system instance
    char **reachable_name; // Names of the reachable variables of the system instance
    term_t *reachable;  // Reachable variables of the system instance

    size_t ncurrent;    // Number of current variables of the system instance
    char **current_name; // Names of the current variables of the system instance
    term_t *current;    // Current variables of the system instance

    size_t nquery;      // Number of query attributes of the system instance
    char **query_name;  // Names of the query attributes of the system instance
    size_t *query_size; // Sizes of the query attributes of the system instance
    char ***query;      // Query attributes of the system instance
} check_system_cmd_t;

/**
 * A command to do nothing.
 */
typedef struct none_cmd {
    void *unused;
} none_cmd_t;

/**
 * A command.
 */
typedef struct command {
    cmd_type_t type;
    union {
        set_logic_cmd_t set_logic;
        declare_enum_sort_cmd_t declare_enum_sort;
        declare_fun_cmd_t declare_fun;
        declare_sort_cmd_t declare_sort;
        declare_const_cmd_t declare_const;
        define_fun_cmd_t define_fun;
        define_sort_cmd_t define_sort;
        define_system_cmd_t define_system;
        check_system_cmd_t check_system;
        none_cmd_t none;
    } data;
} command_t;

void reset_command(command_t *cmd);

void set_exit_cmd(command_t* cmd);

void set_reset_cmd(command_t* cmd);

void set_assert_cmd(command_t* cmd, term_t term);

void set_echo_cmd(command_t* cmd, char* str);

void set_set_logic_cmd(command_t* cmd, logic_type_t logic);

void set_declare_enum_sort_cmd(command_t* cmd, char* str, size_t nvalues,
                               char** values);

void set_declare_sort_cmd(command_t* cmd, char* str, size_t arity);

void set_define_sort_cmd(command_t* cmd, char* str, size_t nargs, sort_t* args,
    sort_macro_t body);

void set_declare_const_cmd(command_t* cmd, char* str, sort_t sort);

void set_define_const_cmd(command_t* cmd, char* str, sort_t sort, term_t term);

void set_declare_fun_cmd(command_t* cmd, char* str, size_t nargs, sort_t* args,
                         sort_t ret);

void set_define_fun_cmd(command_t* cmd, char* str, size_t nargs, sort_t* args,
                        sort_t ret, term_t term);

void set_define_system_cmd(command_t* cmd, char* str,  
                           size_t ninput, term_t* input, term_t* input_next,
                           size_t noutput, term_t* output, term_t* output_next,
                           size_t nlocal, term_t* local, term_t* local_next,
                           term_t init, term_t trans, term_t inv);

void set_define_system_subsys_cmd(command_t* cmd, char* str, 
                                  size_t ninput, term_t* input, term_t* input_next,
                                  size_t noutput, term_t* output, term_t* output_next,
                                  size_t nlocal, term_t* local, term_t* local_next,
                                  term_t init, term_t trans, term_t inv,
                                  size_t nsubsys, char** subsys, char** subsys_type,
                                  size_t* nsubsys_input, term_t** subsys_input,
                                  size_t* nsubsys_output, term_t** subsys_output);

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
                          char*** query);

void set_none_cmd(command_t* cmd);

#endif
