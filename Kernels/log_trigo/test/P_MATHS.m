% /* ----------------------------------------------------------------------
%  * Project:      CMSIS DSP Library
%  * Title:        P_MATH.m
%  * Description:  generation of tables + test
%  *
%  * $Date:        March 2025
%  * $Revision:    V.0.0.1
%  *
%  * Target Processor: 
%  * -------------------------------------------------------------------- */
% /*
%  * Copyright (C) 2010-2025 ARM Limited or its affiliates. All rights reserved.
%  *
%  * SPDX-License-Identifier: Apache-2.0
%  *
%  * Licensed under the Apache License, Version 2.0 (the License); you may
%  * not use this file except in compliance with the License.
%  * You may obtain a copy of the License at
%  *
%  * www.apache.org/licenses/LICENSE-2.0
%  *
%  * Unless required by applicable law or agreed to in writing, software
%  * distributed under the License is distributed on an AS IS BASIS, WITHOUT
%  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
%  * See the License for the specific language governing permissions and
%  * limitations under the License.
%  */

clear all; close all; format long;

global create_coef_C_files
global number_of_tests 
global test_type
global debug_plot
global file_path

    create_coef_C_files = 0;        % 0:debug/test performance  1:create the coefficients files 
    %create_coef_C_files = 1;        % 0:debug/test performance  1:create the coefficients files 
    test_type = 1;                  % 0:debug, 1,2 :function tests
    number_of_tests = 10000;        % 10k random tests
    debug_plot = 0;                 % debug plots 
    file_path = '..\fast_math\';    % path to the simulation program

    fprintf(1,'\n------%d test(%d) -------\n', number_of_tests, test_type);
    periods = [16 32 100 200 300 1000]; % must be multiple of 4 for cosines
    %periods = [100 800]; 

    for i=1:length(periods); P = periods(i);
        [a,b,c,d,e,f] = compute_sin_cos(P);     print_result2(P, 'sin    ',a,b,c, 'cos    ',  d,e,f);
    end
    for i=1:length(periods); P = periods(i);
        [a,b,c,d,e,f] = compute_sin_cos_pi(P);  print_result2(P, 'sinPi  ',a,b,c, 'cosPi  ',d,e,f);
    end
    for i=1:length(periods); P = periods(i);
        [a,b,c,d,e,f] = compute_atan_atan2(P);  print_result2(P, 'atan   ',a,b,c, 'atan2  ',d,e,f);
    end
    for i=1:length(periods); P = periods(i);
        [a,b,c,d,e,f] = compute_atanh_tanh(P);  print_result2(P, 'atanh  ',a,b,c, 'tanh   ',d,e,f);
    end
        [a,b,c      ] = compute_invsqrt();      print_result1(0, 'invsqrt',a,b,c);
    for i=1:length(periods); P = periods(i);
        [a,b,c      ] = compute_exp(P);         print_result1(P, 'exp    ',a,b,c);
    end
    for i=1:length(periods); P = periods(i);
        [a,b,c      ] = compute_log(P);         print_result1(P, 'log    ',a,b,c);
    end

    
    
    
%--------------------------------------compute_log2----------------------------------------
function  [errmaxlog, errstdlog, biaslog] = compute_log(Period)
    global number_of_tests 
    global test_type
    
    nfields = 2;
    datasize= Period*nfields * 2; % two tables
    x = zeros(1,Period);         % N+1 items
    D = zeros(1,Period);         % data allocation

    firstHalf = zeros(1,datasize/2);
    secondHalf = zeros(1,datasize/2);        

    % first half of the coefficient table for inpuit |x|<1
    step = (1/Period);
    for i=0:Period-1
        xi = (i*step);
        x0 = log(xi);
        xp09 = log(xi+0.9*step);
        if i == 0
            x0 = log(step/2);
        end
        
        D(i+1) = (xp09 - x0)/0.9;
        x(i+1) = x0;                    % 0.5/0.5 minimizes "error max"
    end
    firstHalf(1: nfields:datasize/2-1) = x;
    firstHalf(2:nfields:datasize/2) = D;       
    
    % second half for |x|>1
    x = 0*x; D = 0*D;
    for i=0:Period
        invxi = (i*step);
        x0 = log(1/invxi);
        x1 = log(1/(invxi+step));

        if i == 0
            x0 = log(1/(step/2));
            D(i+1) = 2*(x1 - x0);
        else
            D(i+1) = (x1 - x0);
        end        
        
        x(i+1) = x0;
    end
    secondHalf(1:nfields:end+1) = x;
    secondHalf(2:nfields:end+1) = D;    
    
    coefs = [firstHalf secondHalf];
    
    if test_type == 0
        x = 4 * [1/number_of_tests:1/number_of_tests:1.2]; x = x(1:number_of_tests);
    end    
    if test_type == 1   % @@@ a low "B" value degrades the perforamnce
        A = 4; B = 0.1; x = B + A * [rand(1,number_of_tests)];
    end
    if test_type == 2
        A = 4; B = 0.1; x = B + A * [rand(1,number_of_tests)];
    end

    y0log = log(x);
    [errmaxlog, errstdlog, biaslog] = print_run_test('log_', 'log', Period, x, y0log, coefs);
end

%--------------------------------------compute_exp ----------------------------------------
function  [errmaxexp, errstdexp, biasexp] = compute_exp(Period)
    global number_of_tests 
    global test_type
       
    nfields = 2;
    datasize= Period*nfields * 2; % two tables
    x = zeros(1,Period);         % N+1 items
    D = zeros(1,Period);         % data allocation

    firstHalf = zeros(1,datasize/2);
    secondHalf = zeros(1,datasize/2);        

    % first half for |x|<1
    step = (2/Period);
    for i=0:Period-1
        xi = (i*step) - 1;
        x0 = exp(xi);
        xp09 = exp(xi+0.9*step);
        
        D(i+1) = (xp09 - x0)/0.9;
        x(i+1) = x0;                    % 0.5/0.5 minimizes "error max"
    end
    firstHalf(1: nfields:datasize/2-1) = x;
    firstHalf(2:nfields:datasize/2) = D;       
    
    % second half for |x|>1
    x = 0*x; D = 0*D; maxfp32=3e38; minfp32=-maxfp32;
    for i=0:Period-1
        invxi = (i*step) - 1;
        x0 = exp(1/invxi);
        x1 = exp(1/(invxi+step));
        
        if i == Period/2-1
            x1 = exp(1/(invxi+step/2));
        end        
        if i == Period/2
            x0 = exp(1/(invxi+step/2));
        end        
        
        x0 = max(minfp32, min(maxfp32, x0));
        x1 = max(minfp32, min(maxfp32, x1));

        D(i+1) = (x1 - x0);
        x(i+1) = x0;
    end
    secondHalf(1:nfields:end) = x;
    secondHalf(2:nfields:end) = D;    
    
    x00 = exp(1);
    d00 = 0;
    coefs = [firstHalf secondHalf x00 d00];
    
    if test_type == 0
        x = 2 * [-1:1/number_of_tests:1]; x = x(1:number_of_tests);
    end    
    if test_type == 1   % @@@ warning: too large values degrades performances
        x = 2 * 2*(-0.5 + rand(1,number_of_tests));
    end
    if test_type == 2
        x = 10 * 2*(-0.5 + rand(1,number_of_tests));
    end

    y0exp = exp(x);
    [errmaxexp, errstdexp, biasexp] = print_run_test('exp_', 'exp', Period, x, y0exp, coefs);
end    

%--------------------------------------compute_invsqrt----------------------------------------
function  [errmaxinvsqrt, errstdinvsqrt, biasinvsqrt] = compute_invsqrt()
    global number_of_tests 
    global test_type
  
    if test_type == 0
        A = 100;
        x = A * [0.001:1/number_of_tests:2];
        x = x(1:number_of_tests);
    end    
    if test_type == 1
        A = 100;
        x = A * rand(1,number_of_tests);
        x = x(1:number_of_tests);
    end
    if test_type == 2
        A = 100;
        x = A * rand(1,number_of_tests);
        x = x(1:number_of_tests);
    end
    coefs = [];
    y0invsqrt = 1./sqrt(x);
    [errmaxinvsqrt, errstdinvsqrt, biasinvsqrt] = print_run_test('invsqrt_', 'invsqrt', 0, x, y0invsqrt, coefs);
end

%--------------------------------------compute_tanh_atanh----------------------------------------
function  [errmaxatanh, errstdatanh, biasatanh, errmaxtanh, errstdtanh, biastanh] = compute_atanh_tanh(Period)
    global number_of_tests 
    global test_type
    global debug_plot
    global file_path
    global create_coef_C_files
    
    iatanh = 1; 
    islope = 2;
    nfields = 2;
    datasize= Period*nfields + 2;
    x = zeros(1,Period);         % N+1 items
    D = zeros(1,Period);         % data allocation

    % for |x|<1
    coefs = zeros(1,datasize/2);
    X1 = [-1 : 2/Period : 1]; F = atan(X1); 
    
    step = (2/Period);
    for i=0:Period
        xi = (i*step) - 1;
        if i == 0
            x0 = atanh(0.1*step -1);
            xm05 = atanh(0.09*step -1);
            xp05 = atanh(0.11*step -1);
        else
            x0 = atanh(xi);
            xm05 = atanh(xi-step/2);
            xp05 = atanh(xi+step/2);
        end

        if i == Period
            x0 = atanh(xi - 0.1*step);
            xm05 = atanh(1 - 0.11*step);
            xp05 = atanh(1 - 0.09*step);
        end        
        
        D(i+1) = (xp05 - xm05);
        mid = 0.5 * (xp05 + xm05);      % weight true and interpolated values
        x(i+1) = 0.5 * x0 + 0.5 * mid;  % 0.5/0.5 minimizes "error max"
    end
    
    coefs(iatanh: nfields:datasize) = x;
    coefs(islope:nfields:datasize) = D;       
    
    if debug_plot
        plot (X1, coefs(iatanh:nfields:end),'*-'); hold on; grid on; 
        plot (X1, coefs(islope:nfields:end),'*-');
    end
    
    if test_type == 0
        A = 0.8;
        x = A * [-1:2/number_of_tests:1];
    end    
    if test_type == 1
        S1 = -0.8; S2 = 0.8;
        x = [S1 : (S2-S1)/number_of_tests : S2];
        x = x(1:number_of_tests);
    end
    if test_type == 2
        S1 = -0.98; S2 = 0.98;
        x = [S1 : (S2-S1)/number_of_tests : S2];
        x = x(1:number_of_tests);
    end
    
    y0atanh = atanh(x);
    [errmaxatanh, errstdatanh, biasatanh] = print_run_test('atanh_', 'atanh', Period, x, y0atanh, coefs);

%----------------------------    
    iatan = 1;  
    islope = 2;
    nfields = 2;
    datasize= Period*nfields * 2; % two tables
    x = zeros(1,Period);         % N+1 items
    D = zeros(1,Period);         % data allocation

    firstHalf = zeros(1,datasize/2);
    secondHalf = zeros(1,datasize/2);        

    % first half for |x|<1
    step = (2/Period);
    for i=0:Period-1
        xi = (i*step) - 1;
        x0 = tanh(xi);
        x1 = tanh(xi + step);
        D(i+1) = (x1 - x0);
        x(i+1) = x0;
    end
    firstHalf(iatan: nfields:datasize/2-1) = x;
    firstHalf(islope:nfields:datasize/2) = D;       
    
    % second half for |x|>1
    x = 0*x; D = 0*D;
    for i=0:Period-1
        invxi = (i*step) - 1;
        x0 = tanh(1/invxi);
        x1 = tanh(1/(invxi+step));

        if i == Period/2 -1
            x1 = -pi/2;
        end        
        
        D(i+1) = (x1 - x0);
        x(i+1) = x0;
    end
    secondHalf(iatan: nfields:end) = x;
    secondHalf(islope:nfields:end) = D;    
    
    x00 = tanh(1); d00 = 0;
    coefs = [firstHalf secondHalf x00 d00];
    
    fcoef = sprintf('tanh_%d.txt',Period);
    fcoefname = strcat(file_path,fcoef);    
    fid = fopen(fcoefname, 'wt');  
    if create_coef_C_files; fwd_print_coef_float(fid, coefs); else fprintf(fid, '%12.10f\n', coefs); end
    fclose(fid);    
     
    if test_type == 0
        A = 5;
        x = A * [-1:2/number_of_tests:1]; x = x(1:number_of_tests);
    end    
    if test_type == 1
        S1 = -0.8; S2 = 0.8;
        x = [S1 : (S2-S1)/number_of_tests : S2]; x = x(1:number_of_tests);
    end
    if test_type == 2
        S1 = -0.98; S2 = 0.98;
        x = [S1 : (S2-S1)/number_of_tests : S2]; x = x(1:number_of_tests);
    end
    
    y0tanh = tanh(x);    
    [errmaxtanh, errstdtanh, biastanh] = print_run_test('tanh_', 'tanh', Period, x, y0tanh, coefs);
end     
    
%--------------------------------------compute_atan_atan2----------------------------------------
function  [errmaxatan, errstdatan, biasatan, errmaxatan2, errstdatan2, biasatan2] = compute_atan_atan2(Period)
    global number_of_tests 
    global test_type
    global file_path
       
    iatan = 1;
    islope = 2;
    nfields = 2;
    datasize= Period*nfields * 2; % two tables
    x = zeros(1,Period);         % N+1 items
    D = zeros(1,Period);         % data allocation

    firstHalf = zeros(1,datasize/2);
    secondHalf = zeros(1,datasize/2);        

    % first half for |x|<1

    X1 = [-1 : 1/Period : 1]; F = atan(X1); 
    %plot (X1,F); grid on;
    
    step = (2/Period);
    for i=0:Period-1
        xi = (i*step) - 1;
        x0 = atan(xi);
        x1 = atan(xi + step);
        D(i+1) = (x1 - x0);
        x(i+1) = x0;
    end
    firstHalf(iatan: nfields:datasize/2-1) = x;
    firstHalf(islope:nfields:datasize/2) = D;       

    
    % second half for |x|>1
    x = 0*x; D = 0*D;
    for i=0:Period-1
        invxi = (i*step) - 1;
        x0 = atan(1/invxi);
        x1 = atan(1/(invxi+step));

        if i == Period/2 -1
            x1 = -pi/2;
        end        
        
        D(i+1) = (x1 - x0);
        x(i+1) = x0;
    end
    secondHalf(iatan: nfields:end) = x;
    secondHalf(islope:nfields:end) = D;    
    
    x00 = pi/4;
    d00 = 0;
    coefs = [firstHalf secondHalf x00 d00];
    
    if test_type == 0
        x = 10 * [1/number_of_tests:1/number_of_tests:1];
        xy = 10 * [1:-1/number_of_tests:1/number_of_tests];
    end    
    if test_type == 1
        A = 100;
        x = A * (-0.5 + rand(1,number_of_tests));
        xy = A * (-0.5 + rand(1,number_of_tests));
    end
    if test_type == 2
        A = 100;
        x = A * (-0.5 + rand(1,number_of_tests));
        xy = A * (-0.5 + rand(1,number_of_tests));
    end
    mxy = zeros(1, 2*number_of_tests); mxy(1:2:end) = xy; mxy(2:2:end) = x;

    y0atan = atan(x);
    [errmaxatan, errstdatan, biasatan] = print_run_test('atan_', 'atan', Period, x, y0atan, coefs);
    
    y0atan2 = atan2(xy, x);    
    fcoef = sprintf('%s%d.txt','atan_', Period); fcoefname = strcat(file_path,fcoef);  
    fid = fopen(strcat(file_path,'rawdata_in.txt'), 'wt'); fprintf(fid, '%12.9f %12.9f\n', mxy);  fclose(fid);
    fid = fopen(strcat(file_path,'rawdata_ref.txt'),'wt'); fprintf(fid, '%12.9f\n', y0atan2);  fclose(fid);
    fexe = sprintf('%s%s', file_path, 'x64\Debug\fast_math.exe');
    doscommand = sprintf('%s atan2 %srawdata_in.txt %srawdata_out.txt %s%s %d 0', ...
                          fexe,  file_path,       file_path,     file_path, fcoefname, Period);
    system(doscommand); 
    fid = fopen(sprintf('%srawdata_out.txt', file_path), 'rt'); yres = fscanf(fid,'%f\n'); fclose(fid);
    diff = y0atan2(1:number_of_tests)'-yres(1:number_of_tests);
    errmaxatan2 =  max(abs(diff)); 
    errstdatan2 =  std(diff);
    biasatan2 = mean(diff);
end    


%------------------------------------compute_sin_cos_pi------------------------------------------
function  [errmaxsin, errstdsin, biassin, errmaxcos, errstdcos, biascos] = compute_sin_cos_pi(Period)
    global number_of_tests 
    global test_type
    global debug_plot
    
    NTABS = round(1.25*Period + 1);    % -pi .. +pi = -1024 .. +1024 + 128 for cosines
    
    isin = 1;
    islope = 2;
    nfields = 2;
    datasize=NTABS*nfields;
    coefs = zeros(1,datasize);
    x = zeros(1,NTABS);         % N+1 items
    D = zeros(1,NTABS);         % data allocation

    
    for i=0:NTABS-1
        xm05 = sin(-pi + pi*(i-0.5)/(0.5*Period));
        x0   = sin(-pi + pi*(i    )/(0.5*Period));
        xp05 = sin(-pi + pi*(i+0.5)/(0.5*Period));
        
        D(i+1) = (xp05 - xm05);
        mid = 0.5 * (xp05 + xm05);      % weight true and interpolated values
        x(i+1) = 0.6 * x0 + 0.4 * mid;  % 0.5/0.5 minimizes "error max"
    end
    coefs(isin:nfields:end)   = x;
    coefs(islope:nfields:end) = D;
    
    if debug_plot
        figure;
        plot (coefs(isin:nfields:end)); hold on; grid on; 
        plot (coefs(islope:nfields:end));
    end
    
    if test_type == 0
        x = -pi + 2*pi*[1:number_of_tests]/number_of_tests;
    end    
    if test_type == 1
        A = 1;
        x = pi * [-A*rand(1,number_of_tests/2) A*rand(1,number_of_tests/2)];
    end
    if test_type == 2
        A = 1;
        x = pi * [-A*rand(1,number_of_tests/2) A*rand(1,number_of_tests/2)];
    end
    
    y0sin = sin(x);
    [errmaxsin, errstdsin, biassin] = print_run_test('sinpi_', 'sinpi', Period, x, y0sin, coefs);
    y0cos = cos(x);    
    [errmaxcos, errstdcos, biascos] = print_run_test('cospi_', 'cospi', Period, x, y0cos, coefs);
end    

%--------------------------------------compute_sin_cos----------------------------------------
function  [errmaxsin, errstdsin, biassin, errmaxcos, errstdcos, biascos] = compute_sin_cos(Period)
    global number_of_tests 
    global test_type
    global debug_plot
    
    NTABS = round(2.25*Period + 1);    % -2pi .. +2pi = -1024 .. +1024 + 128 for cosines
    
    isin = 1;
    islope = 2;
    nfields = 2;
    datasize=NTABS*nfields;
    coefs = zeros(1,datasize);
    x = zeros(1,NTABS);         % N+1 items
    D = zeros(1,NTABS);         % data allocation

    
    for i=0:NTABS-1
        xm05 = sin(2*pi*(i-0.5)/Period);
        x0   = sin(2*pi*(i    )/Period);
        xp05 = sin(2*pi*(i+0.5)/Period);
        
        D(i+1) = (xp05 - xm05);
        mid = 0.5 * (xp05 + xm05);      % weight true and interpolated values
        x(i+1) = 0.6 * x0 + 0.4 * mid;  % 0.5/0.5 minimizes "error max"
    end
    coefs(isin:nfields:end)   = x;
    coefs(islope:nfields:end) = D;
    
    if debug_plot
        figure;
        plot (coefs(isin:nfields:end)); hold on; grid on; 
        plot (coefs(islope:nfields:end));
    end
    
    if test_type == 0
        x = pi/2 + 2*pi*[1:number_of_tests]/number_of_tests;
    end    
    if test_type == 1
        phases = 10 * (rand(number_of_tests,1)-0.5);
        amplitude = 10 * (rand(number_of_tests,1)-0.5);        
        x = amplitude.*sin(2*pi*phases);
        x = x';
    end
    if test_type == 2
        A = 100;
        x = [-A*rand(1,number_of_tests/2) A*rand(1,number_of_tests/2)];
    end
    
    y0sin = sin(x);
    [errmaxsin, errstdsin, biassin] = print_run_test('sin_', 'sin', Period, x, y0sin, coefs);
    y0cos = cos(x);    
    [errmaxcos, errstdcos, biascos] = print_run_test('cos_', 'cos', Period, x, y0cos, coefs);
end    


%-------------------------------------- print a pair of results ----------------------------------------
% error max, standard deviation error, error bias => number of bits 
function  [] = print_result2(P, N,a,b,c,M,d,e,f)
    print_result1(P, N, a,b,c);
    print_result1(P, M, d,e,f);
end

%-------------------------------------- print results ----------------------------------------
% error max, standard deviation error, error bias => number of bits 
function  [] = print_result1(P, N,a,b,c)
    fprintf(1,'%s table%5d error=%6.2e peak=%5.1e bias=%8.1e  (%4.1f signed bits) \n',N, P, b,a,c, 1-log10(b)/log10(2));
end

%-------------------------------------- print debug ----------------------------------------
function  [] = print_debug(T)
    fprintf(1,'%s \n',T);
end

%-------------------------------------- print_run_test -------------------------------------
function  [errmax, errstd, bias] = print_run_test(name, tag, Period, x, Ref, coefs)
    global file_path
    global create_coef_C_files
    global number_of_tests 
    
    fcoef = sprintf('%s%d.txt',name, Period); fcoefname = strcat(file_path,fcoef);    
    fid = fopen(fcoefname, 'wt');  
    if create_coef_C_files; fwd_print_coef_float(fid, coefs); else fprintf(fid, '%12.10f\n', coefs); end
    fclose(fid);
    
    fid = fopen(strcat(file_path,'rawdata_in.txt'), 'wt'); fprintf(fid, '%12.9f\n', x);   fclose(fid);
    fid = fopen(strcat(file_path,'rawdata_ref.txt'),'wt'); fprintf(fid, '%12.9f\n', Ref); fclose(fid);
    
    fexe = sprintf('%s%s', file_path, 'x64\Debug\fast_math.exe');
    doscommand = sprintf('%s %s %srawdata_in.txt %srawdata_out.txt %s%s %d 0', ...
                          fexe, tag, file_path,       file_path,     file_path, fcoef, Period);
                   
    system(doscommand); 
    fid = fopen(sprintf('%srawdata_out.txt', file_path), 'rt'); yres = fscanf(fid,'%f\n'); fclose(fid);
    diff = Ref(1:number_of_tests)'-yres(1:number_of_tests);
    errmax =  max(abs(diff)); 
    errstd =  std(diff);
    bias   = mean(diff);  
end

%-------------------------------------- fwd_print_coef_float -------------------------------------
function [ ] = fwd_print_coef_float( fid, data )

    F = 1;
    if abs(max(data)) > 1000
       F = 2;
    end
    
    j = 1;
    NL = 10;
    S = max(size(data));
    R = mod(S,NL);
    for i=1:floor(S/NL)
        for jn = 1:NL
            if F == 1
                fprintf(fid,' %10.7ff,',data(j));j=j+1; 
            else
                fprintf(fid,' %ef,',data(j));j=j+1; 
            end
        end
        fprintf(fid, ' /* %d */\n', j-1);
        end   
        for i=1:R
            if F == 1
                fprintf(fid,' %10.7ff,',data(j));j=j+1; 
            else
                fprintf(fid,' %ef,',data(j));j=j+1; 
            end
        end         
        fprintf(fid, ' /* %d */\n', j-1);
end

