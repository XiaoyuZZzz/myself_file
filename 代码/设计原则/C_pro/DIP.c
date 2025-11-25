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