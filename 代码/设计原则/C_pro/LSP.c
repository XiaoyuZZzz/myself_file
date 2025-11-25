#include <stdio.h>

typedef struct
{
    void (*draw)(void *self);
    double (*area)(void *self);
} Shape;

typedef struct
{
    Shape base;
    double width;
    double height;
} Rectangle;

typedef struct
{
    Shape base;
    double side;
} Square;

Rectangle rect1, rect2;
Square square1, square2;

// 矩形的绘制函数
void rectangle_draw(void *self)
{
    Rectangle *rect = (Rectangle *)self;
    printf("rectangle_draw: width=%.2f, hgiht=%.2f\n", rect->width, rect->height);
}

// 矩形的面积计算
double rectangle_area(void *self)
{
    Rectangle *rect = (Rectangle *)self;
    return rect->width * rect->height;
}

// 正方形的绘制函数
void square_draw(void *self)
{
    Square *square = (Square *)self;
    printf("square_draw: side=%.2f\n", square->side);
}

// 正方形的面积计算
double square_area(void *self)
{
    Square *square = (Square *)self;
    return square->side * square->side;
}

// 初始化矩形对象
void rectangle_init(Rectangle *rect, double width, double height)
{
    rect->base.draw = rectangle_draw;
    rect->base.area = rectangle_area;
    rect->width = width;
    rect->height = height;
}

// 初始化正方形对象
void square_init(Square *square, double side)
{
    square->base.draw = square_draw;
    square->base.area = square_area;
    square->side = side;
}

// 使用形状的函数 - 体现里氏替换原则
void use_shape(Shape *shape)
{
    printf("use_shape:\n");
    shape->draw(shape);
    double area = shape->area(shape);
    printf("area: %.2f\n\n", area);
}

int main()
{

    rectangle_init(&rect1, 5.0, 10.0);
    square_init(&square1, 5.0);

    Shape *shape1 = (Shape *)&rect1;
    Shape *shape2 = (Shape *)&square1;

    use_shape(shape1);
    use_shape(shape2);
    return 0;
}