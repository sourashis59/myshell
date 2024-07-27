#pragma once
#include "../config.h"
#include "../util/SystemCallWrapper.cpp"
#include "../util/Logger.h"
#include<iostream>

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
    virtual ~Command() {
        Logger::get_instance()->log("Destructor called for Command");
    }

    //* this run() functin should never return.
    //* must always exit
    virtual void run() = 0;

    virtual void print() = 0;
};