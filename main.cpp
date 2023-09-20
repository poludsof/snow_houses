#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>

struct node {
    int city_name;
    int weight;
};

void print_matrix(const std::vector<std::vector<int>>& matrix) {
//    int t = 1;
    std::cout << "   ";
    for (int i = 0; i < matrix.size(); ++i)
        std::cout << i << " ";
    std::cout << std::endl;
    for (int i = 0; i < matrix.size(); ++i)
        std::cout << "---";
    std::cout << std::endl;

    for (int i = 0; i < matrix.size(); ++i) {
        std::cout << i << "| ";
        for (int j = 0; j < matrix.size(); ++j) {
//            std::cout << i << "," << j << std::endl;
            if (j == i || matrix[i][j] == 0) {
                std::cout << "- ";
            }
            else
                std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void print_visited(const std::vector<int>& visited) {
    std::cout << "visited: ";
    for (int i : visited) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

void print_cities(const std::vector<node> &city_neighbors) {
    std::cout << "city_neighbors: ";
    for (auto city_neighbor : city_neighbors) {
        std::cout << city_neighbor.city_name << "[" << city_neighbor.weight << "] ";
    }
    std::cout << std::endl;
}

void print_regions(const std::vector<std::vector<int>> &regions, int region_count) {
    for (int i = 0; i < region_count; ++i) {
        std::cout << "REGION #" << i << " :";
        for (int j : regions[i]) {
            std::cout << " " << j;
        }
        std::cout << std::endl;
    }
}

int get_index(const std::vector<node> &city_neighbors, int city) {
    for (int i = 0; i < city_neighbors.size(); ++i) {
        if (city_neighbors[i].city_name == city)
            return i;
    }
    return -1;
}

std::vector<std::vector<int>> distribute_by_regions(const std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>> regions, int d, int t) {
    for (int i = 0; i < d; ++i) {
        regions[i].push_back(i);
    }

    // Find the region for each city
    for (int source = d; source < t; ++source) {
        std::vector<int> dist;
        std::vector<bool> visited;
        std::queue<int> queue;

        std::fill(visited.begin(), visited.end(), -1);
        visited.resize(t);

        dist.resize(t);
//        for (int i = 0; i < t; ++i) {
//            std::cout << dist[i] << std::endl;
//        }

        visited[source] = true;

        queue.push(source);

        while (!queue.empty()) {
            for (int ngb = 0; ngb < matrix.size(); ++ngb) {
                if (matrix[queue.front()][ngb] != 0) {
                    // THE ROAD EXISTS
                    if (!visited[ngb]) { // not visited
                        visited[ngb] = true;
                        queue.push(ngb);
                        dist[ngb] = dist[queue.front()] + 1;
                    }
                }
            }
            queue.pop();
        }
        for (int i = 0; i < t; ++i) {
            std::cout << i << " ";
        } std::cout << std::endl;
        for (int i = 0; i < t; ++i) {
            std::cout << dist[i] << " ";
        } std::cout << std::endl;
        std::cout << std::endl;
        int min = INFINITY;
        int capital;
        for (int i = 0; i < d; ++i) {
            if (dist[i] < min) {
                min = dist[i];
                capital = i;
            }
        }
        regions[capital].push_back(source);
    }
    return regions;
}

/* Prim's algorithm */
void minimum_spanning_tree(const std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>> regions, int d) {
    int weight, city_1 = -1, city_2 = -1;
    for (int reg = 0; reg < d; ++reg) {  // # reg - one region

        // Create a new list of visited cities and add the first city
        std::vector<int> visited;
        visited.push_back(regions[reg][0]);

        std::cout << std::endl;
        std::cout << std::endl;

        // Until all cities in the region have been visited
        while (visited.size() != regions[reg].size()) {
            weight = INFINITY;

            // Find the minimum road from all the cities that were visited
            for (int i : visited) {
                for (int j: regions[reg]) {
                    // The road exists between the cities and the road has lower weight
                    if (matrix[j][i] != 0 && matrix[j][i] < weight) {

                        // If at least one of the cities has not been visited
                        if (std::find(visited.begin(), visited.end(), j) == visited.end() ||
                            std::find(visited.begin(), visited.end(), i) == visited.end()) {

                            // To memorize between cities this road
                            weight = matrix[j][i];
                            city_1 = j;
                            city_2 = i;
                        }
                    }
                }
            }
            if (std::find(visited.begin(), visited.end(), city_1) == visited.end()) {
                std::cout << "ROAD " << city_1 << " - " << city_2 << std::endl;
                visited.push_back(city_1);
            }
            if (std::find(visited.begin(), visited.end(), city_2) == visited.end()) {
                visited.push_back(city_2);
                std::cout << "ROAD " << city_1 << " - " << city_2 << std::endl;
            }
        }
    }
}

int main() {
    int t, d, r;
    std::vector<std::vector<int>> matrix_of_roads;

    std::cin >> t >> d >> r;
    std::cout << "number of cities: " << t << std::endl;
    std::cout << "number of regions: " << d << std::endl;
    std::cout << "number of roads: " << r << std::endl;

    // Fill the matrix with 0
    matrix_of_roads.resize(t);
    for (int i = 0; i < t; ++i)
        matrix_of_roads[i].resize(t);

    // Read the graph from the keyboard into a matrix
    int from, to, weight;
    for (int i = 0; i < r; ++i) {
        std::cin >> from >> to >> weight;
        matrix_of_roads[from][to] = weight;
        matrix_of_roads[to][from] = weight;
    }

    // Distribute cities by regions and write into a 2d vector
    std::vector<std::vector<int>> regions;
    regions.resize(d);

    // #1
    regions = distribute_by_regions(matrix_of_roads, regions, d, t);
    print_regions(regions, d);

    return 0;

    print_matrix(matrix_of_roads);
    print_regions(regions, d);

    // #2
    // Find the minimum spanning tree for each region
    minimum_spanning_tree(matrix_of_roads, regions, d);

    return 0;
}
