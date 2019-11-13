#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include "HeapQueue.hpp"
#include "HuffmanBase.hpp"


class HuffmanTree : public HuffmanTreeBase
{
    public:
        std::string compress(const std::string inputStr);
        std::string serializeTree() const;
        void serialize(HuffmanNode*, std::string& input) const;
        std::string decompress(const std::string inputCode, const std::string serializedTree);
        void fillMap(std::string input);
        void fillPriQueue();
        void removeIntoTree();
        void encode(HuffmanNode*, std::string str);
        HuffmanTree();
        ~HuffmanTree();

    private:
        std::map<char, int> frequency;
        std::string input;
        HuffmanNode* _root;
        std::string str, compressed, serialized = "";
        std::map<char, std::string> encoded;
        HeapQueue<HuffmanNode*, HuffmanNode::Compare> queue;
        std::stack<HuffmanNode*> decompressed;
};