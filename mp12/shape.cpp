#include "shape.hpp"



//Base class
//Please implement Shape's member functions
//constructor, getName()

Shape::Shape(string name){
    //TODO
    name_ = name;
}

string Shape::getName(){
    //TODO
    return name_;
}


//Rectangle
//Please implement the member functions of Rectangle:
//getArea(), getVolume(), operator+, operator-
//copy(), clear()

void Rectangle::copy(const Rectangle& other){
    //TODO
    spec = new double[2];
    spec[0] = other.spec[0];
    spec[1] = other.spec[1];
}
void Rectangle::clear(){
    //TODO
    delete(spec);
}
Rectangle::Rectangle(double width, double length):Shape("Rectangle"){
    spec = new double[2];
    spec[0] = width;
    spec[1] = length;
}
Rectangle::Rectangle(const Rectangle& other):Shape("Rectangle"){
    copy(other);
}
Rectangle::~Rectangle(){
    clear();
}
const Rectangle& Rectangle::operator = (const Rectangle& other){
    clear();
    copy(other);
    return *this;
}
double Rectangle::getArea()const{
    //TODO
    return spec[0]*spec[1];
}
double Rectangle::getVolume()const{
    //TODO
    return 0;
}
Rectangle Rectangle::operator + (const Rectangle& rhs){
    //TODO
    Rectangle temp(0,0);
    temp.spec[0] = spec[0] + rhs.spec[0];
    temp.spec[1] = spec[1] + rhs.spec[1];
    return temp;
}

Rectangle Rectangle::operator - (const Rectangle& rhs){
    //TODO
    Rectangle temp(0.0,0.0);
    temp.spec[0] = max(0.0,spec[0] - rhs.spec[0]);
    temp.spec[1] = max(0.0,spec[1] - rhs.spec[1]);
    return temp;
}

// double * spec;
//spec[0] should be width
//spec[1] should be length
double Rectangle::getWidth()const{return spec? spec[0]:0;}
double Rectangle::getLength()const{return spec? spec[1]:0;}
void Rectangle::setWidth(double width){
    if (spec == NULL)
        return;

    spec[0] = width;
}
void Rectangle::setLength(double length){
    if (spec == NULL)
        return;

    spec[1] = length;
}


//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
void RectPrism::copy(const RectPrism& other){
    //TODO
    spec = new double[3];
    spec[0] = other.spec[0];
    spec[1] = other.spec[1];
    spec[2] = other.spec[2];
}
void RectPrism::clear(){
    //TODO
    delete(spec);
}
RectPrism::RectPrism(double width, double length, double height):Shape("RectPrism"){
    spec = new double[3];
    spec[0] = length;
    spec[1] = width;
    spec[2] = height;
}
RectPrism::RectPrism(const RectPrism& other):Shape("RectPrism"){
    copy(other);
}
RectPrism::~RectPrism(){
    clear();
}
const RectPrism& RectPrism::operator = (const RectPrism& other){
    clear();
    copy(other);
    return *this;
}
double RectPrism::getVolume()const{
    //TODO
    return spec[0]*spec[1]*spec[2];
}
double RectPrism::getArea()const{
    //TODO
    return (2*(spec[0]*spec[1] + spec[0]*spec[2] + spec[1]*spec[2]));
}
RectPrism RectPrism::operator + (const RectPrism& rhs){
    //TODO
    RectPrism temp(0,0,0);
    temp.spec[0] = spec[0] + rhs.spec[0];
    temp.spec[1] = spec[1] + rhs.spec[1];
    temp.spec[2] = spec[2] + rhs.spec[2];
    return temp;
}

RectPrism RectPrism::operator - (const RectPrism& rhs){
    //TODO
    RectPrism temp(0,0,0);
    temp.spec[0] = max(0.0, spec[0] - rhs.spec[0]);
    temp.spec[1] = max(0.0, spec[1] - rhs.spec[1]);
    temp.spec[2] = max(0.0, spec[2] - rhs.spec[2]);
    return temp;
}

// double * spec;
//spec[0] should be length
//spec[1] should be width
//spec[2] should be height
double RectPrism::getWidth()const{return spec? spec[1]:0;}
double RectPrism::getHeight()const{return spec? spec[2]:0;}
double RectPrism::getLength()const{return spec? spec[0]:0;}
void RectPrism::setWidth(double width){
    if (spec == NULL)
        return;

    spec[1] = width;
}
void RectPrism::setHeight(double height){
    if (spec == NULL)
        return;

    spec[2] = height;
}
void RectPrism::setLength(double length){
    if (spec == NULL)
        return;

    spec[0] = length;
}



// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects
vector<Shape*> CreateShapes(char* file_name){
    //@@Insert your code here

    ifstream ifs (file_name, std::ifstream::in);
    double num_shapes = 0;
    ifs >> num_shapes;
    vector<Shape*> shape_ptrs(num_shapes, NULL);
    //TODO

    double length = 0.0, width = 0.0, height = 0.0;
    string name;
    Shape* temp = NULL;

    for(int i = 0; i<num_shapes;i++){
      ifs >> name;
      if (name == "Rectangle"){
        ifs >> width >> length;
        temp = new Rectangle(width,length);
        shape_ptrs.at(i) = temp;
      }
      if (name == "RectPrism"){
        ifs >> width >> length >> height;
        temp = new RectPrism(width,length,height);
        shape_ptrs.at(i) = temp;
      }
    }

    ifs.close();
    return shape_ptrs;
}

// call getArea() of each object
// return the max area
double MaxArea(vector<Shape*> shapes){
    double max_area = 0;
    //@@Insert your code here
    // for(int i = 0; i < shapes.size();i++){
    //   if(max_area < shapes[i]->getArea()){
    //     max_area = shapes[i]->getArea();
    //   }
    // }
    return max_area;
}


// call getVolume() of each object
// return the max volume
double MaxVolume(vector<Shape*> shapes){
    double max_volume = 0;
    //@@Insert your code here
    // for(int i = 0; i < shapes.size();i++){
    //   if(max_volume < shapes[i]->getVolume()){
    //     max_volume = shapes[i]->getVolume();
    //   }
    // }
    return max_volume;
}
