/*
 *
 */

#ifndef ROADGRAPHWRAPPER
#define ROADGRAPHWRAPPER

#include "RoadNetwork.h"

class RoadGraphWrapper {
   struct Edge {

   };

   RoadGraphWrapper(RoadNetwork rn);

   int V() const;

   template <typename Func>
   void forEachEdge(Func f) const;

   template <typename Func>
   void forEachAdjacentEdge(int v, Func f) const;
};

#endif
