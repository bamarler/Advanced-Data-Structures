#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>

using namespace boost;
using namespace std;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS,
                        property<vertex_name_t, std::string>,
                        property<edge_capacity_t, int,
                        property<edge_residual_capacity_t, int,
                        property<edge_reverse_t, Traits::edge_descriptor>>>> Graph;

typedef Traits::vertex_descriptor Vertex;
typedef Traits::edge_descriptor Edge;

void addEdge(Vertex u, Vertex v, int cap, Graph& g) {
    property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
    property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(edge_residual_capacity, g);
    property_map<Graph, edge_reverse_t>::type rev = get(edge_reverse, g);

    Edge e1, e2;
    bool inserted1, inserted2;
    tie(e1, inserted1) = add_edge(u, v, g);
    tie(e2, inserted2) = add_edge(v, u, g);
    capacity[e1] = cap;
    capacity[e2] = 0;
    residual_capacity[e1] = cap;
    residual_capacity[e2] = 0;
    rev[e1] = e2;
    rev[e2] = e1;
}

void print_capacities(Graph& g) {
    property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
    cout << "Capacities:\n";
    for (auto e : make_iterator_range(edges(g))) {
        if (capacity[e] > 0) {
            cout << source(e, g) << " -> " << target(e, g) << " : " << capacity[e] << "\n";
        }
    }
}

void print_residuals(Graph& g) {
    property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
    property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(edge_residual_capacity, g);

    cout << "Residuals:\n";
    for (auto e : make_iterator_range(edges(g))) {
        if (capacity[e] > 0) {
            cout << source(e, g) << " -> " << target(e, g) << " : " << capacity[e] - residual_capacity[e] << "\n";
        }
    }
}

int push_relabel(Graph& g, Vertex source, Vertex sink) {
    property_map<Graph, edge_capacity_t>::type capacity = get(edge_capacity, g);
    property_map<Graph, edge_residual_capacity_t>::type residual_capacity = get(edge_residual_capacity, g);
    property_map<Graph, edge_reverse_t>::type rev = get(edge_reverse, g);

    int V = num_vertices(g);

    vector<int> excess(V);
    vector<int> height(V);

    auto push = [&](Vertex u, Vertex v, Edge e) {
        //Edge e; bool exists;
        //tie(e, exists) = edge(u, v, g);

        //cout << "Pushing " << u << " to " << v << " with residual capacity " << residual_capacity[e] << endl;

        //if (!exists) return;
        //if (residual_capacity[e] <= 0) return;
        //if (height[u] != height[v] + 1) return;

        int delta = min(excess[u], residual_capacity[e]);
        /*if (capacity[e] > 0) {
            residual_capacity[e] -= delta;
        }
        else {
            residual_capacity[rev[e]] += delta;
        } */

       residual_capacity[e] -= delta;
       residual_capacity[rev[e]] += delta;

        excess[u] -= delta;
        excess[v] += delta;

        //cout << "Pushed " << delta << " units of flow from " << u << " to " << v << endl;
    };

    auto relabel = [&](Vertex u) {
        int min_height = INT_MAX;
        for (auto e : make_iterator_range(out_edges(u, g))) {
            if (residual_capacity[e] > 0 && height[target(e, g)] < min_height) {
                min_height = height[target(e, g)];
            }
        }
        if (min_height < INT_MAX) {
            height[u] = min_height + 1;
        }
        //cout << "Relabeled " << u << " to height " << height[u] << endl;
    };

    auto initialize_preflow = [&](Vertex s) {
        for (Vertex v : make_iterator_range(vertices(g))) {
            height[v] = 0;
            excess[v] = 0;
        }
        for (auto e : make_iterator_range(edges(g))) {
            residual_capacity[e] = capacity[e];
        }
        height[s] = V;
        for (auto e : make_iterator_range(out_edges(s, g))) {
            Vertex v = target(e, g);
            residual_capacity[e] = 0;
            residual_capacity[rev[e]] = capacity[e];
            excess[v] = capacity[e];
            excess[s] -= capacity[e];
        }
    };

    initialize_preflow(source);

    queue<Vertex> active;
    for (Vertex u : make_iterator_range(vertices(g))) {
        if (u != source && u != sink && excess[u] > 0) {
            active.push(u);
            //cout << "Queued " << u << endl;
        }
    }

    while (!active.empty()) {
        Vertex u = active.front();
        active.pop();

        bool pushed = false;
        for (auto e : make_iterator_range(out_edges(u, g))) {
            Vertex v = target(e, g);
            //cout << "Checking edge " << u << " -> " << v << " with residual capacity " << residual_capacity[e] << endl;
            if (residual_capacity[e] > 0 && height[u] == height[v] + 1) {
                //cout << "Pushing " << u << " to " << v << " with residual capacity " << residual_capacity[e] << endl;
                push(u, v, e);
                pushed = true;
                if (excess[u] > 0) {
                    active.push(u);
                    //cout << "1 Queued " << u << " with excess " << excess[u] << endl;
                }
            }
            if (v != source && v != sink && excess[v] > 0) {
                active.push(v);
                //cout << "2 Queued " << v << " with excess " << excess[v] << endl;
            }
        }
        if (!pushed && excess[u] > 0) {
            relabel(u);
            active.push(u);
            //cout << "3 Queued " << u << " with excess " << excess[u] << endl;
        }
        /*for (Vertex v : make_iterator_range(vertices(g))) {
            if (v != source && v != sink && excess[v] > 0) {
                active.push(v);
                //cout << "4 Queued " << v << " with excess " << excess[v] << endl;
            }
        } */
        //print_residuals(g);
    }

    int max_flow = 0;
    for (auto e : make_iterator_range(out_edges(source, g))) {
        max_flow += capacity[e] - residual_capacity[e];
    }
    return max_flow;
}

int main() {
    Graph g;

    // Example 1:
    Vertex A = add_vertex(g);
    Vertex B = add_vertex(g);
    Vertex C = add_vertex(g);

    addEdge(A, B, 10, g);
    addEdge(B, C, 5, g);

    print_capacities(g);

    Vertex source = A, sink = C;
    int eFlow = push_relabel_max_flow(g, source, sink);
    cout << "Expected Max Flow: " << eFlow << "\n";
    print_residuals(g);

    int aFlow = push_relabel(g, source, sink);
    cout << "Actual Max Flow: " << aFlow << "\n";
    print_residuals(g);
    cout << "\n";

    // Example 2:
    Vertex D = add_vertex(g);
    addEdge(A, D, 15, g);
    addEdge(D, C, 10, g); 

    print_capacities(g);

    eFlow = push_relabel_max_flow(g, source, sink);
    cout << "Expected Max flow: " << eFlow << "\n";
    print_residuals(g);

    aFlow = push_relabel(g, source, sink);
    cout << "Actual Max flow: " << aFlow << "\n";
    print_residuals(g);
    cout << "\n";

    // Example 3: TA Graph
    Graph g2;
    property_map<Graph, edge_capacity_t>::type capacity2 = get(edge_capacity, g2);
    property_map<Graph, edge_reverse_t>::type rev2 = get(edge_reverse, g2);

    Vertex zero = add_vertex(g2);
    Vertex one = add_vertex(g2);
    Vertex two = add_vertex(g2);
    Vertex three = add_vertex(g2);
    Vertex four = add_vertex(g2);
    Vertex five = add_vertex(g2);

    addEdge(zero, one, 16, g2);
    addEdge(zero, two, 13, g2);
    addEdge(two, one, 4, g2);
    addEdge(one, two, 10, g2);
    addEdge(one, three, 12, g2);
    addEdge(three, two, 9, g2);
    addEdge(two, four, 14, g2);
    addEdge(four, three, 7, g2);
    addEdge(three, five, 20, g2);
    addEdge(four, five, 4, g2);

    ofstream dot_file("graph.dot");
    write_graphviz(dot_file, g2);

    print_capacities(g2);

    source = zero; sink = five;

    eFlow = push_relabel_max_flow(g2, source, sink);
    cout << "Expected Max flow: " << eFlow << "\n";
    print_residuals(g2);

    aFlow = push_relabel(g2, source, sink);
    cout << "Actual Max flow: " << aFlow << "\n";
    print_residuals(g2);
    cout << "\n";

    return 0;
}
