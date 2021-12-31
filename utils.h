//
// Created by 12859 on 2021/12/27.
//

#ifndef BTOPK_UTILS_H
#define BTOPK_UTILS_H
#include <vector>
#include <string>

std::vector<std::vector<float>> LoadData(std::string &datafile, int dim);

std::vector<std::vector<float>> toKOnion(std::vector<std::vector<float>>& input, int k);

void generate_query(int q_num, std::vector<std::vector<float>>& q_list, int dim);

template<typename T>
T operator*(const std::vector<T> &v1, const std::vector<T> &v2) {
    assert(v1.size() == v2.size());
    T ret = T();
    for (int i = 0; i < v1.size(); ++i) {
        ret += v1[i] * v2[i];
    }
    return ret;
}

template<typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
    if (v.empty()) {
        return out;
    }
    out << v[0];
    for (auto i = 1; i < v.size(); ++i) {
        out << std::string(", ") << v[i];
    }
    return out;
}

template<typename V, typename U>
bool v1_dominate_v2(const V &v1, const U &v2, size_t size) {
    /*
     * /tpara V array, pointer
     */
    for (auto i = 0; i < size; ++i) {
        if (v1[i] < v2[i]) {
            return false;
        }
    }
    return true;
}

template<typename V>
bool v1_dominate_v2(const V &v1, const V &v2) {
    /*
     * /tpara V vector, vector
     */
    return v1_dominate_v2(v1, v2, v1.size());
}

#endif //BTOPK_UTILS_H