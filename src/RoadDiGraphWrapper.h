/*
 *
 */

#ifndef ROADDIGRAPHWRAPPER
#define ROADDIGRAPHWRAPPER

#include "RoadNetwork.h"
#include "EdgeWeightedDigraph.h"

template <typename edgeWeightType = int>  // Type du poids, par exemple int ou double
                                          // T doit etre comparable, et être un parametre
                                          // valide pour std::numeric_limits
class RoadDiGraphWrapper {
public:
   // Définition de Edge
   using Edge = WeightedDirectedEdge<edgeWeightType>;

   // Constructeur avec la fonction de coût simple
   RoadDiGraphWrapper(const RoadNetwork& rn) : rn(rn) {}

   // Constructeur avec fonction de coût par paramètre
   RoadDiGraphWrapper(const RoadNetwork& rn, std::function<edgeWeightType (const RoadNetwork::Road&)> weightFunction)
   : rn(rn), weightFunction(weightFunction) {}

   // Permet de manipuler une route comme un arc sortant de la ville vDepart et non comme
   // un arc bidirectionnel
   Edge roadToEdge(int vDepart, const RoadNetwork::Road& r) const {
      if (vDepart != r.cities.first) {
         return Edge(vDepart, r.cities.first, weightFunction(r));
      }
      return Edge(vDepart, r.cities.second, weightFunction(r));
   }

   // Permet de manipuler une route comme un arc bidirectionnel
   // On retourne une paire d'arcs représentant les routes A->B et B->A
   std::pair<Edge, Edge> roadToEdges(const RoadNetwork::Road& r) const {
      return std::make_pair(Edge(r.cities.first, r.cities.second, weightFunction(r)),
                            Edge(r.cities.second, r.cities.first, weightFunction(r)));
   }

   // Retourne la taille du réseau routier
   int V() const {
      return (int) rn.cities.size();
   }

   // Parcours de toutes les routes du réseau routier.
   // la fonction f doit prendre un seul argument de type
   // EdgeWeightedDiGraph::Edge
   template <typename Func>
   void forEachEdge(Func f) const {
      for (const RoadNetwork::Road& r : rn.roads) {
         // TODO Vérifier si on peut mieux faire !!
         std::pair<Edge, Edge> edges = roadToEdges(r);
         f(edges.first);   // Arc route A->B
         f(edges.second);  // Arc route B->A
      }
   }

   // Parcours de toutes les routes partant d'une ville
   // la fonction f doit prendre un seul argument de type
   // EdgeWeightedDiGraph::Edge
   template <typename Func>
   void forEachAdjacentEdge(int v, Func f) const {
      std::vector<int> vroads = rn.cities.at(v).roads;
      for(int i : vroads) {
         f(roadToEdge(v, rn.roads.at(i)));
      }
   }

   // Parcours de toutes les villes du réseau routier
   // La fonction f doit prendre un seul argument de type int
   template <typename Func>
   void forEachVertex(Func f) const {
      for (int i = 0; i < V(); i++) {
         f(i);
      }
   }

private:
   const RoadNetwork& rn; // Référence au réseau routier
   // Fonction de coût
   const std::function<edgeWeightType(const RoadNetwork::Road&)> weightFunction =
      [](const RoadNetwork::Road& r) { return r.length; };
};

#endif
