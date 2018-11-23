/*
 *
 */

#ifndef ROADGRAPHWRAPPER
#define ROADGRAPHWRAPPER

#include "RoadNetwork.h"
#include "EdgeWeightedGraph.h"

template <typename edgeWeightType = int> // Type du poids, par exemple int ou double
                                         // T doit etre comparable, et être un parametre
                                         // valide pour std::numeric_limits
class RoadGraphWrapper {
public:
   // Définition de Edge
   using Edge = WeightedEdge<edgeWeightType>;

   // Constructeur avec la fonction de coût simple
   RoadGraphWrapper(const RoadNetwork& rn) : rn(rn) {}

   // Constructeur avec fonction de coût par paramètre
   RoadGraphWrapper(const RoadNetwork& rn, std::function<edgeWeightType (const RoadNetwork::Road&)> weightFunction)
   : rn(rn), weightFunction(weightFunction) {}

   // Permet de manipuler une route comme un arc
   Edge roadToEdge(const RoadNetwork::Road& r) const {
      return Edge(r.cities.first, r.cities.second, weightFunction(r));
   }

   // Retourne la taille du réseau routier
   int V() const {
      return (int) rn.cities.size();
   }

   template <typename Func>
   void forEachEdge(Func f) const {
      for (const RoadNetwork::Road& r : rn.roads) {
         f(roadToEdge(r));
      }
   }

   template <typename Func>
   void forEachAdjacentEdge(int v, Func f) const {
      std::vector<int> vroads = rn.cities.at(v).roads;
      for (int i : vroads) {
         f(rn.roads.at(i));
      }
   }

private:
   RoadNetwork rn;
   std::function<edgeWeightType(const RoadNetwork::Road&)> weightFunction = [](const RoadNetwork::Road& r) { return r.length; };
};

#endif
