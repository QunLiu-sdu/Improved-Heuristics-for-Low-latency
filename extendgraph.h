#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<list>
#include <algorithm>

using namespace std;
class SingleGraph {
public:
    vector<string> Node;
    map<string, vector<vector<string>>> NodeFro;
    map<string, vector<string>> NodeOut;
    vector<string> TopologyList;
};

class ExtendGraph {
public:
    int Size = 32;
    vector<string> Base;
    vector<string> TargetNode;
    map<int, int> Value;
    map<int, vector<int>> HammingSet;

    vector<string> Node;
    map<string, vector<vector<string>>> NodeFro;
    map<string, vector<string>> NodeOut;
    vector<string> TopologyList;

    map<string, vector<string>> CanGetSet;
    map<string, vector<string>> CanFromSet;
    map<string, vector<string>> NumberInNode;
    map<string, vector<string>> CostInNode;
    vector<SingleGraph> AllSingleGraph;
    map<int, int> outputdict;
    int Xor2Count = 0;

    int MinCost1 = 100000;
    vector<int> MinNum1;
    vector<int> MinDepth1;
    int MinCost0 = 100000;
    vector<int> MinNum0;
    int MinDepth0 = -1;

    int MinCost = 100000;
    vector<int> MinNum;
    vector<int> MinDepth;

    ExtendGraph(int NumInput)
    {
        Size = NumInput;
    }


    void ReadFromXiang() {
        ifstream infile;
        ofstream outfile;





        map<int, int> nodedict;
        int nextnodenum = Size;
        for (int i = 0; i < Size; i++) {
            nodedict[i] = i;
        }
        infile.open("seq.txt");
        outfile.open("myseq.txt");
        char str[100] = {};
        infile.getline(str, 50);
        if (strlen(str) != 0) {
            outfile << str << endl;
            infile.getline(str, 50);
        }
        while (strlen(str) != 0) {
            vector<string> num;
            num = findall(str);
            int num0 = atoi(num[0].c_str());
            int num1 = atoi(num[1].c_str());
            int num2 = atoi(num[2].c_str());

            num1 = nodedict[num1];
            num2 = nodedict[num2];
            nodedict[num0] = nextnodenum;
            num0 = nextnodenum;
            nextnodenum += 1;
            if (num.size() == 3) {
                outfile << "t[" << num0 << "] = t[" << num1 << "] + t[" << num2 << "]" << endl;
            }
            else {
                outfile << "t[" << num0 << "] = t[" << num1 << "] + t[" << num2 << "]   y[" << num[3] << "]" << endl;
            }
            infile.getline(str, 50);
        }
        infile.close();
        outfile.close();
    }

    void ReadFromSeq() {
        int num = 1;
        for (int i = 0; i < Size; i++) {
            Value[i] = num;
            vector<int> h;
            HammingSet.emplace(i, h);
            HammingSet[i].push_back(i);

            num *= 2;

        }
        ifstream infile;
        infile.open("myseq.txt");
        char line[50] = {};
        infile.getline(line, 50);
        string s;
        if (strlen(line) > 0) {
            for (int i = 0; i < strlen(line); i++) {

                if (line[i] >= '0' && line[i] <= '9') {
                    s += line[i];

                }
            }
            Xor2Count = atoi(s.c_str());
            //cout<<Xor2Count<<endl;
        }

        while (strlen(line) != 0) {
            infile.getline(line, 100);
            if (strlen(line) == 0) {
                break;
            }


            int symbol120 = 0;
            int symbol121 = 0;

            vector<string> Num = findall(line);

            string str = line;
            int char_y = str.find('y');
            //cout<<char_y<<endl;
            if (char_y != string::npos) {
                symbol121 = 1;
            }
            else {
                symbol120 = 1;
            }


            string s1 = Num[0];
            Node.push_back(s1);

            string s2 = Num[1];

            string s3 = Num[2];


            if (find(Node.begin(), Node.end(), s2) == Node.end()) {
                Node.push_back(s2);
            }
            if (find(Node.begin(), Node.end(), s3) == Node.end()) {
                Node.push_back(s3);
            }

            if (symbol121 == 1) {
                TargetNode.push_back(s1);
                outputdict.emplace(atoi(Num[3].c_str()), atoi(s1.c_str()));
            }

            if (NodeOut.find(s1) == NodeOut.end()) {  //num[0] not in NodeOut
                vector<string> v1;
                NodeOut.emplace(s1, v1);
            }
            if (NodeOut.find(s2) == NodeOut.end()) {  //num[1] not in NodeOut
                vector<string> v2;
                NodeOut.emplace(s2, v2);
            }
            if (NodeOut.find(s3) == NodeOut.end()) {  //num[2] not in NodeOut
                vector<string> v3;
                NodeOut.emplace(s3, v3);
            }
            NodeOut[s2].push_back(s1);
            NodeOut[s3].push_back(s1);


            if (NodeFro.find(s1) == NodeFro.end()) {  //num[0] not in NodeFro

                vector<vector<string>> v1;
                NodeFro.emplace(s1, v1);
            }
            if (NodeFro.find(s2) == NodeFro.end()) {  //num[1] not in NodeFro

                vector<vector<string>> v2;
                NodeFro.emplace(s2, v2);
            }
            if (NodeFro.find(s3) == NodeFro.end()) {  //num[2] not in NodeFro
                vector<vector<string>> v3;
                NodeFro.emplace(s3, v3);
            }
            vector<string> vec = { s2,s3 };
            NodeFro[s1].push_back(vec);





            Value[atoi(s1.c_str())] = Value[atoi(s2.c_str())] ^ Value[atoi(s3.c_str())];


            HammingSet[atoi(s1.c_str())].assign(HammingSet[atoi(s2.c_str())].begin(), HammingSet[atoi(s2.c_str())].end());
            for (auto& i : HammingSet[atoi(s3.c_str())]) {
                if (find(HammingSet[atoi(s1.c_str())].begin(), HammingSet[atoi(s1.c_str())].end(), i) == HammingSet[atoi(s1.c_str())].end()) {
                    HammingSet[atoi(s1.c_str())].push_back(i);
                }
                else {
                    HammingSet[atoi(s1.c_str())].erase(remove(HammingSet[atoi(s1.c_str())].begin(), HammingSet[atoi(s1.c_str())].end(), i), HammingSet[atoi(s1.c_str())].end());
                }
            }
        }

        for (auto& node : Node) {
            if (NodeFro[node].size() == 0) {
                Base.push_back(node);
                NodeFro.erase(node);
            }
        }

    }

    void ComputeCanGetSet() {

        CanGetSet.clear();
        for (auto& ii : Node) {
            if (find(Base.begin(), Base.end(), ii) == Base.end()) {
                vector<string> v;
                CanGetSet[ii] = v;
                vector<string> newNodes;
                newNodes.assign(NodeOut[ii].begin(), NodeOut[ii].end());
                vector<string> newnewNodes;
                while (!newNodes.empty()) {
                    for (auto& jj : newNodes) {
                        CanGetSet[ii].push_back(jj);
                        for (auto& i : NodeOut[jj]) {
                            newnewNodes.push_back(i);
                        }
                    }
                    newNodes.assign(newnewNodes.begin(), newnewNodes.end());
                    newnewNodes.clear();
                }



            }
        }
    }

    void GenerateExtendGraph_with_XOR2() {
        for (auto& node : Node) {
            if (find(Base.begin(), Base.end(), node) == Base.end()) {

                vector<string> AvailableNode;

                for (auto& onenode : Node) {
                    if (find(CanGetSet[node].begin(), CanGetSet[node].end(), onenode) == CanGetSet[node].end() && onenode != node) {
                        AvailableNode.push_back(onenode);
                    }
                }

                while (true) {
                    int symbol = 0;
                    for (auto& onenode : AvailableNode) {
                        if (isSubset(HammingSet[atoi(onenode.c_str())], HammingSet[atoi(node.c_str())])) {
                            AvailableNode.erase(remove(AvailableNode.begin(), AvailableNode.end(), onenode), AvailableNode.end());
                            //ThisNode.erase(remove(ThisNode.begin(), ThisNode.end(), ii), ThisNode.end());
                            symbol = 1;
                            break;
                        }
                    }
                    if (symbol == 1) {
                        continue;
                    }
                    else {
                        break;
                    }
                }
                if (AvailableNode.size() < 2) {
                    continue;
                }
                for (int i = 0; i < AvailableNode.size() - 1; i++) {
                    for (int j = i + 1; j < AvailableNode.size(); j++) {
                        string node1 = AvailableNode[i];
                        string node2 = AvailableNode[j];
                        if (Value[atoi(node.c_str())] == (Value[atoi(node1.c_str())] ^ Value[atoi(node2.c_str())])) {
                            vector<string> vec1 = { node1,node2 };
                            vector<string> vec2 = { node2,node1 };
                            if (find(NodeFro[node].begin(), NodeFro[node].end(), vec1) == NodeFro[node].end() && find(NodeFro[node].begin(), NodeFro[node].end(), vec2) == NodeFro[node].end())
                            {
                                NodeFro[node].push_back(vec1);
                                //cout<<vec[1]<<" "<<vec[2] <<endl;
                            }
                        }
                    }
                }
            }
        }
    }

    void GenerateSingleGraph_with_XOR2() {

        map<string, int> NodeFromNumber;
        for (auto& node : NodeFro) {
            NodeFromNumber[node.first] = NodeFro[node.first].size();
        }

        int number_single_graph = 1;
        for (auto& node : NodeFro) {
            int num = NodeFro[node.first].size();
            number_single_graph *= num;
            if (number_single_graph >= pow(2, 13))
                break;
        }

        if (number_single_graph <= pow(2, 12)) {
            SingleGraph a;
            for (auto& node : Node) {
                if (find(Base.begin(), Base.end(), node) == Base.end()) {
                    vector<vector<string>> v;
                    a.NodeFro[node] = v;
                }
            }
            AllSingleGraph.push_back(a);
            for (auto& node : Node) {
                if (find(Base.begin(), Base.end(), node) == Base.end()) {
                    if (NodeFromNumber[node] == 1) {
                        for (auto& onegraph : AllSingleGraph) {
                            onegraph.NodeFro[node].push_back(NodeFro[node][0]);
                        }
                    }
                    else {
                        int times = NodeFromNumber[node];
                        vector<SingleGraph> AllSingleGraph_COPY;
                        AllSingleGraph_COPY.assign(AllSingleGraph.begin(), AllSingleGraph.end());
                        AllSingleGraph.clear();
                        for (int i = 0; i < times; i++) {
                            for (auto& onegraph : AllSingleGraph_COPY) {
                                SingleGraph newgraph = onegraph;
                                newgraph.NodeFro[node].push_back(NodeFro[node][i]);
                                AllSingleGraph.push_back(newgraph);
                            }
                        }
                    }
                }
            }
        }
        else {
            SingleGraph a;
            for (auto& node : Node) {
                if (find(Base.begin(), Base.end(), node) == Base.end()) {
                    vector<string> line;
                    line.assign(NodeFro[node][0].begin(), NodeFro[node][0].end());
                    a.NodeFro[node].clear();
                    a.NodeFro[node].push_back(line);
                }
            }
            AllSingleGraph.push_back(a);
            for (auto& node : Node) {
                if (find(Base.begin(), Base.end(), node) == Base.end() && NodeFro[node].size() > 1) {
                    int leng = NodeFro[node].size();
                    for (int i = 1; i < leng; i++) {
                        SingleGraph b;
                        b = a;
                        b.NodeFro[node].clear();
                        vector<string> line;
                        line.assign(NodeFro[node][i].begin(), NodeFro[node][i].end());
                        b.NodeFro[node].push_back(line);
                        AllSingleGraph.push_back(b);

                    }
                }
            }
        }

        for (auto& onegraph : AllSingleGraph) {

            for (auto& node : Node) {
                vector<string> vec;
                onegraph.NodeOut[node] = vec;
            }

            while (true) {
                int symbol = 0;

                for (auto& node : onegraph.NodeOut) {
                    onegraph.NodeOut[node.first].clear();
                }
                for (auto& oneedge : onegraph.NodeFro) {
                    string node0 = oneedge.first;
                    for (auto& node : onegraph.NodeFro[oneedge.first][0]) {
                        onegraph.NodeOut[node].push_back(node0);
                    }
                }
                for (auto& node : onegraph.NodeOut) {
                    if (find(Base.begin(), Base.end(), node.first) == Base.end() && find(TargetNode.begin(), TargetNode.end(), node.first) == TargetNode.end()) {
                        if (onegraph.NodeOut[node.first].empty()) {
                            onegraph.NodeFro.erase(onegraph.NodeFro.find(node.first));
                            onegraph.NodeOut.erase(onegraph.NodeOut.find(node.first));
                            symbol = 1;
                            break;
                        }
                    }
                }
                if (symbol == 1) {
                    continue;
                }
                else {
                    break;
                }
            }

            for (auto& node : onegraph.NodeOut) {
                onegraph.Node.push_back(node.first);
            }
        }

        int goodGraph = -1;
        while (true) {
            int symbol = 0;
            for (int i = goodGraph + 1; i < AllSingleGraph.size(); i++) {
                SingleGraph onegraph = AllSingleGraph[i];

                vector<string> result = TopologyOut(onegraph.Node, onegraph.NodeFro, onegraph.NodeOut, Base);
                onegraph.TopologyList = result;
                if (result.empty()) {
                    AllSingleGraph.erase(AllSingleGraph.begin() + i);
                    goodGraph = i - 1;
                    symbol = 1;
                    break;
                }


            }
            if (symbol == 1) {
                continue;
            }
            else {
                break;
            }
        }
        int test = 1;
    }

    void FindBestXOR2(int Xor, int depth) {
        MinCost = 100000;
        MinCost0 = 100000;
        MinNum.clear();
        MinDepth.clear();
        MinDepth0 = -1;

        for (int i = 0; i < AllSingleGraph.size(); i++) {

            Node = AllSingleGraph[i].Node;
            NodeFro = AllSingleGraph[i].NodeFro;
            NodeOut = AllSingleGraph[i].NodeOut;
            TopologyList = AllSingleGraph[i].TopologyList;

            int ThisCost = NodeFro.size();
            int ThisDepth = ExtendGraph::GetDepth(Base, NodeFro);
            if (ThisCost < Xor && ThisDepth == depth) {
                cout << "--------" << ThisCost << "/" << ThisDepth << "--------" << endl;

                for (auto& node : NodeFro) {
                    vector<string> nodes;
                    for (auto& onenode : NodeFro[node.first][0]) {
                        nodes.push_back(onenode);
                    }
                    cout << node.first << ":" << nodes.front() << "+" << nodes.back() << endl;
                }
                cout << "find" << endl;
            }
            //if (i == 0) {
            //    MinCost0 = ThisCost;
            //    MinDepth0 = ThisDepth;
            //}


            if (ThisCost < MinCost && ThisDepth == depth) {
                MinCost = ThisCost;
                MinNum.clear();
                MinNum.push_back(i);

                MinDepth.clear();
                MinDepth.push_back(ThisDepth);
            }
            else if (ThisCost == MinCost && ThisDepth == depth) {
                MinNum.push_back(i);
                MinDepth.push_back(ThisDepth);
            }
        }

        

        cout << "MinCost:" << MinCost << endl;

        for (int i : MinDepth) {
            std::cout << i << " ";
        }
        cout << endl;
        for (int i : MinNum) {
            std::cout << i << " ";
        }
        cout << endl;

    }

    static int GetDepth(vector<string> Base, map<string, vector<vector<string>>> NodeFro) {
        map<string, int> NodeDepth;
        for (auto& i : Base) {
            NodeDepth[i] = 0;
        }
        vector<string> NodeSet;
        for (auto& i : NodeFro) {
            NodeSet.push_back(i.first);
        }
        while (!NodeSet.empty()) {
            int Delete = -1;
            for (auto& node : NodeSet) {
                int nofound = -1;
                int depth = -1;
                for (auto& onenode : NodeFro[node][0]) {
                    if (NodeDepth.find(onenode) != NodeDepth.end()) {
                        depth = max(depth, NodeDepth[onenode] + 1);

                    }
                    else {
                        nofound = 1;
                    }
                }
                if (nofound == 1) {
                    continue;
                }
                else {
                    Delete = 1;
                    NodeDepth[node] = depth;
                    NodeSet.erase(remove(NodeSet.begin(), NodeSet.end(), node), NodeSet.end());
                    break;
                }
            }
            if (Delete == 1) {
                continue;
            }
        }
        int MaxDepth = -1;
        for (auto& node : NodeDepth) {
            MaxDepth = max(MaxDepth, NodeDepth[node.first]);
        }
        return MaxDepth;
    }

    static vector<string> findall(char* str1) {
        int cont = 0;
        string s1;
        string s2;
        string s3, s4;
        int len = strlen(str1);
        for (int i = 0; i < len; i++) {
            if (str1[i] == '[') {
                cont++;
                continue;
            }
            if (str1[i] == ']') {
                cont++;
                continue;
            }

            if (cont == 1) {
                char c1 = str1[i];
                s1 = s1 + c1;
            }
            if (cont == 3) {
                char c2 = str1[i];
                s2 = s2 + c2;
            }
            if (cont == 5) {
                char c3 = str1[i];
                s3 = s3 + c3;
            }
            if (cont == 7) {
                char c4 = str1[i];
                s4 = s4 + c4;
            }
        }
        vector<string> s;
        if (!s4.empty()) {
            s = { s1,s2,s3,s4 };
        }
        else {
            s = { s1,s2,s3 };
        }

        return s;
    }

    static vector<string> TopologyOut(vector<string> Node, map<string, vector<vector<string>>> NodeFro, map<string, vector<string>> NodeOut, vector<string> Base) {
        vector<string> ThisNode;
        ThisNode.assign(Node.begin(), Node.end());

        vector<string> NodeList;

        map<string, int> NodeFromNumber;
        map<string, int> NodeOutNumber;
        for (auto& node : NodeFro)
        {
            NodeFromNumber[node.first] = 0;
            vector<vector<string>> fromset = NodeFro[node.first];
            for (int i = 0; i < fromset.size(); i++) {
                NodeFromNumber[node.first] += fromset[i].size();
            }
        }
        for (auto& ii : NodeOut) {
            NodeOutNumber[ii.first] = NodeOut[ii.first].size();
        }

        while (true) {
            int symbol = 0;
            for (auto& node : ThisNode) {
                if (find(Base.begin(), Base.end(), node) != Base.end() || NodeFromNumber[node] == 0) {


                    string s = node;
                    ThisNode.erase(find(ThisNode.begin(), ThisNode.end(), s));

                    NodeList.push_back(s);

                    for (auto& outnode : NodeOut[s]) {
                        NodeFromNumber[outnode] -= 1;
                    }
                    symbol = 1;
                    break;
                }
            }
            if (symbol == 1) {
                continue;
            }
            else {
                break;
            }
        }

        if (!ThisNode.empty()) {
            vector<string> null;
            return null;
        }
        else {
            return NodeList;
        }
    }

    bool isSubset(vector<int> v1, vector<int> v2) {
        int i = 0, j = 0;
        int m = v1.size();
        int n = v2.size();
        if (m < n) {
            return false;
        }
        sort(v1.begin(), v1.end());
        sort(v2.begin(), v2.end());
        while (i < n && j < m) {
            if (v1[j] < v2[i]) {
                j++;
            }
            else if (v1[j] == v2[i]) {
                j++;
                i++;
            }
            else if (v1[j] > v2[i]) {
                return false;
            }
        }
        if (i < n) {
            return false;
        }
        else {
            return true;
        }
    }

};