#ifndef FRACTALSYSTEM_H
#define FRACTALSYSTEM_H
#include <ngl/Vec3.h>
#include "grammar.h"
#include <vector>
#include <ngl/AbstractVAO.h>
#include <memory>

struct State
{
    //start point for the next segment
    ngl::Vec3 pos;
    ngl::Vec3 dir;
   bool operator==(const State&ref) const;
};

struct trunk
{

    ngl::Vec3 startPos;
    ngl::Vec3 endPos;
    std::vector<ngl::Vec3> middlePos;


};

class FractalSystem
{
public:
    FractalSystem() = default;
    FractalSystem(ngl::Vec3 _dir, ngl::Vec3 _pos);
    void generatePath();
    void addGeneration();
    void renderVAO();
    //std::unique_ptr<ngl::AbstractVAO> getVAO();
    std::vector<State> stateStack;
    Grammar g;
    State getCurrentStates();
    float distance = 1.0f;
    //std::vector<ngl::Vec3> m_points;
    std::vector<trunk> m_tree;
    std::vector<ngl::Vec3> m_plant;

private:
    State currentState;
    std::unique_ptr<ngl::AbstractVAO> m_vao;
    std::vector<ngl::Vec3> line;
    //points must be the local variable can't be global.
    //在方法结束时，m_points 将会被销毁
    std::vector<ngl::Vec3> m_points;



};
#endif
