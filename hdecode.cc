#include <iostream>
#include <fstream>
#include <cstdlib>
#include "priqueue.h"
#include "list.h"
#include <cmath>
using namespace std;

/*
"""
*******************************************************************************
FILE: hencode.cc

AUTHOR: Justin Zhou

ASSIGMENT: Project #7

DATE: December 10, 2014

DESCRIPTION:
Create a program that decodes the Huffman-encoded text with a Huffman tree and 
basically unzips the file.
*******************************************************************************
"""
*/

// struct node
struct h_node{
  int ascii_val;
  int count;
  h_node * left;
  h_node * right;
  h_node (int a, int c): ascii_val(a), count(c), left(NULL), right(NULL){}
  h_node (int a): ascii_val(a), left(NULL), right(NULL){}
  h_node (h_node * left, h_node * right): left(left), right(right){}
};

// change ascii characters to binary
string char2bin(int c)
{
  if(c==0) 
    return "0";
  if(c==1) 
    return "1";
  if(c % 2) 
    return char2bin(c/2) + "1";
  else; return char2bin(c/2) + "0";
}

// reads and reconstructs tree
h_node * readtree(ifstream & in)
{
  char ch = in.get();
  char lch;
  if (ch == 'L'){
    lch = in.get();
    return new h_node(lch);
  }
  else{
    return new h_node(readtree(in), readtree(in));
  }
}

// changes binary into decoded characters
void bin2char(ifstream & in, ofstream & out, string & binary, h_node * tree, 
	      int & count, int size, h_node * treetop)
{
  int c;
  // base case - if correct number of characters have been decoded, then finish
  if (count == size){
    return;
  }
  
  // if binary runs out of characters, then get another ascii to get more binary
  if (binary.length() == 0 and count < size){
    c = in.get();
    binary = char2bin(c);
    while (binary.length() < 8){
      binary = "0" + binary;
    }
    bin2char(in, out, binary, tree, count, size, treetop);
    return;
  }

  // reached a leaf
  else if (tree->right == NULL and tree -> left == NULL){
    out << (unsigned char)tree -> ascii_val;
    count += 1;
    tree = treetop;
    bin2char(in, out, binary, tree, count, size, treetop);
  }
  
  // reached an internal node
  else{
    // go left at internal node
    if (binary.substr(0,1) == "0"){
      binary = binary.substr(1);
      bin2char(in, out, binary, tree->left, count, size, treetop);
    }
    // go right at internal node
    else{
      binary = binary.substr(1);
      bin2char(in, out, binary, tree->right, count, size, treetop);
    }
  }
}

void hdecode(ifstream & in, ofstream & out)
{
  // step 1 - reconstruct tree
  int size; 
  // get the length of the file
  in >> size;
  //gets the tree and creates a pointer to the top of the tree
  h_node * tree = readtree(in);
  h_node * treetop = tree;
  // step 2 - turn ascii into binary
  string binary;
  string new_binary;
  int c;
  int count = 0;
  c = in.get();
  // gets the binary to start
  binary = char2bin(c);
  while (binary.length() < 8){
    binary = "0" + binary;
  }
  // turn binary into characters
  bin2char(in, out, binary, tree, count, size, treetop);
}

// main function that gets the file
int main(int argc, char ** argv)
{
  ifstream in(argv[1]);
  string in_file_name = argv[1];
  int file_name_length = in_file_name.length()-4;
  ofstream out((in_file_name.substr(0, file_name_length).c_str()));
  hdecode(in, out);
  remove(argv[1]);
  return 0;
}