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

DATE: December 1, 2014

DESCRIPTION:
Create a program that encodes a the text to a Huffman tree and basically 
compresses the file. 
*******************************************************************************
"""
*/

// structure of my h_node, used to store ascii values and their corresponding
// priority in the priority queue. 
struct h_node{
  int ascii_val;
  int count;
  h_node * left;
  h_node * right;
  h_node (int a, int c): ascii_val(a), count(c), left(NULL), right(NULL){}
  h_node (h_node * left, h_node * right): count(left->count + right -> count),
				      left(left), right(right){}
};

// global variables - arrays that hold number of counts and then the binary 
// codes of the ascii values
int counts[256];
string codes[256];

// create binary code by prepending a '0' or a '1' in the binary codes array,
// in order to create path in tree to the letters in each tree
void prepend_all(h_node * tree, char c)
{
  // base case - if left branch is null, it's a leaf, so return
  if(tree->left == NULL)
    codes[tree->ascii_val] = c + codes[tree->ascii_val];
  // otherwise, go left, or go right, or both and continue recursively
  else{
    prepend_all(tree->left, c);
    prepend_all(tree->right, c);
  }
}

// print the tree in the output file through recursion
void write_tree(h_node * tree, ofstream & out)
{
  // if the left is null, then it's a leaf, so record L and then the character
  if (tree->left == NULL)
    out << 'L' << (char)tree->ascii_val;
  // otherwise, it's an internal node, so output I and then continue recursively
  else{
    out << "I";
    write_tree(tree->left, out);
    write_tree(tree->right, out);
  }
}

// create an ascii value through the binary codes
char create_ascii(string binary){
  char ascii_value = 0;
  for(int i = 0; i < 8; i += 1){
    if(binary.substr(i, 1) == "1")
      ascii_value += pow(2,(7-i));
  }
  char ascii_char = ascii_value;
  return ascii_char;
}

// allows creation of priority queue
int priority_fn(h_node* const & tree){
  return tree->count;
}

// the encoding function that takes an input file and an output file
void hencode(ifstream & in, ofstream & out, ifstream & checkin)
{
  int c = in.get();
  int size = 0;
  // step 1 - read through .txt file to find counts of all characters in file
  while (c != -1){
    counts[c] ++;
    size = size + 1;
    c = in.get();
  }
  //output size
  out << size;
  // step 2 - build Huffman tree by taking 1st two and then re-entering new 
  // entry into priority queue
  priqueue <h_node*>Q(priority_fn);
  for (size_t i = 0; i < 256; i += 1)
    if (counts[i] != 0)
      Q.add(new h_node(i, counts[i]));
  // combine the h_nodes in the priority queue until only 1 remains
  while(Q.size() != 1){
    h_node * left = Q.front();
    Q.remove_front();
    h_node * right = Q.front();
    Q.remove_front();
    Q.add(new h_node(left, right));
    prepend_all(left, '0');
    prepend_all(right, '1');
  }

  // print tree in the output file 
  write_tree(Q.front(), out);

  // step 3 - read through text again to generate binary code
  string binary = "";
  string new_binary = "";
  //ifstream in2(argv[1]);
  // need to go back to the top of the file. 
  while((c = checkin.get()) != -1){
    binary = binary + codes[c];
    if(binary.length() >= 8){
      new_binary = binary.substr(0,8);
      binary = binary.substr(8);
      out << create_ascii(new_binary);
    }
  }
  while(binary.length() < 8){
    binary += "0";
  }
  out << create_ascii(binary);
}

// main function that checks if the input file exists and executes function
int main (int argc, char ** argv)
{
  ifstream in(argv[1]);
  ifstream checkin(argv[1]);
  // creates new .huf file with same filename as input
  string in_file_name = argv[1];
  ofstream out((in_file_name + ".huf").c_str());
  // if invalid input file, then output following error message
  if (!in){
    cout << "invalid input file; please enter existing input file." << endl;
    return 0;
  }
  // calls void function to generate encoded text
  hencode(in, out, checkin);
  // closes the input file
  remove(argv[1]);
  in.close();
  return 0;
}
