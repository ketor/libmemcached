/*  vim:expandtab:shiftwidth=2:tabstop=2:smarttab:
 *
 *  Data Differential YATL (i.e. libtest)  library
 *
 *  Copyright (C) 2012 Data Differential, http://datadifferential.com/
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 *      * Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *
 *      * Redistributions in binary form must reproduce the above
 *  copyright notice, this list of conditions and the following disclaimer
 *  in the documentation and/or other materials provided with the
 *  distribution.
 *
 *      * The names of its contributors may not be used to endorse or
 *  promote products derived from this software without specific prior
 *  written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once

#include <ctime>
#include <ostream>

#ifdef __MACH__
#  include <mach/clock.h>
#  include <mach/mach.h>
#else
#  include <sys/time.h>
#endif


namespace libtest {

class Timer {
public:

  Timer()
  {
    _begin.tv_sec= 0;
    _begin.tv_nsec= 0;
    _end.tv_sec= 0;
    _end.tv_nsec= 0;
  }

  void reset()
  {
    _end.tv_sec= 0;
    _end.tv_nsec= 0;
    _time(_begin);
  }

  void sample()
  {
    _time(_end);
  }

  void difference(struct timespec& arg) const
  {
    if ((_end.tv_nsec -_begin.tv_nsec) < 0)
    {
      arg.tv_sec= _end.tv_sec -_begin.tv_sec-1;
      arg.tv_nsec= 1000000000 +_end.tv_nsec -_begin.tv_nsec;

    }
    else
    {
      arg.tv_sec= _end.tv_sec -_begin.tv_sec;
      arg.tv_nsec= _end.tv_nsec -_begin.tv_nsec;
    }
  }

private:
  void _time(struct timespec& ts)
  {
#ifdef __MACH__ // OSX lacks clock_gettime()
    clock_serv_t _clock_serv;
    mach_timespec_t _mach_timespec;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &_clock_serv);
    clock_get_time(_clock_serv, &_mach_timespec);
    mach_port_deallocate(mach_task_self(), _clock_serv);
    ts.tv_sec= _mach_timespec.tv_sec;
    ts.tv_nsec= _mach_timespec.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif
  }

private:
  struct timespec _begin;
  struct timespec _end;
};

std::ostream& operator<<(std::ostream& output, const libtest::Timer& arg);

} // namespace libtest
