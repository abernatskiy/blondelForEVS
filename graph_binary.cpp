// File: graph_binary.cpp
// -- graph handling source
//-----------------------------------------------------------------------------
// Community detection
// Based on the article "Fast unfolding of community hierarchies in large networks"
// Copyright (C) 2008 V. Blondel, J.-L. Guillaume, R. Lambiotte, E. Lefebvre
//
// This program must not be distributed without agreement of the above mentionned authors.
//-----------------------------------------------------------------------------
// Author   : E. Lefebvre, adapted by J.-L. Guillaume
// Email    : jean-loup.guillaume@lip6.fr
// Location : Paris, France
// Time	    : February 2008
//-----------------------------------------------------------------------------
// see readme.txt for more details

#include <sys/mman.h>
#include <fstream>
#include <sstream>
#include "graph_binary.h"
#include "math.h"

vector<string>& split(const string& s, char delim, vector<string>& elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
		elems.push_back(item);
	return elems;
}

vector<string> split(const string& s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}

inline int iabs(int i) {
	return i>0 ? i : -1*i;
}

Graph::Graph() {
  nb_nodes     = 0;
  nb_links     = 0;
  total_weight = 0;
}

Graph::Graph(const char *filename, const char *filename_w, int type) {
  ifstream finput;
  finput.open(filename,fstream::in | fstream::binary);

  // Read number of nodes on 4 bytes
  finput.read((char *)&nb_nodes, 4);
  assert(finput.rdstate() == ios::goodbit);

  // Read cumulative degree sequence: 8 bytes for each node
  // cum_degree[0]=degree(0); cum_degree[1]=degree(0)+degree(1), etc.
  degrees.resize(nb_nodes);
  finput.read((char *)&degrees[0], nb_nodes*8);

  // Read links: 4 bytes for each link (each link is counted twice)
  nb_links=degrees[nb_nodes-1];
  links.resize(nb_links);
  finput.read((char *)(&links[0]), (long)nb_links*8);

  // IF WEIGHTED : read weights: 4 bytes for each link (each link is counted twice)
  weights.resize(0);
  total_weight=0;
  if (type==WEIGHTED) {
    ifstream finput_w;
    finput_w.open(filename_w,fstream::in | fstream::binary);
    weights.resize(nb_links);
    finput_w.read((char *)&weights[0], (long)nb_links*4);
  }

  // Compute total weight
  for (unsigned int i=0 ; i<nb_nodes ; i++) {
    total_weight += (double)weighted_degree(i);
  }
}

/*Graph::Graph(int n, int m, double t, int *d, int *l, float *w) {
	nb_nodes     = n;
	nb_links     = m;
	total_weight = t;
	degrees      = d;
	links        = l;
	weights      = w;
}
*/

Graph::Graph(const string& annGenotype, const vector<int>& annTopology) {
	// Topology specificaition is zero OR two or more integers
	// If the topology is empty, the netowrk is assumed to be fully connected (e.g. random Boolean networks)
	// Otherwise, the integers are denoting the numbers of nodes in each layer starting from the top one.
	// Positive integers mean there are no self-recurrent connections in the layer;
	// negative ones mean that the layer is fully connected to its past self.
	// The first integer must be positive, since inputs cannot connect to themselves.
	assert(annTopology.size() != 1);
	assert(annTopology.size() > 0); // self-connected networks not supported yet

	auto weightStrings = split(annGenotype, ' ');
	vector<double> weightsG;
	weightsG.resize(weightStrings.size()-1);
	transform(weightStrings.begin()+1, weightStrings.end(), weightsG.begin(), [](string s){return stod(s);});

	unsigned int nbWeakLinks; // number of links including zero-weight ones

	// Compute number of nodes and links
	if(annTopology.size() == 0) {
		nb_nodes = weightStrings.size();
		nbWeakLinks = nb_nodes*nb_nodes;
	}
	else {
		assert(annTopology[0] > 0);
		nb_nodes = 0;
		nbWeakLinks = 0;
		for(auto layerSize : annTopology) {
			assert(layerSize != 0);
			nb_nodes += iabs(layerSize);
		}
		for(auto it=annTopology.begin()+1; it!=annTopology.end(); it++) {
			nbWeakLinks += iabs( (*(it-1)) * (*it) );
			if((*it) < 0)
				nbWeakLinks += (*it) * (*it);
		}
	}

	// Make sure we have enough values
	assert(nbWeakLinks == weightsG.size());

	// Iterate through entire topology to find:
	//  - nb_links
	//  - total_weight
	//  - noncumulative degrees
	int curLayerSize, prevLayerSize, inNode, outNode, floor=0, pos=0;
	double curWeight;
	total_weight = 0.0;
	int recurrentLinks = 0;
	nb_links = 0;
	degrees.resize(nb_nodes);
	fill(degrees.begin(), degrees.end(), 0);
	for(auto it=annTopology.begin()+1; it!=annTopology.end(); it++) {
		curLayerSize = iabs(*(it)); prevLayerSize = iabs(*(it-1));
		for(int i=0; i<prevLayerSize; i++) {
			for(int j=0; j<curLayerSize; j++) {
				curWeight = weightsG[pos];
				pos++;
				if(curWeight != 0.0) {
					nb_links++;
					total_weight += curWeight;
					outNode = floor + i;
					inNode = floor + prevLayerSize + j;
					degrees[outNode]++; degrees[inNode]++;
				}
			}
		}
		floor += prevLayerSize;
		if((*it) < 0) {
			for(int i=0; i<curLayerSize; i++) {
				for(int j=0; j<curLayerSize; j++) {
					curWeight = weightsG[pos];
					pos++;
					if(curWeight != 0.0) {
						recurrentLinks++;
						total_weight += curWeight;
						outNode = floor + i;
						inNode = floor + j;
						degrees[outNode]++;
//						if(inNode != outNode) degrees[inNode]++;
					}
				}
			}
		}
	}
	nb_links = 2*nb_links + recurrentLinks;
	total_weight *= 2.0;

	// Obtain cumulative degree
	for(auto it=degrees.begin()+1; it!=degrees.end(); it++)
		*it += *(it-1);

	// Iterate through entire topology once more to find links and weights
	links.resize(nb_links);
	weights.resize(nb_links);
	vector<unsigned int> linksFound;
	linksFound.resize(nb_nodes);
	fill(linksFound.begin(), linksFound.end(), 0);
	pos=0; floor=0;
	int outLinkPos, inLinkPos;

	for(auto it=annTopology.begin()+1; it!=annTopology.end(); it++) {
		curLayerSize = iabs(*(it)); prevLayerSize = iabs(*(it-1));
		for(int i=0; i<prevLayerSize; i++) {
			for(int j=0; j<curLayerSize; j++) {
				curWeight = weightsG[pos];
				pos++;
				if(curWeight != 0.0) {
					outNode = floor + i;
					inNode = floor + prevLayerSize + j;

					outLinkPos = first_link_idx(outNode)+linksFound[outNode];
					links[outLinkPos] = inNode;
					weights[outLinkPos] = curWeight;
					linksFound[outNode]++;

					inLinkPos = first_link_idx(inNode)+linksFound[inNode];
					links[inLinkPos] = outNode;
					weights[inLinkPos] = curWeight;
					linksFound[inNode]++;
				}
			}
		}
		floor += prevLayerSize;

		if((*it) < 0) {
			for(int i=0; i<curLayerSize; i++) {
				for(int j=0; j<curLayerSize; j++) {
					curWeight = weightsG[pos];
					pos++;
					if(curWeight != 0.0) {
						outNode = floor + i;
						inNode = floor + j;

						outLinkPos = first_link_idx(outNode)+linksFound[outNode];
						links[outLinkPos] = inNode;
						weights[outLinkPos] = curWeight;
						linksFound[outNode]++;

//						if(inNode != outNode) {
//							inLinkPos = first_link_idx(inNode)+linksFound[inNode];
//							links[inLinkPos] = outNode;
//							weights[inLinkPos] = curWeight;
//							linksFound[inNode]++;
//						}
					}
				}
			}
		}
	}

	/* This function may involve a bit of graph rewriting in future, since ANNs are
	   directional and the networks described in Blondel are not. When we have the situation
	   when there are two nodes in the hidden layer connected recursively in both ways, but
	   with nonequal weights in forward and backward directions, the reduction to the
	   nondirected network becomes ambiguous. This isn't dealt with for now.

	   Additionally, we may want to take absolute values of all weights.
	 */
}

void
Graph::display() {
  for (unsigned int node=0 ; node<nb_nodes ; node++) {
    pair<vector<unsigned int>::iterator, vector<float>::iterator > p = neighbors(node);
    cout << node << ":" ;
    for (unsigned int i=0 ; i<nb_neighbors(node) ; i++) {
      if (true) {
				if (weights.size()!=0)
					cout << " (" << *(p.first+i) << " " << *(p.second+i) << ")";
				else
					cout << " " << *(p.first+i);
			}
		}
		cout << endl;
	}
}

void
Graph::display_reverse() {
  for (unsigned int node=0 ; node<nb_nodes ; node++) {
    pair<vector<unsigned int>::iterator, vector<float>::iterator > p = neighbors(node);
    for (unsigned int i=0 ; i<nb_neighbors(node) ; i++) {
      if (node>*(p.first+i)) {
	if (weights.size()!=0)
	  cout << *(p.first+i) << " " << node << " " << *(p.second+i) << endl;
	else
	  cout << *(p.first+i) << " " << node << endl;
      }
    }
  }
}


bool
Graph::check_symmetry() {
  int error=0;
  for (unsigned int node=0 ; node<nb_nodes ; node++) {
    pair<vector<unsigned int>::iterator, vector<float>::iterator > p = neighbors(node);
    for (unsigned int i=0 ; i<nb_neighbors(node) ; i++) {
      unsigned int neigh = *(p.first+i);
      float weight = *(p.second+i);

      pair<vector<unsigned int>::iterator, vector<float>::iterator > p_neigh = neighbors(neigh);
      for (unsigned int j=0 ; j<nb_neighbors(neigh) ; j++) {
	unsigned int neigh_neigh = *(p_neigh.first+j);
	float neigh_weight = *(p_neigh.second+j);

	if (node==neigh_neigh && weight!=neigh_weight) {
	  cout << node << " " << neigh << " " << weight << " " << neigh_weight << endl;
	  if (error++==10)
	    exit(0);
	}
      }
    }
  }
  return (error==0);
}


void
Graph::display_binary(char *outfile) {
  ofstream foutput;
  foutput.open(outfile ,fstream::out | fstream::binary);

  foutput.write((char *)(&nb_nodes),4);
  foutput.write((char *)(&degrees[0]),4*nb_nodes);
  foutput.write((char *)(&links[0]),8*nb_links);
}
