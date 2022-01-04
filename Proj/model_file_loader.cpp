#include "model_file_loader.hpp"
#include "errors.hpp"

model_file_loader::model_file_loader()
{
    file = std::shared_ptr<std::ifstream>(new std::ifstream);
    //builder = std::shared_ptr<model_builder>(new model_builder);
}

model_file_loader::~model_file_loader()
{
    close();
}

void model_file_loader::open(std::string &name)
{
    if (is_open())
        close();

    file->open(name);
    if (!file)
    {
        std::string message = "Error while open file";
        throw file_error(message);
    }
}

void model_file_loader::close()
{
    try
    {
        file->close();
    } catch (std::ifstream::failure &error)
    {
        error.what();
    }
}

bool model_file_loader::is_open()
{
    return file->is_open();
}

std::shared_ptr<object> model_file_loader::load()
{
}

Dot model_file_loader::read_center()
{
    if (!is_open())
    {
        std::string message = "Error in reading poits from file";
        throw file_error(message);
    }

    double x, y, z;
    *file >> x >> y >> z;
    center.setX(x);
    center.setY(y);
    center.setZ(z);
    Dot point(x * 50 + 100, y * 50 + 300, 10000 + z * 50);

    return point;
}

Dot model_file_loader::read_collision_center()
{
    if (!is_open())
    {
        std::string message = "Error in reading poits from file";
        throw file_error(message);
    }

    double x, y, z;
    *file >> x >> y >> z;
    Dot point(x * 50 + 100, y * 50 + 300, 10000 + z * 50);

    return point;
}

std::vector<Dot> model_file_loader::read_points()
{
    if (!is_open())
    {
        std::string message = "Error in reading poits from file";
        throw file_error(message);
    }

    std::vector<Dot> points;
    long points_count;
    *file >> points_count;

    for (long i = 0; i < points_count; i++)
    {
        double x, y, z;
        *file >> x >> y >> z;
        points.push_back(Dot((x + center.getX()) * 50 + 100,
                               (y + center.getY()) * 50 + 300,
                               (z + center.getZ()) * 50 + 10000));
    }

    return points;
}

std::vector<surface> model_file_loader::read_edges()
{
    if (!is_open())
    {
        std::string message = "Error in reading edges from file";
        throw file_error(message);
    }

    std::vector<surface> edges;
    long edges_count;
    *file >> edges_count;

    for (long i = 0; i < edges_count; i++)
    {
        long point1, point2, point3;
        *file >> point1 >> point2 >> point3;
        edges.push_back(surface(point1, point2, point3));
    }

    return edges;
}
