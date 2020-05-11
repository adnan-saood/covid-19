
labelFolderTrain = '../results/multi/im_test';
labelFoldersegout = '../results/multi/SegNET';
labelFoldermotsu = '../results/multi/multiOTSU';
labelFolderlabel = '../results/multi/label_test';
labelFoldersimple = '../results/multi/arbitrary'

classes = ["C0" "C1" "C2" "C3"];
labels  = [  0  85 170 255];


p1 = pixelLabelDatastore(labelFolderlabel,classes,labels);


p{1} = pixelLabelDatastore(labelFoldermotsu,classes,labels);
p{2} = pixelLabelDatastore(labelFoldersimple,classes,labels);
p{3} = pixelLabelDatastore(labelFoldersegout,classes,labels);


m{1} = evaluateSemanticSegmentation(p{1},p1);
m{2} = evaluateSemanticSegmentation(p{2},p1);
m{3} = evaluateSemanticSegmentation(p{3},p1);


con{1} = m{1}.ConfusionMatrix.Variables;
con{2} = m{2}.ConfusionMatrix.Variables;
con{3} = m{3}.ConfusionMatrix.Variables;

%% Calculate TP, FN, FP, TN.
for method = 1:3
    for cls = 1:4

       TP(method,cls) = con{method}(cls,cls);
       FN(method,cls) = sum(con{method}(cls,:)) - TP(method,cls);
       FP(method,cls) = sum(con{method}(:,cls)) - TP(method,cls);
       TN(method,cls) = sum(sum(con{method})) - TP(method,cls) - FP(method,cls) - FN(method,cls);
    end
end

for method = 1:3
    for cls = 1:4
        acc(method,cls)=  (TN(method,cls) + TP(method,cls))/(TN(method,cls)+TP(method,cls)+FN(method,cls)+FP(method,cls));
        sens(method,cls) = TP(method,cls)/(TP(method,cls)+FN(method,cls));
        iou(method,cls) = TP(method,cls)/(TP(method,cls)+FN(method,cls) + FP(method,cls));
        spec(method,cls) = TN(method,cls)/(TN(method,cls) + FP(method,cls));
        dice_(method,cls) = (2*TP(method,cls))/(2*TP(method,cls) + FP(method,cls) + FN(method,cls));
        prec(method,cls) = TP(method,cls)/(TP(method,cls)+FP(method,cls));
        
    end
    subplot(3,1,method);
    h = heatmap([sens(method,:); iou(method,:); dice_(method,:); prec(method,:)]);
    colormap hot
    h.YDisplayLabels = {'Sens','IoU', 'Dice', 'Prec'};
    h.XDisplayLabels = {'C0','C1','C2','C3'};
end

