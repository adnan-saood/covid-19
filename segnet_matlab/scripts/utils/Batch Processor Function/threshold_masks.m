function results = threshold_masks(im)

results = uint8(im>2)*255;
