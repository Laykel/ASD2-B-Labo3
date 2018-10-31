/*
 * File:   RoadNetwork.cpp
 * Author: Olivier Cuisenaire
 *
 * Created on 18. novembre 2014, 10:48
 */


#include "RoadNetwork.h"

int RoadNetwork::str2int(const std::string& s) {
   std::istringstream iss(s);
   int i;
   iss >> i;
   return i;
}

RoadNetwork::RoadNetwork(const std::string& filename)
{
   std::ifstream s(filename);

   int N;
   s >> N ;

   cities.resize(N);
   for(int i=0;i<N;++i) {
      std::string name;
      std::getline(s,name);
      if (!name.empty() && name[name.size() - 1] == '\r')
         name.erase(name.size() - 1);
      if(name.size()) {
         cities[i].name = name;
         cityIdx[name] = i;
      } else --i;
   }

   while (s.good()) {
      std::string line;
      std::getline(s,line);
      auto elements = split(line,'\t');

      int c1 = cityIdx[elements[0]];
      int c2 = cityIdx[elements[1]];

      int rIdx = int(roads.size());
      cities[c1].roads.push_back(rIdx);
      cities[c2].roads.push_back(rIdx);
      roads.push_back(Road(c1,c2,
               str2int(elements[2]),
               str2int(elements[3]),
               str2int(elements[4])
               ));
   }
   s.close();
}
