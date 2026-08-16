#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "mpu6050.h"

float Pitch, Roll, Yaw;

static void Init_Failed(const char *stage, u8 code)
{
    printf("%s ERROR, CODE=%u\r\n", stage, (unsigned int)code);
    while (1)
    {
        PCout(13) = !PCout(13);
        delay_ms(200);
    }
}

int main(void)
{
    u8 who = 0;
    u8 iic_status;
    u8 mpu_status;
    u8 dmp_status;
    u8 print_divider = 0;

    uart_init(115200);
    delay_init();
    LED_Init();

    delay_ms(100);
    IIC_Init();
    delay_ms(20);

    printf("\r\nMPU6050 DMP VERIFY START\r\n");
    iic_status = i2cRead(MPU6050_DEFAULT_ADDRESS,
                         MPU6050_RA_WHO_AM_I,
                         1,
                         &who);
    printf("I2C_READ=%u, WHO_AM_I=0x%02X\r\n",
           (unsigned int)iic_status,
           (unsigned int)who);

    if (iic_status != 0)
        Init_Failed("I2C", iic_status);
    if (who != MPU_DEVICE_WHO_AM_I)
        Init_Failed("WHO_AM_I", who);

    mpu_status = MPU6050_initialize();
    if (mpu_status != MPU6050_INIT_OK)
        Init_Failed("MPU INIT", mpu_status);
    printf("MPU INIT OK\r\n");

    dmp_status = DMP_Init();
    if (dmp_status != DMP_INIT_OK)
        Init_Failed("DMP INIT", dmp_status);
    printf("DMP INIT OK, 200HZ\r\n");

    while (1)
    {
        if (Read_DMP(&Pitch, &Roll, &Yaw))
        {
            print_divider++;
            if (print_divider >= 10)
            {
                print_divider = 0;
                printf("Pitch=%.2f, Roll=%.2f, Yaw=%.2f\r\n",
                       Pitch,
                       Roll,
                       Yaw);
            }
        }
        delay_ms(2);
    }
}
