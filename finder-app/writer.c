#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    // Open syslog with LOG_USER facility
    openlog("writer", LOG_PID, LOG_USER);
    
    // Check if exactly two arguments are provided
    if (argc != 3) {
        syslog(LOG_ERR, "Invalid number of arguments: %d (expected 2)", argc - 1);
        fprintf(stderr, "Error: Two arguments required: <writefile> <writestr>\n");
        closelog();
        return 1;
    }
    
    const char *writefile = argv[1];
    const char *writestr = argv[2];
    
    // Log the write operation with LOG_DEBUG level
    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);
    
    // Open file for writing (create or truncate)
    FILE *fp = fopen(writefile, "w");
    if (fp == NULL) {
        syslog(LOG_ERR, "Could not open file %s for writing: %s", 
               writefile, strerror(errno));
        fprintf(stderr, "Error: Could not write to file %s\n", writefile);
        closelog();
        return 1;
    }
    
    // Write the string to the file
    if (fprintf(fp, "%s\n", writestr) < 0) {
        syslog(LOG_ERR, "Error writing to file %s: %s", 
               writefile, strerror(errno));
        fprintf(stderr, "Error: Could not write to file %s\n", writefile);
        fclose(fp);
        closelog();
        return 1;
    }
    
    // Close the file
    if (fclose(fp) != 0) {
        syslog(LOG_ERR, "Error closing file %s: %s", 
               writefile, strerror(errno));
        closelog();
        return 1;
    }
    
    // Close syslog
    closelog();
    
    return 0;
}
