//
// Created by 12859 on 2021/12/27.
//

#include <chrono>
#include "ch.h"
#include "utils.h"
#include "qhull_adapter.h"
using namespace std;



ch::ch(vector<int> &idxes, vector<vector<float>> &point_set, int dim) : pointSet(point_set) {
    this->rskyband = idxes;
    this->rest = unordered_set<int>(idxes.begin(), idxes.end());
    this->all = unordered_set<int>(idxes.begin(), idxes.end());
//        this->pointSet=point_set;
    this->d = dim;
    build_do_re(idxes, point_set, dim);
}

void ch::fast_non_dominate_sort(
        const unordered_map<int, unordered_set<int>> &do_map,
        unordered_map<int, int> &dominated_cnt,
        const vector<int> &last_layer) {
    for (int opt:last_layer) {
        auto iter = do_map.find(opt);
        if (iter != do_map.end()) {
            for (int dominated:iter->second) {
                auto cnt_iter = dominated_cnt.find(dominated);
                if (cnt_iter != dominated_cnt.end()) {
                    cnt_iter->second -= 1;
                }
            }
        }
    }
}

void ch::build_do_re(vector<int> &idxes, vector<vector<float>> &point_set, int dim) {
    for (int i:idxes) {
        dominated_cnt[i] = 0;
        do_map[i] = unordered_set<int>();
        dominated_map[i] = unordered_set<int>();
    }
    for (int ii = 0; ii < idxes.size(); ++ii) {
        int i = idxes[ii];
        for (int ji = ii + 1; ji < idxes.size(); ++ji) {
            int j = idxes[ji];
            if (v1_dominate_v2(point_set[i], point_set[j], dim)) {
                do_map[i].insert(j);
                dominated_map[j].insert(i);
                ++dominated_cnt[j];
            } else if (v1_dominate_v2(point_set[j], point_set[i], dim)) {
                do_map[j].insert(i);
                dominated_map[i].insert(j);
                ++dominated_cnt[i];
//                }else{// non-dominate
            }
        }
    }
}

const vector<int> &ch::get_next_layer() {
    vector<vector<double>> square_vertexes(d + 1, vector<double>(d));
    if (!chs.empty()) {
        fast_non_dominate_sort(do_map, dominated_cnt, chs[chs.size() - 1]);
    }
    vector<int> rest_v;
    for (int i:rest) {
        auto iter = dominated_cnt.find(i);
        if (iter != dominated_cnt.end() && iter->second <= 0) {
            rest_v.push_back(i);
        }
    }
//        cout<<"no. of points to build convex hull: "<<rest_v.size()<<endl;
    vector<int> ch;
    if (rest_v.size() >= d + 1) {
        Qhull q;
        qhull_user qu;
        auto begin = chrono::steady_clock::now();
        build_qhull(rest_v, pointSet, square_vertexes, &q);
        auto end = chrono::steady_clock::now();
        chrono::duration<double> elapsed_seconds = end - begin;
//            cout<<"finish build convex hull: "<<elapsed_seconds.count()<<endl;
        ch = qu.get_CH_pointID(q, rest_v);
        qu.get_neiVT_of_VT(q, rest_v, A_p);
    } else {
        for (int i:rest_v) {
            vector<int> tmp;
            for (int j:rest_v) {
                if (i != j) {
                    tmp.push_back(j);
                }
            }
            A_p[i] = tmp;
            ch.push_back(i);
        }
    }
    chs.push_back(ch);
    for (int idx:ch) {
        pdtid_layer[idx] = chs.size();
        rest.erase(idx);
    }

    return chs.back();
}

int ch::get_option_layer(int option) {
    auto iter = pdtid_layer.find(option);
    if (iter != pdtid_layer.end()) {
        return iter->second;
    } else {
        return -1; // not in current i layers
    }
}

const vector<int> &ch::get_neighbor_vertex(int option) {
//        assert(option>=0 && option <=objCnt);
    auto lazy_get = A_p.find(option);
    if (lazy_get != A_p.end()) {
        return lazy_get->second;
    } else {
        return EMPTY;
    }
}

const vector<int> &ch::get_layer(int which_layer) {
    // layer is starting from 1
    while (chs.size() < which_layer && !rest.empty()) {
        this->get_next_layer();
    }
    if (chs.size() < which_layer || which_layer <= 0) {
        return EMPTY;
    }
    return this->chs[which_layer - 1];
}

ch::~ch() {
}
