/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_org_desc[] = "Simple row-wise scan transpose";
void trans_org(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

// 可以引入 k 减少代码重复（本实验关注 cache miss，不关注 CPE）
// 32x32、64x64 等 2 的幂尺寸矩阵容易出现规律性冲突，也更适合做针对性优化（如更细分块与重排写入顺序）
// 61x67 这类非规则尺寸难以做强针对优化，通常使用较大分块 + 边界处理，实现访问局部化，并将冲突控制在局部范围内
char trans_submit_desc[] = "Transpose submission";
void trans_submit(int M, int N, int A[N][M], int B[M][N]){
    int i, j, k;
    int t0, t1, t2, t3, t4, t5, t6, t7;

    if (M == 32 && N == 32) {
        for (i = 0; i < 32; i += 8) {
            for (j = 0; j < 32; j += 8) {
                for (k = i; k < i + 8; ++k) {
                    t0 = A[k][j];
                    t1 = A[k][j + 1];
                    t2 = A[k][j + 2];
                    t3 = A[k][j + 3];
                    t4 = A[k][j + 4];
                    t5 = A[k][j + 5];
                    t6 = A[k][j + 6];
                    t7 = A[k][j + 7];
                    B[j][k] = t0;
                    B[j + 1][k] = t1;
                    B[j + 2][k] = t2;
                    B[j + 3][k] = t3;
                    B[j + 4][k] = t4;
                    B[j + 5][k] = t5;
                    B[j + 6][k] = t6;
                    B[j + 7][k] = t7;
                }
            }
        }
        return;
    }

    if (M == 64 && N == 64) {
        for (i = 0; i < 64; i += 8) {
            for (j = 0; j < 64; j += 8) {
                //用B块中前4行的后四列作为缓冲区，尽量减小冲突
                for (k = 0; k < 4; ++k) {
                    t0 = A[i + k][j];
                    t1 = A[i + k][j + 1];
                    t2 = A[i + k][j + 2];
                    t3 = A[i + k][j + 3];
                    t4 = A[i + k][j + 4];
                    t5 = A[i + k][j + 5];
                    t6 = A[i + k][j + 6];
                    t7 = A[i + k][j + 7];

                    B[j][i + k] = t0;
                    B[j + 1][i + k] = t1;
                    B[j + 2][i + k] = t2;
                    B[j + 3][i + k] = t3;
                    B[j][i + k + 4] = t4;
                    B[j + 1][i + k + 4] = t5;
                    B[j + 2][i + k + 4] = t6;
                    B[j + 3][i + k + 4] = t7;
                }

                //读取时B缓冲区中的数据是按照行进行读取的，且读完以后直接最多造成一次cache miss，但是每次读取A块的后四列多次cache miss无法避免
                //引入三次异或操作一次搬运16个数据
                for (k = 0; k < 4; k+=2) {
                    /*
                    t0 = A[i + 4][j + k];
                    t1 = A[i + 5][j + k];
                    t2 = A[i + 6][j + k];
                    t3 = A[i + 7][j + k];

                    t4 = B[j + k][i + 4];
                    t5 = B[j + k][i + 5];
                    t6 = B[j + k][i + 6];
                    t7 = B[j + k][i + 7];

                    B[j + k][i + 4] = t0;
                    B[j + k][i + 5] = t1;
                    B[j + k][i + 6] = t2;
                    B[j + k][i + 7] = t3;

                    B[j + 4 + k][i] = t4;
                    B[j + 4 + k][i + 1] = t5;
                    B[j + 4 + k][i + 2] = t6;
                    B[j + 4 + k][i + 3] = t7;
                    */
                    t0 = A[i + 4][j + k];
                    t1 = A[i + 5][j + k];
                    t2 = A[i + 6][j + k];
                    t3 = A[i + 7][j + k];
                    t4 = A[i + 4][j + k+1];
                    t5 = A[i + 5][j + k+1];
                    t6 = A[i + 6][j + k+1];
                    t7 = A[i + 7][j + k+1];

                    t0=t0^B[j + k][i + 4];
                    B[j + k][i + 4]=t0^B[j + k][i + 4];
                    t0=t0^B[j + k][i + 4];

                    t1=t1^B[j + k][i + 5];
                    B[j + k][i + 5]=t1^B[j + k][i + 5];
                    t1=t1^B[j + k][i + 5];

                    t2=t2^B[j + k][i + 6];
                    B[j + k][i + 6]=t2^B[j + k][i + 6];
                    t2=t2^B[j + k][i + 6];

                    t3=t3^B[j + k][i + 7];
                    B[j + k][i + 7]=t3^B[j + k][i + 7];
                    t3=t3^B[j + k][i + 7];

                    t4=t4^B[j + k+1][i + 4];
                    B[j + k+1][i + 4]=t4^B[j + k+1][i + 4];
                    t4=t4^B[j + k+1][i + 4];

                    t5=t5^B[j + k+1][i + 5];
                    B[j + k+1][i + 5]=t5^B[j + k+1][i + 5];
                    t5=t5^B[j + k+1][i + 5];

                    t6=t6^B[j + k+1][i + 6];
                    B[j + k+1][i + 6]=t6^B[j + k+1][i + 6];
                    t6=t6^B[j + k+1][i + 6];

                    t7=t7^B[j + k+1][i + 7];
                    B[j + k+1][i + 7]=t7^B[j + k+1][i + 7];
                    t7=t7^B[j + k+1][i + 7];

                    B[j + 4 + k][i] = t0;
                    B[j + 4 + k][i + 1] = t1;
                    B[j + 4 + k][i + 2] = t2;
                    B[j + 4 + k][i + 3] = t3;
                    B[j + 5 + k][i] = t4;
                    B[j + 5 + k][i + 1] = t5;
                    B[j + 5 + k][i + 2] = t6;
                    B[j + 5 + k][i + 3] = t7;
                }

                //增加临时变量降低cache miss
                for (k=i + 4;k < i + 8;k+=2) {
                    t0=A[k][j+4];
                    t1=A[k][j+5];
                    t2=A[k][j+6];
                    t3=A[k][j+7];
                    t4=A[k+1][j+4];
                    t5=A[k+1][j+5];
                    t6=A[k+1][j+6];
                    t7=A[k+1][j+7];

                    B[j+4][k]=t0;
                    B[j+5][k]=t1;
                    B[j+6][k]=t2;
                    B[j+7][k]=t3;
                    B[j+4][k+1]=t4;
                    B[j+5][k+1]=t5;
                    B[j+6][k+1]=t6;
                    B[j+7][k+1]=t7;
                }
            }
        }
        return;
    }

    /* 61x67 (and other non-square tails): 16x16 blocked transpose */
    //这种常见的形状使用较大的块做优化效果反倒更好
    for (i = 0; i < N; i += 16) {
        for (j = 0; j < M; j += 16) {
            int i_end = (i + 16 < N) ? (i + 16) : N;
            int j_end = (j + 16 < M) ? (j + 16) : M;
            int ii, jj;
            for (ii = i; ii < i_end; ++ii) {
                for (jj = j; jj < j_end; ++jj) {
                    B[jj][ii] = A[ii][jj];
                }
            }
        }
    }
}

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
 char trans_old_submit_desc[] = "Transpose old submission";
 void trans_old_submit(int M, int N, int A[N][M], int B[M][N]){
    int i,j;
    for(i=0;i<N-7;i+=8){
        for(j=0;j<M-7;j+=8){
            B[j][i]=A[i][j];
            B[j+1][i]=A[i][j+1];    
            B[j+2][i]=A[i][j+2];
            B[j+3][i]=A[i][j+3];   
            B[j+4][i]=A[i][j+4];
            B[j+5][i]=A[i][j+5];    
            B[j+6][i]=A[i][j+6];
            B[j+7][i]=A[i][j+7];     
            
            B[j][i+1]=A[i+1][j];
            B[j+1][i+1]=A[i+1][j+1];    
            B[j+2][i+1]=A[i+1][j+2];
            B[j+3][i+1]=A[i+1][j+3];   
            B[j+4][i+1]=A[i+1][j+4];
            B[j+5][i+1]=A[i+1][j+5];    
            B[j+6][i+1]=A[i+1][j+6];
            B[j+7][i+1]=A[i+1][j+7];  
                
            B[j][i+2]=A[i+2][j];
            B[j+1][i+2]=A[i+2][j+1];    
            B[j+2][i+2]=A[i+2][j+2];
            B[j+3][i+2]=A[i+2][j+3];   
            B[j+4][i+2]=A[i+2][j+4];
            B[j+5][i+2]=A[i+2][j+5];    
            B[j+6][i+2]=A[i+2][j+6];
            B[j+7][i+2]=A[i+2][j+7];  
            
            B[j][i+3]=A[i+3][j];
            B[j+1][i+3]=A[i+3][j+1];    
            B[j+2][i+3]=A[i+3][j+2];
            B[j+3][i+3]=A[i+3][j+3];   
            B[j+4][i+3]=A[i+3][j+4];
            B[j+5][i+3]=A[i+3][j+5];    
            B[j+6][i+3]=A[i+3][j+6];
            B[j+7][i+3]=A[i+3][j+7]; 

            B[j][i+4]=A[i+4][j];
            B[j+1][i+4]=A[i+4][j+1];    
            B[j+2][i+4]=A[i+4][j+2];
            B[j+3][i+4]=A[i+4][j+3];   
            B[j+4][i+4]=A[i+4][j+4];
            B[j+5][i+4]=A[i+4][j+5];    
            B[j+6][i+4]=A[i+4][j+6];
            B[j+7][i+4]=A[i+4][j+7]; 

            B[j][i+5]=A[i+5][j];
            B[j+1][i+5]=A[i+5][j+1];    
            B[j+2][i+5]=A[i+5][j+2];
            B[j+3][i+5]=A[i+5][j+3];   
            B[j+4][i+5]=A[i+5][j+4];
            B[j+5][i+5]=A[i+5][j+5];    
            B[j+6][i+5]=A[i+5][j+6];
            B[j+7][i+5]=A[i+5][j+7]; 

            B[j][i+6]=A[i+6][j];
            B[j+1][i+6]=A[i+6][j+1];    
            B[j+2][i+6]=A[i+6][j+2];
            B[j+3][i+6]=A[i+6][j+3];   
            B[j+4][i+6]=A[i+6][j+4];
            B[j+5][i+6]=A[i+6][j+5];    
            B[j+6][i+6]=A[i+6][j+6];
            B[j+7][i+6]=A[i+6][j+7]; 

            B[j][i+7]=A[i+7][j];
            B[j+1][i+7]=A[i+7][j+1];    
            B[j+2][i+7]=A[i+7][j+2];
            B[j+3][i+7]=A[i+7][j+3];   
            B[j+4][i+7]=A[i+7][j+4];
            B[j+5][i+7]=A[i+7][j+5];    
            B[j+6][i+7]=A[i+7][j+6];
            B[j+7][i+7]=A[i+7][j+7]; 
        }
        for(;j<M;j++){
            B[j][i]=A[i][j];
            B[j][i+1]=A[i+1][j];
            B[j][i+2]=A[i+2][j];
            B[j][i+3]=A[i+3][j];
            B[j][i+4]=A[i+4][j];
            B[j][i+5]=A[i+5][j];
            B[j][i+6]=A[i+6][j];
            B[j][i+7]=A[i+7][j];
        }
    }
    for(;i<N;i++){
        for(j=0;j<M;j++){
            B[j][i]=A[i][j];
        }
    }
 }

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(trans_org, trans_org_desc); 
    registerTransFunction(trans_old_submit,trans_old_submit_desc);
    /* Register any additional transpose functions */
    registerTransFunction(trans_submit, trans_submit_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

