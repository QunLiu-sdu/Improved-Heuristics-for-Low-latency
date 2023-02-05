#define _CRT_SECURE_NO_WARNINGS 1


#include "FrameV1Extend.h"


using namespace std;



int main(int argc, char *argv[])
{
    //string FILENAME_MATRIX("test_matrix.txt");


	if (OPTION <= 3)
	{
		int times = 1000;
		int thistimes = 0;
		while (thistimes < times)
		{

			cout << endl << endl << "times: " << thistimes << endl << endl;

			string MYSEQ("myseq.txt");
			read_myseq(NodeFro, isBase, isTarget, Value, Depth, HanmmingSet, MYSEQ);
			(NumTargets < NumInputs) ? SIZE = NumInputs : SIZE = NumTargets;


			get_reduced(NodeFro, isBase, isTarget, Value, Depth, HanmmingSet);

			thistimes++;
		}
	}
	else if (OPTION == 5)
	{
		string MYSEQ("myseq.txt");
		read_myseq(NodeFro, isBase, isTarget, Value, Depth, HanmmingSet, MYSEQ);
		(NumTargets < NumInputs) ? SIZE = NumInputs : SIZE = NumTargets;


		get_reduced(NodeFro, isBase, isTarget, Value, Depth, HanmmingSet);
	}

}

void ReadTargetMatrixLiu(uint64_t* NumInputs, uint64_t* NumTargets, map<int, nodefrom>& NodeFro, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, vector<int>& BaseNodes, vector<int>& TargetNodes, std::string FILENAME_MATRIX)
{
	uint64_t N_in = 0;
	uint64_t N_out = 0;



	ifstream matrixfile(FILENAME_MATRIX, ios::in);

	matrixfile >> N_out;
	matrixfile >> N_in;
	*NumInputs = N_in;
	*NumTargets = N_out;



	nodefrom BaseNodeFro;
	BaseNodeFro.from1 = -1;
	BaseNodeFro.from2 = -1;
	for (int i = 0; i < *NumInputs; i++)
	{
		NodeFro[i] = BaseNodeFro;
		Value[i] = (int)pow(2.0, i);
		Depth[i] = 0;
		vector<int> tmpHamming;
		tmpHamming.push_back(i);
		HanmmingSet[i] = tmpHamming;
		BaseNodes.push_back(i);
	}

	int TargetNumber = *NumInputs;
	for (int i = 0; i < *NumTargets; i++)
	{
		Value[TargetNumber + i] = 0;
		Depth[TargetNumber + i] = 0;
		vector<int> tmpHamming;
		HanmmingSet[TargetNumber + i] = tmpHamming;
		TargetNodes.push_back(TargetNumber + i);
	}

	int bit;
	for (int i = 0; i < *NumTargets; i++)
	{
		vector<int> thisHamming;
		uint64_t PowerOfTwo = 1;

		for (int j = 0; j < *NumInputs; j++)
		{
			matrixfile >> bit;
			if (bit)
			{
				HanmmingSet[TargetNumber + i].push_back(j);
				Value[TargetNumber + i] = Value[TargetNumber + i] + PowerOfTwo;
			}
			PowerOfTwo = PowerOfTwo << 1;
		}
	}
}

void ReadTargetMatrix(uint64_t* NumInputs, uint64_t* NumTargets, uint64_t* Target, uint64_t* Dist, uint64_t* InitDist, std::string FILENAME_MATRIX)
{
    uint64_t N_in = 0;
    uint64_t N_out = 0;

    ifstream matrixfile(FILENAME_MATRIX, ios::in);

    matrixfile >> N_out;
    matrixfile >> N_in;
    *NumInputs = N_in;
    *NumTargets = N_out;
    int bit;
    for (int i = 0; i < *NumTargets; i++)
    {
        uint64_t PowerOfTwo = 1;
        Target[i] = 0;
        Dist[i] = -1; //initial distance from Target[i] is Hamming weight - 1
        for (int j = 0; j < *NumInputs; j++)
        {
            matrixfile >> bit;
            if (bit)
            {
                Dist[i]++; //after using randomly permutation, change the Dist[]!!!!!!;
                Target[i] = Target[i] + PowerOfTwo;
            }
            PowerOfTwo = PowerOfTwo << 1;
        }
    }
    for (int k = 0; k < *NumInputs; k++) InitDist[k] = Dist[k];
}

void read_myseq(map<int, nodefrom>& NodeFro, map<int, int>& isBase, map<int, int>& isTarget, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, string MYSEQ)
{
	fstream f(MYSEQ);
	string line;


	NodeFro.clear();
	isBase.clear();
	isTarget.clear();
	Value.clear();
	Depth.clear();
	HanmmingSet.clear();

	for (int i = 0; i < NumInputs; i++)
	{
		nodefrom tmp;
		tmp.from1 = -1;
		tmp.from2 = -1;
		NodeFro[i] = tmp;
		isBase[i] = 1;
		isTarget[i] = -1;
		Value[i] = (int)pow(2.0, i);
		Depth[i] = 0;

		vector<int> tmpHamming;
		tmpHamming.push_back(i);
		HanmmingSet[i] = tmpHamming;
	}

	while (getline(f, line))
	{
		vector<int> tmp_value;
		int num = 0;
		for (int i = 0; i < line.length(); i++)
		{
			if ((line[i] >= '0') && (line[i] <= '9'))
			{
				num = num * 10 + (line[i] - '0');
			}
			else if (line[i] == ']')
			{
				tmp_value.push_back(num);
				num = 0;
			}
		}

		nodefrom tmp;
		int node0 = tmp_value[0];
		int node1 = tmp_value[1];
		int node2 = tmp_value[2];
		tmp.from1 = node1;
		tmp.from2 = node2;
		isBase[node0] = 0;
		NodeFro[node0] = tmp;
		Value[node0] = Value[node1] ^ Value[node2];
		(Depth[node1] > Depth[node2]) ? Depth[node0] = Depth[node1] + 1 : Depth[node0] = Depth[node2] + 1;
		vector<int> tmpHamming;
		for (int i = 0; i < HanmmingSet[node1].size(); i++)
		{
			vector<int>::iterator iter;
			iter = find(HanmmingSet[node2].begin(), HanmmingSet[node2].end(), HanmmingSet[node1][i]);
			if (iter == HanmmingSet[node2].end())
			{
				tmpHamming.push_back(HanmmingSet[node1][i]);
			}
		}
		for (int i = 0; i < HanmmingSet[node2].size(); i++)
		{
			vector<int>::iterator iter;
			iter = find(HanmmingSet[node1].begin(), HanmmingSet[node1].end(), HanmmingSet[node2][i]);
			if (iter == HanmmingSet[node1].end())
			{
				tmpHamming.push_back(HanmmingSet[node2][i]);
			}
		}
		HanmmingSet[node0] = tmpHamming;

		if (tmp_value.size() == 3)
		{
			isTarget[node0] = -1;
		}
		else if (tmp_value.size() == 4)
		{
			isTarget[node0] = tmp_value[3];
		}

	}
	cout << "1" << endl;
}

int get_reduced(map<int, nodefrom>& NodeFro, map<int, int>& isBase, map<int, int>& isTarget, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet)
{

	cout << "Original myseq.size() = " << NodeFro.size() - NumInputs << endl;

	vector<int> TargetNodes;
	vector<int> BaseNodes;
	vector<int> NonBaseNodes;
	
	map<int, int>::iterator BaseNodeIter = isBase.begin();
	for (; BaseNodeIter != isBase.end(); BaseNodeIter++)
	{
		if (BaseNodeIter->second == 1)
		{
			BaseNodes.push_back(BaseNodeIter->first);
		}
	}
	map<int, int>::iterator TargetNodeIter = isTarget.begin();
	for (; TargetNodeIter != isTarget.end(); TargetNodeIter++)
	{
		if (TargetNodeIter->second != -1)
		{
			TargetNodes.push_back(TargetNodeIter->first);
		}
	}


	GetTopology(NodeFro, isBase, NonBaseNodes);


	if (OPTION <= 4)
		BeginOptimize(NodeFro, isBase, isTarget, Value, Depth, HanmmingSet, TargetNodes, BaseNodes, NonBaseNodes);
	else if (OPTION == 5)
		BeginOptimizeOPTION5(NodeFro, isBase, isTarget, Value, Depth, HanmmingSet, TargetNodes, BaseNodes, NonBaseNodes);



	return 0;
}

int GetTopology(map<int, nodefrom>& NodeFro, map<int, int>& isBase, vector<int>& NonBaseNodes)
{
	map<int, int> NodeOutNumber;
	map<int, int> NodeFromNumber;
	vector<int> NodeInNodeFro;
	map<int, nodefrom>::iterator mapIter = NodeFro.begin();
	for (; mapIter != NodeFro.end(); mapIter++)
	{
		if (isBase[mapIter->first] != 1)
		{
			NodeInNodeFro.push_back(mapIter->first);
		}
	}
	while (true)
	{
		bool symbol = false;
		for (int i = 0; i < NodeInNodeFro.size(); i++)
		{
			int thisnode = NodeInNodeFro[i];
			int node1 = NodeFro[thisnode].from1;
			int node2 = NodeFro[thisnode].from2;
			bool usednode1 = false;
			bool usednode2 = false;
			if (isBase[node1] == 1)
			{
				usednode1 = true;
			}
			else
			{
				vector<int>::iterator iter;
				iter = find(NonBaseNodes.begin(), NonBaseNodes.end(), node1);
				if (iter == NonBaseNodes.end())
				{
					usednode1 = false;
				}
				else
				{
					usednode1 = true;
				}
			}
			if (isBase[node2] == 1)
			{
				usednode2 = true;
			}
			else
			{
				vector<int>::iterator iter;
				iter = find(NonBaseNodes.begin(), NonBaseNodes.end(), node2);
				if (iter == NonBaseNodes.end())
				{
					usednode2 = false;
				}
				else
				{
					usednode2 = true;
				}
			}

			if (usednode1 && usednode2)
			{
				NodeInNodeFro.erase(NodeInNodeFro.begin() + i);
				NonBaseNodes.push_back(thisnode);
				symbol = true;
				break;
			}
		}
		if (symbol)
		{
			continue;
		}
		else
		{
			break;
		}
	}


	return 0;
}

int BeginOptimize(map<int, nodefrom>& NodeFro, map<int, int>& isBase, map<int, int>& isTarget, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, vector<int>& TargetNodes, vector<int>& BaseNodes, vector<int>& NonBaseNodes)
{
	for (int preNumber = 1; preNumber < NonBaseNodes.size(); preNumber++)
	//for (int preNumber =  5; preNumber < NonBaseNodes.size(); preNumber)
	{
		vector<int> PredecessorSet = BaseNodes;
		for (int i = 0; i < preNumber; i++)
		{
			PredecessorSet.push_back(NonBaseNodes[i]);
		}
		for (int WorkingNumber = 1; WorkingNumber < NonBaseNodes.size() - preNumber + 1; WorkingNumber++)
		{
			int testnum = 0;
			for (int WorkingSite = 0; WorkingSite + WorkingNumber + preNumber < NonBaseNodes.size() + 1; WorkingSite++)
			{
				vector<int> WorkingSet;
				for (int j = preNumber + WorkingSite; j < preNumber + WorkingSite + WorkingNumber; j++)
				{
					WorkingSet.push_back(NonBaseNodes[j]);
				}


				map<int, nodefrom> partNodeFro;
				map<int, int> partValue(Value);
				map<int, vector<int>> partHanmmingSet(HanmmingSet);


				//if (preNumber == 0 && WorkingNumber == 2 && WorkingSite == 38)
				//{
				//	cout << "2";
				//}
				
				if (OPTION == 1)
				{

				}

				else if (OPTION == 2)
				{
					OptFromLiuOnce(PredecessorSet, WorkingSet, partNodeFro, partValue, partHanmmingSet);
				}
				
				else if (OPTION == 3)
				{
					OptFromLiuOnce34random(PredecessorSet, WorkingSet, partNodeFro, partValue, partHanmmingSet);
				}




				map<int, nodefrom> newNodeFro;
				vector<int> thisNonBasenode;
				int havecycle = 0;
				havecycle = fromParttoNewCircuit(partNodeFro, partValue, partHanmmingSet, TargetNodes, BaseNodes, NodeFro, newNodeFro, thisNonBasenode, NumInputs);
				if (havecycle == 1)
				{
					cout << "cycle!" << endl;
					continue;
				}

				//cout << newNodeFro.size() - NumInputs << endl;

				//if (preNumber == 0 && WorkingNumber == 6 && WorkingSite == 9)
				//{
				//	cout << "2";
				//}
				


				int oldcircuitDepth;
				int oldcircuitXor;
				int newcircuitDepth;
				int newcircuitXor;

				newcircuitDepth = fromCircuittoDepth(newNodeFro, thisNonBasenode);
				oldcircuitXor = NodeFro.size() - NumInputs;
				newcircuitXor = newNodeFro.size() - NumInputs;

				if (newcircuitXor < oldcircuitXor && newcircuitDepth == 3)
				{
					cout << "find";
				}

				//cout << "old" << oldcircuitXor << endl;
				//if (newcircuitXor < oldcircuitXor + 1 && newcircuitDepth == 3)
				cout << "new" << newcircuitXor << " " << newcircuitDepth << " " << " preNumber " << preNumber << " WorkingNumber " << WorkingNumber << " WorkingSite " << WorkingSite << "  ";

				//if (newcircuitXor == 95)
				//	cout << "needsee" << endl;



				ExtendGraph extendgraph(NumInputs);
				updateExtendGraph(extendgraph, newNodeFro, TargetNodes, BaseNodes, NumInputs, isTarget, thisNonBasenode);
				extendgraph.ComputeCanGetSet();
				extendgraph.GenerateExtendGraph_with_XOR2();
				extendgraph.GenerateSingleGraph_with_XOR2();
				extendgraph.FindBestXOR2(oldcircuitXor - adiitionalXORnumber, 3);
				cout << "size:" << extendgraph.AllSingleGraph.size() << endl;



			}
			

		}
	}

	return 0;
}

int BeginOptimizeOPTION5(map<int, nodefrom>& NodeFro, map<int, int>& isBase, map<int, int>& isTarget, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet, vector<int>& TargetNodes, vector<int>& BaseNodes, vector<int>& NonBaseNodes)
{
	for (int preNumber = 0; preNumber < 1; preNumber++)
	//for (int preNumber =  5; preNumber < NonBaseNodes.size(); preNumber)
	{
		vector<int> PredecessorSet = BaseNodes;
		for (int i = 0; i < preNumber; i++)
		{
			PredecessorSet.push_back(NonBaseNodes[i]);
		}
		for (int WorkingNumber = 1; WorkingNumber < 2; WorkingNumber++)
		{
			int testnum = 0;
			for (int WorkingSite = 0; WorkingSite < 1; WorkingSite++)
			{
				vector<int> WorkingSet;
				for (int j = preNumber + WorkingSite; j < preNumber + WorkingSite + WorkingNumber; j++)
				{
					WorkingSet.push_back(NonBaseNodes[j]);
				}


				map<int, nodefrom> partNodeFro;
				map<int, int> partValue(Value);
				map<int, vector<int>> partHanmmingSet(HanmmingSet);


				//if (preNumber == 0 && WorkingNumber == 2 && WorkingSite == 38)
				//{
				//	cout << "2";
				//}

				if (OPTION == 1)
				{

				}

				else if (OPTION == 2)
				{
					OptFromLiuOnce(PredecessorSet, WorkingSet, partNodeFro, partValue, partHanmmingSet);
				}

				else if (OPTION == 3)
				{
					OptFromLiuOnce34random(PredecessorSet, WorkingSet, partNodeFro, partValue, partHanmmingSet);
				}




				map<int, nodefrom> newNodeFro;
				vector<int> thisNonBasenode;
				int havecycle = 0;
				havecycle = fromParttoNewCircuit(partNodeFro, partValue, partHanmmingSet, TargetNodes, BaseNodes, NodeFro, newNodeFro, thisNonBasenode, NumInputs);
				if (havecycle == 1)
				{
					cout << "cycle!" << endl;
					continue;
				}

				//cout << newNodeFro.size() - NumInputs << endl;

				//if (preNumber == 0 && WorkingNumber == 6 && WorkingSite == 9)
				//{
				//	cout << "2";
				//}



				int oldcircuitDepth;
				int oldcircuitXor;
				int newcircuitDepth;
				int newcircuitXor;

				newcircuitDepth = fromCircuittoDepth(newNodeFro, thisNonBasenode);
				oldcircuitXor = NodeFro.size() - NumInputs;
				newcircuitXor = newNodeFro.size() - NumInputs;

				if (newcircuitXor < oldcircuitXor && newcircuitDepth == 3)
				{
					cout << "find";
				}

				//cout << "old" << oldcircuitXor << endl;
				//if (newcircuitXor < oldcircuitXor + 1 && newcircuitDepth == 3)
				cout << "new" << newcircuitXor << " " << newcircuitDepth << " " << " preNumber " << preNumber << " WorkingNumber " << WorkingNumber << " WorkingSite " << WorkingSite << "  ";

				//if (newcircuitXor == 95)
				//	cout << "needsee" << endl;



				ExtendGraph extendgraph(NumInputs);
				updateExtendGraph(extendgraph, newNodeFro, TargetNodes, BaseNodes, NumInputs, isTarget, thisNonBasenode);
				extendgraph.ComputeCanGetSet();
				extendgraph.GenerateExtendGraph_with_XOR2();
				extendgraph.GenerateSingleGraph_with_XOR2();
				extendgraph.FindBestXOR2(PreviousXORnumber, PreviousDEPTH);
				cout << "size:" << extendgraph.AllSingleGraph.size() << endl;



			}


		}
	}

	return 0;
}

int BeginOptimizeMatrix(map<int, nodefrom>& NodeFro, map<int, int>& Value, map<int, int>& Depth, map<int, vector<int>>& HanmmingSet,vector<int>& BaseNodes, vector<int>& TargetNodes)
{
	vector<int> PredecessorSet = BaseNodes;
	vector<int> WorkingSet = TargetNodes;

	map<int, nodefrom> partNodeFro;
	map<int, int> partValue(Value);
	map<int, vector<int>> partHanmmingSet(HanmmingSet);

	OptFromLiuOnce(PredecessorSet, WorkingSet, partNodeFro, partValue, partHanmmingSet);



	return partNodeFro.size();
}




int OptFromLiuOnce(vector<int>& PredecessorSet, vector<int>& WorkingSet, map<int, nodefrom>& partNodeFro, map<int, int>& partValue, map<int, vector<int>>& partHanmmingSet)
{


	map<int, int>::iterator valueIter = Value.begin();
	int nextNodeNum = -1;
	for (; valueIter != Value.end(); valueIter++)
	{
		if (valueIter->first > nextNodeNum)
		{
			nextNodeNum = valueIter->first;
		}
	}
	nextNodeNum++;

	map<int, int> thisdepth;
	int maxDepth = 0;
	int currentDepth = 0;


	//currentDepth = maxDepth;
	vector<int> newgeneratenodeset = WorkingSet;
	vector<int> tmp_PredecessorSet = PredecessorSet;
	while (newgeneratenodeset.size() > 0)
	{
		newgeneratenodeset.clear();
		while (WorkingSet.size() > 0)
		{
			//cout << "1";
			bool symbol = false;
			random_shuffle(WorkingSet.begin(), WorkingSet.end());
			random_shuffle(PredecessorSet.begin(), PredecessorSet.end());



			// rule2
			for (auto i = 0; i < WorkingSet.size(); i++)
			{
				for (auto j1 = 0; j1 < PredecessorSet.size() - 1; j1++)
				{
					for (auto j2 = j1 + 1; j2 < PredecessorSet.size(); j2++)
					{
						int node = WorkingSet[i];
						int node1 = PredecessorSet[j1];
						int node2 = PredecessorSet[j2];
						if (partValue[node] == (partValue[node1] ^ partValue[node2]))
						{
							symbol = true;

							nodefrom tmpfrom;
							tmpfrom.from1 = node1;
							tmpfrom.from2 = node2;


							partNodeFro[node] = tmpfrom;


							WorkingSet.erase(WorkingSet.begin() + i);
							break;
						}
					}
					if (symbol)
						break;
				}
				if (symbol)
					break;
			}
			if (symbol)
				continue;

			// rule3
			for (auto i = 0; i < WorkingSet.size(); i++)
			{
				for (auto j = 0; j < PredecessorSet.size(); j++)
				{
					int node = WorkingSet[i];
					int node1 = PredecessorSet[j];
					int node2;

					int node2Value = partValue[node] ^ partValue[node1];
					int node2Hammingweight = getHammingWeight(node2Value);

					if (partHanmmingSet[node1].size() == 1)
						continue;

					if ((fromNumtoDepth(node2Hammingweight) < fromNumtoDepth(partHanmmingSet[node].size())) && (fromNumtoDepth(partHanmmingSet[node1].size()) < fromNumtoDepth(partHanmmingSet[node].size())))
					{
						symbol = true;

						int oldnode = -1;
						//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
						//{
						//	if (partValue[onevalue] == node2Value)
						//	{
						//		oldnode = onevalue;
						//		node2 = onevalue;
						//		break;
						//	}
						//}
						for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
						{
							if (onevalue->second == node2Value)
							{
								oldnode = onevalue->first;
								node2 = onevalue->first;
								break;
							}
						}

						if (oldnode == -1)
							node2 = nextNodeNum++;

						nodefrom tmpfrom;
						tmpfrom.from1 = node1;
						tmpfrom.from2 = node2;


						partNodeFro[node] = tmpfrom;


						PredecessorSet.push_back(node2);
						newgeneratenodeset.push_back(node2);


						if (oldnode == -1)
							partValue[node2] = node2Value;


						if (oldnode == -1)
						{
							vector<int> tmpHamming;
							xorHammingSet(partHanmmingSet[node], partHanmmingSet[node1], tmpHamming);
							partHanmmingSet[node2] = tmpHamming;
						}


						WorkingSet.erase(WorkingSet.begin() + i);


						break;
					}
				}
				if (symbol)
					break;
			}
			if (symbol)
				continue;

			//rule4
			if (WorkingSet.size() > 1)
			{
				for (auto i1 = 0; i1 < WorkingSet.size() - 1; i1++)
				{
					for (auto i2 = i1 + 1; i2 < WorkingSet.size(); i2++)
					{
						int worknode1 = WorkingSet[i1];
						int worknode2 = WorkingSet[i2];

						int newnodemid;
						int newnode1;
						int newnode2;

						int nodemidvalue = partValue[worknode1] & partValue[worknode2];
						int node1value = nodemidvalue ^ partValue[worknode1];
						int node2value = nodemidvalue ^ partValue[worknode2];

						int nodemidHamming = getHammingWeight(nodemidvalue);
						int node1Hamming = getHammingWeight(node1value);
						int node2Hamming = getHammingWeight(node2value);

						if ((node1Hamming == 1) || (node1Hamming == 1) || (node2Hamming == 1))
							continue;



						if (fromNumtoDepth(nodemidHamming) < fromNumtoDepth(partHanmmingSet[worknode1].size()) && fromNumtoDepth(nodemidHamming) < fromNumtoDepth(partHanmmingSet[worknode2].size()) && fromNumtoDepth(node1Hamming) < fromNumtoDepth(partHanmmingSet[worknode1].size()) && fromNumtoDepth(node2Hamming) < fromNumtoDepth(partHanmmingSet[worknode2].size()))
						{
							symbol = true;

							int oldnodemid = -1;
							//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
							//{
							//	if (partValue[onevalue] == nodemidvalue)
							//	{
							//		oldnodemid = onevalue;
							//		newnodemid = onevalue;
							//		break;
							//	}
							//}
							for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
							{
								if (onevalue->second == nodemidvalue)
								{
									oldnodemid = onevalue->first;
									newnodemid = onevalue->first;
									break;
								}
							}
							if (oldnodemid == -1)
								newnodemid = nextNodeNum++;

							int oldnode1 = -1;
							//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
							//{
							//	if (partValue[onevalue] == node1value)
							//	{
							//		oldnode1 = onevalue;
							//		newnode1 = onevalue;
							//		break;
							//	}
							//}
							for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
							{
								if (onevalue->second == node1value)
								{
									oldnode1 = onevalue->first;
									newnode1 = onevalue->first;
									break;
								}
							}
							if (oldnode1 == -1)
								newnode1 = nextNodeNum++;

							int oldnode2 = -1;
							//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
							//{
							//	if (partValue[onevalue] == node2value)
							//	{
							//		oldnode2 = onevalue;
							//		newnode2 = onevalue;
							//		break;
							//	}
							//}
							for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
							{
								if (onevalue->second == node2value)
								{
									oldnode2 = onevalue->first;
									newnode2 = onevalue->first;
									break;
								}
							}
							if (oldnode2 == -1)
								newnode2 = nextNodeNum++;


							nodefrom tmpfrom;
							tmpfrom.from1 = newnodemid;
							tmpfrom.from2 = newnode1;
							partNodeFro[worknode1] = tmpfrom;
							tmpfrom.from2 = newnode2;
							partNodeFro[worknode2] = tmpfrom;


							PredecessorSet.push_back(newnodemid);
							PredecessorSet.push_back(newnode1);
							PredecessorSet.push_back(newnode2);
							newgeneratenodeset.push_back(newnodemid);
							newgeneratenodeset.push_back(newnode1);
							newgeneratenodeset.push_back(newnode2);


							if (oldnodemid == -1)
								partValue[newnodemid] = nodemidvalue;
							if (oldnode1 == -1)
								partValue[newnode1] = node1value;
							if (oldnode2 == -1)
								partValue[newnode2] = node2value;


							if (oldnodemid == -1)
							{
								vector<int> tmpHamming;
								andHammingSet(partHanmmingSet[worknode1], partHanmmingSet[worknode2], tmpHamming);
								partHanmmingSet[newnodemid] = tmpHamming;
							}
							if (oldnode1 == -1)
							{
								vector<int> tmpHamming;
								xorHammingSet(partHanmmingSet[worknode1], partHanmmingSet[newnodemid], tmpHamming);
								partHanmmingSet[newnode1] = tmpHamming;
							}
							if (oldnode2 == -1)
							{
								vector<int> tmpHamming;
								xorHammingSet(partHanmmingSet[worknode2], partHanmmingSet[newnodemid], tmpHamming);
								partHanmmingSet[newnode2] = tmpHamming;
							}


							WorkingSet.erase(WorkingSet.begin() + i2);
							WorkingSet.erase(WorkingSet.begin() + i1);


							break;

						}
					}
					if (symbol)
						break;
				}
				if (symbol)
					continue;
			}

			// rule5
			for (auto i = 0; i < WorkingSet.size(); i++)
			{
				int node = WorkingSet[i];
				int nodeDepth = fromNumtoDepth(partHanmmingSet[node].size());
				int node1Depth = nodeDepth - 1;
				int valibleHammingweight = fromDepthtoNum(node1Depth);
				vector<int> tmpHamming = partHanmmingSet[node];
				vector<int> node1Hamming;
				random_shuffle(tmpHamming.begin(), tmpHamming.end());
				for (auto num = 0; num < valibleHammingweight; num++)
				{
					node1Hamming.push_back(tmpHamming[num]);
				}
				int node1Value = 0;
				for (auto num = 0; num < node1Hamming.size(); num++)
				{
					int thisnode = node1Hamming[num];
					node1Value += partValue[thisnode];
				}
				int node2Value = partValue[node] ^ node1Value;

				symbol = true;

				int node1, node2;
				int oldnode1 = -1;
				//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
				//{
				//	if (partValue[onevalue] == node1Value)
				//	{
				//		node1 = onevalue;
				//		break;
				//	}
				//}
				for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
				{
					if (onevalue->second == node1Value)
					{
						node1 = onevalue->first;
						oldnode1 = onevalue->first;
						break;
					}
				}
				int oldnode2 = -1;
				//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
				//{
				//	if (partValue[onevalue] == node2Value)
				//	{
				//		node2 = onevalue;
				//		break;
				//	}
				//}
				for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
				{
					if (onevalue->second == node2Value)
					{
						node2 = onevalue->first;
						oldnode2 = onevalue->first;
						break;
					}
				}
				if (oldnode1 == -1)
					node1 = nextNodeNum++;
				if (oldnode2 == -1)
					node2 = nextNodeNum++;

				nodefrom tmpfrom;
				tmpfrom.from1 = node1;
				tmpfrom.from2 = node2;


				partNodeFro[node] = tmpfrom;


				PredecessorSet.push_back(node1);
				PredecessorSet.push_back(node2);
				newgeneratenodeset.push_back(node1);
				newgeneratenodeset.push_back(node2);


				if (oldnode1 == -1)
					partValue[node1] = node1Value;
				if (oldnode2 == -1)
					partValue[node2] = node2Value;


				if (oldnode1 == -1)
				{
					partHanmmingSet[node1] = node1Hamming;
				}
				if (oldnode2 == -1)
				{
					vector<int> tmpHamming;
					xorHammingSet(partHanmmingSet[node], partHanmmingSet[node1], tmpHamming);
					partHanmmingSet[node2] = tmpHamming;
				}


				WorkingSet.erase(WorkingSet.begin() + i);


				break;
			}
			if (symbol)
				continue;

		}

		if (newgeneratenodeset.size() > 0)
		{
			WorkingSet = newgeneratenodeset;
			PredecessorSet = tmp_PredecessorSet;


			bool thissymbol = true;
			while (thissymbol)
			{
				thissymbol = false;
				for (int onenode = 0; onenode < WorkingSet.size(); onenode++)
				{
					vector<int>::iterator tmp_PreIter = tmp_PredecessorSet.begin();
					tmp_PreIter = find(tmp_PredecessorSet.begin(), tmp_PredecessorSet.end(), WorkingSet[onenode]);
					if (tmp_PreIter != tmp_PredecessorSet.end())
					{
						thissymbol = true;
						WorkingSet.erase(WorkingSet.begin() + onenode);
						break;
					}
				}
			}

		}
	}


	return 0;

}

int OptFromLiuOnce34random(vector<int>& PredecessorSet, vector<int>& WorkingSet, map<int, nodefrom>& partNodeFro, map<int, int>& partValue, map<int, vector<int>>& partHanmmingSet)
{


	map<int, int>::iterator valueIter = Value.begin();
	int nextNodeNum = -1;
	for (; valueIter != Value.end(); valueIter++)
	{
		if (valueIter->first > nextNodeNum)
		{
			nextNodeNum = valueIter->first;
		}
	}
	nextNodeNum++;

	map<int, int> thisdepth;
	int maxDepth = 0;
	int currentDepth = 0;




	//currentDepth = maxDepth;
	vector<int> newgeneratenodeset = WorkingSet;
	vector<int> tmp_PredecessorSet = PredecessorSet;
	while (newgeneratenodeset.size() > 0)
	{
		newgeneratenodeset.clear();
		while (WorkingSet.size() > 0)
		{
			//cout << "1";
			bool symbol = false;
			random_shuffle(WorkingSet.begin(), WorkingSet.end());
			random_shuffle(PredecessorSet.begin(), PredecessorSet.end());



			// rule2
			for (auto i = 0; i < WorkingSet.size(); i++)
			{
				for (auto j1 = 0; j1 < PredecessorSet.size() - 1; j1++)
				{
					for (auto j2 = j1 + 1; j2 < PredecessorSet.size(); j2++)
					{
						int node = WorkingSet[i];
						int node1 = PredecessorSet[j1];
						int node2 = PredecessorSet[j2];
						if (partValue[node] == (partValue[node1] ^ partValue[node2]))
						{
							symbol = true;

							nodefrom tmpfrom;
							tmpfrom.from1 = node1;
							tmpfrom.from2 = node2;


							partNodeFro[node] = tmpfrom;


							WorkingSet.erase(WorkingSet.begin() + i);
							break;
						}
					}
					if (symbol)
						break;
				}
				if (symbol)
					break;
			}
			if (symbol)
				continue;

			vector<int> thisrandom;
			thisrandom.push_back(0);
			thisrandom.push_back(1);
			random_shuffle(thisrandom.begin(), thisrandom.end());

			if (thisrandom[0] == 0)
			{
				// rule3
				for (auto i = 0; i < WorkingSet.size(); i++)
				{
					for (auto j = 0; j < PredecessorSet.size(); j++)
					{
						int node = WorkingSet[i];
						int node1 = PredecessorSet[j];
						int node2;

						int node2Value = partValue[node] ^ partValue[node1];
						int node2Hammingweight = getHammingWeight(node2Value);

						if (partHanmmingSet[node1].size() == 1)
							continue;

						if ((fromNumtoDepth(node2Hammingweight) < fromNumtoDepth(partHanmmingSet[node].size())) && (fromNumtoDepth(partHanmmingSet[node1].size()) < fromNumtoDepth(partHanmmingSet[node].size())))
						{
							symbol = true;

							int oldnode = -1;
							//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
							//{
							//	if (partValue[onevalue] == node2Value)
							//	{
							//		oldnode = onevalue;
							//		node2 = onevalue;
							//		break;
							//	}
							//}
							for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
							{
								if (onevalue->second == node2Value)
								{
									oldnode = onevalue->first;
									node2 = onevalue->first;
									break;
								}
							}

							if (oldnode == -1)
								node2 = nextNodeNum++;

							nodefrom tmpfrom;
							tmpfrom.from1 = node1;
							tmpfrom.from2 = node2;


							partNodeFro[node] = tmpfrom;


							PredecessorSet.push_back(node2);
							newgeneratenodeset.push_back(node2);


							if (oldnode == -1)
								partValue[node2] = node2Value;


							if (oldnode == -1)
							{
								vector<int> tmpHamming;
								xorHammingSet(partHanmmingSet[node], partHanmmingSet[node1], tmpHamming);
								partHanmmingSet[node2] = tmpHamming;
							}


							WorkingSet.erase(WorkingSet.begin() + i);


							break;
						}
					}
					if (symbol)
						break;
				}
				if (symbol)
					continue;

							//rule4
			if (WorkingSet.size() > 1)
			{
				for (auto i1 = 0; i1 < WorkingSet.size() - 1; i1++)
				{
					for (auto i2 = i1 + 1; i2 < WorkingSet.size(); i2++)
					{
						int worknode1 = WorkingSet[i1];
						int worknode2 = WorkingSet[i2];

						int newnodemid;
						int newnode1;
						int newnode2;

						int nodemidvalue = partValue[worknode1] & partValue[worknode2];
						int node1value = nodemidvalue ^ partValue[worknode1];
						int node2value = nodemidvalue ^ partValue[worknode2];

						int nodemidHamming = getHammingWeight(nodemidvalue);
						int node1Hamming = getHammingWeight(node1value);
						int node2Hamming = getHammingWeight(node2value);

						if ((node1Hamming == 1) || (node1Hamming == 1) || (node2Hamming == 1))
							continue;



						if (fromNumtoDepth(nodemidHamming) < fromNumtoDepth(partHanmmingSet[worknode1].size()) && fromNumtoDepth(nodemidHamming) < fromNumtoDepth(partHanmmingSet[worknode2].size()) && fromNumtoDepth(node1Hamming) < fromNumtoDepth(partHanmmingSet[worknode1].size()) && fromNumtoDepth(node2Hamming) < fromNumtoDepth(partHanmmingSet[worknode2].size()))
						{
							symbol = true;

							int oldnodemid = -1;
							//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
							//{
							//	if (partValue[onevalue] == nodemidvalue)
							//	{
							//		oldnodemid = onevalue;
							//		newnodemid = onevalue;
							//		break;
							//	}
							//}
							for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
							{
								if (onevalue->second == nodemidvalue)
								{
									oldnodemid = onevalue->first;
									newnodemid = onevalue->first;
									break;
								}
							}
							if (oldnodemid == -1)
								newnodemid = nextNodeNum++;

							int oldnode1 = -1;
							//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
							//{
							//	if (partValue[onevalue] == node1value)
							//	{
							//		oldnode1 = onevalue;
							//		newnode1 = onevalue;
							//		break;
							//	}
							//}
							for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
							{
								if (onevalue->second == node1value)
								{
									oldnode1 = onevalue->first;
									newnode1 = onevalue->first;
									break;
								}
							}
							if (oldnode1 == -1)
								newnode1 = nextNodeNum++;

							int oldnode2 = -1;
							//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
							//{
							//	if (partValue[onevalue] == node2value)
							//	{
							//		oldnode2 = onevalue;
							//		newnode2 = onevalue;
							//		break;
							//	}
							//}
							for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
							{
								if (onevalue->second == node2value)
								{
									oldnode2 = onevalue->first;
									newnode2 = onevalue->first;
									break;
								}
							}
							if (oldnode2 == -1)
								newnode2 = nextNodeNum++;


							nodefrom tmpfrom;
							tmpfrom.from1 = newnodemid;
							tmpfrom.from2 = newnode1;
							partNodeFro[worknode1] = tmpfrom;
							tmpfrom.from2 = newnode2;
							partNodeFro[worknode2] = tmpfrom;


							PredecessorSet.push_back(newnodemid);
							PredecessorSet.push_back(newnode1);
							PredecessorSet.push_back(newnode2);
							newgeneratenodeset.push_back(newnodemid);
							newgeneratenodeset.push_back(newnode1);
							newgeneratenodeset.push_back(newnode2);


							if (oldnodemid == -1)
								partValue[newnodemid] = nodemidvalue;
							if (oldnode1 == -1)
								partValue[newnode1] = node1value;
							if (oldnode2 == -1)
								partValue[newnode2] = node2value;


							if (oldnodemid == -1)
							{
								vector<int> tmpHamming;
								andHammingSet(partHanmmingSet[worknode1], partHanmmingSet[worknode2], tmpHamming);
								partHanmmingSet[newnodemid] = tmpHamming;
							}
							if (oldnode1 == -1)
							{
								vector<int> tmpHamming;
								xorHammingSet(partHanmmingSet[worknode1], partHanmmingSet[newnodemid], tmpHamming);
								partHanmmingSet[newnode1] = tmpHamming;
							}
							if (oldnode2 == -1)
							{
								vector<int> tmpHamming;
								xorHammingSet(partHanmmingSet[worknode2], partHanmmingSet[newnodemid], tmpHamming);
								partHanmmingSet[newnode2] = tmpHamming;
							}


							WorkingSet.erase(WorkingSet.begin() + i2);
							WorkingSet.erase(WorkingSet.begin() + i1);


							break;

						}
					}
					if (symbol)
						break;
				}
				if (symbol)
					continue;
				}
			}

			if (thisrandom[0] == 1)
			{
				//rule4
				if (WorkingSet.size() > 1)
				{
					for (auto i1 = 0; i1 < WorkingSet.size() - 1; i1++)
					{
						for (auto i2 = i1 + 1; i2 < WorkingSet.size(); i2++)
						{
							int worknode1 = WorkingSet[i1];
							int worknode2 = WorkingSet[i2];

							int newnodemid;
							int newnode1;
							int newnode2;

							int nodemidvalue = partValue[worknode1] & partValue[worknode2];
							int node1value = nodemidvalue ^ partValue[worknode1];
							int node2value = nodemidvalue ^ partValue[worknode2];

							int nodemidHamming = getHammingWeight(nodemidvalue);
							int node1Hamming = getHammingWeight(node1value);
							int node2Hamming = getHammingWeight(node2value);

							if ((node1Hamming == 1) || (node1Hamming == 1) || (node2Hamming == 1))
								continue;



							if (fromNumtoDepth(nodemidHamming) < fromNumtoDepth(partHanmmingSet[worknode1].size()) && fromNumtoDepth(nodemidHamming) < fromNumtoDepth(partHanmmingSet[worknode2].size()) && fromNumtoDepth(node1Hamming) < fromNumtoDepth(partHanmmingSet[worknode1].size()) && fromNumtoDepth(node2Hamming) < fromNumtoDepth(partHanmmingSet[worknode2].size()))
							{
								symbol = true;

								int oldnodemid = -1;
								//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
								//{
								//	if (partValue[onevalue] == nodemidvalue)
								//	{
								//		oldnodemid = onevalue;
								//		newnodemid = onevalue;
								//		break;
								//	}
								//}
								for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
								{
									if (onevalue->second == nodemidvalue)
									{
										oldnodemid = onevalue->first;
										newnodemid = onevalue->first;
										break;
									}
								}
								if (oldnodemid == -1)
									newnodemid = nextNodeNum++;

								int oldnode1 = -1;
								//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
								//{
								//	if (partValue[onevalue] == node1value)
								//	{
								//		oldnode1 = onevalue;
								//		newnode1 = onevalue;
								//		break;
								//	}
								//}
								for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
								{
									if (onevalue->second == node1value)
									{
										oldnode1 = onevalue->first;
										newnode1 = onevalue->first;
										break;
									}
								}
								if (oldnode1 == -1)
									newnode1 = nextNodeNum++;

								int oldnode2 = -1;
								//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
								//{
								//	if (partValue[onevalue] == node2value)
								//	{
								//		oldnode2 = onevalue;
								//		newnode2 = onevalue;
								//		break;
								//	}
								//}
								for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
								{
									if (onevalue->second == node2value)
									{
										oldnode2 = onevalue->first;
										newnode2 = onevalue->first;
										break;
									}
								}
								if (oldnode2 == -1)
									newnode2 = nextNodeNum++;


								nodefrom tmpfrom;
								tmpfrom.from1 = newnodemid;
								tmpfrom.from2 = newnode1;
								partNodeFro[worknode1] = tmpfrom;
								tmpfrom.from2 = newnode2;
								partNodeFro[worknode2] = tmpfrom;


								PredecessorSet.push_back(newnodemid);
								PredecessorSet.push_back(newnode1);
								PredecessorSet.push_back(newnode2);
								newgeneratenodeset.push_back(newnodemid);
								newgeneratenodeset.push_back(newnode1);
								newgeneratenodeset.push_back(newnode2);


								if (oldnodemid == -1)
									partValue[newnodemid] = nodemidvalue;
								if (oldnode1 == -1)
									partValue[newnode1] = node1value;
								if (oldnode2 == -1)
									partValue[newnode2] = node2value;


								if (oldnodemid == -1)
								{
									vector<int> tmpHamming;
									andHammingSet(partHanmmingSet[worknode1], partHanmmingSet[worknode2], tmpHamming);
									partHanmmingSet[newnodemid] = tmpHamming;
								}
								if (oldnode1 == -1)
								{
									vector<int> tmpHamming;
									xorHammingSet(partHanmmingSet[worknode1], partHanmmingSet[newnodemid], tmpHamming);
									partHanmmingSet[newnode1] = tmpHamming;
								}
								if (oldnode2 == -1)
								{
									vector<int> tmpHamming;
									xorHammingSet(partHanmmingSet[worknode2], partHanmmingSet[newnodemid], tmpHamming);
									partHanmmingSet[newnode2] = tmpHamming;
								}


								WorkingSet.erase(WorkingSet.begin() + i2);
								WorkingSet.erase(WorkingSet.begin() + i1);


								break;

							}
						}
						if (symbol)
							break;
					}
					if (symbol)
						continue;
				}

				// rule3
				for (auto i = 0; i < WorkingSet.size(); i++)
				{
					for (auto j = 0; j < PredecessorSet.size(); j++)
					{
						int node = WorkingSet[i];
						int node1 = PredecessorSet[j];
						int node2;

						int node2Value = partValue[node] ^ partValue[node1];
						int node2Hammingweight = getHammingWeight(node2Value);

						if (partHanmmingSet[node1].size() == 1)
							continue;

						if ((fromNumtoDepth(node2Hammingweight) < fromNumtoDepth(partHanmmingSet[node].size())) && (fromNumtoDepth(partHanmmingSet[node1].size()) < fromNumtoDepth(partHanmmingSet[node].size())))
						{
							symbol = true;

							int oldnode = -1;
							//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
							//{
							//	if (partValue[onevalue] == node2Value)
							//	{
							//		oldnode = onevalue;
							//		node2 = onevalue;
							//		break;
							//	}
							//}
							for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
							{
								if (onevalue->second == node2Value)
								{
									oldnode = onevalue->first;
									node2 = onevalue->first;
									break;
								}
							}

							if (oldnode == -1)
								node2 = nextNodeNum++;

							nodefrom tmpfrom;
							tmpfrom.from1 = node1;
							tmpfrom.from2 = node2;


							partNodeFro[node] = tmpfrom;


							PredecessorSet.push_back(node2);
							newgeneratenodeset.push_back(node2);


							if (oldnode == -1)
								partValue[node2] = node2Value;


							if (oldnode == -1)
							{
								vector<int> tmpHamming;
								xorHammingSet(partHanmmingSet[node], partHanmmingSet[node1], tmpHamming);
								partHanmmingSet[node2] = tmpHamming;
							}


							WorkingSet.erase(WorkingSet.begin() + i);


							break;
						}
					}
					if (symbol)
						break;
				}
				if (symbol)
					continue;
			}
			

			

			// rule5
			for (auto i = 0; i < WorkingSet.size(); i++)
			{
				int node = WorkingSet[i];
				int nodeDepth = fromNumtoDepth(partHanmmingSet[node].size());
				int node1Depth = nodeDepth - 1;
				int valibleHammingweight = fromDepthtoNum(node1Depth);
				vector<int> tmpHamming = partHanmmingSet[node];
				vector<int> node1Hamming;
				random_shuffle(tmpHamming.begin(), tmpHamming.end());
				for (auto num = 0; num < valibleHammingweight; num++)
				{
					node1Hamming.push_back(tmpHamming[num]);
				}
				int node1Value = 0;
				for (auto num = 0; num < node1Hamming.size(); num++)
				{
					int thisnode = node1Hamming[num];
					node1Value += partValue[thisnode];
				}
				int node2Value = partValue[node] ^ node1Value;

				symbol = true;

				int node1, node2;
				int oldnode1 = -1;
				//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
				//{
				//	if (partValue[onevalue] == node1Value)
				//	{
				//		node1 = onevalue;
				//		break;
				//	}
				//}
				for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
				{
					if (onevalue->second == node1Value)
					{
						node1 = onevalue->first;
						oldnode1 = onevalue->first;
						break;
					}
				}
				int oldnode2 = -1;
				//for (auto onevalue = 0; onevalue < partValue.size(); onevalue++)
				//{
				//	if (partValue[onevalue] == node2Value)
				//	{
				//		node2 = onevalue;
				//		break;
				//	}
				//}
				for (map<int, int>::iterator onevalue = partValue.begin(); onevalue != partValue.end(); onevalue++)
				{
					if (onevalue->second == node2Value)
					{
						node2 = onevalue->first;
						oldnode2 = onevalue->first;
						break;
					}
				}
				if (oldnode1 == -1)
					node1 = nextNodeNum++;
				if (oldnode2 == -1)
					node2 = nextNodeNum++;

				nodefrom tmpfrom;
				tmpfrom.from1 = node1;
				tmpfrom.from2 = node2;


				partNodeFro[node] = tmpfrom;


				PredecessorSet.push_back(node1);
				PredecessorSet.push_back(node2);
				newgeneratenodeset.push_back(node1);
				newgeneratenodeset.push_back(node2);


				if (oldnode1 == -1)
					partValue[node1] = node1Value;
				if (oldnode2 == -1)
					partValue[node2] = node2Value;


				if (oldnode1 == -1)
				{
					partHanmmingSet[node1] = node1Hamming;
				}
				if (oldnode2 == -1)
				{
					vector<int> tmpHamming;
					xorHammingSet(partHanmmingSet[node], partHanmmingSet[node1], tmpHamming);
					partHanmmingSet[node2] = tmpHamming;
				}


				WorkingSet.erase(WorkingSet.begin() + i);


				break;
			}
			if (symbol)
				continue;

		}

		if (newgeneratenodeset.size() > 0)
		{
			WorkingSet = newgeneratenodeset;
			PredecessorSet = tmp_PredecessorSet;


			bool thissymbol = true;
			while (thissymbol)
			{
				thissymbol = false;
				for (int onenode = 0; onenode < WorkingSet.size(); onenode++)
				{
					vector<int>::iterator tmp_PreIter = tmp_PredecessorSet.begin();
					tmp_PreIter = find(tmp_PredecessorSet.begin(), tmp_PredecessorSet.end(), WorkingSet[onenode]);
					if (tmp_PreIter != tmp_PredecessorSet.end())
					{
						thissymbol = true;
						WorkingSet.erase(WorkingSet.begin() + onenode);
						break;
					}
				}
			}
			
		}
	}


	return 0;

}


int fromNumtoDepth(int num)
{
	if (num == 0)
		return -1;
	else if (num == 1)
		return 0;
	else if (num == 2)
		return 1;
	else if (3 <= num && num <= 4)
		return 2;
	else if (5 <= num && num <= 8)
		return 3;
	else if (9 <= num && num <= 16)
		return 4;
	else if (17 <= num && num <= 32)
		return 5;
	else
		return -2;
}

int fromDepthtoNum(int depth)
{
	if (depth == 0)
		return 1;
	else if (depth == 1)
		return 2;
	else if (depth == 2)
	{
		vector<int> tmpset;
		tmpset.push_back(3);
		tmpset.push_back(4);
		random_shuffle(tmpset.begin(), tmpset.end());
		return tmpset[0];
	}
	else if (depth == 3)
	{
		vector<int> tmpset;
		for (auto i = 5; i <= 8; i++)
			tmpset.push_back(i);
		random_shuffle(tmpset.begin(), tmpset.end());
		return tmpset[0];
	}
	else
		return -1;
}

int getHammingWeight(int n)
{
	int c = 0;
	for (c = 0; n; ++c)
	{
		n &= (n - 1);
	}
	return c;
}

int xorHammingSet(vector<int>& set1, vector<int>& set2, vector<int>& tmpset)
{
	for (int k = 0; k < set1.size(); k++)
	{
		int onebit = set1[k];
		vector<int>::iterator iter;
		iter = find(set2.begin(), set2.end(), onebit);
		if (iter == set2.end())
		{
			tmpset.push_back(onebit);
		}
	}
	for (int k = 0; k < set2.size(); k++)
	{
		int onebit = set2[k];
		vector<int>::iterator iter;
		iter = find(set1.begin(), set1.end(), onebit);
		if (iter == set1.end())
		{
			tmpset.push_back(onebit);
		}
	}

	return 0;
}

int andHammingSet(vector<int>& set1, vector<int>& set2, vector<int>& tmpset)
{
	for (int k = 0; k < set1.size(); k++)
	{
		int onebit = set1[k];
		vector<int>::iterator iter;
		iter = find(set2.begin(), set2.end(), onebit);
		if (iter != set2.end())
		{
			tmpset.push_back(onebit);
		}
	}

	return 0;
}

int fromParttoNewCircuit(map<int, nodefrom>& partNodeFro, map<int, int>& partValue, map<int, vector<int>>& partHanmmingSet, vector<int>& TargetNodes, vector<int>& BaseNodes, map<int, nodefrom>& NodeFro, map<int, nodefrom>& newNodeFro, vector<int>& thisNonBasenode, uint64_t NumInputs)
{
	newNodeFro = NodeFro;


	map<int, nodefrom>::iterator partIter = partNodeFro.begin();
	for (; partIter != partNodeFro.end(); partIter++)
	{
		newNodeFro[partIter->first] = partIter->second;
	}


	map<int, int> NodeOut;
	map<int, nodefrom>::iterator nodeIter = newNodeFro.begin();

	for (; nodeIter != newNodeFro.end(); nodeIter++)  
	{
		NodeOut[nodeIter->first] = 0;
	}


	nodeIter = newNodeFro.begin();
	for (; nodeIter != newNodeFro.end(); nodeIter++)
	{
		int node1 = nodeIter->second.from1;
		int node2 = nodeIter->second.from2;
		if (node1 != -1)
			NodeOut[node1]++;
		if (node2 != -1)
			NodeOut[node2]++;
	}


	bool symbol = true;		
	while (symbol)
	{
		symbol = false;
		map<int, int>::iterator nodeoutIter = NodeOut.begin();
		for (; nodeoutIter != NodeOut.end(); nodeoutIter++)
		{
			if (nodeoutIter->second == 0 && find(TargetNodes.begin(), TargetNodes.end(), nodeoutIter->first) == TargetNodes.end() && find(BaseNodes.begin(), BaseNodes.end(), nodeoutIter->first) == BaseNodes.end())
			{

				int node1 = newNodeFro[nodeoutIter->first].from1;
				int node2 = newNodeFro[nodeoutIter->first].from2;
				if (NodeOut[node1] > 0)
					NodeOut[node1]--;
				if (NodeOut[node2] > 0)
					NodeOut[node2]--;

				newNodeFro.erase(nodeoutIter->first);
				NodeOut.erase(nodeoutIter->first);
				symbol = true;
				break;
			}
		}
	}


	//vector<int> thisNonBasenode;
	map<int, int> thisisBase = isBase;
	map<int, nodefrom>::iterator newNodeIter = newNodeFro.begin();
	for (; newNodeIter != newNodeFro.end(); newNodeIter++)
	{
		int thisnode = newNodeIter->first;
		if (thisisBase.find(thisnode) == thisisBase.end())
		{
			thisisBase[thisnode] = 0;
		}
	}
	GetTopology(newNodeFro, thisisBase, thisNonBasenode);
	if (newNodeFro.size() > thisNonBasenode.size() + NumInputs)
	{
		return 1;
	}


	return 0;
}

int fromParttoNewCircuitMatrix(map<int, nodefrom>& partNodeFro, map<int, int>& partValue, map<int, vector<int>>& partHanmmingSet, vector<int>& TargetNodes, vector<int>& BaseNodes, map<int, nodefrom>& NodeFro, map<int, nodefrom>& newNodeFro, vector<int>& thisNonBasenode, uint64_t NumInputs)
{
	newNodeFro = NodeFro;


	map<int, nodefrom>::iterator partIter = partNodeFro.begin();
	for (; partIter != partNodeFro.end(); partIter++)
	{
		newNodeFro[partIter->first] = partIter->second;
	}


	map<int, int> NodeOut;
	map<int, nodefrom>::iterator nodeIter = newNodeFro.begin();

	for (; nodeIter != newNodeFro.end(); nodeIter++)
	{
		NodeOut[nodeIter->first] = 0;
	}


	nodeIter = newNodeFro.begin();
	for (; nodeIter != newNodeFro.end(); nodeIter++)
	{
		int node1 = nodeIter->second.from1;
		int node2 = nodeIter->second.from2;
		if (node1 != -1)
			NodeOut[node1]++;
		if (node2 != -1)
			NodeOut[node2]++;
	}


	bool symbol = true;
	while (symbol)
	{
		symbol = false;
		map<int, int>::iterator nodeoutIter = NodeOut.begin();
		for (; nodeoutIter != NodeOut.end(); nodeoutIter++)
		{
			if (nodeoutIter->second == 0 && find(TargetNodes.begin(), TargetNodes.end(), nodeoutIter->first) == TargetNodes.end() && find(BaseNodes.begin(), BaseNodes.end(), nodeoutIter->first) == BaseNodes.end())
			{

				int node1 = newNodeFro[nodeoutIter->first].from1;
				int node2 = newNodeFro[nodeoutIter->first].from2;
				if (NodeOut[node1] > 0)
					NodeOut[node1]--;
				if (NodeOut[node2] > 0)
					NodeOut[node2]--;

				newNodeFro.erase(nodeoutIter->first);
				NodeOut.erase(nodeoutIter->first);
				symbol = true;
				break;
			}
		}
	}


	//vector<int> thisNonBasenode;
	map<int, int> thisisBase = isBase;
	map<int, nodefrom>::iterator newNodeIter = newNodeFro.begin();
	for (; newNodeIter != newNodeFro.end(); newNodeIter++)
	{
		int thisnode = newNodeIter->first;
		if (thisisBase.find(thisnode) == thisisBase.end())
		{
			thisisBase[thisnode] = 0;
		}
	}
	GetTopology(newNodeFro, thisisBase, thisNonBasenode);
	if (newNodeFro.size() > thisNonBasenode.size() + NumInputs)
	{
		return 1;
	}


	return 0;
}


int fromCircuittoDepth(map<int, nodefrom>& Circuit, vector<int>& thisNonBasenode)
{
	map<int, int> thisDepth;

	for (int i = 0; i < NumInputs; i++)
	{
		thisDepth[i] = 0;
	}

	for (int i = 0; i < thisNonBasenode.size(); i++)
	{
		int node0 = thisNonBasenode[i];
		int node1 = Circuit[node0].from1;
		int node2 = Circuit[node0].from2;
		(thisDepth[node1] > thisDepth[node2]) ? thisDepth[node0] = thisDepth[node1] + 1 : thisDepth[node0] = thisDepth[node2] + 1;
	}


	int maxDepth = 0;
	map<int, int>::iterator depthIter = thisDepth.begin();
	for (; depthIter != thisDepth.end(); depthIter++)
	{
		if (depthIter->second > maxDepth)
		{
			maxDepth = depthIter->second;
		}
	}

	return maxDepth;
}

int updateExtendGraph(ExtendGraph& extendgraph, map<int, nodefrom>& newNodeFro, vector<int> TargetNodes, vector<int> BaseNodes, uint64_t NumInputs, map<int, int> isTarget, vector<int> thisNonBasenode)
{


	int num = 1;
	for (int i = 0; i < extendgraph.Size; i++) {
		extendgraph.Value[i] = num;
		vector<int> h;
		extendgraph.HammingSet.emplace(i, h);
		extendgraph.HammingSet[i].push_back(i);
		num *= 2;
	}

	extendgraph.Xor2Count = newNodeFro.size() - NumInputs;

	//map<int, nodefrom>::iterator newNodeFroIter = newNodeFro.begin();
	//for (int i = 0; i < NumInputs; i++)
	//	newNodeFroIter++;
	for (int chooseone = 0; chooseone < thisNonBasenode.size(); chooseone++)
	//for (; newNodeFroIter != newNodeFro.end(); newNodeFroIter++)
	{

		string Num[10];
		Num[0] = to_string(thisNonBasenode[chooseone]);
		Num[1] = to_string(newNodeFro[thisNonBasenode[chooseone]].from1);
		Num[2] = to_string(newNodeFro[thisNonBasenode[chooseone]].from2);

		string s1 = Num[0];
		string s2 = Num[1];
		string s3 = Num[2];


		if ((find(extendgraph.Node.begin(), extendgraph.Node.end(), s1) == extendgraph.Node.end()))
		{
			extendgraph.Node.push_back(s1);
		}

		if ((find(extendgraph.Node.begin(), extendgraph.Node.end(), s2) == extendgraph.Node.end()))
		{
			extendgraph.Node.push_back(s2);
		}
		if ((find(extendgraph.Node.begin(), extendgraph.Node.end(), s3) == extendgraph.Node.end()))
		{
			extendgraph.Node.push_back(s3);
		}



		if (extendgraph.NodeOut.find(s1) == extendgraph.NodeOut.end()) {  //num[0] not in NodeOut
			vector<string> v1;
			extendgraph.NodeOut.emplace(s1, v1);
		}
		if (extendgraph.NodeOut.find(s2) == extendgraph.NodeOut.end()) {  //num[1] not in NodeOut
			vector<string> v2;
			extendgraph.NodeOut.emplace(s2, v2);
		}
		if (extendgraph.NodeOut.find(s3) == extendgraph.NodeOut.end()) {  //num[2] not in NodeOut
			vector<string> v3;
			extendgraph.NodeOut.emplace(s3, v3);
		}

		extendgraph.NodeOut[s2].push_back(s1);
		extendgraph.NodeOut[s3].push_back(s1);
		


		if (extendgraph.NodeFro.find(s1) == extendgraph.NodeFro.end()) {  //num[0] not in NodeFro

			vector<vector<string>> v1;
			extendgraph.NodeFro.emplace(s1, v1);
		}
		if (extendgraph.NodeFro.find(s2) == extendgraph.NodeFro.end()) {  //num[1] not in NodeFro

			vector<vector<string>> v2;
			extendgraph.NodeFro.emplace(s2, v2);
		}
		if (extendgraph.NodeFro.find(s3) == extendgraph.NodeFro.end()) {  //num[2] not in NodeFro
			vector<vector<string>> v3;
			extendgraph.NodeFro.emplace(s3, v3);
		}
		vector<string> vec = { s2,s3 };
		extendgraph.NodeFro[s1].push_back(vec);


		extendgraph.Value[atoi(s1.c_str())] = extendgraph.Value[atoi(s2.c_str())] ^ extendgraph.Value[atoi(s3.c_str())];


		extendgraph.HammingSet[atoi(s1.c_str())].assign(extendgraph.HammingSet[atoi(s2.c_str())].begin(), extendgraph.HammingSet[atoi(s2.c_str())].end());
		for (auto& i : extendgraph.HammingSet[atoi(s3.c_str())]) {
			if (find(extendgraph.HammingSet[atoi(s1.c_str())].begin(), extendgraph.HammingSet[atoi(s1.c_str())].end(), i) == extendgraph.HammingSet[atoi(s1.c_str())].end()) {
				extendgraph.HammingSet[atoi(s1.c_str())].push_back(i);
			}
			else {
				extendgraph.HammingSet[atoi(s1.c_str())].erase(remove(extendgraph.HammingSet[atoi(s1.c_str())].begin(), extendgraph.HammingSet[atoi(s1.c_str())].end(), i), extendgraph.HammingSet[atoi(s1.c_str())].end());
			}
		}

	}


	for (int i = 0; i < TargetNodes.size(); i++)
	{
		extendgraph.TargetNode.push_back(to_string(TargetNodes[i]));
	}

	map<int, int>::iterator maptargetIter = isTarget.begin();
	for (; maptargetIter != isTarget.end(); maptargetIter++)
	{
		if (maptargetIter->second != -1)
		{
			extendgraph.outputdict[maptargetIter->second] = maptargetIter->first;
		}
	}



	for (auto& node : extendgraph.Node) {
		if (extendgraph.NodeFro[node].size() == 0) {
			extendgraph.Base.push_back(node);
			extendgraph.NodeFro.erase(node);
		}
	}


	return 0;
}

