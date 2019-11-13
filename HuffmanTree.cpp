#include <iostream>
#include "HuffmanTree.hpp"

 HuffmanTree::HuffmanTree()
{
} 

//The queue and trees should all be empty at the end of their function. 
HuffmanTree::~HuffmanTree()
{
}

//Takes a string and gives it the tree location
std::string HuffmanTree::compress(const std::string inputStr)
{
    std::string input = inputStr; //Makes a copy because of the 'const'
    std::string returned;
    
    //Fills the map with the characters and their frequency
    fillMap(input);
    //fills the priority queue with the frequency map
    fillPriQueue();

    //continues until there is only one thing in the priority queue
    while(queue.size() > 1)
        //Removes it from the queue and makes it into a tree
        removeIntoTree();
    
    //The root is the only ay to now access the tree
    _root = queue.min(); 
    queue.removeMin();
    //Priority queue should now be empty

    encode(_root,"");

    //Gives the 'compressed' version of the string
    for(int i = 0; i < input.size(); i++)
        returned = returned + encoded[inputStr[i]];

    encoded.clear();
    return returned;
}

void HuffmanTree::encode(HuffmanNode* temp, std::string str)
{
    //In case there is an empty/nullptr that is passed
    if(temp == nullptr)
        return;

    //The leaf is the one that houses all of the information
    if(temp->isLeaf())
    {
        //Puts the information back into encoded map that has the locations 
        encoded.insert(std::pair<char, std::string>(temp->getCharacter(), str)); 
        return;
    }
    //This actually makes the str based on the navigation of the tree
    if(temp->left != nullptr)
        encode(temp->left, str + "0");
    if(temp->right!= nullptr)
        encode(temp->right, str + "1"); 
}

//Fills a map with the characters of a string and the frequency that they appear
void HuffmanTree::fillMap(std::string input)
{
    for(int i = 0; i < input.size(); i++)
    {
        //If the character is not present in the map, add it into the map and set it to one.
        if(frequency.find(input[i]) == frequency.end())
            frequency.insert(std::pair<char,int>(input[i], 1));
        //the character has been seen before and is in the map. Add one to it
        else
            frequency.at(input[i])++;
    }
}

void HuffmanTree::fillPriQueue()
{
    for(std::map<char,int>::iterator it = frequency.begin(); it != frequency.end(); ++it)
    {
        //create a new node that houses the information of the frequency map
        HuffmanNode* node = new HuffmanNode(it->first, it->second);
        //insert the node into the priority queue
        queue.insert(node);
    }
}

//This function removes a total of one value from the queue until there is only one left
void HuffmanTree::removeIntoTree()
{
    //The queue is sorted and you take the smallest two and remove from the queue
    HuffmanNode* node1 = queue.min();
    queue.removeMin();
    HuffmanNode* node2 = queue.min();
    queue.removeMin();

    //A tree node that points to other nodes. This makes the tree. 
    HuffmanNode* tree = new HuffmanNode('\0', node1->getFrequency()+node2->getFrequency());
    tree->left = node1; 
    tree->right = node2;

    //This puts the made tree back into the priority queue so that it can be added to the next part of the tree later
    queue.insert(tree);
}

//This function organizes the characters in the nodes of the tree in post order
//Similar to encode()
std::string HuffmanTree::serializeTree() const
{
    //can't modify any outside variables
    std::string strsample = ""; 
    HuffmanNode* temp = _root;
    //Also utilizes recursion.C
    serialize(temp, strsample);
    return strsample;
}

void HuffmanTree::serialize(HuffmanNode* temp, std::string& input) const
{
    //In case there is an empty/nullptr that is passed
    if(temp == nullptr)
        return;

    //navigates the tree
    if(temp->left != nullptr)
        serialize(temp->left, input);
    if(temp->right!= nullptr)
        serialize(temp->right, input); 

    if(temp->isLeaf())
    {
        //updates the string by reference
        input = input + "L" + temp->getCharacter();
        return;
    }

    if(temp->isBranch())
    {
        //updates the string
        input = input + "B";
        return;
    }
}

//Utilizes the results of compress() and serializeTree(). Essentially translates
std::string HuffmanTree::decompress(const std::string inputCode, const std::string serializedTree)
{
    std::string returned = "";
    compressed = inputCode;
    serialized = serializedTree;

    //This code section creates a tree based off of the serialized string. Similar to the previous functions
    for(int i = 0; i < serialized.size(); i++)
    {
        if (serialized[i] == 'L')
        {
            HuffmanNode* temp = new HuffmanNode(serialized[i + 1], 0);
            decompressed.push(temp);
            i++; //Necessary because of the possibility of double L's
        }
        
        else if (serialized[i] == 'B')
        {
            HuffmanNode* node1 = decompressed.top();
            decompressed.pop();
            HuffmanNode* node2 = decompressed.top();
            decompressed.pop();
            //This constructor sets the left and right children accordingly
            HuffmanNode* parent = new HuffmanNode('\0', 0, nullptr, node2, node1);
            decompressed.push(parent);
        }
    }

    //Similar to the compress function. All that is left is the node/root. Grab it and then pop it.
    HuffmanNode* node = decompressed.top();
    _root = node;
    decompressed.pop();

    //Navigate down the tree with the compressed string using the 0/1 until you get to the leaf. 
    for(int i = 0; i < compressed.size(); i++)
    {
        if (compressed[i] == '0')
            node = node->left;
        if (compressed[i] == '1')
            node = node->right;
        if (node->isLeaf())
        {
            returned = returned + node->getCharacter(); //Take the character and add it to the string.
            node = _root;
        }
    }

    return returned;
}