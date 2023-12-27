#ifndef GRAMMAR_H_
#define GRAMMAR_H_
#include <string>
#include <tuple>
#include <vector>
class Grammar
{
    //using generation to generate the result of the Gname(initial value is start) by level times.
    //L-system Grammar
public:
    Grammar()=default;
    ~Grammar();
    void setGrammarName(const std::string& ref);
    void setStart(const std::string& ref);
   // std::vector<std::string> &operator +=(const std::vector<std::string> &rhs);
    void setLevel(int num);
    std::string getGrammarName();
    [[nodiscard]]int getLevel();
    std::string getResult();
    std::string getStart();

    int fLeft(char ch);
    std::vector<std::string> gRight(const char& ch);
    void addGeneration(const char& ch, const std::string& ref);
    void iterate(int num);
    std::vector<std::tuple<char, std::vector<std::string>>> generations;
    [[nodiscard]] const std::vector<std::tuple<char, std::vector<std::string>>>& getGenerations() const;


private:
    std::string Gname;
    std::string start;
    std::string result;
    int level = 0;
    //tuple for pairs
    int id;
};


#endif