/*
 * File:   main.cpp
 * ASD 2 - Labo 3
 * Author: Olivier Cuisenaire
 *
 * Created on 18. novembre 2014, 14:58
 */

#include <cstdlib>
#include <iostream>
#include <ctime>

#include "RoadNetwork.h"
#include "MinimumSpanningTree.h"
#include "ShortestPath.h"

#include "EdgeWeightedGraph.h"
#include "EdgeWeightedDiGraph.h"

#include "RoadGraphWrapper.h"
#include "RoadDiGraphWrapper.h"

using namespace std;

// Calcule et affiche le plus court chemin de la ville depart a la ville arrivee
// en passant par le reseau routier rn. Le critere a optimiser est la distance.
void PlusCourtChemin(const string& depart, const string& arrivee, RoadNetwork& rn) {
   // Voir le réseau de routes comme un graphe orienté
   RoadDiGraphWrapper<> rdgw(rn);
   int idxDepart  = rn.cityIdx[depart];  // Calculer l'index de la ville de départ
   int idxArrivee = rn.cityIdx[arrivee]; // Calculer l'index de la ville d'arrivée

   // Construire les plus courts chemins depuis idxDepart
   DijkstraSP<RoadDiGraphWrapper<>> sp(rdgw, idxDepart);

   // Afficher chaque ville dans le PCC
   for (RoadDiGraphWrapper<>::Edge edge : sp.PathTo(idxArrivee)) {
      cout << rn.cities.at(edge.From()).name << " -> ";
   }
   // Afficher la ville d'arrivée
   cout << rn.cities.at(idxArrivee).name << endl;
   // Afficher la distance totale
   cout << "Distance totale : " << sp.DistanceTo(idxArrivee) << "km" << endl;
}

// Calcule et affiche le plus rapide chemin de la ville depart a la ville arrivee via la ville "via"
// en passant par le reseau routier rn. Le critere a optimiser est le temps de parcours
// sachant que l'on roule a 120km/h sur autoroute et 70km/h sur route normale.
void PlusRapideChemin(const string& depart, const string& arrivee, const string& via, RoadNetwork& rn) {
   // Voir le réseau de routes comme un graphe orienté
   // Spécifier la fonction de coût pour utiliser des minutes et non des km
   RoadDiGraphWrapper<double> rdgw(rn, [](const RoadNetwork::Road& r) {
      double timeOnMotorway = (r.length * r.motorway.Value()) / 120;
      double timeOnNormalRoad = (r.length * (1 - r.motorway.Value())) / 70;
      // Return time in minutes
      return (timeOnMotorway + timeOnNormalRoad) * 60;
   });

   int idxDepart  = rn.cityIdx[depart];  // Calculer l'index de la ville de départ
   int idxVia     = rn.cityIdx[via];     // Calculer l'index de la ville de départ
   int idxArrivee = rn.cityIdx[arrivee]; // Calculer l'index de la ville d'arrivée

   // Construire les plus courts chemins depuis idxDepart
   DijkstraSP<RoadDiGraphWrapper<double>> spFromStart(rdgw, idxDepart);
   // Afficher chaque ville dans le PCC, jusqu'à via
   for (RoadDiGraphWrapper<double>::Edge edge : spFromStart.PathTo(idxVia)) {
      cout << rn.cities.at(edge.From()).name << " -> ";
   }

   // Construire les plus courts chemins depuis idxVia
   DijkstraSP<RoadDiGraphWrapper<double>> spFromVia(rdgw, idxVia);
   // Afficher chaque ville dans le PCC, jusqu'à via
   for (RoadDiGraphWrapper<double>::Edge edge : spFromVia.PathTo(idxArrivee)) {
      cout << rn.cities.at(edge.From()).name << " -> ";
   }
   // Afficher la ville d'arrivée
   cout << rn.cities.at(idxArrivee).name << endl;
   // Afficher le temps total
   cout << "Temps total : "
   << spFromStart.DistanceTo(idxVia) + spFromVia.DistanceTo(idxArrivee)
   << " minutes" << endl;
}

// Calcule et affiche le plus reseau a renover couvrant toutes les villes le moins
// cher, en sachant que renover 1km d'autoroute coute 15 MF, et renover 1km de route normale
// coute 7 MF.
void ReseauLeMoinsCher(RoadNetwork &rn) {
   /* A IMPLEMENTER */
}

// compare les algorithmes Dijkstra et BellmanFord pour calculer les plus courts chemins au
// sommet 0 dans le graphe defini par filename.
// a utiliser pour tester votre implementation de Dijkstra
void testShortestPath(string filename) {
   cout << "Testing " << filename << endl;

   bool ok = true;

   typedef EdgeWeightedDiGraph<double> Graph;
   Graph ewd(filename);

   clock_t startTime = clock();

   BellmanFordSP<Graph> referenceSP(ewd,0);

   cout << "Bellman-Ford: " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;
   startTime = clock();

   DijkstraSP<Graph> testSP(ewd,0);

   cout << "Dijkstra:     " << double( clock() - startTime ) / (double)CLOCKS_PER_SEC<< " seconds." << endl;

   for (int v=0; v<ewd.V(); ++v) {
      if (referenceSP.DistanceTo(v) != testSP.DistanceTo(v) ) {
         cout << "Oops: vertex" << v << " has " << referenceSP.DistanceTo(v) << " != " <<  testSP.DistanceTo(v) << endl;
         ok = false;
         break;
      }
   }

   if(ok) cout << " ... test succeeded " << endl << endl;
}

int main(int argc, const char * argv[]) {
   /* testShortestPath("tinyEWD.txt"); */
   /* testShortestPath("mediumEWD.txt"); */
   /* testShortestPath("1000EWD.txt"); */
   /* testShortestPath("10000EWD.txt"); */

   RoadNetwork rn("reseau.txt"); // Création du réseau routier depuis fichier

   cout << "1. Chemin le plus court entre Geneve et Emmen" << endl;

   PlusCourtChemin("Geneve", "Emmen", rn);

   cout << "2. Chemin le plus court entre Lausanne et Bale" << endl;

   PlusCourtChemin("Lausanne", "Basel", rn);

   cout << "3. Chemin le plus rapide entre Geneve et Emmen en passant par Yverdon" << endl;

   PlusRapideChemin("Geneve", "Emmen", "Yverdon-Les-Bains", rn);

   cout << "4. Chemin le plus rapide entre Geneve et Emmen en passant par Vevey" << endl;

   PlusRapideChemin("Geneve", "Emmen", "Vevey", rn);

   cout << "5. Quelles routes doivent etre renovees ? Quel sera le cout de la renovation de ces routes ?" << endl;

   ReseauLeMoinsCher(rn);

   return EXIT_SUCCESS;
}
