%% *Universal stuff*

blur_ = true;
imSize = '128';

classes = 2; %numer of classes
%classes = 4;

MiniBatchSize = 8;
InitialLearnRate = 0.001;
MaxEpochs = 200;


train_folder = ['..\dataset\im\' imSize];
label_folder = ['..\dataset\mask\binary\' imSize];


save_results_as_images = 0;
result_folder = './';
%% imds
imds = imageDatastore(train_folder);

%% Load classes
if(classes == 2)
    classes = ["regular" "irregular"];
    labels  = [0   255];
elseif (classes == 4)
    classes = ["C0" "C1" "C2" "C3"];
    labels  = [0 85 170 255];
end

pxds = pixelLabelDatastore(label_folder,classes,labels);

%% Dataset analysis

counts = countEachLabel(pxds);


%% Prepare Training and test data partitions
[imdsTrain,imdsVal,imdsTest,pxdsTrain,pxdsVal,pxdsTest] = partitionData(imds,pxds);



%% Network Creation

im_Size = [str2double(imSize) str2double(imSize) 1];
N = numel(classes);
net_graph = segnetLayers(im_Size,N,3);


%% Balance Classes Using Median Class Weighting

freq_pixels = counts.PixelCount ./ counts.ImagePixelCount;
W = median(freq_pixels) ./ freq_pixels;


%% Mod. the class weights

pxLayer = pixelClassificationLayer('Name','labels','Classes',counts.Name,'ClassWeights',W)
net_graph = removeLayers(net_graph,'pixelLabels');
net_graph = addLayers(net_graph, pxLayer);
net_graph = connectLayers(net_graph,'softmax','labels');

%%
pximdsVal = pixelLabelImageDatastore(imdsVal,pxdsVal);


%% Training Options

chkpointDir = ['net' imSize '_'...
    num2str(MiniBatchSize) ...
    '_' ...
    num2str(InitialLearnRate) ...
    '_' ...
    num2str(MaxEpochs) ...
    date...
    ];
system(join(['mkdir ' chkpointDir]));
options = trainingOptions('adam', ...
    'LearnRateSchedule','piecewise', ...
    'LearnRateDropFactor',0.95, ...
    'LearnRateDropPeriod',12,...
    'Plots', 'training-progress', ...
    'InitialLearnRate',InitialLearnRate, ...
    'L2Regularization',0.0001, ...
    'MaxEpochs',MaxEpochs, ...
    'MiniBatchSize',MiniBatchSize, ...
    'ValidationData',pximdsVal,...
    'ValidationFrequency', 10,...
    'ValidationData',pximdsVal,...
     'ValidationFrequency', 50,...
    'Shuffle','every-epoch', ...
    'VerboseFrequency',2,...
    'CheckpointPath', chkpointDir);

%% Data Augmentation

augmenter = imageDataAugmenter('RandYReflection',true,...
    'RandXShear', [-10 10], 'RandYShear', [-10 10]);

%% Config inputs to trainer
pximds = pixelLabelImageDatastore(imdsTrain,pxdsTrain);%,'DataAugmentation',augmenter);
%% Train the fucktard

[net, train_info] = trainNetwork(pximds,net_graph,options);


%% Testing


pxdsResults = semanticseg(imdsTest,net, ...
    'MiniBatchSize',4, ...
    'WriteLocation',tempdir, ...
    'Verbose',true);

test_metrics = evaluateSemanticSegmentation(pxdsResults,pxdsTest,'Verbose',false);


%% Plot test images and train images

figure;
for index = 1:numel(imdsTest.Files)
    
    imgTest = imdsTest.readimage(index);
    true_mask = pxdsTest.readimage(index);
    true_mask = (double(true_mask) - 1)*85;
    
    C = semanticseg(imgTest,net);
    B = (uint8(C)-1)*85;
    subplot(numel(imdsTest.Files)/3,3,index);
    imagesc([true_mask B], [0 255])
    axis equal
end

figure;
h = heatmap(test_metrics.NormalizedConfusionMatrix.Variables);
colormap hot
%% Save Images
if (save_results_as_images)
    
    for index = 1:numel(imdsTest.Files)

        imgTest = imdsTest.readimage(index);
        true_mask = pxdsTest.readimage(index);
        true_mask = (double(true_mask) - 1)*255;
        C = semanticseg(imgTest,net);
        B = (uint8(C)-1)*255;


        imwrite(imgTest,[result_folder 'im_test/test_' imdsTest.Files{index}(end-7:end)]);
        imwrite(B, [result_folder 'im_segnet' num2str(classes) 'out/segnet2_' imdsTest.Files{index}(end-7:end)]);
        imwrite(true_mask,[result_folder 'testlabels/test_' pxdsTest.Files{index}(end-14:end)]);
    end
    
end
%%
function [imdsTrain,imdsVal, imdsTest, pxdsTrain,pxdsVal, pxdsTest] = partitionData(imds,pxds)
% Partition CamVid data by randomly selecting 60% of the data for training. The
% rest is used for testing.

% Set initial random state for example reproducibility.
rng(0);
numFiles = numel(imds.Files);
shuffledIndices = randperm(numFiles);

% Use 72% of the images for training.
N_Train = round(0.72 * numFiles);
N_Val = round(0.1 * numFiles);
trainingIdx = shuffledIndices(1:N_Train);

% Use the rest for testing and validation
valIdx = shuffledIndices(N_Train+1:N_Train+N_Val);
testIdx = shuffledIndices(N_Train+N_Val+1:end);

% Create image datastores for training and test.
trainingImages = imds.Files(trainingIdx);
valImages = imds.Files(valIdx);
testImages = imds.Files(testIdx);
imdsTrain = imageDatastore(trainingImages);
imdsVal = imageDatastore(valImages);
imdsTest = imageDatastore(testImages);

% Extract class and label IDs info.
classes = pxds.ClassNames;
labelIDs = [0 255];

% Create pixel label datastores for training and test.
trainingLabels = pxds.Files(trainingIdx);
valLabels = pxds.Files(valIdx);
testLabels = pxds.Files(testIdx);
pxdsTrain = pixelLabelDatastore(trainingLabels, classes, labelIDs);
pxdsVal = pixelLabelDatastore(valLabels, classes, labelIDs);
pxdsTest = pixelLabelDatastore(testLabels, classes, labelIDs);
end