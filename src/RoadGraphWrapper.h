/*
 *
 */

#ifndef ROADGRAPHWRAPPER
#define ROADGRAPHWRAPPER

#include "RoadNetwork.h"
#include "EdgeWeightedGraph.h"

class RoadGraphWrapper {
public:
   // ...
   using Edge = WeightedEdge<double>;

   RoadGraphWrapper(RoadNetwork rn) : rn(&rn) {}

   /* WeightedEdge<double> roadToEdge(RoadNetwork::Road r) { */

   /* } */

   // Returns graph size
   int V() const {
      return (int) rn->cities.size();
   }

   template <typename Func>
   void forEachEdge(Func f) const {
      /* for (const WeightedEdge<double> e : rn->) { */
      /*    f(e); */
      /* } */
   }

   template <typename Func>
   void forEachAdjacentEdge(int v, Func f) const;

private:
   RoadNetwork* rn;
};

#endif
