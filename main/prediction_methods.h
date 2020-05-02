#pragma once

#define PRED_GLOBAL_THRESH 0
#define PRED_GLOBAL_OTSU 1
#define PRED_MULTI_OTSU 2

#define PRED_RES_PATH "G:\\"

#define HIST_IMG_FOLDER "histo_folder"
#define HIST_IMG_NAME "histograms_0z"

#define THRESH_IMG_FOLDER "thresholded_folder"
#define THRESH_IMG_NAME "thresh_0z"

#define MASKED_LUNGS_IMGS_FOLDER "m"
#define MASKED_LUNGS_IMGS_NAME "i"

#define DEBUG 1

#define MASK_CH 2
#define PRED_CH 0


#define TP (0, 0)
#define FN (1, 0)
#define FP (0, 1)
#define TN (1, 1)