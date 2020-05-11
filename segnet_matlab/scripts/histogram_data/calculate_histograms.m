path = '../../dataset/im/512';
imds = imageDatastore(path);
acc_hist = zeros(256,1);
surf_hist = zeros(256,1);
[X_h,Y_h] = meshgrid(256,100);
for i = 1:100
    fprintf("Calculating Here: %d\n", i);
    img = imds.readimage(i);
    hist = imhist(img); hist(1) = 0;
    surf_hist(:,i) = hist';
    acc_hist = acc_hist + hist;
end

