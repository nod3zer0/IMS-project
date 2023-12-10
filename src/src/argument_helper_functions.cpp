/**
 * @file argument_helper_functions.cpp
 * @author Rene Ceska xceska06 (xceska06@stud.fit.vutbr.cz)
 * @date 2023-11-19
 */
#include "inc/argument_helper_functions.h"

argsT parseArguments(int argc, const char **argv) {

  // initialize args
  argsT args;
  args.err = false;
  args.gas_cars = -1;
  args.electric_cars = -1;
  args.morning_boxes = -1;
  args.afternoon_boxes = -1;

  // main loop for parsing arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--electric_cars") == 0) {
      // check if there is value for port and if it is in range
      if (i + 1 >= argc || atoi(argv[i + 1]) < 0 || atoi(argv[i + 1]) > 65535) {
        args.err = true;
        return args;
      }
      args.electric_cars = atoi(argv[i + 1]);
      i++; // skip next argument
      continue;
    }
    if (strcmp(argv[i], "--gas_cars") == 0) {
      // check if there is value for port and if it is in range
      if (i + 1 >= argc || atoi(argv[i + 1]) < 0 || atoi(argv[i + 1]) > 65535) {
        args.err = true;
        return args;
      }
      args.gas_cars = atoi(argv[i + 1]);
      i++; // skip next argument
      continue;
    }
    if (strcmp(argv[i], "--morning_boxes") == 0) {
      // check if there is value for port and if it is in range
      if (i + 1 >= argc || atoi(argv[i + 1]) < 0 || atoi(argv[i + 1]) > 65535) {
        args.err = true;
        return args;
      }
      args.morning_boxes = atoi(argv[i + 1]);
      i++; // skip next argument
      continue;
    }
    if (strcmp(argv[i], "--afternoon_boxes") == 0) {
      // check if there is value for port and if it is in range
      if (i + 1 >= argc || atoi(argv[i + 1]) < 0 || atoi(argv[i + 1]) > 65535) {
        args.err = true;
        return args;
      }
      args.afternoon_boxes = atoi(argv[i + 1]);
      i++; // skip next argument
      continue;
    }
    if (strcmp(argv[i], "--solar_panel_output") == 0) {
      // check if there is value for port and if it is in range
      if (i + 1 >= argc || atoi(argv[i + 1]) < 0 || atoi(argv[i + 1]) > 65535) {
        args.err = true;
        return args;
      }
      args.solar_panel_output = atoi(argv[i + 1]);
      i++; // skip next argument
      continue;
    }
    if (strcmp(argv[i], "--help") == 0) {
      args.help = true;
      return args;
    }
    // check if there is unknown argument
    fprintf(stderr, "Unknown argument: %s\n", argv[i]);
    args.err = true;
    return args;
  }
  return args;
}