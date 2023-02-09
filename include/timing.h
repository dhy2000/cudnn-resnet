#ifndef _TIMING_H_
#define _TIMING_H_


#ifdef __cplusplus
extern "C" {
#endif

__attribute((constructor)) void _timing_before_main();
__attribute((destructor)) void _timing_after_main();

void _timing_start_time(const int timer_id);
void _timing_stop_time(const int timer_id);
void _timing_register_timer(const int timer_id, const char* name);

#define registertimer(timer_id, name) _timing_register_timer(timer_id, name)
#define starttime(timer_id) _timing_start_time(timer_id)
#define stoptime(timer_id) _timing_stop_time(timer_id)

#ifdef __cplusplus
}
#endif


#endif // _TIMING_H_