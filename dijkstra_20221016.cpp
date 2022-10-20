#define _USE_MATH_DEFINES

#include "dijkstra_20220514_2.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>


namespace LibCospace {
using std::get;
using std::vector;

vector<vector<std::pair<int, int>>>
Dijkstra::operator()(std::pair<int, int> start,
                     std::vector<std::pair<int, int>> goals) {
  route_map.clear();
  route_map.resize(MAP_WIDTH);
  cost_map.clear();
  cost_map.resize(MAP_WIDTH);
  for (mapsize_t i = 0; i < MAP_WIDTH; i++) {
    route_map[i].resize(MAP_HEIGHT);
    cost_map[i].resize(MAP_HEIGHT);
    for (mapsize_t j = 0; j < MAP_HEIGHT; j++) {
      cost_map[i][j] = -1;
      route_map[i][j] = {-1, -1};
    }
  }

  // 初期位置の代入
  mapsize_t start_x = static_cast<mapsize_t>(start.first / MAP_SCALE);
  mapsize_t start_y = static_cast<mapsize_t>(start.second / MAP_SCALE);

  route_map[start_x][start_y] = {start_x, start_y};
  std::priority_queue<QueType, std::vector<QueType>, Greater> que;
  que.emplace(start_x, start_y, 1);

  while (!que.empty()) {
    auto calc_point = que.top();
    que.pop();

    size_t calc_x = get<0>(calc_point);
    size_t calc_y = get<1>(calc_point);
    cost_t calc_cost = get<2>(calc_point);

    if (confirmed_map.count({calc_x, calc_y})) {
      // コストが確定済みの場合は、飛ばす
      continue;
    }

    // 確定済みに追加
    confirmed_map.emplace(calc_x, calc_y);

    for (size_t x = std::max<size_t>(0, calc_x - 1);
         x <= std::min<size_t>(calc_x + 1, MAP_WIDTH - 1); x++) {
      for (size_t y = std::max<size_t>(0, calc_y - 1);
           y <= std::min<size_t>(calc_y + 1, MAP_HEIGHT - 1); y++) {
        if (x == calc_x && y == calc_y) {
          continue;
        }

        floor_t floor_type = floor_map_.at(x).at(y);
        // 床の種類に対応したコスト
        cost_t cost = floor_type_cost_.at(floor_type);
        if (x != calc_x && y != calc_y) {
          cost *= 1.4;
        }

        cost += calc_cost;

        // 床の種類(1: 黄色など)

        if (cost < cost_map[x][y] || cost_map[x][y] < 0) {
          cost_map[x][y] = cost;
          route_map[x][y] = {calc_x, calc_y};
          que.push({x, y, cost});
        }
      }
    }
  }

  vector<vector<std::pair<int, int>>> routes = {};
  for (auto &goal : goals) {
    coord_t point = {
        static_cast<mapsize_t>(std::floor(goal.first / MAP_SCALE)),
        static_cast<mapsize_t>(std::floor(goal.second / MAP_SCALE))};

    routes.push_back({});

    while (point.first != start_x || point.second != start_y) {
      routes.back().emplace_back(point.first * MAP_SCALE,
                                 point.second * MAP_SCALE);
      point = route_map.at(point.first).at(point.second);
    }

    std::reverse(std::begin(routes.back()), std::end(routes.back()));
  }

  return routes;
}
} // namespace LibCospace

std::vector<std::vector<LibCospace::Dijkstra::floor_t>> map_data =
    {//0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1
1	2	2	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	2	2	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	1	1	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	3	3	3	3	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	3	0	0	0	1	1	1	1	1	1	1	1	1	1	1	0	0	0	0	0	0	0	1
1	0	0	0	3	0	0	0	1	1	1	1	1	1	1	1	1	1	1	0	0	0	0	0	0	0	1
1	0	0	0	3	0	0	0	1	1	1	1	1	1	1	1	1	1	1	0	0	0	0	0	0	0	1
1	0	0	0	3	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	1
1	0	0	0	0	3	3	3	3	3	3	3	3	3	3	3	3	3	3	3	0	0	0	0	0	0	1
1	1	1	1	1	0	0	0	0	0	0	0	0	1	1	1	1	0	0	0	0	0	0	0	0	0	1
1	1	1	1	1	0	0	0	0	0	0	0	0	1	2	2	1	0	0	0	0	0	0	0	0	0	1
1	1	1	1	1	0	0	0	0	0	0	0	0	1	2	2	1	0	0	0	0	0	0	0	0	0	1
1	1	1	1	1	0	0	0	0	0	0	0	0	1	1	1	1	0	0	0	0	0	0	0	0	0	1
1	1	1	1	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	3	3	3	1
1	1	1	1	1	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	0	3	3	3	1
1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1	1


std::unordered_map<LibCospace::Dijkstra::floor_t, LibCospace::Dijkstra::cost_t>
    floor_type_cost = {
        {0, 3}, // マップ配列での0番のコストは10
        {1, 1000}, // マップ配列での1番のコストは100(トラップなので)
        {2, 800},
        {3, 2000}};

LibCospace::Dijkstra dijkstra = LibCospace::Dijkstra(
    {36, 27}, 10, map_data, {{0}}, // 使っていないので適当な値を入れている
    floor_type_cost);
/*
int main() {
  auto routes = dijkstra({180, 180}, {{300, 200}, {100, 100}});
  for (auto &route : routes) {
    std::cout << "[route] calculated route" << std::endl;
    for (auto &[x, y] : route) {
      std::cout << "[route] " << x << " " << y << std::endl;
    }
  }
}
*/
