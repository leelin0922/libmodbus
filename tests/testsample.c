/*
 * Copyright © 2008-2010 Stéphane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include <modbus.h>

#define G_MSEC_PER_SEC 1000

uint32_t gettime_ms(void)
{
    struct timeval tv;
    gettimeofday (&tv, NULL);

    return (uint32_t) tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

enum {
    TCP,
    RTU
};

/* Tests based on PI-MBUS-300 documentation */
int main(int argc, char *argv[])
{
    uint8_t *tab_bit;
    uint16_t *tab_reg;
    modbus_t *ctx;
    int i;
    int nb_points;
    double elapsed;
    uint32_t start;
    uint32_t end;
    uint32_t bytes;
    uint32_t rate;
    int rc;
    int n_loop;
    int use_backend;
	int addr=1;

    if (argc > 1) {
        if (strcmp(argv[1], "tcp") == 0) {
            use_backend = TCP;
            n_loop = 100000;
        } else if (strcmp(argv[1], "rtu") == 0) {
            use_backend = RTU;
            n_loop = 100;
        } else {
            printf("Usage:\n  %s [tcp|rtu] - Modbus client to measure data bandwith\n\n", argv[0]);
            exit(1);
        }
    } else {
        /* By default */
        use_backend = TCP;
        n_loop = 100000;
    }

    if (use_backend == TCP) {
        ctx = modbus_new_tcp("127.0.0.1", 1502);
    } else {
        ctx = modbus_new_rtu("/dev/ttyO1", 9600, 'N', 8, 1);
        modbus_set_slave(ctx, addr);
    }

	modbus_set_debug(ctx, 1);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connexion failed: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    /* Allocate and initialize the memory to store the status */
    tab_bit = (uint8_t *) malloc(MODBUS_MAX_READ_BITS * sizeof(uint8_t));
    memset(tab_bit, 0, MODBUS_MAX_READ_BITS * sizeof(uint8_t));

    /* Allocate and initialize the memory to store the registers */
    tab_reg = (uint16_t *) malloc(MODBUS_MAX_READ_REGISTERS * sizeof(uint16_t));
    memset(tab_reg, 0, MODBUS_MAX_READ_REGISTERS * sizeof(uint16_t));

    nb_points = MODBUS_MAX_READ_BITS;

    printf("READ REGISTERS\n\n");

    rc = modbus_read_registers(ctx,0,32,tab_reg);
    if (rc == -1) {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
        return -1;
    }

    /* Free the memory */
    free(tab_bit);
    free(tab_reg);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
