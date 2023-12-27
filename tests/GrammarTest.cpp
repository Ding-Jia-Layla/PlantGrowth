#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "grammar.h"
#include "FractalSystem.h"
#include <tuple>
#include <string>
#include <iostream>
TEST(Grammar,ctor)
{
    Grammar g;
    EXPECT_EQ(g.getLevel(),0);
    EXPECT_EQ(g.getResult(), "");
    EXPECT_EQ(g.getGrammarName(), "");
    EXPECT_EQ(g.getResult(),"");

}
TEST(Grammar, setLevel)
{
    Grammar g;
    g.setLevel(2);
    auto lv =  g.getLevel();
    EXPECT_EQ(lv,2);
}
TEST(Grammar, setGrammarName)
{
    Grammar g;
    g.setGrammarName("l-system");
    auto name = g.getGrammarName();
    EXPECT_EQ(name,"l-system");

}
TEST(Grammar, setStart)
{
    Grammar g;
    g.setStart("F");
    auto st = g.getStart();
    EXPECT_EQ(st,"F");

}

TEST(Grammar,Left_Right)
{
    Grammar g;
    std::vector<std::string> wordsA = {"a","b","c"};
    std::vector<std::string> wordsB = {"d","e","f"};
    g.generations.push_back(std::make_tuple('X',wordsA));
    g.generations.push_back(std::make_tuple('F',wordsB));
    int left = g.fLeft('F');
    std::vector<std::string> right = g.gRight('F');
    EXPECT_EQ(left,1);
    EXPECT_EQ(right,wordsB);

}
TEST(GrammarTest, AddGeneration) {
    Grammar g;
    g.setGrammarName("l-system");
    char letterA = 'A';
    std::string wordA = "first rule";
    int id = g.fLeft(letterA);
    EXPECT_EQ(id,-1);
    g.addGeneration(letterA, wordA);
    EXPECT_EQ(g.generations.size(),1);
    char letterB = 'B';
    std::string wordB = "second rule";
    int index = g.fLeft(letterB);
    EXPECT_EQ(index,-1);
    g.setStart("A");
    g.addGeneration(letterB, wordB);
    const auto& generations = g.getGenerations();
    ASSERT_EQ(2, generations.size());
    const auto& gA = generations[0];
    EXPECT_EQ(letterA, std::get<0>(gA));
    EXPECT_EQ(wordA, std::get<1>(gA).front());
    EXPECT_EQ(1,std::get<1>(gA).size());
    const auto& gB = generations[1];
    EXPECT_EQ(letterB, std::get<0>(gB));
    EXPECT_EQ(wordB, std::get<1>(gB).front());
    EXPECT_EQ(1,std::get<1>(gB).size());

}
TEST(Grammar,iterate)
{
    Grammar g;
    g.setGrammarName("l-system");
    char letterA = 'A';
    std::string wordA = "B";
    g.addGeneration(letterA,wordA);
    char letterB = 'B';
    std::string wordB = "C";
    g.addGeneration(letterB, wordB);
    g.setStart("A");
    g.iterate(2);
    std::string r =g.getResult();
    std::cout << "Result: " << r << std::endl;
    EXPECT_EQ(r,"C");
}

TEST(FractalSystem,ctor)
{
    FractalSystem frac(ngl::Vec3(0.0f,1.0f,0.0f),ngl::Vec3(0.0f,1.0f,0.0f));
    auto cstates = frac.getCurrentStates().dir;
    EXPECT_EQ(cstates.m_x,0.0f);
    EXPECT_EQ(cstates.m_y,1.0f);
    EXPECT_EQ(cstates.m_z,0.0f);
    auto cpos = frac.getCurrentStates().pos;
    EXPECT_EQ(cpos.m_x,0.0f);
    EXPECT_EQ(cpos.m_y,1.0f);
    EXPECT_EQ(cpos.m_z,0.0f);
}
TEST(FractalSystem,addGeneration)
{
    FractalSystem frac(ngl::Vec3(0.0f,1.0f,0.0f),ngl::Vec3(0.0f,0.0f,0.0f));
    frac.addGeneration();
    frac.g.iterate(1);
    std::string r =frac.g.getResult();
    //TODO:WRITE THE ACTUAL RESULT OF IT
    EXPECT_EQ(r,"F[FX]F]F");

}
TEST(FractalSystem,fracMethod)
{
    FractalSystem frac(ngl::Vec3(0.0f,1.0f,0.0f),ngl::Vec3(0.0f,0.0f,0.0f));
    //default state has been setted.
    frac.addGeneration();
    frac.g.iterate(1);
    frac.generatePath();
    EXPECT_EQ(frac.m_tree.size(),4);
    std::unique_ptr<FractalSystem> n_frac;
    n_frac = std::make_unique<FractalSystem>(ngl::Vec3(0.0f,1.0f,0.0f),ngl::Vec3(0.0f,0.0f,0.0f));
//    FractalSystem frac(ngl::Vec3(0.0f,1.0f,0.0f),ngl::Vec3(0.0f,0.0f,0.0f));
    EXPECT_NE(n_frac, nullptr);
    //default state has been setted.
    n_frac->addGeneration();
    n_frac->g.iterate(1);
    std::string r =n_frac->g.getResult();
    EXPECT_EQ(r,"F[FX]F]F");
    n_frac->generatePath();
    EXPECT_EQ(frac.stateStack.size(),0);
    EXPECT_EQ(n_frac->m_tree.size(),4);

}
//TEST(GL,restartCount)
//{
//    std::unique_ptr<FractalSystem> n_frac;
//    n_frac = std::make_unique<FractalSystem>(ngl::Vec3(0.0f,1.0f,0.0f),ngl::Vec3(0.0f,0.0f,0.0f));
//    int restartCount = 0;
//    n_frac->addGeneration();
//    n_frac->g.iterate(1);
//    auto reStart = n_frac.index;
//    for (auto idx : index)
//    {
//        if (idx == restart)
//        {
//            restartCount++;
//        }
//    }
//
//    EXPECT_EQ(restartCount, /* expected restart count */);
//}





