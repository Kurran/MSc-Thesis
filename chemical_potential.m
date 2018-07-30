clear;

T_t = linspace(0.0, 2.0, 21); mu_t = zeros(1,length(T_t));

N = 5000;
x_max = 20;
mu_try = 1.0;
mu_t(1) = mu_try; % We know that mu_tilde is 1 when T=0

function integral = calculate_integral(mu, T, x_max, N)
  x = linspace(0,x_max,N+1);
  a = sqrt(x);
  b = 1 + exp((x-mu)/T);
  y = a./b;
  delta = x_max/N;
  integral = sum(y(2:end));
  integral = integral + (0.5*y(1));
  integral = integral*delta;
end

for k=2:length(T_t)
  increment = 0.5;
  for attempt = 1:30
    % How far is the current estimate from the answer?
    difference = (2/3) - calculate_integral(mu_try, T_t(k), x_max, N);
    if (abs(difference) < 1.0e-5)
      mu_t(k) = mu_try;
      break;
    end
    % Simple binary search algorithm to converge on the answer
    increment = increment / 2;
    if(difference < 0 )
      mu_try = mu_try - increment;
    else
      mu_try = mu_try + increment;
    end
  end
 end

figure;
set(0, 'defaultlinelinewidth', 1.5);
plot(T_t, mu_t, '-b');
axis ([0 2 -2.5 1.2]);
xlabel ('{\fontsize{24}T\prime}');
ylabel ('{\fontsize{24}\mu\prime}');
grid on;


filename = 'chemical_potential_vs_temperature.dat';
fid = fopen (filename, 'w');
for k=1:length(T_t)
  fprintf (fid, '%f    %f\n',T_t(k), mu_t(k));
end
fclose (fid);





%print -dpng chemical_potential_vs_temperature.png
