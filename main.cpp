#include <iostream>
#include <string>
#include <vector>
#include "Bridges.h"
#include "DataSource.h"
#include "BSTElement.h"
#include "data_src/MovieActorWikidata.h"
#include <queue>
//#include <map>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <iterator>
#include <algorithm>
#include <set>
#include "movieGraph.h"
using namespace std;
using namespace std::chrono;
using namespace bridges;

int main(int argc, char **argv)
{
    cout << "Bridges: IMDB Data in a BST\n";

    //gets Bridges data
   Bridges bridges(1, USERNAME, APIKEY);

   bridges.setTitle("Accessing WikiMovie Data");
   DataSource ds (&bridges);

   vector<MovieActorWikidata> v = ds.getWikidataActorMovie(1940,2019);
   //cout << "Data Records in 1955-2000: " << v.size() << "\n";

   //adds all actor-movie pairs to movieGraph object
   string from;
   string to;
   movieGraph myGraph;
   for (int i = 0; i < v.size(); i++)
   {
       from = v[i].getActorName();
       to = v[i].getMovieName();
       myGraph.addVertex(from,to);
   }
   //calculates Bacon Numbers of every actor
   myGraph.calculateBaconDists();

   //prints interactive options screen
   cout << "Options: " << endl;
   cout << "0 Exit" << endl;
   cout << "1 Print length of shortest path between two actors (BFS)" << endl;
   cout << "2 Print length of shortest path between two actors (Dijkstras)" << endl;
   cout << "3 Print steps in shortest path between two actors" << endl;
   cout << "4 Print every film two actors have starred in together" << endl;
   cout << "5 Print every actor credited in a given film" << endl;
   cout << "6 Print the " << "\"" << "Bacon Number" << "\"" << " of an actor" << endl;
   cout << "7 Print the k actors with the smallest Bacon Numbers" << endl;


   //code for interactive screen
   //prints the time it takes to execute each function
   string command;
   getline(cin, command);
   while (stoi(command) != 0)
   {
       if (stoi(command) == 1) {
           cout << "Enter two actors: " << endl;
           string line;
           string actor1;
           string actor2;
           getline(cin, line);
           line.erase(0, 1);
           actor1 = line.substr(0, line.find('"'));
           line.erase(0, line.find('"') + 3);
           actor2 = line.substr(0, line.find('"'));
           auto start = high_resolution_clock::now();
           myGraph.printShortestPathBFS(actor1,actor2);
           //cout << getShortestPathsBFS(actor1, actor2, graph) << endl;
           auto stop = high_resolution_clock::now();
           auto duration = duration_cast<microseconds>(stop - start);
           cout << "The function took " << duration.count() << " microseconds to run." << endl;
       }


       else if (stoi(command) == 2) {
           cout << "Enter two actors: " << endl;
           string line;
           string actor1;
           string actor2;
           getline(cin, line);
           line.erase(0, 1);
           actor1 = line.substr(0, line.find('"'));
           line.erase(0, line.find('"') + 3);
           actor2 = line.substr(0, line.find('"'));
           auto start = high_resolution_clock::now();
           myGraph.printShortestPathDijkstra(actor1,actor2);
           //cout << getShortestPathDijkstra(actor1,actor2,graph) << endl;
           auto stop = high_resolution_clock::now();
           auto duration = duration_cast<microseconds>(stop - start);
           cout << "The function took " << duration.count() << " microseconds to run." << endl;
       }


       else if (stoi(command) == 3) {
           cout << "Enter two actors: " << endl;
           string line;
           string actor1;
           string actor2;
           getline(cin, line);
           line.erase(0, 1);
           actor1 = line.substr(0, line.find('"'));
           line.erase(0, line.find('"') + 3);
           actor2 = line.substr(0, line.find('"'));
           auto start = high_resolution_clock::now();
           myGraph.printShortestPath(actor1, actor2);
           auto stop = high_resolution_clock::now();
           auto duration = duration_cast<microseconds>(stop - start);
           cout << "The function took " << duration.count() << " microseconds to run." << endl;
       }


       else if (stoi(command) == 4)
       {
           cout << "Enter two actors: " << endl;
           string line;
           string actor1;
           string actor2;
           getline(cin, line);
           line.erase(0, 1);
           actor1 = line.substr(0, line.find('"'));
           line.erase(0, line.find('"') + 3);
           actor2 = line.substr(0, line.find('"'));
           auto start = high_resolution_clock::now();
           myGraph.printSharedFilms(actor1, actor2);
           auto stop = high_resolution_clock::now();
           auto duration = duration_cast<microseconds>(stop - start);
           cout << "The function took " << duration.count() << " microseconds to run." << endl;
       }


       else if (stoi(command) == 5)
       {
           cout << "Enter a film: " << endl;
           string line;
           string film;
           getline(cin, line);
           line.erase(0,1);
           film = line.substr(0,line.find('"'));
           auto start = high_resolution_clock::now();
           myGraph.listActorsInMovie(film);
           auto stop = high_resolution_clock::now();
           auto duration = duration_cast<microseconds>(stop - start);
           cout << "The function took " << duration.count() << " microseconds to run." << endl;
       }


       else if (stoi(command) == 6)
       {
           cout << "Enter an actor: " << endl;
           string line;
           string actor1;
           getline(cin, line);
           line.erase(0, 1);
           actor1 = line.substr(0, line.find('"'));
           auto start = high_resolution_clock::now();
           myGraph.printShortestPathBacon(actor1);
           //cout << getShortestPathsBFS("Kevin Bacon", actor1, graph) << endl;
           auto stop = high_resolution_clock::now();
           auto duration = duration_cast<microseconds>(stop - start);
           cout << "The function took " << duration.count() << " microseconds to run." << endl;
       }


       else if (stoi(command) == 7)
       {
           cout << "Enter an integer: " << endl;
           string line;
           string k;
           getline(cin, line);
           k = line;
           auto start = high_resolution_clock::now();
           myGraph.printKSmallestBacon(stoi(k));
           auto stop = high_resolution_clock::now();
           auto duration = duration_cast<microseconds>(stop - start);
           cout << "The function took " << duration.count() << " microseconds to run." << endl;
       }


       else
       {
           cout << "Command is not valid" << endl;
       }


       getline(cin, command);
   }

   cout << "Thank you for using the program" << endl;

   return 0;
}
