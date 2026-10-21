/**
 * This example shows how to use the parser iterator to parse a file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yices.h>
#include "moxiparser.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <moxi file>\n", argv[0]);
        return 1;
    }

    yices_init();
    moxi_parser_t *parser = new_moxi_parser(argv[1]);
    moxi_command_t *cmd;
    char *s0, *s1;
    const char *const_name;
    size_t i, j;

    while ((cmd = moxi_parser_next_command(parser))) {
        switch (cmd->type) {
            case MOXI_SET_LOGIC:
                printf("(set-logic %s)\n", cmd->str);
                break;
            case MOXI_DECLARE_ENUM_SORT:
                printf("(declare-enum-sort %s (", cmd->str);
                for (i = 0; i < cmd->strings.size; i++) {
                    printf("%s%s", i == 0 ? "" : " ", cmd->strings.data[i]);
                }
                printf("))\n");
                break;
            case MOXI_DECLARE_SORT:
                printf("(declare-sort %s %zu)\n", cmd->str, cmd->arity);
                break;
            case MOXI_DEFINE_SORT:
                printf("(define-sort %s (", cmd->str);
                for (i = 0; i < cmd->sorts.size; i++) {
                    s0 = yices_type_to_string(cmd->sorts.data[i], 80, 20, 0);
                    printf("%s%s", i == 0 ? "" : " ", s0);
                    yices_free_string(s0);
                }
                s0 = yices_type_to_string(cmd->sort, 80, 20, 0);
                printf(") %s)\n", s0);
                yices_free_string(s0);
                break;
            case MOXI_DECLARE_CONST:
                s0 = yices_type_to_string(cmd->sort, 80, 20, 0);
                printf("(declare-const %s %s)\n", cmd->str, s0);
                yices_free_string(s0);
                break;
            case MOXI_DECLARE_FUN:
                printf("(declare-fun %s (", cmd->str);
                for (i = 0; i < cmd->sorts.size; i++) {
                    s0 = yices_type_to_string(cmd->sorts.data[i], 80, 20, 0);
                    printf("%s(%s)", i == 0 ? "" : " ", s0);
                    yices_free_string(s0);
                }
                s0 = yices_type_to_string(cmd->sort, 80, 20, 0);
                printf(") %s)\n", s0);
                yices_free_string(s0);
                break;
            case MOXI_DEFINE_FUN:
                printf("(define-fun %s (", cmd->str);
                for (i = 0; i < cmd->terms.size; i++) {
                    s0 = yices_term_to_string(cmd->terms.data[i], 80, 20, 0);
                    s1 = yices_type_to_string(yices_type_of_term(cmd->terms.data[i]), 80, 20, 0);
                    printf("%s(%s %s)", i == 0 ? "" : " ", s0, s1);
                    yices_free_string(s0);
                    yices_free_string(s1);
                }
                s0 = yices_type_to_string(cmd->sort, 80, 20, 0);
                printf(") %s)\n", s0);
                yices_free_string(s0);
                break;
            case MOXI_DEFINE_SYSTEM:
                printf("(define-system %s\n", cmd->str);

                if (cmd->system.input.size > 0) {
                    printf("  :input (");
                    for (i = 0; i < cmd->system.input.size; i++) {
                        s0 = yices_term_to_string(cmd->system.input.data[i], 80, 20, 0);
                        s1 = yices_type_to_string(yices_type_of_term(cmd->system.input.data[i]), 80, 20, 0);
                        printf("%s(%s %s)", i == 0 ? "" : " ", s0, s1);
                        yices_free_string(s0);
                        yices_free_string(s1);
                    }
                    printf(")\n");
                }

                if (cmd->system.output.size > 0) {
                    printf("  :output (");
                    for (i = 0; i < cmd->system.output.size; i++) {
                        s0 = yices_term_to_string(cmd->system.output.data[i], 80, 20, 0);
                        s1 = yices_type_to_string(yices_type_of_term(cmd->system.output.data[i]), 80, 20, 0);
                        printf("%s(%s %s)", i == 0 ? "" : " ", s0, s1);
                        yices_free_string(s0);
                        yices_free_string(s1);
                    }
                    printf(")\n");
                }

                if (cmd->system.local.size > 0) {
                    printf("  :local (");
                    for (i = 0; i < cmd->system.local.size; i++) {
                        s0 = yices_term_to_string(cmd->system.local.data[i], 80, 20, 0);
                        s1 = yices_type_to_string(yices_type_of_term(cmd->system.local.data[i]), 80, 20, 0);
                        printf("%s(%s %s)", i == 0 ? "" : " ", s0, s1);
                        yices_free_string(s0);
                        yices_free_string(s1);
                    }
                    printf(")\n");
                }

                if (cmd->system.init_term != NULL_TERM && cmd->system.init_term != yices_true()) {
                    s0 = yices_term_to_string(cmd->system.init_term, 80, 20, 0);
                    printf("  :init %s\n", s0);
                    yices_free_string(s0);
                }

                if (cmd->system.trans_term != NULL_TERM && cmd->system.trans_term != yices_false()) {
                    s0 = yices_term_to_string(cmd->system.trans_term, 80, 20, 0);
                    printf("  :trans %s\n", s0);
                    yices_free_string(s0);
                }

                if (cmd->system.inv_term != NULL_TERM && cmd->system.inv_term != yices_true()) {
                    s0 = yices_term_to_string(cmd->system.inv_term, 80, 20, 0);
                    printf("  :inv %s\n", s0);
                    yices_free_string(s0);
                }

                for (i = 0; i < cmd->system.subsystems.size; i++) {
                    printf("  :subsys (%s (%s", 
                        cmd->system.subsystems.data[i].name,
                        cmd->system.subsystems.data[i].system
                    );
                    for (j = 0; j < cmd->system.subsystems.data[i].input.size; j++) {
                        s0 = yices_term_to_string(cmd->system.subsystems.data[i].input.data[j], 80, 20, 0);
                        printf(" %s", s0);
                        yices_free_string(s0);
                    }
                    for (j = 0; j < cmd->system.subsystems.data[i].output.size; j++) {
                        s0 = yices_term_to_string(cmd->system.subsystems.data[i].output.data[j], 80, 20, 0);
                        printf(" %s", s0);
                        yices_free_string(s0);
                    }
                    printf(")\n");
                }

                printf(")\n");
                break;
            case MOXI_CHECK_SYSTEM:
                printf("(check-system %s\n", cmd->str);

                if (cmd->check_system.input.size > 0) {
                    printf("  :input (");
                    for (i = 0; i < cmd->check_system.input.size; i++) {
                        s0 = yices_term_to_string(cmd->check_system.input.data[i], 80, 20, 0);
                        s1 = yices_type_to_string(yices_type_of_term(cmd->check_system.input.data[i]), 80, 20, 0);
                        printf("%s(%s %s)", i == 0 ? "" : " ", s0, s1);
                        yices_free_string(s0);
                        yices_free_string(s1);
                    }
                    printf(")\n");
                }

                if (cmd->check_system.output.size > 0) {
                    printf("  :output (");
                    for (i = 0; i < cmd->check_system.output.size; i++) {
                        s0 = yices_term_to_string(cmd->check_system.output.data[i], 80, 20, 0);
                        s1 = yices_type_to_string(yices_type_of_term(cmd->check_system.output.data[i]), 80, 20, 0);
                        printf("%s(%s %s)", i == 0 ? "" : " ", s0, s1);
                        yices_free_string(s0);
                        yices_free_string(s1);
                    }
                    printf(")\n");
                }

                if (cmd->check_system.local.size > 0) {
                    printf("  :local (");
                    for (i = 0; i < cmd->check_system.local.size; i++) {
                        s0 = yices_term_to_string(cmd->check_system.local.data[i], 80, 20, 0);
                        s1 = yices_type_to_string(yices_type_of_term(cmd->check_system.local.data[i]), 80, 20, 0);
                        printf("%s(%s %s)", i == 0 ? "" : " ", s0, s1);
                        yices_free_string(s0);
                        yices_free_string(s1);
                    }
                    printf(")\n");
                }

                if (cmd->check_system.assumption.size > 0) {
                    for (i = 0; i < cmd->check_system.assumption.size; i++) {
                        const_name = cmd->check_system.assumption.names[i];
                        s1 = yices_term_to_string(cmd->check_system.assumption.data[i], 80, 20, 0);
                        printf("  :assumption (%s %s)\n", const_name, s1);
                        yices_free_string(s1);
                    }
                }

                if (cmd->check_system.fairness.size > 0) {
                    for (i = 0; i < cmd->check_system.fairness.size; i++) {
                        const_name = cmd->check_system.fairness.names[i];
                        s1 = yices_term_to_string(cmd->check_system.fairness.data[i], 80, 20, 0);
                        printf("  :fairness (%s %s)\n", const_name, s1);
                        yices_free_string(s1);
                    }
                }

                if (cmd->check_system.reachable.size > 0) {
                    for (i = 0; i < cmd->check_system.reachable.size; i++) {
                        const_name = cmd->check_system.reachable.names[i];
                        s1 = yices_term_to_string(cmd->check_system.reachable.data[i], 80, 20, 0);
                        printf("  :reachable (%s %s)\n", const_name, s1);
                        yices_free_string(s1);
                    }
                }

                if (cmd->check_system.current.size > 0) {
                    for (i = 0; i < cmd->check_system.current.size; i++) {
                        const_name = cmd->check_system.current.names[i];
                        s1 = yices_term_to_string(cmd->check_system.current.data[i], 80, 20, 0);
                        printf("  :current (%s %s)\n", const_name, s1);
                        yices_free_string(s1);
                    }
                }

                if (cmd->check_system.query.size > 0) {
                    for (i = 0; i < cmd->check_system.query.size; i++) {
                        printf("  :query (%s", cmd->check_system.query.names[i]);
                        for (j = 0; j < cmd->check_system.query.sizes[i]; j++) {
                            printf(" %s", cmd->check_system.query.strings[i][j]);
                        }
                        printf(")\n");
                    }
                }

                printf(")\n");
                break;
            default:
                printf("UNKNOWN COMMAND\n");
                break;
        }
    }
    delete_moxi_parser(parser);
    yices_exit();
}
