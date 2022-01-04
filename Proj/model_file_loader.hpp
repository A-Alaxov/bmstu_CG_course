#ifndef SOURCE_LOADER_HPP
#define SOURCE_LOADER_HPP

#include "dot.hpp"
#include "surface.hpp"
#include "base_loader.hpp"

#include <vector>
#include <string>
#include <fstream>

class model_file_loader : public base_model_loader
{
public:
    model_file_loader();
    explicit model_file_loader(std::shared_ptr<std::ifstream> &file) : file(file) {}
    ~model_file_loader();

    void open(std::string &name) override;
    void close() override;
    bool is_open() override;
    std::shared_ptr<object> load() override;

    Dot read_center();
    Dot read_collision_center();
    std::vector<Dot> read_points();
    std::vector<surface> read_edges();

private:
    Dot center;
    std::shared_ptr<std::ifstream> file;
};

#endif // SOURCE_LOADER_HPP
