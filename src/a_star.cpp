#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

class Point {
public:
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    bool operator == (const Point& other)const
    {
        return this->x == other.x && this->y == other.y;
    }
};

class Node {
public:
    Point location;
    int g, h, f;
    Node *parent;

    Node(Point location, int g, int h, Node *parent) :
        location(location), g(g), h(h), f(g + h), parent(parent) {}

    bool operator > (const Node& other)const
    {
        return this->f > other.f;
    }
};

int heuristic(Point a, Point b) {
    // 用示例的Manhattan距离作为heuristic函数
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<Point> reconstruct_path(Node *n) {
    std::vector<Point> path;
    while (n != nullptr) {
        path.push_back(n->location);
        n = n->parent;
    }
    return path;
}

std::vector<Point> a_star(Point start, Point goal, std::vector<std::vector<int>> &map) {
    std::vector<Point> closedList;
    std::priority_queue<Node*> openList;

    Node *startNode = new Node(start, 0, heuristic(start, goal), nullptr);
    openList.push(startNode);

    while (!openList.empty()) {
        Node *current = openList.top();
        openList.pop();

        if (current->location == goal) {
            return reconstruct_path(current);
        }

        closedList.push_back(current->location);

        Point neighbors[] = { Point(current->location.x - 1, current->location.y), // left
                               Point(current->location.x + 1, current->location.y), // right
                               Point(current->location.x, current->location.y - 1), // up
                               Point(current->location.x, current->location.y + 1)  // down
                            };
        for (auto &neighbor : neighbors) {
            if (neighbor.x >= 0 && neighbor.y >= 0 && neighbor.x < map.size() && neighbor.y < map[0].size() && map[neighbor.x][neighbor.y] != 1 
            && std::find(closedList.begin(), closedList.end(), neighbor) == closedList.end()) {
                Node *neighborNode = new Node(neighbor, current->g + 1, heuristic(neighbor, goal), current);
                openList.push(neighborNode);
            }
        }
    }

    return {}; // No path found
}

int main(void)
{
    // 示例地图，0表示可通，1表示障碍物
    std::vector<std::vector<int>> map = {
        {0, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 0, 0}
    };

    Point start(0, 0);
    Point goal(4, 4);

    std::vector<Point> path = a_star(start, goal, map);

    if (path.empty()) {
        std::cout << "No path found!" << std::endl;
    } else {
        std::cout << "Path found with length: " << path.size() - 1 << std::endl;
        for (Point &p : path) {
            std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
        }
    }

    return 0;
}
