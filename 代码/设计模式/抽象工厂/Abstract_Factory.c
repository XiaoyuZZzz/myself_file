#include <stdio.h>
#include <stdlib.h>

// ==================== 抽象产品接口 ====================

// 抽象传感器接口
typedef struct
{
    void (*read)(void *self);
    void (*init)(void *self);
    int (*get_id)(void *self);
} SensorVTable;

typedef struct
{
    const SensorVTable *vtable;
} Sensor;

// 抽象执行器接口
typedef struct
{
    void (*activate)(void *self);
    void (*deactivate)(void *self);
    int (*get_status)(void *self);
} ActuatorVTable;

typedef struct
{
    const ActuatorVTable *vtable;
} Actuator;

// ==================== 具体产品实现 ====================

// 温度传感器
typedef struct
{
    Sensor base;
    int id;
    float temperature;
} TemperatureSensor;

void temp_sensor_read(void *self)
{
    TemperatureSensor *sensor = (TemperatureSensor *)self;
    sensor->temperature = 25.0f + (rand() % 100) * 0.1f; // 模拟温度读数
    printf("Temperature Sensor %d: %.1f\n", sensor->id, sensor->temperature);
}

void temp_sensor_init(void *self)
{
    TemperatureSensor *sensor = (TemperatureSensor *)self;
    printf("Temperature Sensor %d initialized\n", sensor->id);
}

int temp_sensor_get_id(void *self)
{
    TemperatureSensor *sensor = (TemperatureSensor *)self;
    return sensor->id;
}

static const SensorVTable temp_sensor_vtable = {
    .read = temp_sensor_read,
    .init = temp_sensor_init,
    .get_id = temp_sensor_get_id};

// 湿度传感器
typedef struct
{
    Sensor base;
    int id;
    float humidity;
} HumiditySensor;

void humidity_sensor_read(void *self)
{
    HumiditySensor *sensor = (HumiditySensor *)self;
    sensor->humidity = 40.0f + (rand() % 400) * 0.1f; // 模拟湿度读数
    printf("Humidity Sensor %d: %.1f%%\n", sensor->id, sensor->humidity);
}

void humidity_sensor_init(void *self)
{
    HumiditySensor *sensor = (HumiditySensor *)self;
    printf("Humidity Sensor %d initialized\n", sensor->id);
}

int humidity_sensor_get_id(void *self)
{
    HumiditySensor *sensor = (HumiditySensor *)self;
    return sensor->id;
}

static const SensorVTable humidity_sensor_vtable = {
    .read = humidity_sensor_read,
    .init = humidity_sensor_init,
    .get_id = humidity_sensor_get_id};

// LED执行器
typedef struct
{
    Actuator base;
    int id;
    int is_on;
} LedActuator;

void led_activate(void *self)
{
    LedActuator *led = (LedActuator *)self;
    led->is_on = 1;
    printf("LED %d: ON\n", led->id);
}

void led_deactivate(void *self)
{
    LedActuator *led = (LedActuator *)self;
    led->is_on = 0;
    printf("LED %d: OFF\n", led->id);
}

int led_get_status(void *self)
{
    LedActuator *led = (LedActuator *)self;
    return led->is_on;
}

static const ActuatorVTable led_vtable = {
    .activate = led_activate,
    .deactivate = led_deactivate,
    .get_status = led_get_status};

// 电机执行器
typedef struct
{
    Actuator base;
    int id;
    int speed;
    int is_running;
} MotorActuator;

void motor_activate(void *self)
{
    MotorActuator *motor = (MotorActuator *)self;
    motor->is_running = 1;
    motor->speed = 100;
    printf("Motor %d: Running at %d%% speed\n", motor->id, motor->speed);
}

void motor_deactivate(void *self)
{
    MotorActuator *motor = (MotorActuator *)self;
    motor->is_running = 0;
    motor->speed = 0;
    printf("Motor %d: Stopped\n", motor->id);
}

int motor_get_status(void *self)
{
    MotorActuator *motor = (MotorActuator *)self;
    return motor->is_running;
}

static const ActuatorVTable motor_vtable = {
    .activate = motor_activate,
    .deactivate = motor_deactivate,
    .get_status = motor_get_status};

// ==================== 抽象工厂接口 ====================

typedef struct
{
    Sensor *(*create_sensor)(int id);
    Actuator *(*create_actuator)(int id);
} DeviceFactory;

// ==================== 具体工厂实现 ====================

// 环境监测工厂 - 创建温度传感器和LED
Sensor *create_environment_sensor(int id)
{
    static TemperatureSensor sensor;
    sensor.base.vtable = &temp_sensor_vtable;
    sensor.id = id;
    sensor.temperature = 0.0f;
    sensor.base.vtable->init(&sensor);
    return (Sensor *)&sensor;
}

Actuator *create_environment_actuator(int id)
{
    static LedActuator actuator;
    actuator.base.vtable = &led_vtable;
    actuator.id = id;
    actuator.is_on = 0;
    return (Actuator *)&actuator;
}

// 运动控制工厂 - 创建湿度传感器和电机
Sensor *create_motion_sensor(int id)
{
    static HumiditySensor sensor;
    sensor.base.vtable = &humidity_sensor_vtable;
    sensor.id = id;
    sensor.humidity = 0.0f;
    sensor.base.vtable->init(&sensor);
    return (Sensor *)&sensor;
}

Actuator *create_motion_actuator(int id)
{
    static MotorActuator actuator;
    actuator.base.vtable = &motor_vtable;
    actuator.id = id;
    actuator.is_running = 0;
    actuator.speed = 0;
    return (Actuator *)&actuator;
}

// 全局工厂实例
const DeviceFactory EnvironmentFactory = {
    .create_sensor = create_environment_sensor,
    .create_actuator = create_environment_actuator};

const DeviceFactory MotionFactory = {
    .create_sensor = create_motion_sensor,
    .create_actuator = create_motion_actuator};

// ==================== 客户端代码 ====================

void test_device_system(const DeviceFactory *factory, int sensor_id, int actuator_id)
{
    printf("\n=== Creating devices from factory ===\n");

    // 使用工厂创建产品
    Sensor *sensor = factory->create_sensor(sensor_id);
    Actuator *actuator = factory->create_actuator(actuator_id);

    // 使用抽象接口操作产品
    printf("Testing devices:\n");
    sensor->vtable->read(sensor);
    actuator->vtable->activate(actuator);

    // 可以安全地向下转型调用具体方法
    if (factory == &EnvironmentFactory)
    {
        TemperatureSensor *temp_sensor = (TemperatureSensor *)sensor;
        LedActuator *led = (LedActuator *)actuator;
        printf("Environment devices: Temp Sensor %d, LED %d\n",
               temp_sensor->id, led->id);
    }
    else
    {
        HumiditySensor *humidity_sensor = (HumiditySensor *)sensor;
        MotorActuator *motor = (MotorActuator *)actuator;
        printf("Motion devices: Humidity Sensor %d, Motor %d\n",
               humidity_sensor->id, motor->id);
    }
}

int main(void)
{
    printf("=== Embedded Abstract Factory Demo ===\n");

    // 使用环境监测工厂
    test_device_system(&EnvironmentFactory, 101, 201);

    // 使用运动控制工厂
    test_device_system(&MotionFactory, 102, 202);

    // 再次使用环境监测工厂创建另一组设备
    test_device_system(&EnvironmentFactory, 103, 203);

    return 0;
}