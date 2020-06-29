#include "application/application.h"

#include <iostream>

int main()
{
    my::Canvas& canvas = std::cout;
    my::Application app;
    const my::Document::Ptr& doc = app.create_document();

    const my::IShape::Ptr& circle_ptr = doc->add_circle(my::Point{ 100, 150 }, 35);
    circle_ptr->color(my::Color{ 155, 200, 255, 0 });
    const my::IShape::Ptr& segment_ptr = doc->add_segment(my::Point{ 75, 50 }, my::Point{ -30, -210 });
    segment_ptr->color(my::Color{ 255, 0, 0, 60 });

    doc->render(canvas);
}
