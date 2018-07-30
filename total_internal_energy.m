
% mu_tilde and T_tilde extracted from previous script
clear all;

T_tilde = [0.000000,
0.100000,
0.200000,
0.300000,
0.400000,
0.500000,
0.600000,
0.700000,
0.800000,
0.900000,
1.000000,
1.100000,
1.200000,
1.300000,
1.400000,
1.500000,
1.600000,
1.700000,
1.800000,
1.900000,
2.000000 ];

mu_tilde = [ 1.000000,
0.991699 ,
0.964630 ,
0.914642 ,
0.840393 ,
0.743164 ,
0.624908 ,
0.487671 ,
0.333191 ,
0.163055 ,
-0.021393,
-0.219055,
-0.428894,
-0.650024,
-0.881714,
-1.123352,
-1.374268,
-1.633911,
-1.901917,
-2.177734,
-2.460999];

N = 5000;
x_max = 20;

function integral = calculate_integral(mu_tilde, T_tilde, x_max, N)
  x = linspace(0, x_max, N+1);
  a = power(x,1.5);
  b = 1 + exp((x-mu_tilde)./T_tilde);
  y = a./b;
  delta = x_max/N;
  integral = sum(y(2:end));
  integral = integral + (0.5*y(1));
  integral = integral*delta;
endfunction

% Debug
function integral = plot_function(mu_tilde, T_tilde, x_max, N)
  x = linspace(0, x_max, N+1);
  a = power(x,1.5);
  b = 1 + exp((x.-mu_tilde)./T_tilde);
  y = a./b;
  plot(x,y);
endfunction

if (length(T_tilde) ~= length(mu_tilde))
  error('Temperature and mu are different lengths');
end

internal_energy = zeros(1, length(T_tilde) - 1);

figure;
hold on;

filename = 'total_internal_energy_vs_temperature.dat';
fid = fopen (filename, 'w');
for i=2:length(mu_tilde)
  plot_function(mu_tilde(i), T_tilde(i), x_max, N);
  internal_energy(i-1) = calculate_integral(mu_tilde(i), T_tilde(i), x_max, N);
  fprintf(fid, '%f    %f\n',T_tilde(i), internal_energy(i-1));
endfor;
fclose (fid);
hold off;

figure;
plot(T_tilde(2:end)', internal_energy);
