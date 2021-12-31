//
// Created by 12859 on 2021/12/27.
//

#include "utils.h"
#include "ch.h"
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

void generate_query(int q_num, vector<vector<float>>& q_list, int dim) {
    srand(0); // random seed
    q_list.clear();
    for (int i=0;i<q_num;i++){ // generate user preference uniform under the constraint \sum v_i=1,
        // to understand this code, you may need some knowledge of definite in
        vector<float> v(dim,0.0);
        float res=1.0;
        for (int d=0;d<dim-1;d++){
            v[d] = res*(1.0-pow((double)random()/RAND_MAX,  1.0/(dim-d)));
            res-=v[d];
        }
        v.push_back(res);
        q_list.push_back(v);
    }
}

vector<vector<float>> LoadData(string &datafile, int dim) {
    fstream fin(datafile, ios::in);
    vector<vector<float>> OriginD;
    vector<float> cl(dim);
    vector<float> cu(dim);
    while (true) {
        int id;

        fin >> id;
        if (fin.eof())
            break;

        vector<float> tmp; tmp.clear();
        for (int d = 0; d < dim; d++) fin >> cl[d];
        for (int d = 0; d < dim; d++) fin >> cu[d];
        for (int d = 0; d < dim; d++) tmp.push_back((cl[d]+cu[d])/2.0);

        OriginD.push_back(tmp);

        //log information
        if (OriginD.size() % 1000 == 0)
            cout << ".";
        if (OriginD.size() % 10000 == 0)
            cout << OriginD.size() << " objects loaded" << endl;
    }

    cout << "Total number of objects: " << OriginD.size() << endl;
    fin.close();
    return OriginD;
}



vector<int> kskyband(vector<vector<float>> &data, int k){
    vector<int> ret;
    vector<int> do_cnt(data.size(), 0);
    vector<int> k_cnt(k, 0);
    for (auto i = 0; i < data.size(); ++i) {
        for (auto j = i + 1; j < data.size(); ++j) {
            if (do_cnt[i] >= k) {
                break;
            }
            if (v1_dominate_v2(data[i], data[j])) {
                ++do_cnt[j];
            } else if (v1_dominate_v2(data[j], data[i])) {
                ++do_cnt[i];
            }
        }
        if (do_cnt[i] < k) {
            ret.push_back(i);
            k_cnt[do_cnt[i]]+=1;
        }
    }
    cout<<"for k-skyband, # of options:"<<endl;
    for (int l = 0; l <k ; ++l) {
        cout<<"@k="<<l+1<<":num="<<k_cnt[l]<<endl;
    }
    return ret;
}

vector<int> onionlayer(vector<int>& ret_layer ,vector<int> &candidate, vector<vector<float>> &data, int k) {
    vector<int> ret;
    if (data.empty()) {
        return ret;
    }
    int dim = data[0].size();
    ch c(candidate, data, dim);
    cout<<"for onion layer, # of options:"<<endl;
    for (int i = 1; i <= k; ++i) {
        for (int id: c.get_layer(i)) {
            ret.push_back(id);
            ret_layer.push_back(i);
        }
        cout << "#k=" << i << ";num=" << c.get_layer(i).size() << endl;
    }
    cout << "#total=" << ret.size() << endl;
    return ret;
}

vector<vector<float>> toKOnion(vector<vector<float>>& input, int k){
//    vector<int> candidate_skyband, candidate_onionlayer;
    clock_t st = clock();
    vector<int> candidate_skyband=kskyband(input, k);
    cout << "The number of options after kskyband: " << candidate_skyband.size() << std::endl;
    //k-onionlayer
    vector<int> layer;
    layer.clear();
    st = clock();
    vector<int> candidate_onionlayer=onionlayer(layer, candidate_skyband, input, k);
    cout << "onionlayer Cost: " << (clock() - st) / (float) CLOCKS_PER_SEC << endl;
    vector<vector<float>> ret;
    for (int i:candidate_onionlayer) {
        ret.emplace_back(input[i]);
    }
    return ret;
}