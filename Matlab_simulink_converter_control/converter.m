% Converter model
% 2022 for ESP Assignment
% Tuomo Lindh
%  for model details see: Juhamatti Korhonen   "Effect of Current Measurement Timing
%  and Antialiasing Filter in a Single-Phase Inverter" 
%
clear all

close all; clc;

c1 = 9.1e-6;
c2 = 2.4e-6;
c3 = 9.3e-6;

rc1 = 19e-3;
rc2 = 41e-3;
rc3 = 23e-3;


l1 = 410e-6;
l2 = 2.8e-6;
l3 = 2.8e-6;

rl1 = 107e-3;
rl2 = 15e-3;
rl3 = 16e-3;

% Create continuous converter model

%states [i1 (t) ; u1 (t) ; i2 (t) ; u2 (t) ; i3 (t) ; u3 (t)]
state_matrix_A = [ -rc1/l1 - rl1/l1, -1/l1, rc1/l1, 0, 0, 0;
	    1/c1, 0, -1/c1, 0, 0, 0;
	    rc1/l2, 1/l2, -rc1/l2 - rc2/l2 - rl2/l2, -1/l2, rc2/l2, 0;
	    0, 0, 1/c2, 0, -1/c2, 0;
	    0, 0, rc2/l3, 1/l3, -rc2/l3 - rc3/l3 - rl3/l3,-1/l3;
	    0, 0, 0, 0, 1/c3, 0];

input_B = [1/l1, 0, 0, 0, 0, 0]';% rc3/l3/10, 1/c3]';


output_voltage_C = [0, 0, 0, 0, 0, 1];
D = 0;

state_model_of_inverter = ss(state_matrix_A, input_B, output_voltage_C,D);

%% Discretize
figure(1);
 step(state_model_of_inverter)

figure(2);
hold off
bode(state_model_of_inverter);
hold on;
Fs=50e3;
h=1/Fs;

% 'tustin' Gives non physical representation

%sysD= c2d(state_model_of_inverter,h,'tustin')  

% impulse invariant discretization
 sysD= c2d(state_model_of_inverter,h,'impulse')

 
% Zero order hold would be correct way also
%sysD= c2d(state_model_of_inverter,h,'zoh')


bode(sysD,'r')

figure; pzmap(sysD);
figure(3);
step(sysD,'r')
hold on;
[Ad,Bd,Cd,Dd,TS] = ssdata(sysD);

%Automatically tuned values
Kp=0.00242147503917996;
Ki=242.147503917996;

simout=sim('control_simulation.slx');


%%
figure
plot(simout.voltage.Time,simout.voltage.Data)
xlabel('Time (s)')
ylabel('Voltage (V)')
hold on

sim_results_zybo=importfile('sim_results_zybo_v2.txt');
time=sim_results_zybo.time;

%figure
plot(time,sim_results_zybo.y);

legend('simulink','Zybo')
xlim([0 0.05])


