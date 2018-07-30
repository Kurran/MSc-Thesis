
#ifndef _MICROSTATE_ENUMERATOR_H_
#define _MICROSTATE_ENUMERATOR_H_

#include <stdio.h>
#include <vector>
#include <cmath>
#include <cstdint>

class microstate_enumerator
{
public:
  microstate_enumerator(const int buckets, const int particles);

  void reset();
  void next();
  int  energy();
  bool last();
  void print_configuration();
  int  degerancy();

  uint64_t expected_microstates();

  int get_uppermost_energy();
  int get_fermi_energy();

private:

  const int _buckets;
  const int _particles;

  int _fermi_energy;
  int _uppermost_energy;

  std::vector<int> _bucket_population;
  std::vector<int> _microconfiguration;

  bool _last_bucket_state;
  bool _last_microsubstate;
  bool _final_microstate;

  uint64_t _state_counter;
  uint64_t _iteration_counter;

  void set_ground_state();
  void set_uppermost_state();

  void next_bucket_population();
  void next_microsubstate();
};

#endif
