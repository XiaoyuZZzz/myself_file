# SOLID原则
> 前言： 不要做一个程序员，而是做一个工程师
## S：单一职责原则
```c
#include <stdio.h>
#include <stdlib.h>

// ==================== 温度数据职责 ====================

typedef struct {
    double celsius;
    double fahrenheit;
    double kelvin;
} Temperature;

Temperature* create_temperature(double celsius) {
    Temperature* temp = (Temperature*)malloc(sizeof(Temperature));
    if (temp) {
        temp->celsius = celsius;
    }
    return temp;
}

void destroy_temperature(Temperature* temp) {
    free(temp);
}

// ==================== 温度转换职责 ====================

void celsius_to_all(Temperature* temp) {
    if (temp) {
        temp->fahrenheit = (temp->celsius * 9.0 / 5.0) + 32.0;
        temp->kelvin = temp->celsius + 273.15;
    }
}

// ==================== 显示输出职责 ====================

void display_temperature(const Temperature* temp) {
    if (temp) {
        printf("\n温度转换结果:\n");
        printf("=============\n");
        printf("摄氏温度: %.2f°C\n", temp->celsius);
        printf("华氏温度: %.2f°F\n", temp->fahrenheit);
        printf("开氏温度: %.2fK\n", temp->kelvin);
        printf("=============\n");
    }
}

// ==================== 主程序 ====================

int main() {
    double input_temp;
    
    printf("请输入摄氏温度: ");
    scanf("%lf", &input_temp);
    
    // 创建温度数据
    Temperature* temp = create_temperature(input_temp);
    if (!temp) {
        printf("内存分配失败\n");
        return 1;
    }
    
    // 进行温度转换
    celsius_to_all(temp);
    
    // 显示结果
    display_temperature(temp);
    
    // 清理资源
    destroy_temperature(temp);
    
    return 0;
}

输出结果：

请输入摄氏温度: 23

温度转换结果:
=============
摄氏温度: 23.00°C
华氏温度: 73.40°F
开氏温度: 296.15K
=============
```
### 单一职责片段总结
单一职责在C语言中就是使用一个函数去做一件事，打印就是打印，保存就是保存，转换就是转换，函数专注于自己的工作，由主程序去调用使用

## O：开闭原则

```c
#include <stdio.h>
#include <stdint.h>

typedef void (*action_func_t)(void);

// 函数实现
void action1(void) { printf("Action 1 executed\n"); }
void action2(void) { printf("Action 2 executed\n"); }
void action3(void) { printf("Action 3 executed\n"); }

// 定义一个结束标记
#define ACTION_END (action_func_t)0

// 将函数指针放入指定段中，并在末尾添加结束标记
__attribute__((section(".action_table"), used))
action_func_t action_table[] = {
    action1,
    action2,
    action3,
    ACTION_END};

void traverse_custom_section(void)
{
    action_func_t *current = action_table;

    printf("Custom section starts at %p\n", current);

    while (*current != ACTION_END)
    {
        (*current)();
        current++;
    }
}

int main()
{
    traverse_custom_section();
    return 0;
}

```

这里很简单的使用了一个gcc的特性，将数据保存至特定的段中，然后从段起始地址开始访问至结束。这样的好处就是可以直接添加函数至数据段中，不需要添加任何的swtich-case语句，在嵌入式开发中，如果我们需要对串口数据进行访问操作，那么这样的好处就是可以直接添加函数，而不需要进行其他的操作，而访问的条件只需要进行判断对应的if-else即可
### 开闭原则片段总结
开闭原则顾名思义，就是对操作开放，对修改关闭，这样在嵌入式开发中我们需要扩展新的业务逻辑，只需要这样就可以了。

## LSP:里氏替换原则
```c
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
```
### 里氏替换片段总结
可以根据这个例子看出来，所谓里氏替换原则就是子类继承父类，并且进行扩展。
这里的父类是创建和绘画，一个正方形一个圆形，使用draw进行创建对应的子类，area进行绘画，这样的好处是只需要传入新的子类，然后就能够使用一开始的area函数。
转换成一个具体的业务场景，我都是获取温湿度，但是我的底层可能是使用DHT11可能是使用DHT22或者内温传，那么我对代码进行修改的时候添加新的模块，不可能从头改到尾，而使用里氏替换原则，只需要添加初始化，然后注册句柄，就可以实现对应的操作了。

## I:接口隔离
```c
#include <stdio.h>
#include <string.h>

typedef struct {
    void (*work)(void* self);
} Workable;

typedef struct {
    void (*rest)(void* self);
} Restable;

typedef struct {
    void (*study)(void* self);
} Learnable;

typedef struct {
    Workable work_interface;
    Restable rest_interface;
    Learnable learn_interface;
    char name[20];
} Programmer;

void programmer_work(void* self) {
    Programmer* p = (Programmer*)self;
    printf("%s is writing code\n", p->name);
}

void programmer_rest(void* self) {
    Programmer* p = (Programmer*)self;
    printf("%s is resting and drinking coffee\n", p->name);
}

void programmer_study(void* self) {
    Programmer* p = (Programmer*)self;
    printf("%s is learning new technology\n", p->name);
}

void programmer_init(Programmer* p, const char* name) {
    p->work_interface.work = programmer_work;
    p->rest_interface.rest = programmer_rest;
    p->learn_interface.study = programmer_study;
    strncpy(p->name, name, sizeof(p->name) - 1);
}

typedef struct {
    Workable work_interface;
    char name[20];
} Robot;

void robot_work(void* self) {
    Robot* r = (Robot*)self;
    printf("%s is performing tasks\n", r->name);
}

void robot_init(Robot* r, const char* name) {
    r->work_interface.work = robot_work;
    strncpy(r->name, name, sizeof(r->name) - 1);
}

typedef struct {
    Learnable learn_interface;
    Restable rest_interface;
    char name[20];
} Student;

void student_study(void* self) {
    Student* s = (Student*)self;
    printf("%s is studying courses\n", s->name);
}

void student_rest(void* self) {
    Student* s = (Student*)self;
    printf("%s is taking a break between classes\n", s->name);
}

void student_init(Student* s, const char* name) {
    s->learn_interface.study = student_study;
    s->rest_interface.rest = student_rest;
    strncpy(s->name, name, sizeof(s->name) - 1);
}

void work_manager(Workable* worker) {
    printf("Work manager: ");
    worker->work(worker);
}

void study_manager(Learnable* learner) {
    printf("Study manager: ");
    learner->study(learner);
}

void rest_manager(Restable* rester) {
    printf("Rest manager: ");
    rester->rest(rester);
}

int main() {
    printf("=== Interface Segregation Principle Demo ===\n\n");
    
    Programmer alice;
    Robot robot1;
    Student bob;
    
    programmer_init(&alice, "Alice");
    robot_init(&robot1, "Robot001");
    student_init(&bob, "Bob");
    
    printf("1. Work manager test:\n");
    work_manager((Workable*)&alice);
    work_manager((Workable*)&robot1);
    
    printf("\n2. Study manager test:\n");
    study_manager((Learnable*)&alice);
    study_manager((Learnable*)&bob);
    
    printf("\n3. Rest manager test:\n");
    rest_manager((Restable*)&alice);
    rest_manager((Restable*)&bob);
    
    printf("\n4. Full capabilities demo:\n");
    printf("Programmer capabilities: \n");
    alice.work_interface.work(&alice);
    alice.rest_interface.rest(&alice);
    alice.learn_interface.study(&alice);
    
    printf("\nRobot capabilities: \n");
    robot1.work_interface.work(&robot1);
    
    printf("\nStudent capabilities: \n");
    bob.learn_interface.study(&bob);
    bob.rest_interface.rest(&bob);
    
    return 0;
}
```
### 接口隔离片段总结
接口隔离原则就是尽量的减少接口的范围，使得客户端不必使用不需要的行为，比如说程序员可以学习、工作、休息，学生可以学习、休息。对于学生来说工作是不需要的，那么这样我们在创建对应的句柄实例的时候不需要创建对应的休息指针，只需要创建学习、休息。当然接口的颗粒度不是越小越好，接口过多久就意味着代码的复杂性，在具体的业务场景中需要平衡接口的数量。


## D:依赖倒置原则
```c
#include <stdio.h>
#include <stdint.h>

/* Dependency Inversion Principle */
struct hal_api
{
    void (*get_data)(void);
    const char *(*get_type)(void);
};

struct monitor_t
{
    struct hal_api *monitor[5];
    int monitor_count;
};

void monitor_register(struct monitor_t *monitor, struct hal_api *api)
{
    if (monitor->monitor_count < 5)
    {
        monitor->monitor[monitor->monitor_count++] = api;
    }
}

void get_monitor_data(struct monitor_t *monitor)
{

    for (uint8_t i = 0; i < monitor->monitor_count; i++)
    {
        printf("type:%s    ", monitor->monitor[i]->get_type());
        monitor->monitor[i]->get_data();
        printf("\r\n");
    }
}

void dht11_get_data(void)
{
    printf("DHT11 Data");
}

const char *dht11_get_type(void)
{
    return "DHT11";
}

void dht22_get_data(void)
{
    printf("DHT22 Data");
}

const char *dht22_get_type(void)
{
    return "DHT22";
}

int main(void)
{

    struct hal_api dht11 = {.get_data = dht11_get_data, .get_type = dht11_get_type};
    struct hal_api dht22 = {.get_data = dht22_get_data, .get_type = dht22_get_type};

    struct monitor_t monitor = {.monitor_count = 0};

    monitor_register(&monitor, &dht11);
    monitor_register(&monitor, &dht22);

    get_monitor_data(&monitor);

    return 0;
}
```

### 依赖倒置片段总结
依赖倒置原则的本意就是顶层和底层之间切断关系，顶层依赖于抽象接口，底层也依赖于抽象接口。上述的例子很简单，如果我们要去获取一个传感器的值。一般新手可能就是直接使用dht11_get函数，这样虽然可以，但是考虑到后期的更改，如果公司修改了硬件模块，那么我们应该如何快速的去修改呢？dht11只是简单的例子，如果业务比较复杂，在应用层大量调用接口，势必会导致修改的复杂性。如果我们使用依赖倒置原则，底层和顶层之间使用抽象接口，那么我们只需要修改底层的接口内容，应用层依然还是使用对应的接口，这样的好处无需多言，能非常符合我们的业务场景。
```
也许细心的朋友会问，之前的里氏替换和这个依赖倒置不都差不多么，啥时候用依赖倒置什么时候用里氏替换呢？
是的，里氏替换的主要思想就是子类继承父类，而不破坏父类，能够有效的替换。而依赖导致主要是说模块之间的依赖关系，在C中，依赖倒置更多的是去解决接口与实现分离，而里氏替换是去解决函数指针的一致性问题。如果说依赖倒置是横向的模块问题，那么里氏替换则是纵向问题。通常这两种思想都是互通的，使用依赖倒置创建抽象接口，而使用里氏替换去保证不同的实现可以正确的替换。
```