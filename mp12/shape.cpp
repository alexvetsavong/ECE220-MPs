#include "shape.hpp"



//Base class
//Please implement Shape's member functions
//constructor, getName()

Shape::Shape(string name){
    //TODO
}
string Shape::getName(){
    //TODO
    return "name";
}


//Rectangle
//Please implement the member functions of Rectangle:
//getArea(), getVolume(), operator+, operator-
//copy(), clear()

void Rectangle::copy(const Rectangle& other){
    //TODO
}
void Rectangle::clear(){
    //TODO
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
    return 0;
}
double Rectangle::getVolume()const{
    //TODO
    return 1;
}
Rectangle Rectangle::operator + (const Rectangle& rhs){
    //TODO
    return Rectangle(0,0);
}

Rectangle Rectangle::operator - (const Rectangle& rhs){
    //TODO
    return Rectangle(0,0);
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
}
void RectPrism::clear(){
    //TODO
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
    return 0;
}
double RectPrism::getArea()const{
    //TODO
    return 0;
}  
RectPrism RectPrism::operator + (const RectPrism& rhs){
    //TODO
    return RectPrism(0,0,0);
}

RectPrism RectPrism::operator - (const RectPrism& rhs){
    //TODO
    return RectPrism(0,0,0);
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
    

    ifs.close();
    return shape_ptrs;
}

// call getArea() of each object 
// return the max area
double MaxArea(vector<Shape*> shapes){
    double max_area = 0;
    //@@Insert your code here
    
    return max_area;
}


// call getVolume() of each object 
// return the max volume
double MaxVolume(vector<Shape*> shapes){
    double max_volume = 0;
    //@@Insert your code here

    return max_volume;
}

