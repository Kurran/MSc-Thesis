

#include "three_level_configurations.h"
#include "microstate_enumerator.h"


#include <stdio.h>
#include <vector>
#include <cmath>
#include <cstdint>


int main(void)
{
  const int total_particles = 10;
  const int total_buckets = 5;

  microstate_enumerator m_state(total_buckets, total_particles);

  printf("Fermi energy is : %d \n", m_state.get_fermi_energy());
  printf("Max   energy is : %d \n", m_state.get_uppermost_energy());

  std::vector<int> results( m_state.get_uppermost_energy() );
  for(auto& r : results)
    r = 0;

  int count = 0;

  uint64_t expected_states = m_state.expected_microstates();

  printf("Expected states %I64d \n",expected_states );
  printf("Tick size %d \n\n", 0x001FFFFF );

  uint64_t n;

  for(n = 0; n < 48000000; ++n)
  {
    if(m_state.last())
    {
      break;
    }

    results.at( m_state.energy() - 1) += m_state.degerancy();
    count+= m_state.degerancy();
    m_state.next();
  }

  printf("n %I64d\n", n);

  int energy = 1;
  double partition_function = 0.0;
  const double k_B_temperature = 0.5 * m_state.get_fermi_energy();


  
  FILE* fp;
  fopen_s(&fp, "results.dat", "w");

  for(const auto& n_states : results)
  {
    double probability = 0.0;
    if(n_states > 0)
    {
      probability += exp( -1.0 * static_cast<double>(energy) / k_B_temperature );
      probability *= n_states;
    }
    partition_function += probability;
    printf("%5d %12d %12.6f  %12.4f  %12.2f\n", energy, n_states, exp( -1.0 * static_cast<double>(energy) / k_B_temperature ),
      probability, partition_function);

    fprintf(fp, "%5d %12d %12.6f  %12.4f  %12.2f\n", energy, n_states, exp( -1.0 * static_cast<double>(energy) / k_B_temperature ),
        probability, partition_function);

    energy++;
  }

  fclose(fp);

  printf("\n\n");
  printf("%d \n", count);
  printf("%I64u \n", m_state.expected_microstates());

  for(int i=1; i<=30;i++)
  {
     printf("%d : %d\n", i, i*i);
     if(i%3 == 0)
     {
       printf("\n");
     }
  }

  return 0;

}
