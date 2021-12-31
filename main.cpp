#include <iostream>
#include <string>
#include <fstream>
#include "utils.h"
#include "rtree_adapter.h"

using namespace std;



int main() {
    int dim=4,q_num=30,k=15;
    string datafile, logfile, idxfile, root_directory, filename, func_str,query_str, build_str;
    fstream log;

    root_directory="/home/kemingli/klevel/";
    filename="real/HOTEL4D";

    datafile=root_directory+"data/"+filename+".dat";
    cout<<datafile<<endl;

    auto ori_data=LoadData(datafile, dim);

    auto data=toKOnion(ori_data, k);

    clock_t rtime=clock();
    unordered_map<long int, RtreeNode *> ramTree;
    Rtree *rtree_rt= nullptr;
    build_rtree(rtree_rt, ramTree, data);
    cout << "build rtree time: " << (clock() - rtime) / (float)CLOCKS_PER_SEC << endl;


    vector<vector<float>> q_list;
    generate_query(q_num, q_list, dim);
    clock_t gbcur_time=clock();
    for(auto& w:q_list){
        clock_t qb=clock();
        auto topk_ids=rtree_topk(rtree_rt, ramTree, w, k);
        cout<<topk_ids<<endl;
        clock_t qe=clock();
        cout << "query time: " << (qe - qb) / (float)CLOCKS_PER_SEC << endl;
//        for(int i:topk_ids){
//            cout<<data[i]<<endl;
//        }
//        cout<<"***************************************************"<<endl;
    }
    cout << "Average topk query time: " << (clock() - gbcur_time) / (float)CLOCKS_PER_SEC / (float) q_num << endl;
    return 0;
}
