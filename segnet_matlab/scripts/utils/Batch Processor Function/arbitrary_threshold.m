function results = arbitrary_threshold(im)

thresh = [70 110 175];
res = imquantize(im, thresh);
results = uint8(res - 1)*85;
