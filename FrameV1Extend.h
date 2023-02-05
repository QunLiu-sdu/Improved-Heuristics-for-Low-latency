#pragma once

#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>  
#include <fstream>  
#include <string>  
#include <vector>  
#include <map>  
#include<cmath> 
#include <algorithm>
#include "extendgraph.h"


using namespace std;

#define OPTION 3


// 2: liu_local
// 3: liuRandom_local

// 5: from python run extend

int adiitionalXORnumber = 0;
int PreviousXORnumber = 48;  // for OPTION 5
int PreviousDEPTH = 3;  // for OPTION 5

//const int MaxBaseSize = 1000;
int SIZE;  
uint64_t NumInputs = 32;  
uint64_t NumTargets = 32;  


const int MaxBaseSize = 1000;

typedef struct
{
	int from1;  
	int from2;  
}nodefrom;  

map<int, nodefrom> NodeFro;  
map<int, int> isBase;  
map<int, int> isTarget;  
map<int, int> Value;  
map<int, int> Depth;  
map<int, vector<int>> HanmmingSet;  

void ReadTargetMatrixLiu(uint64_t* NumInputs, uint64_t* NumTargets, map<int, nodefrom>& NodeFro, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, vector<int>& BaseNodes, vector<int>& TargetNodes, std::string FILENAME_MATRIX);
int fromNumtoDepth(int num);  
int fromDepthtoNum(int depth);  
int getHammingWeight(int n);  
int xorHammingSet(vector<int>& set1, vector<int>& set2, vector<int>& tmpset);  
int andHammingSet(vector<int>& set1, vector<int>& set2, vector<int>& tmpset);  
void ReadTargetMatrix(uint64_t* NumInputs, uint64_t* NumTargets, uint64_t* Target, uint64_t* Dist, uint64_t* InitDist, std::string FILENAME_MATRIX);
void read_myseq(map<int, nodefrom>& NodeFro, map<int, int>& isBase, map<int, int>& isTarget, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, string MYSEQ);
int get_reduced(map<int, nodefrom>& NodeFro, map<int, int>& isBase, map<int, int>& isTarget, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet);
int GetTopology(map<int, nodefrom>& NodeFro, map<int, int>& isBase, vector<int>& NonBaseNodes);  
int BeginOptimize(map<int, nodefrom>& NodeFro, map<int, int>& isBase, map<int, int>& isTarget, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, vector<int>& TargetNodes, vector<int>& BaseNodes, vector<int>& NonBaseNodes);
int BeginOptimizeOPTION5(map<int, nodefrom>& NodeFro, map<int, int>& isBase, map<int, int>& isTarget, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, vector<int>& TargetNodes, vector<int>& BaseNodes, vector<int>& NonBaseNodes);
int BeginOptimizeMatrix(map<int, nodefrom>& NodeFro, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, vector<int>& BaseNodes, vector<int>& TargetNodes);
int OptFromLiuOnce(vector<int>& PredecessorSet, vector<int>& WorkingSet, map<int, nodefrom>& partNodeFro, map<int, int>& partValue, map<int, vector<int>>& partHanmmingSet);
int OptFromLiuOnce34random(vector<int>& PredecessorSet, vector<int>& WorkingSet, map<int, nodefrom>& partNodeFro, map<int, int>& partValue, map<int, vector<int>>& partHanmmingSet);
int fromParttoNewCircuit(map<int, nodefrom>& partNodeFro, map<int, int>& partValue, map<int, vector<int>>& partHanmmingSet, vector<int>& TargetNodes, vector<int>& BaseNodes, map<int, nodefrom>& NodeFro, map<int, nodefrom>& newNodeFro, vector<int>& thisNonBasenode, uint64_t NumInputs);
int fromParttoNewCircuitMatrix(map<int, nodefrom>& partNodeFro, map<int, int>& partValue, map<int, vector<int>>& partHanmmingSet, vector<int>& TargetNodes, vector<int>& BaseNodes, map<int, nodefrom>& NodeFro, map<int, nodefrom>& newNodeFro, vector<int>& thisNonBasenode, uint64_t NumInputs);
int fromCircuittoDepth(map<int, nodefrom>& Circuit, vector<int>& thisNonBasenode);


int updateExtendGraph(ExtendGraph& extendgraph, map<int, nodefrom>& newNodeFro, vector<int> TargetNodes, vector<int> BaseNodes, uint64_t NumInputs, map<int, int> isTarget, vector<int> thisNonBasenode);
