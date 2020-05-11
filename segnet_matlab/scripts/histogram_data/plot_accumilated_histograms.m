function plot_accumilated_histograms

calculate_histograms;

figure;
colormap(hot);

% Create axes
axes1 = axes;
hold(axes1,'on');

% Create surf
surf(surf_hist,'BackFaceLighting','lit','EdgeLighting','flat','LineWidth',1,...
    'FaceColor','interp',...
    'EdgeColor','interp');

view(axes1,[90.0 75]);
axis(axes1,'tight');
end