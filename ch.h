//
// Created by 12859 on 2021/12/27.
//

#ifndef BTOPK_CH_H
#define BTOPK_CH_H
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include "Qhull.h"
#include "QhullUser.h"
#include "RboxPoints.h"
#include "QhullError.h"
#include "QhullQh.h"
#include "QhullFacet.h"
#include "QhullFacetList.h"
#include "QhullFacetSet.h"
#include "QhullLinkedList.h"
#include "QhullPoint.h"
#include "QhullUser.h"
#include "QhullVertex.h"
#include "QhullVertexSet.h"
#include "Qhull.h"

using orgQhull::Qhull;
using orgQhull::QhullError;
using orgQhull::QhullFacet;
using orgQhull::QhullFacetList;
using orgQhull::QhullFacetSet;
using orgQhull::QhullPoint;
using orgQhull::QhullPoints;
using orgQhull::QhullQh;
using orgQhull::QhullUser;
using orgQhull::QhullVertex;
using orgQhull::QhullVertexSet;
using orgQhull::RboxPoints;
using orgQhull::Coordinates;

class ch{
    std::unordered_set<int> rest;
    std::unordered_map<int, int> pdtid_layer;
    std::unordered_map<int, int> dominated_cnt;// only use for build k-convex-hull

    std::vector<std::vector<int>> chs;

    std::vector<int> EMPTY;
    std::vector<std::vector<float>>& pointSet;
    int d;
public:
    std::unordered_set<int> all;
    std::vector<int> rskyband;
    std::unordered_map<int, std::vector<int>> A_p;
    std::unordered_map<int, std::unordered_set<int>> do_map;
    std::unordered_map<int, std::unordered_set<int>> dominated_map;
    ch(std::vector<int> &idxes, std::vector<std::vector<float>> &point_set, int dim);

    void fast_non_dominate_sort(
            const std::unordered_map<int, std::unordered_set<int>> &do_map,
            std::unordered_map<int, int>& dominated_cnt,
            const std::vector<int> &last_layer);

    void build_do_re(std::vector<int> &idxes, std::vector<std::vector<float>> &point_set, int dim);

    const std::vector<int>& get_next_layer();

    int get_option_layer(int option);

    const std::vector<int>& get_neighbor_vertex(int option);

    const std::vector<int>& get_layer(int which_layer);

    ~ch();
};

template<typename VVF>
void build_qhull(const std::vector<int> &opt_idxes, VVF &PG, std::vector<std::vector<double>> &square_vertexes, Qhull *q_ptr) {
    int dim = square_vertexes[0].size();
    square_vertexes.clear();
    square_vertexes.emplace_back(dim);
    std::string s = std::to_string(dim) + " " + std::to_string(opt_idxes.size() + square_vertexes.size()) + " ";
    for (int opt_idx:opt_idxes) {
        for (int i = 0; i < dim; ++i) {
            s += std::to_string(PG[opt_idx][i]) + " ";
        }
    }
    for (std::vector<double> &square_vertex : square_vertexes) {
        for (float j : square_vertex) {
            s += std::to_string(j) + " ";
        }
    }
    std::istringstream is(s);
    RboxPoints rbox;
    rbox.appendPoints(is);
    q_ptr->runQhull(rbox, "QJ");
}

#endif //BTOPK_CH_H
