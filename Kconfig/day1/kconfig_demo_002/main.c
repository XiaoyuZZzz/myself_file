#include <stdio.h>
#include "config.h"

void uart_init(void);
void ble_init(void);

int main(void) {
#if CONFIG_UART
    uart_init();
#endif

#if CONFIG_BLE
    ble_init();
#endif

    while(1);

    return 0;
}
