
labelFolderTrain = '../results/binary/im_test2';
labelFoldersegout = '../results/binary/SegNET2';
labelFolderglobal = '../results/binary/GlobalThresh';
labelFolderotsu = '../results/binary/OTSU';
labelFolderlabel = '../results/binary/label_test2';

classes = ["regular" "irregular"];
labels  = [  0   255];


pLabel = pixelLabelDatastore(labelFolderlabel,classes,labels);

p{1} = pixelLabelDatastore(labelFolderglobal,classes,labels);
p{2} = pixelLabelDatastore(labelFolderotsu, classes, labels);
p{3} = pixelLabelDatastore(labelFoldersegout,classes,labels);

hm = figure;
for i=1:3
    m{i} = evaluateSemanticSegmentation(p{i},pLabel);
    con{i} = m{i}.NormalizedConfusionMatrix.Variables;
    subplot(1,3,i);
    heatmap(con{i});
    colormap hot
end
%%
for i=1:3
    TP(i) = con{i}(2,2);
    TN(i) = con{i}(1,1);
    FP(i) = con{i}(1,2);
    FN(i) = con{i}(2,1);
    acc(i) = (TN(i) + TP(i))/(TN(i)+TP(i)+FN(i)+FP(i));
    sens(i) = TP(i)/(TP(i)+FN(i));
    iou(i) = TP(i)/(TP(i)+FN(i) + FP(i));
    spec(i) = TN(i)/(TN(i) + FP(i));
    dice_(i) = (2*TP(i))/(2*TP(i) + FP(i) + FN(i));
    prec(i) = TP(i)/(TP(i)+FP(i));
end
figure;



bar([acc', sens', iou', dice_'])



