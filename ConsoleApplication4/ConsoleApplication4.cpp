#include "stdafx.h"
#include <iostream>
#include "Consumer.h"
#include "Producer.h"
#include <time.h>
#include <fstream>

using namespace std;

void SimpleTopology()
{
	vector<Packet *> allDataObjects;
	vector <Node *> nodes;

	Consumer * consumer = new Consumer();
	consumer->nodeId = 1;
	Producer * producer = new Producer("test/com/");
	producer->nodeId = 2;
	Node * router = new Node();
	router->nodeId = 3;
	Node * router2 = new Node();
	router2->nodeId = 4;
	nodes.push_back(consumer);
	nodes.push_back(producer);
	nodes.push_back(router);
	nodes.push_back(router2);
	int option = 0;

	consumer->links.push_back(router);
	router->links.push_back(consumer);

	router->links.push_back(router2);
	router2->links.push_back(router);

	router2->links.push_back(producer);
	producer->links.push_back(router2);

	consumer->datas = producer->datas = router->datas = &allDataObjects;

	while (option != 3)
	{
		int option = 2;
		cout << "1. View a node" << endl;
		cout << "2. Continue" << endl;
		cout << "3. Quit" << endl;
		cout << "Select an option: ";
		cin >> option;

		while (option == 1)
		{
			int nodeid;
			cout << "Enter node id: ";
			cin >> nodeid;

			for (int i = 0; i < nodes.size(); i++)
			{
				if (nodes[i]->nodeId == nodeid)
				{
					nodes[i]->View();
				}
			}

			cout << "Enter new option (1/3):";
			cin >> option;
		}

		producer->Act();
		consumer->Act();
		router->Act();
		router2->Act();
	}

	system("pause");
}

void DisplayTopology(vector<Node*> nodeList)
{
	cout << "Topology" << endl;
	for (int i = 0; i < nodeList.size(); i++)
	{
		Node * n = nodeList[i];

		cout << "Node ID: " << n->nodeId << endl;
		cout << "Node X: " << n->xPos << endl;
		cout << "Node Y: " << n->yPos << endl;
		cout << "Links" << endl;

		for (int f = 0; f < n->links.size(); f++)
		{
			cout << n->nodeId << "->" << n->links[f]->nodeId << endl;
		}
	}
}

string getRandomString()
{
	string x;

	for (int i = 0; i < 5; i++)
	{
		x += (char)(rand() % 26 + 65);
	}
	
	return x;
}

int main()
{
	srand(time(NULL));

	ofstream out("Trial_" + getRandomString() + ".txt");
	streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

	vector<Packet *> allDataObjects;
	vector <Node *> nodes;
	int numNodes = 100;
	int minDistanceForLink = 40;
	int option = 0;


	/* Random initialization */
	for (int i = 0; i < numNodes; i++)
	{
		int type = rand() % 3;
		Node * n;
		if (type == 0)
		{
			n = new Consumer();
			cout << "Node " << i << " is a consumer" << endl;
			n->nodeType = 0;
			((Consumer*)n)->timeToSatisfaction = 0;
		}
		else if (type == 1)
		{
			n = new Producer("test/com");
			cout << "Node " << i << " is a producer" << endl;
			n->nodeType = 1;
 		}
		else
		{
			n = new Node();
			cout << "Node " << i << " is a router" << endl;
			n->nodeType = 2;
		}

		n->nodeId = i;
		n->xPos = rand() % 300;
		n->yPos = rand() % 300;

		nodes.push_back(n);
		n->datas = &allDataObjects;
	}

	/* Links set up */
	for (int i = 0; i < numNodes; i++)
	{
		Node * n = nodes[i];

		for (int f = 0; f < numNodes; f++)
		{
			Node * o = nodes[f];

			if (n->nodeId != o->nodeId && abs(n->xPos - o->xPos) < minDistanceForLink && abs(n->yPos - o->yPos) < minDistanceForLink)
			{
				n->links.push_back(o);
			}
		}
	}

	DisplayTopology(nodes);

	for (int i = 0; i < 10; i++)
	{
		cout << "Iteration: " << i << endl;

		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i]->View();
			nodes[i]->Act();
		}

		cout << "End of Iteration " << i << endl;
	}

	/*while(option != 3)
	{
		int option = 2;
		cout << "1. View a node" << endl;
		cout << "2. Continue" << endl;
		cout << "3. Quit" << endl;
		cout << "Select an option: ";
		cin >> option;

		while (option == 1)
		{
			int nodeid;
			cout << "Enter node id: ";
			cin >> nodeid;

			for (int i = 0; i < nodes.size(); i++)
			{
				if (nodes[i]->nodeId == nodeid)
				{
					nodes[i]->View();
				}
			}

			cout << "Enter new option (1/3):";
			cin >> option;
		}

		for (int i = 0; i < nodes.size(); i++)
			nodes[i]->Act();
	}*/

	cout.rdbuf(coutbuf); //reset to standard output again
	std::cout << "Done.";  //output to the standard input

	return 0;
}