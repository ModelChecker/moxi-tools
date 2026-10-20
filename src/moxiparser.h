/**
 *
 */
#ifndef __MOXI_PARSER_H__
#define __MOXI_PARSER_H__

#include <yices.h>

#if __cplusplus
extern "C" {
#endif

typedef struct moxi_subsystem moxi_subsystem_t;
typedef struct moxi_system moxi_system_t;
typedef struct moxi_check_system moxi_check_system_t;
typedef struct moxi_command moxi_command_t;
typedef struct moxi_parser moxi_parser_t;

typedef struct moxi_string_vector moxi_string_vector_t;
typedef struct moxi_term_vector moxi_term_vector_t;
typedef struct moxi_sort_vector moxi_sort_vector_t;
typedef struct moxi_subsystem_vector moxi_subsystem_vector_t;
typedef struct moxi_named_term_vector moxi_named_term_vector_t;
typedef struct moxi_named_string_list_vector moxi_named_string_list_vector_t;
typedef struct moxi_term_map moxi_term_map_t;


typedef enum moxi_error_type {
    MOXI_SORT_ERROR,
    MOXI_PARSE_ERROR,
    MOXI_GENERIC_ERROR,
} moxi_error_type_t;

typedef struct moxi_error {
    moxi_error_type_t type;
    char *message;
} moxi_error_t;

struct moxi_string_vector {
    uint32_t capacity;
    uint32_t size;
    char **data;
};

struct moxi_term_vector {
    uint32_t capacity;
    uint32_t size;
    term_t *data;
};

struct moxi_sort_vector {
    uint32_t capacity;
    uint32_t size;
    type_t *data;
};

struct moxi_subsystem_vector {
    uint32_t capacity;
    uint32_t size;
    moxi_subsystem_t *data;
};

struct moxi_named_term_vector {
    uint32_t capacity;
    uint32_t size;
    char **names;    // Array of names of length `size`.
    term_t *data;    // Array of terms of length `size`.
};

struct moxi_named_string_list_vector {
    uint32_t capacity;
    uint32_t size;
    char **names;    // Array of names of length `size`.
    uint32_t *sizes; // Array of sizes of length `size`.
    char ***strings; // Array of arrays of strings of length `size`.
                     // `strings[i]` is an array of null-terminated strings of 
                     // length `sizes[i]`.
};

/**
 * Returns the term mapped to `term` if `term` is present in `map`. Otherwise,
 * returns NULL_TERM.
 */
term_t moxi_term_map_find(moxi_term_map_t *map, term_t term);

typedef enum moxi_logic {
    //  Base logics (with quantifiers)
    MOXI_AX,   // arrays
    MOXI_BV,   // bitvectors
    MOXI_IDL,  // integer difference logic
    MOXI_LIA,  // linear integer arithmetic
    MOXI_LRA,  // linear real arithmetic
    MOXI_LIRA, // mixed linear arithmetic
    MOXI_NIA,  // non-linear integer arithmetic
    MOXI_NRA,  // non-linear real arithmetic
    MOXI_NIRA, // non-linear mixed arithmetic
    MOXI_RDL,  // real difference logic
    MOXI_UF,   // uninterpreted functions

    //  Arrays + some other theory
    MOXI_ABV,   // arrays + bitvectors
    MOXI_ALIA,  // arrays + linear integer arithmetic
    MOXI_ALRA,  // arrays + linear real arithmetic
    MOXI_ALIRA, // arrays + mixed linear arithmetic
    MOXI_ANIA,  // arrays + non-linear integer arithmetic
    MOXI_ANRA,  // arrays + non-linear real arithmetic
    MOXI_ANIRA, // arrays + mixed/non-linear arithmetic
    MOXI_AUF,   // arrays + uninterpreted functions

    //  Uninterpreted function + another theory
    MOXI_UFBV,    // uninterpreted functions + bitvectors
    MOXI_UFBVLIA, // uninterpreted functions + bitvectors + linear integer
                   // arithmetic
    MOXI_UFIDL,   // uninterpreted functions + integer difference logic
    MOXI_UFLIA,   // uninterpreted functions + linear integer arithmetic
    MOXI_UFLRA,   // uninterpreted functions + linear real arithmetic
    MOXI_UFLIRA,  // uninterpreted functions + mixed linear arithmetic
    MOXI_UFNIA,   // uninterpreted functions + non-linear integer arithmetic
    MOXI_UFNRA,   // uninterpreted functions + non-linear real arithmetic
    MOXI_UFNIRA,  // uninterpreted functions + mixed, non-linear arithmetic
    MOXI_UFRDL,   // uninterpreted functions + real difference logic

    //  Arrays + uninterpreted functions + another theory
    MOXI_AUFBV,    // arrays + uninterpreted functions + bitvectors
    MOXI_AUFBVLIA, // arrays + uninterpreted functions + bitvectors + linear
                    // integer arithmetic
    MOXI_AUFBVNIA, // arrays + uninterpreted functions + bitvectors + nonlinear
                    // integer arithmetic
    MOXI_AUFLIA,   // arrays + uninterpreted functions + linear integer
                    // arithmetic
    MOXI_AUFLRA,   // arrays + uninterpreted functions + linear real arithmetic
    MOXI_AUFLIRA, // arrays + uninterpreted functions + mixed linear arithmetic
    MOXI_AUFNIA,  // arrays + uninterpreted functions + non-linear integer
                   // arithmetic
    MOXI_AUFNRA,  // arrays + uninterpreted functions + non-linear real
                   // arithmetic
    MOXI_AUFNIRA, // arrays + uninterpreted functions + mixed, non-linear
                   // arithmetic

    /*
     * Quantifier-free fragments
     */
    MOXI_QF_AX,   // arrays
    MOXI_QF_BV,   // bitvectors
    MOXI_QF_IDL,  // integer difference logic
    MOXI_QF_LIA,  // linear integer arithmetic
    MOXI_QF_LRA,  // linear real arithmetic
    MOXI_QF_LIRA, // mixed linear arithmetic
    MOXI_QF_NIA,  // non-linear integer arithmetic
    MOXI_QF_NRA,  // non-linear real arithmetic
    MOXI_QF_NIRA, // non-linear mixed arithmetic
    MOXI_QF_RDL,  // real difference logic
    MOXI_QF_UF,   // uninterpreted functions

    //  Arrays + some other theory
    MOXI_QF_ABV,   // arrays + bitvectors
    MOXI_QF_ALIA,  // arrays + linear integer arithmetic
    MOXI_QF_ALRA,  // arrays + linear real arithmetic
    MOXI_QF_ALIRA, // arrays + mixed linear arithmetic
    MOXI_QF_ANIA,  // arrays + non-linear integer arithmetic
    MOXI_QF_ANRA,  // arrays + non-linear real arithmetic
    MOXI_QF_ANIRA, // arrays + mixed/non-linear arithmetic
    MOXI_QF_AUF,   // arrays + uninterpreted functions

    //  Uninterpreted function + another theory
    MOXI_QF_UFBV,    // uninterpreted functions + bitvectors
    MOXI_QF_UFBVLIA, // uninterpreted functions + bitvectors + linear integer
                      // arithmetic
    MOXI_QF_UFIDL,   // uninterpreted functions + integer difference logic
    MOXI_QF_UFLIA,   // uninterpreted functions + linear integer arithmetic
    MOXI_QF_UFLRA,   // uninterpreted functions + linear real arithmetic
    MOXI_QF_UFLIRA,  // uninterpreted functions + mixed linear arithmetic
    MOXI_QF_UFNIA,   // uninterpreted functions + non-linear integer arithmetic
    MOXI_QF_UFNRA,   // uninterpreted functions + non-linear real arithmetic
    MOXI_QF_UFNIRA,  // uninterpreted functions + mixed, non-linear arithmetic
    MOXI_QF_UFRDL,   // uninterpreted functions + real difference logic

    //  Arrays + uninterpreted functions + another theory
    MOXI_QF_AUFBV,    // arrays + uninterpreted functions + bitvectors
    MOXI_QF_AUFBVLIA, // arrays + uninterpreted functions + bitvectors + linear
                       // integer arithmetic
    MOXI_QF_AUFBVNIA, // arrays + uninterpreted functions + bitvectors +
                       // nonlinear integer arithmetic
    MOXI_QF_AUFLIA,   // arrays + uninterpreted functions + linear integer
                       // arithmetic
    MOXI_QF_AUFLRA,   // arrays + uninterpreted functions + linear real
                       // arithmetic
    MOXI_QF_AUFLIRA,  // arrays + uninterpreted functions + mixed linear
                       // arithmetic
    MOXI_QF_AUFNIA,   // arrays + uninterpreted functions + non-linear integer
                       // arithmetic
    MOXI_QF_AUFNRA,   // arrays + uninterpreted functions + non-linear real
                       // arithmetic
    MOXI_QF_AUFNIRA,  // arrays + uninterpreted functions + mixed, non-linear
                       // arithmetic

    MOXI_LOGIC_NONE,
    MOXI_LOGIC_ALL,
    MOXI_LOGIC_UNKNOWN,
} moxi_logic_t;

/**
 * A system is a collection of input, output, local variables, a system behavior, and subsystems.
 *
 * Example:
 * ```
 * (define-system A
 *   :input ((i Bool))
 *   :output ((o Bool))
 *   :local ((l Bool))
 *   :init o
 *   :trans (= o' i)
 *   :inv (= l true)
 * )
 * ```
 */
struct moxi_system {
    moxi_term_vector_t input;
    moxi_term_vector_t output;
    moxi_term_vector_t local;

    moxi_term_map_t *next_vars;

    term_t init_term;
    term_t trans_term;
    term_t inv_term;

    moxi_subsystem_vector_t subsystems;
};

/**
 * A subsystem is a subsystem instance of a system.
 *
 * Example with a subsystem instance `Sub` of system `A`:
 * ```
 * (define-system A
 *   :input ((i Bool))
 *   :output ((o Bool))
 *   :local ((l Bool))
 * )
 *
 * (define-system B
 *   :local ((l1 Bool) (l2 Bool))
 *   :subsys (Sub (A l1 l2))
 * )
 * ```
 */
 struct moxi_subsystem {
    char *name;    // Name of the subsystem instance (`Sub` in the example above)
    char *system;  // Name of the system this is a subsystem of (`A` in the example above)
    moxi_term_vector_t input; 
    moxi_term_vector_t output; 
};

/**
 * A check system is a check system in the MoXI language.
 *
 * FIXME: The input, output, and local variables are the same variables as the system, but may have
 * different names.
 */
struct moxi_check_system {
    moxi_term_vector_t input;
    moxi_term_vector_t output;
    moxi_term_vector_t local;

    moxi_term_map_t *next_vars;

    moxi_named_term_vector_t assumption;
    moxi_named_term_vector_t fairness;
    moxi_named_term_vector_t reachable;
    moxi_named_term_vector_t current;

    // A query attribute is a named list of strings
    // Example: ":query (q (a b c))"
    moxi_named_string_list_vector_t query;

    // TODO: Add `queries`
    // A queries attribute is list of named lists of strings
    // Example: ":queries ((q0 (a b c)) (q1 (d e f)))"
    // moxi_string_vector_t queries_name;
    // moxi_term_vector_t queries;
};

/**
 * The type of a command.
 */
typedef enum moxi_command_type {
    MOXI_SET_LOGIC,
    MOXI_DECLARE_ENUM_SORT,
    MOXI_DECLARE_SORT,
    MOXI_DEFINE_SORT,
    MOXI_DECLARE_CONST,
    MOXI_DECLARE_FUN,
    MOXI_DEFINE_FUN,
    MOXI_DEFINE_SYSTEM,
    MOXI_CHECK_SYSTEM,
    MOXI_NONE,
} moxi_command_type_t;

/**
 * A command is a command in the MoXI language.
 */
struct moxi_command {

    /**
     * The type of the command.
     */
    moxi_command_type_t type;

    /**
     * The current logic. Only used for MOXI_SET_LOGIC.
     */
    moxi_logic_t logic;

    /**
     * - MOXI_SET_LOGIC: NULL 
     * - MOXI_DECLARE_ENUM_SORT, MOXI_DECLARE_SORT, MOXI_DEFINE_SORT: The
     *     name of the sort
     * - MOXI_DECLARE_CONST, MOXI_DEFINE_CONST, MOXI_DECLARE_FUN,
     *     MOXI_DEFINE_FUN: The name of the function
     * - MOXI_DEFINE_SYS, MOXI_CHECK_SYS: The name of the system
     */
    const char *str;

    /**
     * - MOXI_DECLARE_ENUM_SORT: The values of the enum sort
     */
    moxi_string_vector_t strings;

    /**
     * - MOXI_DECLARE_FUN, MOXI_DEFINE_FUN: The sort of the output of the function
     * - MOXI_DECLARE_SORT: The sort of the sort
     * - OTHERS: NULL_TYPE
     */
    type_t sort;

    /**
     * - MOXI_DEFINE_SORT: The sort macro of the function
     */
    int32_t sort_macro;

    /**
     * - MOXI_DECLARE_SORT: The arity of the sort
     */
    size_t arity;

    /**
     * - MOXI_DECLARE_FUN, MOXI_DEFINE_SORT: The sorts of the arguments
     */
    moxi_sort_vector_t sorts;

    /**
     * - MOXI_ASSERT, MOXI_ECHO, MOXI_SET_LOGIC: NULL_TERM
     * - MOXI_DECLARE_SORT, MOXI_DEFINE_SORT: The arguments of the sort
     * - MOXI_DECLARE_FUN, MOXI_DEFINE_FUN: The arguments of the function
     * - MOXI_DEFINE_SYS, MOXI_CHECK_SYS: The arguments of the system
     */
    term_t term;

    /**
     * - MOXI_DECLARE_FUN, MOXI_DEFINE_FUN: The arguments of the function
     */
    moxi_term_vector_t terms;

    /**
     * - MOXI_DEFINE_SYS, MOXI_CHECK_SYS: The system
     */
    moxi_system_t system;

    moxi_check_system_t check_system;
};

/**
 * Initializes a parser for the MoXI language. Returns the parser. Must be deleted with
 * `delete_moxi_parser`. Returns NULL if there is an error.
 */
moxi_parser_t *new_moxi_parser(const char *filename);

/**
 * Deletes a parser for the MoXI language.
 */
void delete_moxi_parser(moxi_parser_t *parser);

/**
 * Returns the next command in the parser. Example usage:
 * ```
 * moxi_parser_t parser = new_moxi_parser("filename.moxi");    
 * moxi_command_t *cmd;
 * while ((cmd = moxi_parser_next_command(parser))) {
 *     // process command
 * }
 * delete_moxi_parser(parser);
 * ```
 */
moxi_command_t *moxi_parser_next_command(moxi_parser_t *parser);

#if __cplusplus
}
#endif

#endif