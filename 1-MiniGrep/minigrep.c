#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_BUFFER_SZ 256

// Function prototypes
void usage(char *exename);
int str_len(char *str);
int str_match(char *line, char *pattern, int case_insensitive);

/**
 * usage - prints usage information
 * @exename: the name of the executable
 */
void usage(char *exename) {
    printf("usage: %s [-h|n|i|c|v] \"pattern\" filename\n", exename);
    printf("  -h    prints this help message\n");
    printf("  -n    prints matching lines with line numbers\n");
    printf("  -i    case-insensitive search\n");
    printf("  -c    counts matching lines\n");
    printf("  -v    inverts match (prints non-matching lines) [EXTRA CREDIT]\n");
}

/**
 * str_len - calculates the length of a string
 * @str: pointer to null-terminated string
 * 
 * Returns: length of string (not including null terminator)
 * 
 * TODO: IMPLEMENT THIS FUNCTION
 * You must use pointer arithmetic, no array notation
 * Do NOT use strlen() from standard library
 */
int str_len(char *str) {
    int len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}


/**
 * str_match - searches for pattern in line
 * @line: the line to search in
 * @pattern: the pattern to search for
 * @case_insensitive: if 1, ignore case; if 0, case-sensitive
 * 
 * Returns: 1 if pattern found, 0 if not found
 * 
 * TODO: IMPLEMENT THIS FUNCTION
 * You must use pointer arithmetic, no array notation
 * Hint: For case-insensitive, use tolower() or toupper() on both characters
 * Hint: You need to check if pattern matches starting at ANY position in line
 */

int str_match(char *line, char *pattern, int case_insensitive) {
    char *l_start;
    char *l_ptr;
    char *p_ptr;

    if (*pattern == '\0') {
        return 1;
    }

    l_start = line;

    while (*l_start != '\0') {
        l_ptr = l_start;
        p_ptr = pattern;

        while (*p_ptr != '\0' && *l_ptr != '\0') {
            char lc = *l_ptr;
            char pc = *p_ptr;

            if (case_insensitive) {
                lc = tolower(lc);
                pc = tolower(pc);
            }

            if (lc != pc) {
                break;
            }

            l_ptr++;
            p_ptr++;
        }

        if (*p_ptr == '\0') {
            return 1;
        }

        l_start++;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    char *line_buffer;      // buffer for reading lines from file
    char *pattern;          // the search pattern
    char *filename;         // the file to search
    FILE *fp;               // file pointer
    int show_line_nums = 0; // flag for -n option
    int case_insensitive = 0; // flag for -i option
    int count_only = 0;     // flag for -c option
    int invert_match = 0;   // flag for -v option (extra credit)
    int line_number = 0;    // current line number in file
    int match_count = 0;    // count of matching lines
    int found_match;        // result from str_match()
    
    // Check minimum arguments
    if (argc < 2) {
        usage(argv[0]);
        exit(2);
    }
    
    // Handle -h flag
    if (*argv[1] == '-' && *(argv[1] + 1) == 'h') {
        usage(argv[0]);
        exit(0);
    }
    
    // Parse command line arguments
    int arg_idx = 1;  // current argument index
    
    // Check for option flags (they start with -)
    if (*argv[arg_idx] == '-') {
        char *flag_ptr = argv[arg_idx] + 1;  // skip the '-'
        
        // Process each character in the flag
        while (*flag_ptr != '\0') {
            switch (*flag_ptr) {
                case 'n':
                    show_line_nums = 1;
                    break;
                case 'i':
                    case_insensitive = 1;
                    break;
                case 'c':
                    count_only = 1;
                    break;
                case 'v':
                    invert_match = 1;  // extra credit
                    break;
                default:
                    printf("Error: Unknown option -%c\n", *flag_ptr);
                    usage(argv[0]);
                    exit(2);
            }
            flag_ptr++;  // move to next character in flag
        }
        arg_idx++;  // move to next argument
    }
    
    // Check we have pattern and filename
    if (argc < arg_idx + 2) {
        printf("Error: Missing pattern or filename\n");
        usage(argv[0]);
        exit(2);
    }
    
    pattern = argv[arg_idx];
    filename = argv[arg_idx + 1];

    line_buffer = malloc(LINE_BUFFER_SZ);
    if (line_buffer == NULL) {
        exit(4);
    }

    fp = fopen(filename, "r");
    if (fp == NULL) {
        free(line_buffer);
        exit(3);
    }

    while (fgets(line_buffer, LINE_BUFFER_SZ, fp) != NULL) {
        line_number++;

        found_match = str_match(line_buffer, pattern, case_insensitive);

        if (invert_match) {
            found_match = !found_match;
        }

        if (found_match) {
            match_count++;

            if (!count_only) {
                if (show_line_nums) {
                    printf("%d:", line_number);
                }

                char *c = line_buffer;
                while (*c != '\0') {
                    putchar(*c);
                    c++;
                }
            }
        }
    }

    fclose(fp);

    if (count_only) {
        if (match_count > 0) {
            printf("Matches found: %d\n", match_count);
        } else {
            printf("No matches found\n");
        }
    }

    free(line_buffer);
 
    if (match_count > 0) {
        exit(0);
    } else {
        exit(1);
    }
}
