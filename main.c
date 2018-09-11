/* Deadline:    11-9-2018
 * Course:      Modern Cryptography.
 * Group:       SQL Injection
 * Students:    Levi van der Griendt, Ismail Mahdaoui, Zi Long Zhu, Niels Zwemmer
 * Description: This exercise belongs to week 1. This is the decryption part
 *              for the cipertext as provided by Coursera here:
 * https://www.coursera.org/learn/cryptography/supplement/7mgaE/programming-assignment-1
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Generates a stream from an array. */
void gen_stream(int *stream, int *numbers, int max, int start, int interval) {
    for (int i = 0, j = 0; i < max; i++) {
        if ((i - start) % interval == 0)
            stream[j++] = numbers[i];
    }
}

/* Calculates the sum of all frequencies within a range. */
double calc_sum(int *stream, int max, int start, int end) {
    double sum = 0;
    int freq[256] = {0};
    int count = 0;

    for (int i = 0; i < max; i++) {
        if (stream[i] >= start && stream[i] <= end) {
            freq[stream[i]]++;
            count++;
        }
    }

    for (int i = start; i <= end; i++)
        sum += pow((double) freq[i] / count, 2);

    return sum;
}

int main(int argc, char** argv) {
    int N = 0, len_cipher, len_stream, max, valid, plain;
    double sum, max_sum, min_sum, sums_q[14];
    char line [1000];
    char num[3];
    FILE *fpIn;
    char *cipher;

    if (argc < 2) {
      cipher = "F96DE8C227A259C87EE1DA2AED57C93FE5DA36ED4EC87EF2C63AAE5B9A7EFFD673BE4ACF7BE8923CAB1ECE7AF2DA3DA44FCF7AE29235A24C963FF0DF3CA3599A70E5DA36BF1ECE77F8DC34BE129A6CF4D126BF5B9A7CFEDF3EB850D37CF0C63AA2509A76FF9227A55B9A6FE3D720A850D97AB1DD35ED5FCE6BF0D138A84CC931B1F121B44ECE70F6C032BD56C33FF9D320ED5CDF7AFF9226BE5BDE3FF7DD21ED56CF71F5C036A94D963FF8D473A351CE3FE5DA3CB84DDB71F5C17FED51DC3FE8D732BF4D963FF3C727ED4AC87EF5DB27A451D47EFD9230BF47CA6BFEC12ABE4ADF72E29224A84CDF3FF5D720A459D47AF59232A35A9A7AE7D33FB85FCE7AF5923AA31EDB3FF7D33ABF52C33FF0D673A551D93FFCD33DA35BC831B1F43CBF1EDF67F0DF23A15B963FE5DA36ED68D378F4DC36BF5B9A7AFFD121B44ECE76FEDC73BE5DD27AFCD773BA5FC93FE5DA3CB859D26BB1C63CED5CDF3FE2D730B84CDF3FF7DD21ED5ADF7CF0D636BE1EDB79E5D721ED57CE3FE6D320ED57D469F4DC27A85A963FF3C727ED49DF3FFFDD24ED55D470E69E73AC50DE3FE5DA3ABE1EDF67F4C030A44DDF3FF5D73EA250C96BE3D327A84D963FE5DA32B91ED36BB1D132A31ED87AB1D021A255DF71B1C436BF479A7AF0C13AA14794";
    } else {
      if ((fpIn = fopen(argv[1], "r")) != NULL) {
        if ((fgets(line, sizeof(line), fpIn)) != NULL) {
          cipher = strtok(line, "\n");
          fclose(fpIn);
        }
      } else {
        printf("%s\n", "File not found. Reverting to default hardcoded string.");
        cipher = "F96DE8C227A259C87EE1DA2AED57C93FE5DA36ED4EC87EF2C63AAE5B9A7EFFD673BE4ACF7BE8923CAB1ECE7AF2DA3DA44FCF7AE29235A24C963FF0DF3CA3599A70E5DA36BF1ECE77F8DC34BE129A6CF4D126BF5B9A7CFEDF3EB850D37CF0C63AA2509A76FF9227A55B9A6FE3D720A850D97AB1DD35ED5FCE6BF0D138A84CC931B1F121B44ECE70F6C032BD56C33FF9D320ED5CDF7AFF9226BE5BDE3FF7DD21ED56CF71F5C036A94D963FF8D473A351CE3FE5DA3CB84DDB71F5C17FED51DC3FE8D732BF4D963FF3C727ED4AC87EF5DB27A451D47EFD9230BF47CA6BFEC12ABE4ADF72E29224A84CDF3FF5D720A459D47AF59232A35A9A7AE7D33FB85FCE7AF5923AA31EDB3FF7D33ABF52C33FF0D673A551D93FFCD33DA35BC831B1F43CBF1EDF67F0DF23A15B963FE5DA36ED68D378F4DC36BF5B9A7AFFD121B44ECE76FEDC73BE5DD27AFCD773BA5FC93FE5DA3CB859D26BB1C63CED5CDF3FE2D730B84CDF3FF7DD21ED5ADF7CF0D636BE1EDB79E5D721ED57CE3FE6D320ED57D469F4DC27A85A963FF3C727ED49DF3FFFDD24ED55D470E69E73AC50DE3FE5DA3ABE1EDF67F4C030A44DDF3FF5D73EA250C96BE3D327A84D963FE5DA32B91ED36BB1D132A31ED87AB1D021A255DF71B1C436BF479A7AF0C13AA14794";
      }
    }


    len_cipher = strlen(cipher) / 2;
    int numbers[len_cipher];

    /* Convert the cipher from hex to decimal. */
    num[2] = '\0';
    for (int i = 0, j = 0; i < len_cipher; i++, j += 2) {
		num[0] = cipher[j];
		num[1] = cipher[j + 1];
		numbers[i] = (int) strtol(num, NULL, 16);
    }

    /* Test all possible key lengths and calculate their sum of frequencies.
     * Puts the sums in a sums_q array where the index is the key length.
     */
    for (int test_N = 1; test_N <= 13; test_N++) {
        sum = 0;
        len_stream = len_cipher / test_N + 1;
        int stream[len_stream];

        for (int start = 0; start < test_N; start++) {
            gen_stream(stream, numbers, len_cipher, start, test_N);
            max = len_cipher % test_N > start ? len_stream : len_stream - 1;
            sum += calc_sum(stream, max, 0, 255);
        }

        sums_q[test_N] = sum / test_N;
    }

    /* Finds the key length with the highest sum. */
    max_sum = 0;
    for (int i = 1; i <= 13; i++) {
        if (sums_q[i] > max_sum) {
            max_sum = sums_q[i];
            N = i;
        }
    }

    printf("Found key length N = %d\n", N);

    /* Initializes the array with the first index indicating the stream and
     * the second index the possible keys. The values are the corresponding
     * sums.
     */
    double possible_keys[N][256];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < 256; j++)
            possible_keys[i][j] = 0;
    }

    len_stream = len_cipher / N + 1;
    int stream[len_stream];
    int plain_stream[len_stream];

    /* Calculates all sums, for all possible keys, of every stream.
     * The first loop iterates over every stream.
     */
    for (int start = 0; start < N; start++) {
        gen_stream(stream, numbers, len_cipher, start, N);
        max = len_cipher % N > start ? len_stream : len_stream - 1;

        /* This second loop iterates over all possible keys. */
        for (int B = 1; B < 256; B++) {
            valid = 1;

            /* Checks if this key generates printable ASCII values. */
            for (int i = 0; i < max; i++) {
                plain = stream[i] ^ B;

                if (plain >= 32 && plain < 127) {
                    plain_stream[i] = plain;
                } else {
                    valid = 0;
                    break;
                }
            }

            if (valid) {
                sum = calc_sum(plain_stream, max, 97, 122);

                /* Checks if the calculated sum makes sense. */
                if (sum > 0 && sum < 1)
                    possible_keys[start][B] = sum;
            }
        }
    }

    /* Finds the key with the lowest sum, because this key can almost decrypt
     * the cipher text. Other keys we found, for instance the key with the
     * highest sum, generated only gibberish.
     */
    int key[N];
    for (int i = 0; i <= N; i++) {
        min_sum = 1;

        for (int j = 0; j < 256; j++) {
            if (possible_keys[i][j] > 0 && possible_keys[i][j] < min_sum) {
                min_sum = possible_keys[i][j];
                key[i] = j;
            }
        }
    }

    printf("Found key:");
    for (int i = 0; i < N; i++)
        printf(" %d", key[i]);

    printf("\nPlain text message:\n\n");
    for (int i = 0; i < len_cipher; i++)
        putchar(numbers[i] ^ key[i % N]);
    printf("\n");

    /* We found that the first two bytes of the key are incorrect.
     * After some manual adjustment we found the following key:
     */
    printf("\nAdjusted key: 186 31 145 178 83 205 62\n");
    int my_key[] = {186, 31, 145, 178, 83, 205, 62};

    printf("Plain text message:\n\n");
    for (int i = 0; i < len_cipher; i++)
        putchar(numbers[i] ^ my_key[i % N]);
    printf("\n");

    return EXIT_SUCCESS;
}
