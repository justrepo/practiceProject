//
// Created by nikita on 7/13/20.
//
#pragma once

#include "Graph.h"

using std::vector;
using std::string;

class CLI {
  vector<string> arguments;
  string programPath;

  void printUsage();

  void printUsageOfInteractive();

  void printUsageOfSolve();

  void printUsageOfTime();

  void printUsageOfMatrix();

  void printUsageOfDir();

  size_t parseArgument(size_t number, const string &name);

  void solveOrTime(bool time);

public:
  explicit CLI(vector<string> newArguments);

  void run();
};