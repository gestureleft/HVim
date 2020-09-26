//
// Created by Harrison Marshall on 26/9/20.
//

/**
 * Given an int n, return the number of digits in n
 * @param n The number to count the digits of
 * @return The number of digits in n
 */
int num_digits(int n)
{
    int digits = 0;
    if (n < 0) digits = 1;
    while (n)
    {
        n /= 10;
        digits++;
    }
    return digits;
}