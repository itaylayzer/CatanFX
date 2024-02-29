package com.c1t45.model.Utils;

import java.util.Arrays;

import com.c1t45.model.Interfaces.Action;

public class MatrixUtils {

    public static int[][] duplicate(int[][] mat) {
        return Arrays.stream(mat)
                .map(row -> Arrays.stream(row)
                        .toArray())
                .toArray(int[][]::new);
    }

    public static int[][] multiplyMatrix(int[][] mat1, int rows1, int[][] mat2, int cols2, int rows2cols1) {
        int[][] result = new int[rows1][cols2];

        for (int row_offset = 0; row_offset < rows1; row_offset++) {
            for (int col_offset = 0; col_offset < cols2; col_offset++) {
                for (int k = 0; k < rows2cols1; k++) {
                    result[row_offset][col_offset] += mat1[row_offset][k] * mat2[k][col_offset];
                }
            }
        }

        return result;
    }

    public static void printMatrix(int[][] matrix, int rows, int cols) {
        printMatrix(matrix, rows, cols, null);
    }

    public static void printMatrix(int[][] matrix, int rows, int cols, Action<Integer> print) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (print == null)
                    System.out.print(matrix[i][j] + " ");
                else
                    print.action(matrix[i][j]);

            }
            System.out.println();
        }
    }
}
