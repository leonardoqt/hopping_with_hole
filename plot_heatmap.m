close all;
clear all;
num = 2000;
data = dlmread('heatmap.dat');
sz = size(data,1)/num;
data = permute(reshape(permute(data,[2,1]),[3,sz,num]),[2,1,3]);
colors = [0.1,0.1,0.8;1,0,0;0,0,0;0.9,0.9,0.1];
colormap(colors);
set(gcf, 'Position',  [100, 100, 1000, 1000]);
for t1=1:num
	pp = scatter(data(:,1,t1),data(:,2,t1),300,data(:,3,t1),'filled');
	F(t1) = getframe(gcf);
	t1
	pause(0.03)
end
% make avi
writerObj = VideoWriter('myVideo.avi');
writerObj.FrameRate = 20;
open(writerObj);
for t1=1:num
	frame = F(t1) ;
	writeVideo(writerObj, frame);
	t1
end
close(writerObj);

%{ 3d
close all;
clear all;
num = 2000;
data = dlmread('heatmap.dat');
sz = size(data,1)/num;
data = permute(reshape(permute(data,[2,1]),[4,sz,num]),[2,1,3]);
colors = [0.1,0.1,0.8;1,0,0;0,0,0;0.9,1,0.9];
colormap(colors);
set(gcf, 'Position',  [100, 100, 1000, 1000]);
for t1=1:num
	pp = scatter3(data(:,1,t1),data(:,2,t1),data(:,3,t1),70,data(:,4,t1),'filled','markerfacealpha',0.4);
	pause(0.2)
end
%}
