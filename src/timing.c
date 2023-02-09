#include "timing.h"
#include <time.h>
#include <stdio.h>

/* timer_id: 0 ~ (MAX_TIMER_NUM - 1) */
#define MAX_TIMER_NUM 65536
#define OUTPUT_FILE "timing.out"

struct timer {
    const char *name;
    struct timespec tm_acc;
    struct timespec tm_start;
} _timer[MAX_TIMER_NUM];

unsigned char _timer_used[MAX_TIMER_NUM];
unsigned int _timer_max_idx;

#define NSEC_PER_SEC 1000000000

inline static void _timespec_add(struct timespec* ts, const struct timespec* ts1) {
    ts->tv_sec += ts1->tv_sec;
    ts->tv_nsec += ts1->tv_nsec;
    if (ts->tv_nsec > NSEC_PER_SEC) {
        ts->tv_nsec -= NSEC_PER_SEC;
        ts->tv_sec ++;
    }
}

inline static void _timespec_sub(struct timespec* ts, const struct timespec* ts1) {
    ts->tv_sec -= ts1->tv_sec;
    ts->tv_nsec -= ts1->tv_nsec;
    if (ts->tv_nsec < 0) {
        ts->tv_nsec += NSEC_PER_SEC;
        ts->tv_sec --;
    }
}

__attribute((constructor)) void _timing_before_main() {

}

__attribute((destructor)) void _timing_after_main() {
    struct timespec tm_total;
    tm_total.tv_sec = 0;
    tm_total.tv_nsec = 0;
    FILE *fp = fopen(OUTPUT_FILE, "w");
    for (unsigned int i = 0; i <= _timer_max_idx; i++) {
        if (!_timer_used[i]) continue;
        fprintf(fp, "Timer %d[%s]: %lu.%06lu secs.\n", 
            i, 
            _timer[i].name ? _timer[i].name : "", 
            _timer[i].tm_acc.tv_sec, 
            _timer[i].tm_acc.tv_nsec / 1000);
        _timespec_add(&tm_total, &_timer[i].tm_acc);
    }
    fprintf(fp, "Total: %lu.%06lu secs.\n",
        tm_total.tv_sec,
        tm_total.tv_nsec / 1000);
    fclose(fp);
}

void _timing_start_time(const int timer_id) {
    timespec_get(&_timer[timer_id].tm_start, TIME_UTC);
}


void _timing_stop_time(const int timer_id) {
    struct timespec tm_stop;
    timespec_get(&tm_stop, TIME_UTC);
    _timespec_sub(&tm_stop, &_timer[timer_id].tm_start);
    _timespec_add(&_timer[timer_id].tm_acc, &tm_stop);
    _timer[timer_id].tm_start.tv_sec = 0;
    _timer[timer_id].tm_start.tv_nsec = 0;
}

void _timing_register_timer(const int timer_id, const char *name) {
    if (timer_id > _timer_max_idx) {
        _timer_max_idx = timer_id;
    }
    _timer_used[timer_id] = 1;
    _timer[timer_id].name = name;
}