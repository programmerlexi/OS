int hello_loop = 0;
void hello() {
    while (1) {
        print_string("Hello, world!\n\r");
        hello_loop++;
        if (hello_loop >= 10) {
            quit();
        }
    }
}