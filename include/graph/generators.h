#pragma once
#include "graph/graph.h"

Graph makeCycle(int n);
Graph makeComplete(int n);
Graph makePetersen();
Graph makeRandom(int n, float edgeProbability, unsigned seed = 1);