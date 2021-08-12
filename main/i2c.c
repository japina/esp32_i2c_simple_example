#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "hal/i2c_types.h"


static const char *TAG = "i2c-simple-example";


/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = 0;
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,      
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 22,      
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 1, 1, 0);
}

void intr_handler(void){
    ESP_LOGI(TAG, "interrupt");
}

void app_main(void)
{
    uint8_t data;
    u_int8_t in_data = 0;
    data=65;
    intr_handle_t *inth;
    ESP_ERROR_CHECK(i2c_master_init());
    //ESP_ERROR_CHECK(i2c_isr_register(0, &intr_handler, NULL, inth));
    ESP_LOGI(TAG, "I2C initialized successfully");
    i2c_cmd_handle_t handle = i2c_cmd_link_create();        
    ESP_ERROR_CHECK(i2c_master_start(handle));
    ESP_ERROR_CHECK(i2c_master_write_byte(handle, (0x08 << 1) | I2C_MASTER_WRITE, 0));
    ESP_ERROR_CHECK(i2c_master_write_byte(handle, 65, 0));
    ESP_ERROR_CHECK(i2c_master_cmd_begin(0, handle, 10000));
    ESP_ERROR_CHECK(i2c_master_stop(handle));
    i2c_cmd_link_delete(handle);                    
    ESP_ERROR_CHECK(i2c_driver_delete(0));
    ESP_LOGI(TAG, "I2C unitialized successfully");
}
