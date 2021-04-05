#pragma once

// interface
class Application
{
public:

    virtual ~Application();

    virtual void run() = 0;

protected:

    Application();

    virtual void update();
    virtual void draw();

    bool _running;

};