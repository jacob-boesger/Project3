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
using namespace std;
using namespace std::chrono;
using namespace bridges;


string getMinVertex(unordered_map<string, float> dist, unordered_set<string> vs)
{
   float min = std::numeric_limits<double>::infinity();
   string minVector;
   for (auto it = vs.begin(); it != vs.end(); it++)
   {
       if (dist[*it] <= min)
       {
           min = dist[*it];
           minVector = *it;
       }
   }
   return minVector;
}


int getShortestPathsBFS(string actor1, string actor2, unordered_map<string, vector<string>> graph)
{
   if (graph.find(actor2) == graph.end() || graph.find(actor1) == graph.end())
   {
       cout << "Invalid input" << endl;
       return -1;
   }


   unordered_map<string, bool> visited;
   for (auto it = graph.begin(); it != graph.end(); it++)
   {
       visited[it->first] = false;
   }
   queue<string> q;
   //how to return path instead of just shortest distance
   q.push("null");
   q.push(actor1);
   int numSteps = 0;
   while (!q.empty())
   {
       if(q.front() == "null")
       {
           numSteps++;
           q.pop();
           q.push("null");
       }
       string coords = q.front();
       q.pop();
       string name = coords;
       //bool isActor = coords.second;
       vector<string> adjacencies = graph[coords];


       for(int i = 0; i < adjacencies.size(); i++) {
           string ncoords = adjacencies[i];


           if (!visited[ncoords])
           {
               if (ncoords == actor2)
               {
                   return (numSteps++ / 2);
               }
               q.push(ncoords);
               visited[ncoords] = true;
           }
       }
   }
   //cout << "There is no path between " + actor1 + " and " + actor2 << endl;
   return -1;
}


void printShortestPath(string actor1, string actor2, unordered_map<string, vector<string>> graph)
{
   if (graph.find(actor2) == graph.end() || graph.find(actor1) == graph.end())
   {
       cout << "Invalid input" << endl;
       return;
   }


   unordered_set<string> s;
   unordered_set<string> vs;
   unordered_map<string, float> dist;
   unordered_map<string, string> pred;


   for (auto it = graph.begin(); it != graph.end(); it++)
   {
       vs.insert(it->first);
       dist[it->first] = std::numeric_limits<double>::infinity();
       pred[it->first] = "null";
   }
   dist[actor1] = 0;


   while (!vs.empty())
   {
       string v = getMinVertex(dist,vs);
       //cout << v << dist[v] << endl;
       vs.erase(v);
       s.insert(v);
       //cout << vs.size() << endl;
       for (int i = 0; i < graph[v].size(); i++)
       {
           string y = graph[v][i];
           //cout << y.first;
           if (vs.find(y) != vs.end())
           {
               bool wasRelaxed = false;
               if (dist[y] > dist[v] + 1)
               {
                   dist[y] = dist[v] + 1;
                   wasRelaxed = true;
               }
               if (wasRelaxed)
               {
                   pred[y] = v;
               }
           }
       }
   }


   if (actor2 == actor1)
   {
       cout << "Both inputs are the same actor" << endl;
       return;
   }


   cout << actor2 << " was in " << pred[actor2] << " with " << pred[pred[actor2]];
   string print = pred[pred[actor2]];
   while (print != actor1)
   {
       cout << endl;
       cout << print << " was in " << pred[print] << " with " << pred[pred[print]];
       print = pred[pred[print]];
   }
   cout << endl;
}


void printKSmallestBacon(int k, unordered_map<string, vector<string>> graph)
{
   unordered_set<string> s;
   unordered_set<string> vs;
   unordered_set<string> allActors;
   unordered_map<string, float> dist;
   unordered_map<string, string> pred;
   vector<pair<string,int>> kSmallestBacon;


   for (auto it = graph.begin(); it != graph.end(); it++)
   {
       vs.insert(it->first);
       allActors.insert(it->first);
       dist[it->first] = std::numeric_limits<double>::infinity();
       pred[it->first] = "null";
   }
   dist["Kevin Bacon"] = 0;


   while (!vs.empty())
   {
       string v = getMinVertex(dist,vs);
       //cout << v << dist[v] << endl;
       vs.erase(v);
       s.insert(v);
       //cout << vs.size() << endl;
       for (int i = 0; i < graph[v].size(); i++)
       {
           string y = graph[v][i];
           //cout << y.first;
           if (vs.find(y) != vs.end())
           {
               bool wasRelaxed = false;
               if (dist[y] > dist[v] + 1)
               {
                   dist[y] = dist[v] + 1;
                   wasRelaxed = true;
               }
               if (wasRelaxed)
               {
                   pred[y] = v;
               }
           }
       }
   }


   while (k != 0)
   {
       string possibleActor = getMinVertex(dist,allActors);
       allActors.erase(possibleActor);
       if ((int)dist[possibleActor] % 2 == 0 && (int)dist[possibleActor] != 0)
       {
           kSmallestBacon.push_back(make_pair(possibleActor,(int)dist[possibleActor]));
           k--;
       }
   }


   for (int i = 0; i < kSmallestBacon.size()-1; i++)
   {
       cout << kSmallestBacon[i].first << " has a Bacon Number of " << kSmallestBacon[i].second << endl;
   }
   cout << kSmallestBacon[kSmallestBacon.size()-1].first << " has a Bacon Number of " << kSmallestBacon[kSmallestBacon.size()-1].second << endl;
}


int getShortestPathDijkstra(string actor1, string actor2, unordered_map<string, vector<string>> graph)
{
   if (graph.find(actor2) == graph.end() || graph.find(actor1) == graph.end())
   {
       cout << "Invalid input" << endl;
       return -1;
   }


   //map<pair<string,bool>, bool> visited;
   unordered_set<string> s;
   unordered_set<string> vs;
   unordered_map<string, float> dist;
   unordered_map<string, string> pred;


   for (auto it = graph.begin(); it != graph.end(); it++)
   {
       vs.insert(it->first);
       dist[it->first] = std::numeric_limits<double>::infinity();
       pred[it->first] = "null";
   }
   dist[actor1] = 0;


   while (!vs.empty())
   {
       string v = getMinVertex(dist,vs);
       //cout << v << dist[v] << endl;
       vs.erase(v);
       s.insert(v);
       //cout << vs.size() << endl;
       for (int i = 0; i < graph[v].size(); i++)
       {
           string y = graph[v][i];
           //cout << y.first;
           if (vs.find(y) != vs.end())
           {
               bool wasRelaxed = false;
               if (dist[y] > dist[v] + 1)
               {
                   dist[y] = dist[v] + 1;
                   wasRelaxed = true;
               }
               if (wasRelaxed)
               {
                   pred[y] = v;
               }
           }
       }
   }


   return dist[actor2] / 2;
}


void listActorsInMovie(string movie, unordered_map<string,vector<string>> graph)
{
   vector<string> actorList = graph.at(movie);
   for (int i = 0; i < actorList.size()-1; i++)
   {
       cout << actorList[i] << ", ";
   }
   cout << actorList[actorList.size()-1] << endl;
}


void printSharedFilms(string actor1, string actor2, unordered_map<string,vector<string>> graph)
{
   vector<string> sharedFilms;


   for (auto iter = graph.begin(); iter != graph.end(); iter++)
   {
       vector<string> row = iter->second;
       bool containsActor1 = false;
       bool containsActor2 = false;


       for (int i = 0; i < row.size(); i++)
       {
           if (row[i] == actor1)
           {
               containsActor1 = true;
           }
           if (row[i] == actor2)
           {
               containsActor2 = true;
           }
       }


       if (containsActor1 && containsActor2)
       {
           sharedFilms.push_back(iter->first);
       }
   }


   if (sharedFilms.size() == 0)
   {
       cout << actor1 << " and " << actor2 << " are in no films together." << endl;
   }
   for (int i = 0; i < sharedFilms.size()-1; i++)
   {
       cout << actor1 << " is in " << sharedFilms[i] << " with " << actor2 << endl;
   }
   cout << actor1 << " is in " << sharedFilms[sharedFilms.size()-1] << " with " << actor2 << endl;
}


int main(int argc, char **argv)
{


   // Step 3: Test if the following print statement is being run
   cout << "Bridges: IMDB Data in a BST\n";


   /*
    Step 4: Create a Bridges object by uncommenting the next line
    and inserting the values from steps 1 and 2
   */


   Bridges bridges(1, "jacobboesger", "253649919084");


   /*
      Step 5: Import IMDB data into a vector<ActorMovieIMDB> by
              referring to the Bridges IMDB documentation:
              https://bridgesuncc.github.io/tutorials/Data_IMDB.html
   */


   bridges.setTitle("Accessing WikiMovie Data");
   DataSource ds (&bridges);


   vector<MovieActorWikidata> v = ds.getWikidataActorMovie(1940,2019);


   cout << "Data Records in 1955-2000: " << v.size() << "\n";


   string from;
   string to;
   //bool isActor = true;
   unordered_map<string, vector<string>> graph;
   for (int i = 0; i < v.size(); i++)
   {
       from = v[i].getActorName();
       to = v[i].getMovieName();
       bool containsActor = false;
       bool containsMovie = false;
       for (int j = 0; j < graph[from].size(); j++)
       {
           if (graph[from][j] == to)
           {
               containsMovie = true;
           }
       }
       for (int j = 0; j < graph[to].size(); j++)
       {
           if (graph[to][j] == from)
           {
               containsActor = true;
           }
       }
       if (!containsActor)
       {
           graph[to].push_back(from);
       }
       if(!containsMovie)
       {
           graph[from].push_back(to);
       }
   }


   //printKShortestPathsBFS("Kevin Bacon", "Bob Hoskins", graph, 1);


   cout << "Options: " << endl;
   cout << "0 Exit" << endl;
   cout << "1 Print length of shortest path between two actors (BFS)" << endl;
   cout << "2 Print length of shortest path between two actors (Dijkstras)" << endl;
   cout << "3 Print steps in shortest path between two actors" << endl;
   cout << "4 Print every film two actors have starred in together" << endl;
   cout << "5 Print every actor credited in a given film" << endl;
   cout << "6 Print the " << "\"" << "Bacon Number" << "\"" << " of an actor" << endl;
   cout << "7 Print the k actors with the smallest Bacon Numbers" << endl;


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
           cout << getShortestPathsBFS(actor1, actor2, graph) << endl;
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
           cout << getShortestPathDijkstra(actor1,actor2,graph) << endl;
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
           printShortestPath(actor1, actor2, graph);
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
           printSharedFilms(actor1, actor2, graph);
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
           listActorsInMovie(film, graph);
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
           cout << getShortestPathsBFS("Kevin Bacon", actor1, graph) << endl;
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
           printKSmallestBacon(stoi(k), graph);
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


   return 0;
}
