//
// Created by jacob on 12/2/2023.
//

#ifndef PROGRAMMINGQUIZ4_MOVIEGRAPH_H
#define PROGRAMMINGQUIZ4_MOVIEGRAPH_H


class movieGraph {
private:
    unordered_map<string,vector<string>> graph;
    unordered_map<string,float> baconDists;
    string getMinVertex(unordered_map<string,float> dist, unordered_set<string> vs);

public:
    void printShortestPathBFS(string actor1, string actor2);
    void printShortestPath(string actor1, string actor2);
    void printKSmallestBacon(int k);
    void printShortestPathDijkstra(string actor1, string actor2);
    void listActorsInMovie(string movie);
    void printSharedFilms(string actor1, string actor2);
    void calculateBaconDists();
    void printShortestPathBacon(string actor1);
    void addVertex(string from, string to);
};

//function returns the minimum vertex from the input map "dist" which is also in the map "vs"
//O(n) where n is the number of elements in vs
string movieGraph::getMinVertex(unordered_map<string, float> dist, unordered_set<string> vs)
{
    float min = std::numeric_limits<double>::infinity();
    string minVertex;
    for (auto it = vs.begin(); it != vs.end(); it++)
    {
        if (dist[*it] <= min)
        {
            min = dist[*it];
            minVertex = *it;
        }
    }
    return minVertex;
}

//uses a breadth-first search to get the length of the shortest path between actor1 and actor2
//O(V+E) where V is the number of vertices and E is the number of edges
void movieGraph::printShortestPathBFS(string actor1, string actor2)
{
    //prints "Invalid input" and returns if actor1 or actor2 is not in the graph
    if (graph.find(actor2) == graph.end() || graph.find(actor1) == graph.end())
    {
        cout << "Invalid input" << endl;
        return;
    }

    //returns if the user input the same actor twice
    if (actor2 == actor1)
    {
        cout << "Both inputs are the same actor" << endl;
        return;
    }

    unordered_map<string, bool> visited;
    //O(V) where V is the number of vertices
    for (auto it = graph.begin(); it != graph.end(); it++)
    {
        visited[it->first] = false;
    }
    queue<string> q;
    //pushes a marker "null" to represent when one depth layer has been traversed
    q.push("null");
    q.push(actor1);
    int numSteps = 0;
    //O(E) where E is the number of edges in the graph
    while (!q.empty())
    {
        //iterates numSteps whenever queue traversal hits "null" marker
        if(q.front() == "null")
        {
            numSteps++;
            q.pop();
            q.push("null");
        }
        string name = q.front();
        q.pop();
        //bool isActor = coords.second;
        vector<string> adjacencies = graph[name];

        //goes through all adjacencies to "name" and adds them to the queue if they have not been reached
        for(int i = 0; i < adjacencies.size(); i++) {
            string ncoords = adjacencies[i];

            if (!visited[ncoords])
            {
                //if actor2 is reached, print numSteps/2 (length of numSteps includes movies and actors, but only actors contribute to degrees of separation
                if (ncoords == actor2)
                {
                    cout << "There is a " << (numSteps++ / 2) << " step path between " << actor1 << " and " << actor2 << endl;
                    return;
                }
                q.push(ncoords);
                visited[ncoords] = true;
            }
        }
    }
    //if no path was found, print
    cout << "There is no path between " + actor1 + " and " + actor2 << endl;
}

//prints all the steps in the shortest path between actor1 and actor2 using Dijkstra's algorithm
//O(V^2 + E)
void movieGraph::printShortestPath(string actor1, string actor2)
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

    //populates vs with all actor/movie names, dist with infinity, and pred with "null"
    //O(V)
    for (auto it = graph.begin(); it != graph.end(); it++)
    {
        vs.insert(it->first);
        dist[it->first] = std::numeric_limits<double>::infinity();
        pred[it->first] = "null";
    }
    //initializes the distance between actor1 and actor1 to 0
    dist[actor1] = 0;

    //performs Dijkstra's algorithm
    //O(V^2 + E)
    while (!vs.empty())
    {
        //O(n) where n is the size of vs (n starts at a length of V)
        string v = getMinVertex(dist,vs);
        //cout << v << dist[v] << endl;
        vs.erase(v);
        s.insert(v);
        //cout << vs.size() << endl;
        //O(e) where e is the average number of edges at a vertex
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

    //prints steps in shortest path between the two actors
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

//prints the k smallest Bacon numbers using the baconDists map
//O(k + V)
void movieGraph::printKSmallestBacon(int k)
{
    unordered_set<string> allActors;
    vector<pair<string,int>> kSmallestBacon;

    //populates a set with every name in the graph
    //O(V)
    for (auto it = graph.begin(); it != graph.end(); it++)
    {
        allActors.insert(it->first);
    }

    //populates kSmallestBacon with the actor with the smallest Bacon number that is not already in it k times
    //O(k*V)
    while (k != 0)
    {
        //O(V)
        string possibleActor = getMinVertex(baconDists,allActors);
        allActors.erase(possibleActor);
        if ((int)baconDists[possibleActor] % 2 == 0 && (int)baconDists[possibleActor] != 0)
        {
            //cout << possibleActor << endl;
            //cout << allActors.size();
            kSmallestBacon.push_back(make_pair(possibleActor,(int)baconDists[possibleActor]/2));
            k--;
            //cout << k << endl;
        }
    }

    //prints every actor in kSmallestBacon
    //O(k)
    for (int i = 0; i < kSmallestBacon.size()-1; i++)
    {
        cout << kSmallestBacon[i].first << " has a Bacon Number of " << kSmallestBacon[i].second << endl;
    }
    cout << kSmallestBacon[kSmallestBacon.size()-1].first << " has a Bacon Number of " << kSmallestBacon[kSmallestBacon.size()-1].second << endl;
}

//prints the length of the shortest path using Dijkstra's algorithm
//O(V^2 + E)
void movieGraph::printShortestPathDijkstra(string actor1, string actor2)
{
    if (graph.find(actor2) == graph.end() || graph.find(actor1) == graph.end())
    {
        cout << "Invalid input" << endl;
        return;
    }

    if (actor1 == actor2)
    {
        cout << "Both inputs are the same actor" << endl;
        return;
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

    if (dist[actor2] == std::numeric_limits<double>::infinity())
    {
        cout << "There is no path between " << actor1 << " and " << actor2 << endl;
        return;
    }
    //again, degrees of separation are defined as distances between actors, so the printed distance must be divided by 2 to account for the distances travelled to films
    cout << "There is a " << (dist[actor2] / 2) << " step path between " << actor1 << " and " << actor2 << endl;
}

//prints every actor credited in a given film
//O(m) where m is the largest number of actors in any movie
void movieGraph::listActorsInMovie(string movie)
{
    if (graph.find(movie) == graph.end())
    {
        cout << "Invalid input" << endl;
        return;
    }
    vector<string> actorList = graph.at(movie);
    for (int i = 0; i < actorList.size()-1; i++)
    {
        cout << actorList[i] << ", ";
    }
    cout << actorList[actorList.size()-1] << endl;
}

//prints all films two given actors were in together
//O(E*l)
void movieGraph::printSharedFilms(string actor1, string actor2)
{
    vector<string> sharedFilms;

    //iterates through every name in the graph and adds it to sharedFilms if it is adjacent to both actor1 and actor2
    //O(E)
    for (auto iter = graph.begin(); iter != graph.end(); iter++)
    {
        vector<string> row = iter->second;
        bool containsActor1 = false;
        bool containsActor2 = false;

        //O(e)
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

    //prints all films in sharedFilms
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

//performs Dijkstra's algorithm using Kevin Bacon as a center and stores results for quick access
//O(V^2 + E)
void movieGraph::calculateBaconDists()
{
    unordered_set<string> s;
    unordered_set<string> vs;
    unordered_map<string, string> pred;

    for (auto it = graph.begin(); it != graph.end(); it++)
    {
        vs.insert(it->first);
        baconDists[it->first] = std::numeric_limits<double>::infinity();
        pred[it->first] = "null";
    }
    baconDists["Kevin Bacon"] = 0;

    while (!vs.empty())
    {
        string v = getMinVertex(baconDists,vs);
        //cout << v << baconDists[v] << endl;
        vs.erase(v);
        s.insert(v);
        //cout << vs.size() << endl;
        for (int i = 0; i < graph[v].size(); i++)
        {
            string y = graph[v][i];
            //cout << y;
            if (vs.find(y) != vs.end())
            {
                bool wasRelaxed = false;
                if (baconDists[y] > baconDists[v] + 1)
                {
                    baconDists[y] = baconDists[v] + 1;
                    wasRelaxed = true;
                }
                if (wasRelaxed)
                {
                    pred[y] = v;
                }
            }
        }
    }
}

//adds a vertex to the graph
//O(m*l) where m is the length of the longest adjacency list
void movieGraph::addVertex(string from, string to)
{
    bool containsActor = false;
    bool containsMovie = false;
    //checks that from and to are not in each other's adjacency vectors
    for (int j = 0; j < graph[from].size(); j++)
    {
        //O(l) where l is the max length of a string name
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
    //adds edges going both directions because graph is undirected
    if (!containsActor)
    {
        graph[to].push_back(from);
    }
    if(!containsMovie)
    {
        graph[from].push_back(to);
    }
}

//prints the Bacon number of a given actor using the stored Bacon distances for ease of access
//O(1)
void movieGraph::printShortestPathBacon(string actor1)
{
    if (baconDists.find(actor1) == baconDists.end())
    {
        cout << "Invalid input" << endl;
        return;
    }
    if (actor1 == "Kevin Bacon")
    {
        cout << "You input Kevin Bacon!" << endl;
        return;
    }

    cout << actor1 << "'s Bacon Number is " << baconDists[actor1]/2 << endl;
}

#endif //PROGRAMMINGQUIZ4_MOVIEGRAPH_H
