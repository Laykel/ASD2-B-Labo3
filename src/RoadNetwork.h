/*
 * File:   RoadNetwork.h
 * Author: Olivier Cuisenaire
 *
 * Created on 18. novembre 2014, 10:48
 */

#ifndef __ASD2__RoadNetwork__
#define __ASD2__RoadNetwork__

#include <stdio.h>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <fstream>
#include <sstream>

#include "Util.h"

// Classe lisant et donnant accès au reseau routier

class RoadNetwork {
public:
   // Classe fraction.
   struct Fraction {
   public:
      int numerator;
      int denominator;
      Fraction(int n, int d) : numerator(n), denominator(d) { }
      double Value() const { return (numerator*1.0)/denominator; }
   };

   // Classe City stocke le nom de la ville et l'indice des routes qui
   // en partent dans le tableau roads
   struct City {
   public:
      std::string name;
      std::vector<int> roads;
   };

   // Classe Road, stocke les indices dans cities des deux villes que la route lie,
   // la longueur en km, et la fraction d'autoroute sur ce trajet.
   struct Road {
   public:
      Road(int c1, int c2, int length, int num, int denom) :
         cities(std::make_pair(c1,c2)), length(length), motorway(Fraction(num,denom)) { }

      std::pair<int,int> cities;
      int length;
      Fraction motorway;
   };

   // vecteur des villes du reseau routier
   std::vector<City> cities;

   // vecteur des routes du reseau routier.
   std::vector<Road> roads;

   // permet de trouver l'indice d'une ville dans cities par son nom.
   std::map<std::string,int> cityIdx;

   int str2int(const std::string& s);

   // affiche le reseau
   friend std::ostream& operator << (std::ostream& s, const RoadNetwork& rn) {
      for( auto r : rn.roads )
         s << rn.cities[r.cities.first].name << " - "
            << rn.cities[r.cities.second].name
            << " : \t" << r.length << " km"
            << " , " << r.motorway.numerator << "/" << r.motorway.denominator << " d'autoroute"
            << std::endl;
      return s;
   }

   // Constructeur avec nom de fichier.
   RoadNetwork(const std::string& filename);
};

#endif /* defined(__ASD2__RoadNetwork__) */
