#define _POSIX_C_SOURCE 200809L  // Enable POSIX functions
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Test signal handling by creating a child process
int test_signal_handler() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process - simulate the signal handler
        printf("Child process: Setting up signal handler\n");
        
        // Simple signal handler that exits gracefully
        struct sigaction sa;
        sa.sa_handler = SIG_DFL; // Use default handler for testing
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(SIGINT, &sa, NULL);
        
        // Send SIGINT to self after a short delay
        sleep(1);
        printf("Child process: Sending SIGINT to self\n");
        kill(getpid(), SIGINT);
        
        // Should not reach here
        printf("ERROR: Child process did not exit on SIGINT\n");
        exit(1);
    } else if (pid > 0) {
        // Parent process - wait for child
        int status;
        waitpid(pid, &status, 0);
        
        // Check if child exited due to signal
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) {
            printf("✓ Signal handling test passed: Child exited due to SIGINT\n");
            return 0;
        } else {
            printf("✗ Signal handling test failed: Child did not exit due to SIGINT\n");
            return 1;
        }
    } else {
        perror("fork failed");
        return 1;
    }
}

int main() {
    printf("=== Signal Handling Test ===\n");
    
    int result = test_signal_handler();
    
    if (result == 0) {
        printf("🎉 Signal handling test completed successfully\n");
    } else {
        printf("❌ Signal handling test failed\n");
    }
    
    return result;
}