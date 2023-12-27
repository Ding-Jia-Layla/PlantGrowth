#include "grammar.h"
#include <tuple>
#include <numeric>
#include <vector>
#include <string>

Grammar::~Grammar()
{
    generations.clear();
}

void Grammar::setGrammarName(const std::string& ref)
{
    Gname = ref;
}

void Grammar::setStart(const std::string& ref)
{
    start = ref;
}

void Grammar::setLevel(int num)
{
    level = num;
}

std::string Grammar::getGrammarName()
{
    return Gname;
}
std::string Grammar::getStart()
{
    return start;
}

int Grammar::getLevel()
{
    return level;
}

std::string Grammar::getResult()
{
    return result;
}

int Grammar::fLeft(char ch)
{
    if(generations.empty())
    {
        return -1;
    }
    for(int i =0; i<=generations.size()-1;i++)
    {
        if(std::get<0>(generations[i]) == ch)
        return i;
    }
    return -1;
}

std::vector<std::string> Grammar::gRight(const char &ch)
{
    int id  = fLeft(ch);
    //find the ch and generate the rule by using the ch.
    // get the right part of one of the rules
    if(id ==-1) return (std::vector<std::string>) ch;
    std::vector<std::string> right = std::get<1>(generations[id]);
    //int num = right.size();
    return right;
}
const std::vector<std::tuple<char, std::vector<std::string>>>& Grammar::getGenerations() const
{
    return generations;
}
void Grammar::addGeneration(const char &ch, const std::string &ref)
{
    int id = fLeft(ch);
    if(id==-1)
    {
        std::tuple<char, std::vector<std::string>> temp;
        std::get<0>(temp) = ch;
        std::get<1>(temp).push_back(ref);
        generations.push_back(temp);
        return;
    }
    std::get<1>(generations[id]).push_back(ref);

}

void Grammar::iterate(int num)
{
    setLevel(num);
    result = start;
    for(int o =0;o<num;o++)
    //iterate numbers
    {
        std::vector<std::string> tmp;
        for(int i =0;i<result.size();i++)
        {
            //get the rule of it
            auto right = gRight(result[i]);
            tmp.insert(tmp.end(),right.begin(),right.end());
        }
        std::string tmps = std::accumulate(tmp.begin(),tmp.end(),std::string());
        result = tmps;

    }
}






