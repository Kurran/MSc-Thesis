
#include "three_level_configurations.h"

#include "microstate_enumerator.h"

#include <stdio.h>
#include <vector>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>


microstate_enumerator::microstate_enumerator(const int buckets, const int particles)
:_buckets(buckets)
,_particles(particles)
{
  if(particles > (three_level_system::kCapacity*buckets))
  {
     throw std::invalid_argument("Error: Invalid configuration");
  }

  set_uppermost_state();
  _uppermost_energy = energy();

  set_ground_state();
  _fermi_energy = energy();
}

int microstate_enumerator::get_uppermost_energy()
{
  return _uppermost_energy;
}

int microstate_enumerator::get_fermi_energy()
{
  return _fermi_energy;
}

void microstate_enumerator::set_ground_state()
{
  _bucket_population.resize(0);
  _microconfiguration.resize(0);

  for (int i=0; i < _buckets; ++i )
  {
    _bucket_population.push_back(0);
    _microconfiguration.push_back(0);
  }

  int particles_remaining = _particles;
  int bucket = 0;

  while(particles_remaining > 0)
  {
    if(particles_remaining >= 6)
    {
      _bucket_population[bucket] = 6;
      particles_remaining -=6;
    }
    else
    {
      _bucket_population[bucket] = particles_remaining;
      particles_remaining = 0;
    }
    bucket++;
  }

  _last_bucket_state  = false;
  _last_microsubstate = false;
  _final_microstate   = false;
  _state_counter = 1;
  _iteration_counter = 0;

}

void microstate_enumerator::set_uppermost_state()
{
  _bucket_population.resize(0);
  _microconfiguration.resize(0);

  for (int i=0; i < _buckets; ++i )
  {
    _bucket_population.push_back(0);
    _microconfiguration.push_back(0);
  }

  int particles_remaining = _particles;
  int bucket = _buckets - 1;

  while(particles_remaining > 0)
  {
    if(particles_remaining >= 6)
    {
      _bucket_population[bucket] = 6;
      particles_remaining -=6;
    }
    else
    {
      _bucket_population[bucket] = particles_remaining;
      _microconfiguration[bucket] = three_level_system::configurations.at(particles_remaining).size()-1;
      particles_remaining = 0;
    }
    bucket--;
  }

  _last_bucket_state  = true;
  _last_microsubstate = true;
  _final_microstate   = true;
  _state_counter = expected_microstates();
  _iteration_counter = 0;

}

void microstate_enumerator::next()
{
  if(_final_microstate)
  {
    printf("Final microstate acheived\n");
    return;
  }

  next_microsubstate();

  if(_last_microsubstate)
  {
    next_bucket_population();
    _final_microstate  = _last_bucket_state && _last_microsubstate;
    _last_microsubstate = false;
  }

  _state_counter += degerancy();
  _iteration_counter++;

  //if((_iteration_counter % 131072UL) == 0UL)
  if((_iteration_counter & 0x000000000001FFFF) == 0UL)
  {
    printf("Progress %.1f %%               \r", 100.0f * static_cast<float>(_state_counter) / static_cast<float>(expected_microstates()));
  }

}


int microstate_enumerator::degerancy()
{
  int degenerate_microconfigs = 1;

  for(int n=0; n < _buckets; ++n)
  {
    int p = _bucket_population.at(n);
    int q = _microconfiguration.at(n);
    degenerate_microconfigs *= three_level_system::degerancies.at(p).at(q);
  }

  return degenerate_microconfigs;
}

bool microstate_enumerator::last()
{
  return _final_microstate;
}


uint64_t  microstate_enumerator::expected_microstates()
{
  //printf("\nTotal micro states : %lu \n", it);
  int vacancies = three_level_system::kCapacity * _buckets;
  //printf("Number of levels     : %d \n", (3*total_buckets));
  //printf("Number of vacancies  : %d \n", vacancies);
  //printf("Number of particles  : %d \n", total_particles);
  // Total microstates = vacancies! / particles!*(vacancies-particles)!
  // Assume there are fewer particles than vacancies
  int holes = vacancies - _particles;
  uint64_t expected_microstates = 1;

  for(int m = (holes + 1); m <= vacancies; ++m )
  {
    expected_microstates *= m;
  }

  for(int m = 2; m <= _particles; ++m )
  {
    expected_microstates /= m;
  }

  return expected_microstates;
}

void microstate_enumerator::print_configuration()
{
  for(const auto& p : _bucket_population)
  {
    printf("  %2d", p);
  }
  printf("\n");
  for(const auto& p : _microconfiguration)
  {
    printf("  %2d", p);
  }
  printf("\n");
}


int microstate_enumerator::energy()
{
  int level  = 1;
  int energy = 0;

  for(int n=0; n < _buckets; ++n)
  {
    const int bp  = _bucket_population.at(n);
    const int cfg = _microconfiguration.at(n);

    for(const auto& level_population : three_level_system::configurations.at(bp).at(cfg))
    {
      energy += (level*level*level_population);
      level++;
    }
  }

  return energy;
}


void microstate_enumerator::next_bucket_population()
{
  const int last_bucket = _bucket_population.size() - 1;

  if(_last_bucket_state)
  {
    printf("microstate_enumerator::next_bucket_population() Last state!\n");
    return;
  }

  if(_bucket_population[last_bucket] == 0)
  {
    // Last bucket is empty
    //printf("microstate_enumerator::next_bucket_population() Last bucket empty!\n");
    for(int i = last_bucket-1; i >= 0;  --i)
    {
      if(_bucket_population[i] != 0)
      {
        _bucket_population[i]--;
        _bucket_population[i+1] = 1;
        break;
      }
    }
  }
  else
  {
    // The last bucket is non-empty
    int particles = _bucket_population[last_bucket];
    bool backed_up = _bucket_population[last_bucket] == three_level_system::kCapacity ? true : false;

    for(int i = last_bucket-1; i >= 0;  --i)
    {

      if(backed_up)
      {
        if(i==0)
        {
          printf("\n\nFinal bucket distribution acheived\n\n");
          _last_bucket_state = true;
        }

        if(_bucket_population[i] != three_level_system::kCapacity)
        {
          backed_up = false;
        }

        particles += _bucket_population[i];
      }
      else
      {
        if(_bucket_population[i] == 0)
        {
          if(i==0)
          {
            printf("\n\nFinal distribution acheived\n\n");
            _last_bucket_state = true;
            printf("_state_counter      %I64u\n",_state_counter);
            printf("_iteration_counter  %I64u\n",_iteration_counter);
          }
        }
        else
        {
          _bucket_population[i]--;
          particles++;
          for(int j = i+1; j<=last_bucket; ++j )
          {
            if(particles >= three_level_system::kCapacity )
            {
              _bucket_population[j] = three_level_system::kCapacity;
              particles -= three_level_system::kCapacity;
            }
            else
            {
              _bucket_population[j] = particles;
              particles = 0;
            }
          }
          break;
        }
      }
    }
  }
}


void microstate_enumerator::next_microsubstate()
{
  const int last_bucket = _bucket_population.size() - 1;
  int i;

  _last_microsubstate = true;

  for(i = last_bucket; i >= 0; --i)
  {
    int last_microstate_index = three_level_system::microstates.at(_bucket_population.at(i));

    last_microstate_index--;

    if(_microconfiguration.at(i) < last_microstate_index)
    {
      _microconfiguration.at(i) += 1;
      _last_microsubstate = false;
      break;
    }
  }

  i++;

  // Zero out subsequent microstate indices
  // Iterate forwards find config to increment

  for(int j = i; j <= last_bucket;  ++j)
  {
    _microconfiguration.at(j) = 0;
  }

}
