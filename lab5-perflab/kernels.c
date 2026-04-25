/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "bovik",              /* Team name */

    "Harry Q. Bovik",     /* First member full name */
    "bovik@nowhere.edu",  /* First member email address */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
	        dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i,j;
    for(i=0;i<dim;i+=8){
        for(j=0;j<dim;j+=8){
	        dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
            dst[RIDX(dim-2-j, i, dim)] = src[RIDX(i, j+1, dim)];
            dst[RIDX(dim-3-j, i, dim)] = src[RIDX(i, j+2, dim)];
            dst[RIDX(dim-4-j, i, dim)] = src[RIDX(i, j+3, dim)];
	        dst[RIDX(dim-5-j, i, dim)] = src[RIDX(i, j+4, dim)];
            dst[RIDX(dim-6-j, i, dim)] = src[RIDX(i, j+5, dim)];
            dst[RIDX(dim-7-j, i, dim)] = src[RIDX(i, j+6, dim)];
            dst[RIDX(dim-8-j, i, dim)] = src[RIDX(i, j+7, dim)];

            dst[RIDX(dim-1-j, i+1, dim)] = src[RIDX(i+1, j, dim)];
            dst[RIDX(dim-2-j, i+1, dim)] = src[RIDX(i+1, j+1, dim)];
            dst[RIDX(dim-3-j, i+1, dim)] = src[RIDX(i+1, j+2, dim)];
            dst[RIDX(dim-4-j, i+1, dim)] = src[RIDX(i+1, j+3, dim)];
	        dst[RIDX(dim-5-j, i+1, dim)] = src[RIDX(i+1, j+4, dim)];
            dst[RIDX(dim-6-j, i+1, dim)] = src[RIDX(i+1, j+5, dim)];
            dst[RIDX(dim-7-j, i+1, dim)] = src[RIDX(i+1, j+6, dim)];
            dst[RIDX(dim-8-j, i+1, dim)] = src[RIDX(i+1, j+7, dim)];

            dst[RIDX(dim-1-j, i+2, dim)] = src[RIDX(i+2, j, dim)];
            dst[RIDX(dim-2-j, i+2, dim)] = src[RIDX(i+2, j+1, dim)];
            dst[RIDX(dim-3-j, i+2, dim)] = src[RIDX(i+2, j+2, dim)];
            dst[RIDX(dim-4-j, i+2, dim)] = src[RIDX(i+2, j+3, dim)];
	        dst[RIDX(dim-5-j, i+2, dim)] = src[RIDX(i+2, j+4, dim)];
            dst[RIDX(dim-6-j, i+2, dim)] = src[RIDX(i+2, j+5, dim)];
            dst[RIDX(dim-7-j, i+2, dim)] = src[RIDX(i+2, j+6, dim)];
            dst[RIDX(dim-8-j, i+2, dim)] = src[RIDX(i+2, j+7, dim)];

            dst[RIDX(dim-1-j, i+3, dim)] = src[RIDX(i+3, j, dim)];
            dst[RIDX(dim-2-j, i+3, dim)] = src[RIDX(i+3, j+1, dim)];
            dst[RIDX(dim-3-j, i+3, dim)] = src[RIDX(i+3, j+2, dim)];
            dst[RIDX(dim-4-j, i+3, dim)] = src[RIDX(i+3, j+3, dim)];
	        dst[RIDX(dim-5-j, i+3, dim)] = src[RIDX(i+3, j+4, dim)];
            dst[RIDX(dim-6-j, i+3, dim)] = src[RIDX(i+3, j+5, dim)];
            dst[RIDX(dim-7-j, i+3, dim)] = src[RIDX(i+3, j+6, dim)];
            dst[RIDX(dim-8-j, i+3, dim)] = src[RIDX(i+3, j+7, dim)];

            dst[RIDX(dim-1-j, i+4, dim)] = src[RIDX(i+4, j, dim)];
            dst[RIDX(dim-2-j, i+4, dim)] = src[RIDX(i+4, j+1, dim)];
            dst[RIDX(dim-3-j, i+4, dim)] = src[RIDX(i+4, j+2, dim)];
            dst[RIDX(dim-4-j, i+4, dim)] = src[RIDX(i+4, j+3, dim)];
	        dst[RIDX(dim-5-j, i+4, dim)] = src[RIDX(i+4, j+4, dim)];
            dst[RIDX(dim-6-j, i+4, dim)] = src[RIDX(i+4, j+5, dim)];
            dst[RIDX(dim-7-j, i+4, dim)] = src[RIDX(i+4, j+6, dim)];
            dst[RIDX(dim-8-j, i+4, dim)] = src[RIDX(i+4, j+7, dim)];

            dst[RIDX(dim-1-j, i+5, dim)] = src[RIDX(i+5, j, dim)];
            dst[RIDX(dim-2-j, i+5, dim)] = src[RIDX(i+5, j+1, dim)];
            dst[RIDX(dim-3-j, i+5, dim)] = src[RIDX(i+5, j+2, dim)];
            dst[RIDX(dim-4-j, i+5, dim)] = src[RIDX(i+5, j+3, dim)];
	        dst[RIDX(dim-5-j, i+5, dim)] = src[RIDX(i+5, j+4, dim)];
            dst[RIDX(dim-6-j, i+5, dim)] = src[RIDX(i+5, j+5, dim)];
            dst[RIDX(dim-7-j, i+5, dim)] = src[RIDX(i+5, j+6, dim)];
            dst[RIDX(dim-8-j, i+5, dim)] = src[RIDX(i+5, j+7, dim)];

            dst[RIDX(dim-1-j, i+6, dim)] = src[RIDX(i+6, j, dim)];
            dst[RIDX(dim-2-j, i+6, dim)] = src[RIDX(i+6, j+1, dim)];
            dst[RIDX(dim-3-j, i+6, dim)] = src[RIDX(i+6, j+2, dim)];
            dst[RIDX(dim-4-j, i+6, dim)] = src[RIDX(i+6, j+3, dim)];
	        dst[RIDX(dim-5-j, i+6, dim)] = src[RIDX(i+6, j+4, dim)];
            dst[RIDX(dim-6-j, i+6, dim)] = src[RIDX(i+6, j+5, dim)];
            dst[RIDX(dim-7-j, i+6, dim)] = src[RIDX(i+6, j+6, dim)];
            dst[RIDX(dim-8-j, i+6, dim)] = src[RIDX(i+6, j+7, dim)];

            dst[RIDX(dim-1-j, i+7, dim)] = src[RIDX(i+7, j, dim)];
            dst[RIDX(dim-2-j, i+7, dim)] = src[RIDX(i+7, j+1, dim)];
            dst[RIDX(dim-3-j, i+7, dim)] = src[RIDX(i+7, j+2, dim)];
            dst[RIDX(dim-4-j, i+7, dim)] = src[RIDX(i+7, j+3, dim)];
	        dst[RIDX(dim-5-j, i+7, dim)] = src[RIDX(i+7, j+4, dim)];
            dst[RIDX(dim-6-j, i+7, dim)] = src[RIDX(i+7, j+5, dim)];
            dst[RIDX(dim-7-j, i+7, dim)] = src[RIDX(i+7, j+6, dim)];
            dst[RIDX(dim-8-j, i+7, dim)] = src[RIDX(i+7, j+7, dim)];
        }
    }
}

char rotate_cut_addr_cal_descr[] = "rotate_cut_addr_cal version";
void rotate_cut_addr_cal(int dim, pixel *src, pixel *dst) 
{
    int i,j;
    for(i=0;i<dim;i+=8){
        int src_base_addr0=i*dim;
        int src_base_addr1=(i+1)*dim;
        int src_base_addr2=(i+2)*dim;
        int src_base_addr3=(i+3)*dim;
        int src_base_addr4=(i+4)*dim;
        int src_base_addr5=(i+5)*dim;
        int src_base_addr6=(i+6)*dim;
        int src_base_addr7=(i+7)*dim;       

        for(j=0;j<dim;j+=8){
	        dst[RIDX(dim-1-j, i, dim)] = src[src_base_addr0+j];
            dst[RIDX(dim-2-j, i, dim)] = src[src_base_addr0+j+1];
            dst[RIDX(dim-3-j, i, dim)] = src[src_base_addr0+j+2];
            dst[RIDX(dim-4-j, i, dim)] = src[src_base_addr0+j+3];
	        dst[RIDX(dim-5-j, i, dim)] = src[src_base_addr0+j+4];
            dst[RIDX(dim-6-j, i, dim)] = src[src_base_addr0+j+5];
            dst[RIDX(dim-7-j, i, dim)] = src[src_base_addr0+j+6];
            dst[RIDX(dim-8-j, i, dim)] = src[src_base_addr0+j+7];

	        dst[RIDX(dim-1-j, i+1, dim)] = src[src_base_addr1+j];
            dst[RIDX(dim-2-j, i+1, dim)] = src[src_base_addr1+j+1];
            dst[RIDX(dim-3-j, i+1, dim)] = src[src_base_addr1+j+2];
            dst[RIDX(dim-4-j, i+1, dim)] = src[src_base_addr1+j+3];
	        dst[RIDX(dim-5-j, i+1, dim)] = src[src_base_addr1+j+4];
            dst[RIDX(dim-6-j, i+1, dim)] = src[src_base_addr1+j+5];
            dst[RIDX(dim-7-j, i+1, dim)] = src[src_base_addr1+j+6];
            dst[RIDX(dim-8-j, i+1, dim)] = src[src_base_addr1+j+7];

            dst[RIDX(dim-1-j, i+2, dim)] = src[src_base_addr2+j];
            dst[RIDX(dim-2-j, i+2, dim)] = src[src_base_addr2+j+1];
            dst[RIDX(dim-3-j, i+2, dim)] = src[src_base_addr2+j+2];
            dst[RIDX(dim-4-j, i+2, dim)] = src[src_base_addr2+j+3];
	        dst[RIDX(dim-5-j, i+2, dim)] = src[src_base_addr2+j+4];
            dst[RIDX(dim-6-j, i+2, dim)] = src[src_base_addr2+j+5];
            dst[RIDX(dim-7-j, i+2, dim)] = src[src_base_addr2+j+6];
            dst[RIDX(dim-8-j, i+2, dim)] = src[src_base_addr2+j+7];

            dst[RIDX(dim-1-j, i+3, dim)] = src[src_base_addr3+j];
            dst[RIDX(dim-2-j, i+3, dim)] = src[src_base_addr3+j+1];
            dst[RIDX(dim-3-j, i+3, dim)] = src[src_base_addr3+j+2];
            dst[RIDX(dim-4-j, i+3, dim)] = src[src_base_addr3+j+3];
	        dst[RIDX(dim-5-j, i+3, dim)] = src[src_base_addr3+j+4];
            dst[RIDX(dim-6-j, i+3, dim)] = src[src_base_addr3+j+5];
            dst[RIDX(dim-7-j, i+3, dim)] = src[src_base_addr3+j+6];
            dst[RIDX(dim-8-j, i+3, dim)] = src[src_base_addr3+j+7];

            dst[RIDX(dim-1-j, i+4, dim)] = src[src_base_addr4+j];
            dst[RIDX(dim-2-j, i+4, dim)] = src[src_base_addr4+j+1];
            dst[RIDX(dim-3-j, i+4, dim)] = src[src_base_addr4+j+2];
            dst[RIDX(dim-4-j, i+4, dim)] = src[src_base_addr4+j+3];
	        dst[RIDX(dim-5-j, i+4, dim)] = src[src_base_addr4+j+4];
            dst[RIDX(dim-6-j, i+4, dim)] = src[src_base_addr4+j+5];
            dst[RIDX(dim-7-j, i+4, dim)] = src[src_base_addr4+j+6];
            dst[RIDX(dim-8-j, i+4, dim)] = src[src_base_addr4+j+7];

            dst[RIDX(dim-1-j, i+5, dim)] = src[src_base_addr5+j];
            dst[RIDX(dim-2-j, i+5, dim)] = src[src_base_addr5+j+1];
            dst[RIDX(dim-3-j, i+5, dim)] = src[src_base_addr5+j+2];
            dst[RIDX(dim-4-j, i+5, dim)] = src[src_base_addr5+j+3];
	        dst[RIDX(dim-5-j, i+5, dim)] = src[src_base_addr5+j+4];
            dst[RIDX(dim-6-j, i+5, dim)] = src[src_base_addr5+j+5];
            dst[RIDX(dim-7-j, i+5, dim)] = src[src_base_addr5+j+6];
            dst[RIDX(dim-8-j, i+5, dim)] = src[src_base_addr5+j+7];

            dst[RIDX(dim-1-j, i+6, dim)] = src[src_base_addr6+j];
            dst[RIDX(dim-2-j, i+6, dim)] = src[src_base_addr6+j+1];
            dst[RIDX(dim-3-j, i+6, dim)] = src[src_base_addr6+j+2];
            dst[RIDX(dim-4-j, i+6, dim)] = src[src_base_addr6+j+3];
	        dst[RIDX(dim-5-j, i+6, dim)] = src[src_base_addr6+j+4];
            dst[RIDX(dim-6-j, i+6, dim)] = src[src_base_addr6+j+5];
            dst[RIDX(dim-7-j, i+6, dim)] = src[src_base_addr6+j+6];
            dst[RIDX(dim-8-j, i+6, dim)] = src[src_base_addr6+j+7];

            dst[RIDX(dim-1-j, i+7, dim)] = src[src_base_addr7+j];
            dst[RIDX(dim-2-j, i+7, dim)] = src[src_base_addr7+j+1];
            dst[RIDX(dim-3-j, i+7, dim)] = src[src_base_addr7+j+2];
            dst[RIDX(dim-4-j, i+7, dim)] = src[src_base_addr7+j+3];
	        dst[RIDX(dim-5-j, i+7, dim)] = src[src_base_addr7+j+4];
            dst[RIDX(dim-6-j, i+7, dim)] = src[src_base_addr7+j+5];
            dst[RIDX(dim-7-j, i+7, dim)] = src[src_base_addr7+j+6];
            dst[RIDX(dim-8-j, i+7, dim)] = src[src_base_addr7+j+7];
        }
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);  
    add_rotate_function(&rotate, rotate_descr); 
    add_rotate_function(&rotate_cut_addr_cal,rotate_cut_addr_cal_descr);
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++)
	        dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version,";
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++){
            pixel_sum sum;
            sum.red = sum.green = sum.blue = 0;

            int ii_down=max(i-1, 0);
            int ii_up=min(i+1, dim-1);
            int jj_down=max(j-1, 0);
            int jj_up=min(j+1, dim-1);
            sum.num=(ii_up-ii_down+1)*(jj_up-jj_down+1); 

            for(int ii = ii_down; ii <= ii_up; ii++){
                for(int jj = jj_down; jj <= jj_up; jj++){ 
                    sum.red += src[RIDX(ii, jj, dim)].red;
                    sum.green += src[RIDX(ii, jj, dim)].green;
                    sum.blue += src[RIDX(ii, jj, dim)].blue;      
                }
            }
            dst[RIDX(i, j, dim)].red = (unsigned short) (sum.red/sum.num);
            dst[RIDX(i, j, dim)].green = (unsigned short) (sum.green/sum.num);
            dst[RIDX(i, j, dim)].blue = (unsigned short) (sum.blue/sum.num);
    }      
}

char smooth1_descr[] = "smooth1 version,";
void smooth1(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 0; i < dim; i++)
	    for (j = 0; j < dim; j++){

            int ii_down=i-1>0?i-1:0;
            int ii_up=i+1<dim-1?i+1:dim-1;
            int jj_down=j-1>0?j-1:0;
            int jj_up=j+1<dim-1?j+1:dim-1;

            if(ii_up-ii_down==2){
                if(jj_up-jj_down==2){
                    dst[RIDX(i, j, dim)].red=(src[RIDX(ii_down,jj_down,dim)].red+src[RIDX(ii_down,jj_down+1,dim)].red+src[RIDX(ii_down,jj_up,dim)].red+src[RIDX(ii_down+1,jj_down,dim)].red+src[RIDX(ii_down+1,jj_down+1,dim)].red+src[RIDX(ii_down+1,jj_up,dim)].red+src[RIDX(ii_up,jj_down,dim)].red+src[RIDX(ii_up,jj_down+1,dim)].red+src[RIDX(ii_up,jj_up,dim)].red)/9;

                    dst[RIDX(i, j, dim)].green=(src[RIDX(ii_down,jj_down,dim)].green+src[RIDX(ii_down,jj_down+1,dim)].green+src[RIDX(ii_down,jj_up,dim)].green+src[RIDX(ii_down+1,jj_down,dim)].green+src[RIDX(ii_down+1,jj_down+1,dim)].green+src[RIDX(ii_down+1,jj_up,dim)].green+src[RIDX(ii_up,jj_down,dim)].green+src[RIDX(ii_up,jj_down+1,dim)].green+src[RIDX(ii_up,jj_up,dim)].green)/9;

                    dst[RIDX(i, j, dim)].blue=(src[RIDX(ii_down,jj_down,dim)].blue+src[RIDX(ii_down,jj_down+1,dim)].blue+src[RIDX(ii_down,jj_up,dim)].blue+src[RIDX(ii_down+1,jj_down,dim)].blue+src[RIDX(ii_down+1,jj_down+1,dim)].blue+src[RIDX(ii_down+1,jj_up,dim)].blue+src[RIDX(ii_up,jj_down,dim)].blue+src[RIDX(ii_up,jj_down+1,dim)].blue+src[RIDX(ii_up,jj_up,dim)].blue)/9;
                }
                else{
                    dst[RIDX(i, j, dim)].red=(src[RIDX(ii_down,jj_down,dim)].red+src[RIDX(ii_down,jj_up,dim)].red+src[RIDX(ii_down+1,jj_down,dim)].red+src[RIDX(ii_down+1,jj_up,dim)].red+src[RIDX(ii_up,jj_down,dim)].red+src[RIDX(ii_up,jj_up,dim)].red)/6;

                    dst[RIDX(i, j, dim)].green=(src[RIDX(ii_down,jj_down,dim)].green+src[RIDX(ii_down,jj_up,dim)].green+src[RIDX(ii_down+1,jj_down,dim)].green+src[RIDX(ii_down+1,jj_up,dim)].green+src[RIDX(ii_up,jj_down,dim)].green+src[RIDX(ii_up,jj_up,dim)].green)/6;

                    dst[RIDX(i, j, dim)].blue=(src[RIDX(ii_down,jj_down,dim)].blue+src[RIDX(ii_down,jj_up,dim)].blue+src[RIDX(ii_down+1,jj_down,dim)].blue+src[RIDX(ii_down+1,jj_up,dim)].blue+src[RIDX(ii_up,jj_down,dim)].blue+src[RIDX(ii_up,jj_up,dim)].blue)/6;
                }
            }
            else{
                if(jj_up-jj_down==2){
                    dst[RIDX(i, j, dim)].red=(src[RIDX(ii_down,jj_down,dim)].red+src[RIDX(ii_down,jj_down+1,dim)].red+src[RIDX(ii_down,jj_up,dim)].red+src[RIDX(ii_up,jj_down,dim)].red+src[RIDX(ii_up,jj_down+1,dim)].red+src[RIDX(ii_up,jj_up,dim)].red)/6;

                    dst[RIDX(i, j, dim)].green=(src[RIDX(ii_down,jj_down,dim)].green+src[RIDX(ii_down,jj_down+1,dim)].green+src[RIDX(ii_down,jj_up,dim)].green+src[RIDX(ii_up,jj_down,dim)].green+src[RIDX(ii_up,jj_down+1,dim)].green+src[RIDX(ii_up,jj_up,dim)].green)/6;

                    dst[RIDX(i, j, dim)].blue=(src[RIDX(ii_down,jj_down,dim)].blue+src[RIDX(ii_down,jj_down+1,dim)].blue+src[RIDX(ii_down,jj_up,dim)].blue+src[RIDX(ii_up,jj_down,dim)].blue+src[RIDX(ii_up,jj_down+1,dim)].blue+src[RIDX(ii_up,jj_up,dim)].blue)/6;
                }
                else{
                    dst[RIDX(i, j, dim)].red=(src[RIDX(ii_down,jj_down,dim)].red+src[RIDX(ii_down,jj_up,dim)].red+src[RIDX(ii_up,jj_down,dim)].red+src[RIDX(ii_up,jj_up,dim)].red)/4;

                    dst[RIDX(i, j, dim)].green=(src[RIDX(ii_down,jj_down,dim)].green+src[RIDX(ii_down,jj_up,dim)].green+src[RIDX(ii_up,jj_down,dim)].green+src[RIDX(ii_up,jj_up,dim)].green)/4;

                    dst[RIDX(i, j, dim)].blue=(src[RIDX(ii_down,jj_down,dim)].blue+src[RIDX(ii_down,jj_up,dim)].blue+src[RIDX(ii_up,jj_down,dim)].blue+src[RIDX(ii_up,jj_up,dim)].blue)/4;
                }
            }
    }      
}

char smooth2_descr[] = "smooth2 version,";
void smooth2(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 1; i < dim-1; i++){
	    for (j = 1; j < dim-1; j++){
            dst[RIDX(i, j, dim)].red=(src[RIDX(i-1,j-1,dim)].red+src[RIDX(i-1,j,dim)].red+src[RIDX(i-1,j+1,dim)].red+src[RIDX(i,j-1,dim)].red+src[RIDX(i,j,dim)].red+src[RIDX(i,j+1,dim)].red+src[RIDX(i+1,j-1,dim)].red+src[RIDX(i+1,j,dim)].red+src[RIDX(i+1,j+1,dim)].red)/9;

            dst[RIDX(i, j, dim)].green=(src[RIDX(i-1,j-1,dim)].green+src[RIDX(i-1,j,dim)].green+src[RIDX(i-1,j+1,dim)].green+src[RIDX(i,j-1,dim)].green+src[RIDX(i,j,dim)].green+src[RIDX(i,j+1,dim)].green+src[RIDX(i+1,j-1,dim)].green+src[RIDX(i+1,j,dim)].green+src[RIDX(i+1,j+1,dim)].green)/9;

            dst[RIDX(i, j, dim)].blue=(src[RIDX(i-1,j-1,dim)].blue+src[RIDX(i-1,j,dim)].blue+src[RIDX(i-1,j+1,dim)].blue+src[RIDX(i,j-1,dim)].blue+src[RIDX(i,j,dim)].blue+src[RIDX(i,j+1,dim)].blue+src[RIDX(i+1,j-1,dim)].blue+src[RIDX(i+1,j,dim)].blue+src[RIDX(i+1,j+1,dim)].blue)/9;
        }
    }
    for(j=1;j<dim-1;j++){
        dst[RIDX(0, j, dim)].red=(src[RIDX(0,j-1,dim)].red+src[RIDX(0,j,dim)].red+src[RIDX(0,j+1,dim)].red+src[RIDX(1,j-1,dim)].red+src[RIDX(1,j,dim)].red+src[RIDX(1,j+1,dim)].red)/6;
        dst[RIDX(0, j, dim)].green=(src[RIDX(0,j-1,dim)].green+src[RIDX(0,j,dim)].green+src[RIDX(0,j+1,dim)].green+src[RIDX(1,j-1,dim)].green+src[RIDX(1,j,dim)].green+src[RIDX(1,j+1,dim)].green)/6;
        dst[RIDX(0, j, dim)].blue=(src[RIDX(0,j-1,dim)].blue+src[RIDX(0,j,dim)].blue+src[RIDX(0,j+1,dim)].blue+src[RIDX(1,j-1,dim)].blue+src[RIDX(1,j,dim)].blue+src[RIDX(1,j+1,dim)].blue)/6;
    }
    for(j=1;j<dim-1;j++){
        dst[RIDX(dim-1, j, dim)].red=(src[RIDX(dim-2,j-1,dim)].red+src[RIDX(dim-2,j,dim)].red+src[RIDX(dim-2,j+1,dim)].red+src[RIDX(dim-1,j-1,dim)].red+src[RIDX(dim-1,j,dim)].red+src[RIDX(dim-1,j+1,dim)].red)/6;
        dst[RIDX(dim-1, j, dim)].green=(src[RIDX(dim-2,j-1,dim)].green+src[RIDX(dim-2,j,dim)].green+src[RIDX(dim-2,j+1,dim)].green+src[RIDX(dim-1,j-1,dim)].green+src[RIDX(dim-1,j,dim)].green+src[RIDX(dim-1,j+1,dim)].green)/6;
        dst[RIDX(dim-1, j, dim)].blue=(src[RIDX(dim-2,j-1,dim)].blue+src[RIDX(dim-2,j,dim)].blue+src[RIDX(dim-2,j+1,dim)].blue+src[RIDX(dim-1,j-1,dim)].blue+src[RIDX(dim-1,j,dim)].blue+src[RIDX(dim-1,j+1,dim)].blue)/6;
    }
    for(i=1;i<dim-1;i++){
        dst[RIDX(i, 0, dim)].red=(src[RIDX(i-1, 0, dim)].red+src[RIDX(i, 0, dim)].red+src[RIDX(i+1, 0, dim)].red+src[RIDX(i-1, 1, dim)].red+src[RIDX(i, 1, dim)].red+src[RIDX(i+1, 1, dim)].red)/6;
        dst[RIDX(i, 0, dim)].green=(src[RIDX(i-1, 0, dim)].green+src[RIDX(i, 0, dim)].green+src[RIDX(i+1, 0, dim)].green+src[RIDX(i-1, 1, dim)].green+src[RIDX(i, 1, dim)].green+src[RIDX(i+1, 1, dim)].green)/6;
        dst[RIDX(i, 0, dim)].blue=(src[RIDX(i-1, 0, dim)].blue+src[RIDX(i, 0, dim)].blue+src[RIDX(i+1, 0, dim)].blue+src[RIDX(i-1, 1, dim)].blue+src[RIDX(i, 1, dim)].blue+src[RIDX(i+1, 1, dim)].blue)/6;
    }
    for(i=1;i<dim-1;i++){
        dst[RIDX(i, dim-1, dim)].red=(src[RIDX(i-1, dim-2, dim)].red+src[RIDX(i, dim-2, dim)].red+src[RIDX(i+1, dim-2, dim)].red+src[RIDX(i-1, dim-1, dim)].red+src[RIDX(i, dim-1, dim)].red+src[RIDX(i+1, dim-1, dim)].red)/6;
        dst[RIDX(i, dim-1, dim)].green=(src[RIDX(i-1, dim-2, dim)].green+src[RIDX(i, dim-2, dim)].green+src[RIDX(i+1, dim-2, dim)].green+src[RIDX(i-1, dim-1, dim)].green+src[RIDX(i, dim-1, dim)].green+src[RIDX(i+1, dim-1, dim)].green)/6;
        dst[RIDX(i, dim-1, dim)].blue=(src[RIDX(i-1, dim-2, dim)].blue+src[RIDX(i, dim-2, dim)].blue+src[RIDX(i+1, dim-2, dim)].blue+src[RIDX(i-1, dim-1, dim)].blue+src[RIDX(i, dim-1, dim)].blue+src[RIDX(i+1, dim-1, dim)].blue)/6;
    }
    dst[RIDX(0, 0, dim)].red=(src[RIDX(0, 0, dim)].red+src[RIDX(0, 1, dim)].red+src[RIDX(1, 0, dim)].red+src[RIDX(1, 1, dim)].red)/4;
    dst[RIDX(0, 0, dim)].green=(src[RIDX(0, 0, dim)].green+src[RIDX(0, 1, dim)].green+src[RIDX(1, 0, dim)].green+src[RIDX(1, 1, dim)].green)/4;
    dst[RIDX(0, 0, dim)].blue=(src[RIDX(0, 0, dim)].blue+src[RIDX(0, 1, dim)].blue+src[RIDX(1, 0, dim)].blue+src[RIDX(1, 1, dim)].blue)/4;

    dst[RIDX(0, dim-1, dim)].red=(src[RIDX(0, dim-2, dim)].red+src[RIDX(0, dim-1, dim)].red+src[RIDX(1, dim-2, dim)].red+src[RIDX(1, dim-1, dim)].red)/4;
    dst[RIDX(0, dim-1, dim)].green=(src[RIDX(0, dim-2, dim)].green+src[RIDX(0, dim-1, dim)].green+src[RIDX(1, dim-2, dim)].green+src[RIDX(1, dim-1, dim)].green)/4;
    dst[RIDX(0, dim-1, dim)].blue=(src[RIDX(0, dim-2, dim)].blue+src[RIDX(0, dim-1, dim)].blue+src[RIDX(1, dim-2, dim)].blue+src[RIDX(1, dim-1, dim)].blue)/4;
    
    dst[RIDX(dim-1, 0, dim)].red=(src[RIDX(dim-2, 0, dim)].red+src[RIDX(dim-2, 1, dim)].red+src[RIDX(dim-1, 0, dim)].red+src[RIDX(dim-1, 1, dim)].red)/4;
    dst[RIDX(dim-1, 0, dim)].green=(src[RIDX(dim-2, 0, dim)].green+src[RIDX(dim-2, 1, dim)].green+src[RIDX(dim-1, 0, dim)].green+src[RIDX(dim-1, 1, dim)].green)/4;
    dst[RIDX(dim-1, 0, dim)].blue=(src[RIDX(dim-2, 0, dim)].blue+src[RIDX(dim-2, 1, dim)].blue+src[RIDX(dim-1, 0, dim)].blue+src[RIDX(dim-1, 1, dim)].blue)/4;

    dst[RIDX(dim-1, dim-1, dim)].red=(src[RIDX(dim-2, dim-2, dim)].red+src[RIDX(dim-2, dim-1, dim)].red+src[RIDX(dim-1, dim-2, dim)].red+src[RIDX(dim-1, dim-1, dim)].red)/4;
    dst[RIDX(dim-1, dim-1, dim)].green=(src[RIDX(dim-2, dim-2, dim)].green+src[RIDX(dim-2, dim-1, dim)].green+src[RIDX(dim-1, dim-2, dim)].green+src[RIDX(dim-1, dim-1, dim)].green)/4;
    dst[RIDX(dim-1, dim-1, dim)].blue=(src[RIDX(dim-2, dim-2, dim)].blue+src[RIDX(dim-2, dim-1, dim)].blue+src[RIDX(dim-1, dim-2, dim)].blue+src[RIDX(dim-1, dim-1, dim)].blue)/4;
}
         
char smooth3_descr[] = "smooth3 version,";
void smooth3(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    for (i = 1; i < dim-1; i++){
        int col0_red=src[RIDX(i-1,0,dim)].red+src[RIDX(i,0,dim)].red+src[RIDX(i+1,0,dim)].red;
        int col0_green=src[RIDX(i-1,0,dim)].green+src[RIDX(i,0,dim)].green+src[RIDX(i+1,0,dim)].green;
        int col0_blue=src[RIDX(i-1,0,dim)].blue+src[RIDX(i,0,dim)].blue+src[RIDX(i+1,0,dim)].blue;

        int col1_red=src[RIDX(i-1,1,dim)].red+src[RIDX(i,1,dim)].red+src[RIDX(i+1,1,dim)].red;
        int col1_green=src[RIDX(i-1,1,dim)].green+src[RIDX(i,1,dim)].green+src[RIDX(i+1,1,dim)].green;
        int col1_blue=src[RIDX(i-1,1,dim)].blue+src[RIDX(i,1,dim)].blue+src[RIDX(i+1,1,dim)].blue;

        int col2_red=src[RIDX(i-1,2,dim)].red+src[RIDX(i,2,dim)].red+src[RIDX(i+1,2,dim)].red;
        int col2_green=src[RIDX(i-1,2,dim)].green+src[RIDX(i,2,dim)].green+src[RIDX(i+1,2,dim)].green;
        int col2_blue=src[RIDX(i-1,2,dim)].blue+src[RIDX(i,2,dim)].blue+src[RIDX(i+1,2,dim)].blue;

	    for (j = 1; j < dim-1; j++){
            dst[RIDX(i, j, dim)].red=(col0_red+col1_red+col2_red)/9;

            dst[RIDX(i, j, dim)].green=(col0_green+col1_green+col2_green)/9;

            dst[RIDX(i, j, dim)].blue=(col0_blue+col1_blue+col2_blue)/9;

            col0_red=col1_red;
            col0_blue=col1_blue;
            col0_green=col1_green;
            col1_red=col2_red;
            col1_blue=col2_blue;
            col1_green=col2_green;
            col2_red=src[RIDX(i-1,j+2,dim)].red+src[RIDX(i,j+2,dim)].red+src[RIDX(i+1,j+2,dim)].red;
            col2_green=src[RIDX(i-1,j+2,dim)].green+src[RIDX(i,j+2,dim)].green+src[RIDX(i+1,j+2,dim)].green;
            col2_blue=src[RIDX(i-1,j+2,dim)].blue+src[RIDX(i,j+2,dim)].blue+src[RIDX(i+1,j+2,dim)].blue;
        }
    }
    for(j=1;j<dim-1;j++){
        dst[RIDX(0, j, dim)].red=(src[RIDX(0,j-1,dim)].red+src[RIDX(0,j,dim)].red+src[RIDX(0,j+1,dim)].red+src[RIDX(1,j-1,dim)].red+src[RIDX(1,j,dim)].red+src[RIDX(1,j+1,dim)].red)/6;
        dst[RIDX(0, j, dim)].green=(src[RIDX(0,j-1,dim)].green+src[RIDX(0,j,dim)].green+src[RIDX(0,j+1,dim)].green+src[RIDX(1,j-1,dim)].green+src[RIDX(1,j,dim)].green+src[RIDX(1,j+1,dim)].green)/6;
        dst[RIDX(0, j, dim)].blue=(src[RIDX(0,j-1,dim)].blue+src[RIDX(0,j,dim)].blue+src[RIDX(0,j+1,dim)].blue+src[RIDX(1,j-1,dim)].blue+src[RIDX(1,j,dim)].blue+src[RIDX(1,j+1,dim)].blue)/6;
    }
    for(j=1;j<dim-1;j++){
        dst[RIDX(dim-1, j, dim)].red=(src[RIDX(dim-2,j-1,dim)].red+src[RIDX(dim-2,j,dim)].red+src[RIDX(dim-2,j+1,dim)].red+src[RIDX(dim-1,j-1,dim)].red+src[RIDX(dim-1,j,dim)].red+src[RIDX(dim-1,j+1,dim)].red)/6;
        dst[RIDX(dim-1, j, dim)].green=(src[RIDX(dim-2,j-1,dim)].green+src[RIDX(dim-2,j,dim)].green+src[RIDX(dim-2,j+1,dim)].green+src[RIDX(dim-1,j-1,dim)].green+src[RIDX(dim-1,j,dim)].green+src[RIDX(dim-1,j+1,dim)].green)/6;
        dst[RIDX(dim-1, j, dim)].blue=(src[RIDX(dim-2,j-1,dim)].blue+src[RIDX(dim-2,j,dim)].blue+src[RIDX(dim-2,j+1,dim)].blue+src[RIDX(dim-1,j-1,dim)].blue+src[RIDX(dim-1,j,dim)].blue+src[RIDX(dim-1,j+1,dim)].blue)/6;
    }
    for(i=1;i<dim-1;i++){
        dst[RIDX(i, 0, dim)].red=(src[RIDX(i-1, 0, dim)].red+src[RIDX(i, 0, dim)].red+src[RIDX(i+1, 0, dim)].red+src[RIDX(i-1, 1, dim)].red+src[RIDX(i, 1, dim)].red+src[RIDX(i+1, 1, dim)].red)/6;
        dst[RIDX(i, 0, dim)].green=(src[RIDX(i-1, 0, dim)].green+src[RIDX(i, 0, dim)].green+src[RIDX(i+1, 0, dim)].green+src[RIDX(i-1, 1, dim)].green+src[RIDX(i, 1, dim)].green+src[RIDX(i+1, 1, dim)].green)/6;
        dst[RIDX(i, 0, dim)].blue=(src[RIDX(i-1, 0, dim)].blue+src[RIDX(i, 0, dim)].blue+src[RIDX(i+1, 0, dim)].blue+src[RIDX(i-1, 1, dim)].blue+src[RIDX(i, 1, dim)].blue+src[RIDX(i+1, 1, dim)].blue)/6;
    }
    for(i=1;i<dim-1;i++){
        dst[RIDX(i, dim-1, dim)].red=(src[RIDX(i-1, dim-2, dim)].red+src[RIDX(i, dim-2, dim)].red+src[RIDX(i+1, dim-2, dim)].red+src[RIDX(i-1, dim-1, dim)].red+src[RIDX(i, dim-1, dim)].red+src[RIDX(i+1, dim-1, dim)].red)/6;
        dst[RIDX(i, dim-1, dim)].green=(src[RIDX(i-1, dim-2, dim)].green+src[RIDX(i, dim-2, dim)].green+src[RIDX(i+1, dim-2, dim)].green+src[RIDX(i-1, dim-1, dim)].green+src[RIDX(i, dim-1, dim)].green+src[RIDX(i+1, dim-1, dim)].green)/6;
        dst[RIDX(i, dim-1, dim)].blue=(src[RIDX(i-1, dim-2, dim)].blue+src[RIDX(i, dim-2, dim)].blue+src[RIDX(i+1, dim-2, dim)].blue+src[RIDX(i-1, dim-1, dim)].blue+src[RIDX(i, dim-1, dim)].blue+src[RIDX(i+1, dim-1, dim)].blue)/6;
    }
    dst[RIDX(0, 0, dim)].red=(src[RIDX(0, 0, dim)].red+src[RIDX(0, 1, dim)].red+src[RIDX(1, 0, dim)].red+src[RIDX(1, 1, dim)].red)/4;
    dst[RIDX(0, 0, dim)].green=(src[RIDX(0, 0, dim)].green+src[RIDX(0, 1, dim)].green+src[RIDX(1, 0, dim)].green+src[RIDX(1, 1, dim)].green)/4;
    dst[RIDX(0, 0, dim)].blue=(src[RIDX(0, 0, dim)].blue+src[RIDX(0, 1, dim)].blue+src[RIDX(1, 0, dim)].blue+src[RIDX(1, 1, dim)].blue)/4;

    dst[RIDX(0, dim-1, dim)].red=(src[RIDX(0, dim-2, dim)].red+src[RIDX(0, dim-1, dim)].red+src[RIDX(1, dim-2, dim)].red+src[RIDX(1, dim-1, dim)].red)/4;
    dst[RIDX(0, dim-1, dim)].green=(src[RIDX(0, dim-2, dim)].green+src[RIDX(0, dim-1, dim)].green+src[RIDX(1, dim-2, dim)].green+src[RIDX(1, dim-1, dim)].green)/4;
    dst[RIDX(0, dim-1, dim)].blue=(src[RIDX(0, dim-2, dim)].blue+src[RIDX(0, dim-1, dim)].blue+src[RIDX(1, dim-2, dim)].blue+src[RIDX(1, dim-1, dim)].blue)/4;
    
    dst[RIDX(dim-1, 0, dim)].red=(src[RIDX(dim-2, 0, dim)].red+src[RIDX(dim-2, 1, dim)].red+src[RIDX(dim-1, 0, dim)].red+src[RIDX(dim-1, 1, dim)].red)/4;
    dst[RIDX(dim-1, 0, dim)].green=(src[RIDX(dim-2, 0, dim)].green+src[RIDX(dim-2, 1, dim)].green+src[RIDX(dim-1, 0, dim)].green+src[RIDX(dim-1, 1, dim)].green)/4;
    dst[RIDX(dim-1, 0, dim)].blue=(src[RIDX(dim-2, 0, dim)].blue+src[RIDX(dim-2, 1, dim)].blue+src[RIDX(dim-1, 0, dim)].blue+src[RIDX(dim-1, 1, dim)].blue)/4;

    dst[RIDX(dim-1, dim-1, dim)].red=(src[RIDX(dim-2, dim-2, dim)].red+src[RIDX(dim-2, dim-1, dim)].red+src[RIDX(dim-1, dim-2, dim)].red+src[RIDX(dim-1, dim-1, dim)].red)/4;
    dst[RIDX(dim-1, dim-1, dim)].green=(src[RIDX(dim-2, dim-2, dim)].green+src[RIDX(dim-2, dim-1, dim)].green+src[RIDX(dim-1, dim-2, dim)].green+src[RIDX(dim-1, dim-1, dim)].green)/4;
    dst[RIDX(dim-1, dim-1, dim)].blue=(src[RIDX(dim-2, dim-2, dim)].blue+src[RIDX(dim-2, dim-1, dim)].blue+src[RIDX(dim-1, dim-2, dim)].blue+src[RIDX(dim-1, dim-1, dim)].blue)/4;
}
/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&smooth1,smooth1_descr);
    add_smooth_function(&smooth2,smooth2_descr);
    add_smooth_function(&smooth3,smooth3_descr);
    /* ... Register additional test functions here */
}

