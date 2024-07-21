#pragma once

/*
 * Composite pattern:
 * --------------------------- 
 * Component --> Command (interface)
 * Leaf node --> ExecNode
 * Composite nodes -> BackgroundNode, RedirectNode, SeparatorNode, Pipe 
 * 
 * 
*/
class Command {
public:
    //* this run() functin should never return.
    //* must always exit
    virtual void run() = 0;

    virtual void print() = 0;
};