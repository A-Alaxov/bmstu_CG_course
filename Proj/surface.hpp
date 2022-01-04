#ifndef SURFACE_H
#define SURFACE_H

#include "dot.hpp"

class surface
{
public:
    surface();
    surface(const long point1, const long point2, const long point3);
    ~surface() = default;

    long get_first();
    void set_first(long index);

    long get_second();
    void set_second(long index);

    long get_third();
    void set_third(long index);

private:
    long point1;
    long point2;
    long point3;
};

class Facet
{
public:
    Facet() {}
    Facet(std::vector<size_t> usedVertices_, size_t col, std::vector<bool> existingRibs_ = { true, true, true }) : usedVertices(usedVertices_), color(col), existingRibs(existingRibs_) {}

    const std::vector<size_t> getUsedVertices();
    void setUsedVertices(std::vector<size_t> usedVertices_);

    double getIntens();
    void setIntens(double inten);

    bool checkGuro() { return needsGuro; }
    void setGuro() { needsGuro = true; }

    size_t getColor() { return color; }
    void setColor(size_t col) { color = col; }

    std::vector<bool> getExistingRibs() { return existingRibs; }

private:
    size_t color;
    double intens;
    bool needsGuro = 0;
    std::vector<size_t> usedVertices;
    std::vector<bool> existingRibs;
};

#endif // SURFACE_H
