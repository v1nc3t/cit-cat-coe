#pragma once

class BoardDrawBuilder {
public:
    virtual ~BoardDrawBuilder() = default;
    virtual void background() = 0;
    virtual void grid() = 0;
    virtual void marks() = 0;
    virtual void winLine() = 0;
};
