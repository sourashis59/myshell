#pragma once

class Command {
public:
    //* this run() functin should never return.
    //* must always exit
    virtual void run() = 0;
};