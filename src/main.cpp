#define DEBUG

#include <stdio.h>
#include <iostream>
#include <string.h> /* for string functions like strcmp */
#include <stdlib.h> /* for casting function like atoi */
#include <errno.h>  /* standard linux error codes: https://www.thegeekstuff.com/2010/10/linux-error-codes/ */
#include <stdint.h> /* for portability when dealing with integer data types */

// relevant STB headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// TODO: add a labels.txt which is the classification labels

/* define convenience macros */
#define streq(s1, s2) (!strcmp((s1), (s2)))

using std::string;

int parse_options(int argc, char **argv, string *input, string *output, float *thresh)
{
    /* get provider host and port from command arguments */
    int8_t argv_index_input = -1;
    int8_t argv_index_output = -1;
    int8_t argv_index_thresh = -1;

    // --------------------------------------------------------------------------
    // parse the command arguments

    int8_t argn;
    for (argn = 1; argn < argc; argn++)
    {
        if (streq(argv[argn], "--help") || streq(argv[argn], "-?"))
        {
            printf("wb [options] ...");
            printf("\n  --input    / -i       the file path of the input image");
            printf("\n  --output   / -o      the file path of the input image. default is image path with _wb attached.");
            printf("\n  --thresh   / -t       change histogram thresh. Default 0.0005 (0.05%)");
            printf("\n  --help     / -?       this information\n");

            /* program error exit code */
            /* 11 	EAGAIN 	Try again */
            return EAGAIN;
        }
        else if (streq(argv[argn], "--input") || streq(argv[argn], "-i"))
            argv_index_input = ++argn;
        else if (streq(argv[argn], "--out") || streq(argv[argn], "-o"))
            argv_index_output = ++argn;
        else if (streq(argv[argn], "--thresh") || streq(argv[argn], "-t"))
            argv_index_thresh = ++argn;
        else
        {
            /* print error message */
            printf("Unknown option. Get help: ./wb -?");

            /* program error exit code */
            /* 22 	EINVAL 	Invalid argument */
            return EINVAL;
        }
    }

    // --------------------------------------------------------------------------
    // parse the input image file path

    if (argv_index_input == -1)
    {
        /* print error message */
        printf("no image input path specified. Get help: ./io_demo -?\n");

        /* program error exit code */
        /* 22 	EINVAL 	Invalid argument */
        return EINVAL;
    }
    else
    {
/* printf for documentation purposes only */
#ifdef DEBUG
        printf("image to process: %s\n", argv[argv_index_input]);
#endif
        *input = string(argv[argv_index_input]);
    }

    // --------------------------------------------------------------------------
    // parse the output image write mode option, if given
    if (argv_index_output == -1)
    {
        /* printf for documentation purposes only */
        printf("no output image will be written\n");
        // todo: add wb to the image path here
    }
    else
    {
        *output = string(argv[argv_index_output]);
    }

    // parse the tflite model path option, if given
    if (argv_index_thresh == -1)
    {
        /* printf for documentation purposes only */
#ifdef DEBUG
        printf("no model given\n");
#endif
        *thresh = 0.0005;
    }
    else
    {
        // figure out the type conversion lol
        *thresh = float(argv[argv_index_thresh]);
    }

    return 0;
}

void white_balance(uint8_t *img, int width, int height, int channels, float thresh=0.05)
{
    /* First, run through and create histograms */
    // Allocate histograms
    int r_hist[256] = {};
    int g_hist[256] = {};
    int b_hist[256] = {};

    // Create histograms
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int offset = (channels) * ((width * j) + i);

            int r_px = img[offset];
            int g_px = img[offset + 1];
            int b_px = img[offset + 2];

            r_hist[r_px]++;
            g_hist[g_px]++;
            b_hist[b_px]++;
        }
    }

    // Create cutoff threshold (avoids dust contributing to wb algorithm)
    int threshold = (int)(thresh * width * height);

    // Loop through histograms and search for threshold
    // Loop once per direction
    int r_l = -1;
    int g_l = -1;
    int b_l = -1;
    int r_h = -1;
    int g_h = -1;
    int b_h = -1;

    int r_suml = 0;
    int g_suml = 0;
    int b_suml = 0;
    int r_sumh = 0;
    int g_sumh = 0;
    int b_sumh = 0;

    for (int i = 0; i < 256; i++)
    {
        if (r_suml >= threshold && r_l == -1)
        {
            r_l = i;
        }

        if (g_suml >= threshold && g_l == -1)
        {
            g_l = i;
        }

        if (b_suml >= threshold && b_l == -1)
        {
            b_l = i;
        }

        if (r_sumh >= threshold && r_h == -1)
        {
            r_h = 255 - i;
        }

        if (g_sumh >= threshold && g_h == -1)
        {
            g_h = 255 - i;
        }

        if (b_sumh >= threshold && b_h == -1)
        {
            b_h = 255 - i;
        }

        r_suml += r_hist[i];
        g_suml += g_hist[i];
        b_suml += b_hist[i];

        r_sumh += r_hist[255 - i];
        g_sumh += g_hist[255 - i];
        b_sumh += b_hist[255 - i];

        // Break once we've found them all
        if (r_l > -1 && g_l > -1 && b_l > -1 && r_h > -1 && g_h > -1 && b_h > -1)
        {
            break;
        }
    }

    /* Now generate LUTs from the higher and upper thresholds... */
    // First allocate the luts
    int r_lut[256] = {};
    int g_lut[256] = {};
    int b_lut[256] = {};

    // Generate each lut through interpolation and clamping
    for (int i = 0; i < 256; i++)
    {
        if (i <= r_l)
        {
            r_lut[i] = 0;
        }
        else if (i >= r_h)
        {
            r_lut[i] = 255;
        }
        else
        {
            r_lut[i] = (int)(255 * (i - r_l) / ((float)(r_h - r_l)));
        }

        if (i <= g_l)
        {
            g_lut[i] = 0;
        }
        else if (i >= g_h)
        {
            g_lut[i] = 255;
        }
        else
        {
            g_lut[i] = (int)(255 * (i - g_l) / ((float)(g_h - g_l)));
        }

        if (i <= b_l)
        {
            b_lut[i] = 0;
        }
        else if (i >= b_h)
        {
            b_lut[i] = 255;
        }
        else
        {
            b_lut[i] = (int)(255 * (i - b_l) / ((float)(b_h - b_l)));
        }
    }

    // Now finally, apply the LUT to the image
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            // Memory offset from [0]
            // Row major indexing
            int offset = (channels) * ((width * j) + i);

            img[offset] = r_lut[img[offset]];
            img[offset + 1] = g_lut[img[offset + 1]];
            img[offset + 2] = b_lut[img[offset + 2]];
        }
    }

    // Done!
    return 0;
}

int main(int argc, char **argv)
{
    string img_path;
    string out_path;
    float thresh;
    if (parse_options(argc, argv, &img_path, &out_path, &thresh) == 0)
    {

        int width, height, channels;
        uint8_t *img;

        img = stbi_load(img_path.c_str(), &width, &height, &channels, 0);

        if (img == nullptr)
        {
            printf("error loading image, reason: %s\n", stbi_failure_reason());
            exit(1);
        }

        white_balance(img, width, height, channels);
        stbi_write_png(out_path, width, height, channels, img, width * channels);

        // remember to free the image at the very end
        stbi_image_free(img);
    }

    return 0;
}
