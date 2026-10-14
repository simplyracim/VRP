#pragma once
#include "graph/graph.h"

void randomizePositions(Graph& g, float radius = 2.0f, unsigned seed = 1);
void forceLayout(Graph& g, int iterations = 300, float idealLength = 1.5f);