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