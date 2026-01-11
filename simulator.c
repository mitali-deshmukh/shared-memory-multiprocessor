/* SUBMIT ONLY THIS FILE */

/* NAME: Mitali Manojkumar Deshmukh */
/* UCI ID: deshmum1 */

/* NAME: Nisha Rajesh Gharpure */
/* UCI ID: ngharpur */

/* NAME: Bhoomi Ketan Patani */
/* UCI ID: bpatani */

// only include standard libraries.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "simulator.h" // implements

double PI_VAL = 3.14159265358979323846;
#define MAX_CYCLES 1000000

typedef struct
{
    int request;
    int access_counter;
    int tca;
    int fav;
    int denied;
} Processor;

int raise_req(Processor processor, char dist, int m, int cycle_num)
{

    if (dist == 'u')
    {
        int req = rand_uniform(m);
        return req;
    }
    else if (dist == 'n')
    {
        if (cycle_num == 0)
        {
            return rand_uniform(m);
        }
        else
        {
            return rand_normal_wrap(processor.fav, 5, m);
        }
    }
}

void simulate(double *avg_access_time,
              int avg_access_time_len,
              int procs,
              char dist)
{

    Processor processors[procs];
    int start_idx = 0;
    int first_denied = -1;
    int logical_index = 0;
    double system_wide_avg;
    int isAnyProcDenied = 0;

    for (int m = 1; m <= avg_access_time_len; ++m)
    { // memory modules range between 1 to 512
        // set access count of all processors to 0
        int mem_modules[m];
        double cycle_avg_access_time[MAX_CYCLES];
        first_denied = -1;

        for (int i = 0; i < m; ++i)
        {
            mem_modules[i] = 0; // mark all memory modules as free
        }

        for (int i = 0; i < procs; i++)
        {
            processors[i].access_counter = 0;
            processors[i].denied = 0;
        }

        // for each count of memory modules simulate 10^6 cycles
        for (int c = 0; c < MAX_CYCLES; ++c)
        {

            if (first_denied == -1)
                start_idx = 0;
            else
                start_idx = first_denied;

            for (int i = 0; i < procs; ++i)
            {

                logical_index = (start_idx + i) % procs;

                // if not previously denied, raise a request for a memory module
                if (!processors[logical_index].denied)
                {
                    processors[logical_index].request = raise_req(processors[logical_index], dist, m, c);
                }

                // if c = 0 set processors preference
                if (c == 0 && dist == 'n')
                {
                    processors[i].fav = processors[i].request;
                }

                // assign memory module
                if (mem_modules[processors[logical_index].request] == 0)
                {
                    ++processors[logical_index].access_counter;
                    mem_modules[processors[logical_index].request] = 1;
                    processors[logical_index].denied = 0;
                }
                else
                {
                    processors[logical_index].denied = 1;
                    if (!isAnyProcDenied)
                    {
                        first_denied = logical_index;
                        isAnyProcDenied = 1;
                    }
                }
            }

            // calculate time-cumulative average access time for each processor
            int proc_cnt = 0;
            int tca_sum = 0;
            for (int i = 0; i < procs; ++i)
            {
                if (processors[i].access_counter > 0)
                {
                    processors[i].tca = (c + 1) / processors[i].access_counter;
                    tca_sum += processors[i].tca;
                    proc_cnt++;
                }
            }

            // calculate average system wide memory access-time and store in array
            system_wide_avg = (double)tca_sum / proc_cnt;
            cycle_avg_access_time[c] = system_wide_avg;
            // check for stopping condition
            if (c > 0 && fabs(1.0 - (cycle_avg_access_time[c - 1] / cycle_avg_access_time[c])) < 0.02)
            {

                break;
            }

            // clear first_denied if all processors got their requested memory module
            if (!isAnyProcDenied)
            {
                first_denied = -1;
            }

            // free all mems at end of cycle
            for (int i = 0; i < m; ++i)
            {
                mem_modules[i] = 0; // mark all memory modules as free
            }

            isAnyProcDenied = 0; // track if any processor is left waiting in current cycle
        }

        avg_access_time[m - 1] = system_wide_avg;
    }
}

int rand_uniform(int max)
{
    if (max == 0)
        return max;
    return rand() % max;
}

int rand_normal_wrap(int mean, int dev, int max)
{
    if (max == 0)
        return max;
    static double U, V;
    static int phase = 0;
    double Z;
    if (phase == 0)
    {
        U = (rand() + 1.) / (RAND_MAX + 2.);
        V = rand() / (RAND_MAX + 1.);
        Z = sqrt(-2 * log(U)) * sin(2 * PI_VAL * V);
    }
    else
    {
        Z = sqrt(-2 * log(U)) * cos(2 * PI_VAL * V);
    }
    phase = 1 - phase;
    double res = dev * Z + mean;

    // round result up or down depending on whether
    // it is even or odd. This compensates some bias.
    int res_int;
    // if even, round up. If odd, round down.
    if ((int)res % 2 == 0)
        res_int = (int)(res + 1);
    else
        res_int = (int)(res);

    // wrap result around max
    int res_wrapped = res_int % max;
    // deal with % of a negative number in C
    if (res_wrapped < 0)
        res_wrapped += max;
    return res_wrapped;
}