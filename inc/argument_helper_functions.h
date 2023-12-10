/**
 * @file argument_helper_functions.h
 * @author Rene Ceska xceska06 (xceska06@stud.fit.vutbr.cz)
 * @brief Helper functions for parsing arguments
 * @date 2023-11-19
 */
#ifndef ARGUMENT_HELPER_FUNCTIONS_H
#define ARGUMENT_HELPER_FUNCTIONS_H
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct args_t {
  int gas_cars;
  int electric_cars;
  int morning_boxes;
  int afternoon_boxes;
  int solar_panel_output;
  int morning_shift;
  int afternoon_shift;
  bool help;
  bool err;
} argsT;

/**
 * @brief Parses the arguments from the command line for ldapserver
 *
 * @param argc count of arguments
 * @param argv values of arguments
 * @return argsT
 */
argsT parseArguments(int argc, const char **argv);

#endif