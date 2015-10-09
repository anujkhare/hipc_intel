% estimate MSE of a clustered data
%
% author: saptarshi.das@shell.com

clear;

% Input filenames
%file_names = ['clabels_461809247.csv'; 'clabels_1345528129.csv';
%'clabels_1877050532.csv'; 'clabels_1219643527.csv';
%'clabels_245056688.csv'; 'clabels_531121941.csv';
%'clabels_505640370.csv'; 'clabels_1596330460.csv';
 %'clabels_14612430.csv']

cFName = 'kcm_phase1.csv';		% cluster file name
dFName = 'data.csv';	% Input Dataset

% read data, we will read each record in a column. This will speed
% up processing as MATLAB stores data in column major format.
data = csvread(dFName)';
clus = csvread(cFName);
[M, N] = size(data);
disp(cFName);
if N ~= size(clus, 2)
  error('dimension missmatch');
end

NSam = N;
samp = randi(N, NSam, 1);

% find unique clusters
clusIdx = unique(clus);
NClus   = length(clusIdx);

% if indices begin with 0 increment all by one
if (clusIdx(1)==0)
  clusIdx = clusIdx + 1;
  clus    = clus + 1;
endif

% Allocate space for cluster centroid
cent     = zeros(M, NClus);
clusSize = zeros(NClus, 1);

% find centroid just by avaraging over each class
for i = 1 : NSam 
  iClus = clus(i);
  cent(:, iClus)  = cent(:, iClus) + data(:, i); 
  clusSize(iClus) = clusSize(iClus) + 1;
end
cent = cent * diag(1 ./ clusSize);

% find MSE 
mse = 0; 
for i = 1 : NSam
  iClus = clus(i);
  diffV = data(:, i) - cent(:, iClus);
  mse = mse + sumsq(diffV);
end

mse = mse / NSam;

disp(mse);

