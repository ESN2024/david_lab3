#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include "sys/alt_sys_init.h"
#include "alt_types.h"
#include "io.h"
#include "unistd.h"
#include "stdio.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_timer_regs.h"
#include "opencores_i2c.h"
#include "opencores_i2c_regs.h"

//user defines
#define i2c_write       0
#define i2c_read        1
#define last_byte       1

#define gsensor_addr    0x1d
#define DEVID           0x00
#define OFSX            0x1E
#define OFSY            0x1F
#define OFSZ            0x20
#define BW_RATE         0x2C
#define POWER_CTL       0x2D
#define INT_ENABLE      0x2E
#define INT_SOURCE      0x30
#define DATAX0          0x32
#define DATAY0          0x34
#define DATAZ0          0x36

#define x_axis_calibration 0
#define y_axis_calibration 0
#define z_axis_calibration 0

//réglage empirique de la sensibilité
#define axis_calibration 230

//user global vars
volatile alt_u16 cpt   =0,
                 x_val =0,
                 y_val =0,
                 z_val =0;

volatile enum axis{X, Y, Z} axis_to_print =X;


//user functions
alt_u16 complement_a_2(alt_u16 val){
    return (val ^ 0xffff) +1;
}

void gsensor_i2c_single_write(alt_u8 register_addr, alt_u8 data){
    I2C_start(OPENCORES_I2C_0_BASE, gsensor_addr, i2c_write);
    I2C_write(OPENCORES_I2C_0_BASE, register_addr, !last_byte);
    I2C_write(OPENCORES_I2C_0_BASE, data, last_byte);
}

alt_u8 gsensor_i2c_single_read(alt_u8 register_addr){
    I2C_start(OPENCORES_I2C_0_BASE, gsensor_addr, i2c_write);
    I2C_write(OPENCORES_I2C_0_BASE, register_addr, !last_byte);
    I2C_start(OPENCORES_I2C_0_BASE, gsensor_addr, i2c_read);
    return I2C_read(OPENCORES_I2C_0_BASE, last_byte);
}

alt_u16 gsensor_i2c_multiple_read(alt_u8 register_addr){
    alt_u8  lsb;
    alt_u16 msb;

    I2C_start(OPENCORES_I2C_0_BASE, gsensor_addr, i2c_write);
    I2C_write(OPENCORES_I2C_0_BASE, register_addr, !last_byte);
    I2C_start(OPENCORES_I2C_0_BASE, gsensor_addr, i2c_read);
    lsb= I2C_read(OPENCORES_I2C_0_BASE, !last_byte);
    msb= I2C_read(OPENCORES_I2C_0_BASE, last_byte);

    return (msb<<8) + (lsb);
}

alt_u16 gsensor_to_bcd(alt_u16 gsensor_axis){
    alt_u8 sign =0,
           unites,
           dixiemes;

    if (gsensor_axis >= 512){
        sign= 1;
        gsensor_axis= complement_a_2(gsensor_axis);
    }

    //réglage empirique de la sensibilité
    gsensor_axis= (gsensor_axis*100)/axis_calibration;

    dixiemes= (gsensor_axis/10)%10;
    unites= (gsensor_axis/100)%10;

    return (sign<<8) + (unites<<4) + (dixiemes);
}

void gsensor_print(){
    switch (axis_to_print){
    case X:
        IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_BASE, gsensor_to_bcd(x_val));
        alt_printf("printing X...\n");
        alt_printf("hex \t\t\tX= %x\n", x_val);
        alt_printf("bcd (sign;unit;tenth) \tX= %x\n\n", gsensor_to_bcd(x_val));
        return;

    case Y:
        IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_BASE, gsensor_to_bcd(y_val));
        alt_printf("printing Y...\n");
        alt_printf("hex \t\t\tY= %x\n", y_val);
        alt_printf("bcd (sign;unit;tenth) \tY= %x\n\n", gsensor_to_bcd(y_val));
        return;

    case Z:
        IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_BASE, gsensor_to_bcd(z_val));
        alt_printf("printing Z...\n");
        alt_printf("hex \t\t\tZ= %x\n", z_val);
        alt_printf("bcd (sign;unit;tenth) \tZ= %x\n\n", gsensor_to_bcd(z_val));
        return;

    default:
        alt_printf("printing default\n");
        return;
    }
}

void gsensor_measurement(){
    if((gsensor_i2c_single_read(INT_SOURCE) & 0x80) != 0x80)
        alt_printf("ERROR** Gsensor wasn't ready for measurements\n");

    x_val= gsensor_i2c_multiple_read(DATAX0);
    y_val= gsensor_i2c_multiple_read(DATAY0);
    z_val= gsensor_i2c_multiple_read(DATAZ0);
}


//user interruptions subroutines
static void isr_handler_tim(void* context, alt_u32 id){
    IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0b00);

    gsensor_measurement();
    gsensor_print();
}

static void isr_handler_bp(void* context, alt_u32 id){
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_2_BASE, 0b01);

    switch (axis_to_print){
    case X:
        axis_to_print= Y;
        return;
    case Y:
        axis_to_print= Z;
        return;
    case Z:
        axis_to_print= X;
        return;
    default:
        axis_to_print= X;
        return;
    }
}


//main
int main(){
    alt_printf("\n\nINITIALIZATION\n");

    //BP init
    alt_printf("bp init\n");
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_2_BASE, 0b01);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PIO_2_BASE, 0b01);
    alt_irq_register(PIO_2_IRQ, NULL, (void*)isr_handler_bp);

    //timer init
    alt_printf("timer init\n");
    IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x02FA);
    IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0xF07F);
    IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0b0111);
    alt_irq_register(TIMER_0_IRQ, NULL, (void*)isr_handler_tim);

    //i2c init
    alt_printf("i2c init\n");
    alt_u8 I2C_status =1;
    I2C_init(OPENCORES_I2C_0_BASE, 50000000, 400000);
    I2C_status= I2C_start(OPENCORES_I2C_0_BASE, gsensor_addr, i2c_read);
    if (I2C_status == 1)
        alt_printf("!!! Gsensor I2C nACK\n");

    //gsensor init
    alt_printf("gsensor init\n");
    gsensor_i2c_single_read(DEVID);
    gsensor_i2c_single_write(BW_RATE, 0b00001010);
    gsensor_i2c_single_write(POWER_CTL, 0b00001000);
    gsensor_i2c_single_write(INT_ENABLE, 0b10000000);
    gsensor_i2c_single_write(OFSX, x_axis_calibration);
    gsensor_i2c_single_write(OFSY, y_axis_calibration);
    gsensor_i2c_single_write(OFSZ, z_axis_calibration);

    alt_printf("INITIALIZATION FINISHED\n\n");

    while(1){
    }
    return 0;
}
