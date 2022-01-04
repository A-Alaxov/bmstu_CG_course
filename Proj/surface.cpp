#include "surface.hpp"

surface::surface()
{

}

surface::surface(const long point1, const long point2, const long point3)
    :point1(point1), point2(point2), point3(point3)
{
}

long surface::get_first()
{
    return point1;
}

long surface::get_second()
{
    return point2;
}

long surface::get_third()
{
    return point3;
}

const std::vector<size_t> Facet::getUsedVertices() { return usedVertices; }

void Facet::setUsedVertices(std::vector<size_t> usedVertices_) { usedVertices = usedVertices_; }

double Facet::getIntens() { return intens; }

void Facet::setIntens(double inten) { intens = inten; }
